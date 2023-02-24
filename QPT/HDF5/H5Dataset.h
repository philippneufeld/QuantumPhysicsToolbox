// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5DATASET_H_
#define QPT_HDF5_H5DATASET_H_

#include <optional>
#include <string>

#include "../Serialization.h"
#include "H5Object.h"
#include "H5Types.h"

namespace QPT {

class H5Dataset : public H5Object {
 protected:
  friend class H5Group;
  static std::optional<H5Dataset> Create(hid_t grp, hid_t sType,
                                         const std::string& name,
                                         const std::vector<std::size_t>& shape);
  H5Dataset(hid_t hid);

 public:
  std::vector<std::size_t> GetShape();

  template <typename T, typename = std::enable_if_t<H5TypeIsSerializable_v<T>>>
  bool Get(T& data);
  template <typename T>
  std::enable_if_t<H5TypeIsSerializable_v<T> &&
                       std::is_default_constructible_v<T>,
                   std::optional<T>>
  Get() {
    T t;
    return Get(t) ? std::make_optional(t) : std::nullopt;
  }
  template <typename T, typename = std::enable_if_t<H5TypeIsSerializable_v<T>>>
  bool Set(const T& data);

 protected:
  bool GetRaw(hid_t nType, void* data);
  bool SetRaw(hid_t nType, const void* data);
};

// Template function definitions
template <typename T, typename>
inline bool H5Dataset::Get(T& data) {
  const auto shape = GetShape();
  if (SerializationTraits<T>::GetRank() != shape.size()) return false;
  using TT = H5TypeTraits<typename SerializationTraits<T>::Storage_t>;
  Deserializer<T> des(data, shape);
  if (!GetRaw(TT::GetNativeType(), des.GetData())) return false;
  des.Execute();
  return true;
}

template <typename T, typename>
inline bool H5Dataset::Set(const T& data) {
  if (SerializationTraits<T>::GetShape(data) != GetShape()) return false;
  using TT = H5TypeTraits<typename SerializationTraits<T>::Storage_t>;
  return SetRaw(TT::GetNativeType(), Serialize(data).GetData());
}

}  // namespace QPT

#endif  // !QPT_HDF5_H5DATASET_H_