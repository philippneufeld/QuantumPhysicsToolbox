// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5OBJECT_H_
#define QPT_HDF5_H5OBJECT_H_

#include <hdf5.h>

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../Serialization.h"
#include "H5Types.h"

namespace QPT {

class H5Object {
 public:
  H5Object(hid_t hid);
  virtual ~H5Object();

  H5Object(const H5Object&);
  H5Object(H5Object&& rhs);
  H5Object& operator=(const H5Object&);
  H5Object& operator=(H5Object&& rhs);

  bool IsValid() const;

  bool HasAttribute(const std::string& name);
  std::optional<std::vector<std::size_t>> GetAttributeShape(
      const std::string& name);
  template <typename T, typename = std::enable_if_t<H5TypeIsSerializable_v<T>>>
  bool GetAttribute(const std::string& name, T& data);
  template <typename T>
  std::enable_if_t<H5TypeIsSerializable_v<T> &&
                       std::is_default_constructible_v<T>,
                   std::optional<T>>
  GetAttribute(const std::string& name) {
    T t;
    return GetAttribute(name, t) ? std::make_optional(t) : std::nullopt;
  }
  template <typename T, typename = std::enable_if_t<H5TypeIsSerializable_v<T>>>
  bool SetAttribute(const std::string& name, const T& data);

 protected:
  hid_t GetHandle() const { return m_hid; }

  std::optional<std::vector<std::size_t>> GetAttributeShape(hid_t attr);
  bool GetAttributeRaw(const std::string& name, hid_t nType, std::size_t rank,
                       void* data);
  bool SetAttributeRaw(const std::string& name, hid_t nType, hid_t sType,
                       const std::vector<std::size_t>& shape, const void* data);

 private:
  hid_t m_hid;
};

// Template function definitions
template <typename T, typename>
inline bool H5Object::GetAttribute(const std::string& name, T& data) {
  using TT = H5TypeTraits<typename SerializationTraits<T>::Storage_t>;
  if (auto shape = GetAttributeShape(name)) {
    Deserializer<T> des(data, *shape);
    if (!GetAttributeRaw(name, TT::GetNativeType(),
                         SerializationTraits<T>::GetRank(), des.GetData()))
      return false;
    des.Execute();
    return true;
  }
  return false;
}

template <typename T, typename>
inline bool H5Object::SetAttribute(const std::string& name, const T& data) {
  using TT = H5TypeTraits<typename SerializationTraits<T>::Storage_t>;
  return SetAttributeRaw(name, TT::GetNativeType(), TT::GetStorageType(),
                         SerializationTraits<T>::GetShape(data),
                         Serialize(data).GetData());
}

}  // namespace QPT

#endif  // !QPT_HDF5_H5OBJECT_H_