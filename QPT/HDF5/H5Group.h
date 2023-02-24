// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5GROUP_H_
#define QPT_HDF5_H5GROUP_H_

#include <functional>
#include <optional>
#include <string>

#include "H5Dataset.h"
#include "H5Object.h"
#include "H5Types.h"

namespace QPT {

class H5Group : public H5Object {
 protected:
  H5Group(hid_t hid);

 public:
  bool HasSubgroup(const std::string& name);
  std::optional<H5Group> OpenSubgroup(const std::string& name);

  bool HasDataset(const std::string& name);
  std::optional<H5Dataset> OpenExistingDataset(const std::string& name);
  template <typename T, typename = std::enable_if_t<H5TypeIsSerializable_v<T>>>
  std::optional<H5Dataset> CreateUninitializedDataset(
      const std::string& name, const std::vector<std::size_t>& shape);
  template <typename T, typename = std::enable_if_t<H5TypeIsSerializable_v<T>>>
  std::optional<H5Dataset> CreateDataset(const std::string& name, const T& val);

  void EnumerateSubgroups(std::function<void(const std::string&)> callback);
  void EnumerateDatasets(std::function<void(const std::string&)> callback);
};

// Template function definitions
template <typename T, typename>
inline std::optional<H5Dataset> H5Group::CreateUninitializedDataset(
    const std::string& name, const std::vector<std::size_t>& shape) {
  const auto stype = H5TypeTraits<
      typename SerializationTraits<T>::Storage_t>::GetStorageType();
  return H5Dataset::Create(GetHandle(), stype, name, shape);
}

template <typename T, typename>
inline std::optional<H5Dataset> H5Group::CreateDataset(const std::string& name,
                                                       const T& val) {
  auto optDs = CreateUninitializedDataset<T>(
      name, SerializationTraits<T>::GetShape(val));
  if (!optDs.has_value() || !optDs->Set(val)) return std::nullopt;
  return optDs;
}

}  // namespace QPT

#endif  // !QPT_HDF5_H5GROUP_H_