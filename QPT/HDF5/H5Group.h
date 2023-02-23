// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5GROUP_H_
#define QPT_HDF5_H5GROUP_H_

#include <functional>
#include <optional>
#include <string>

#include "H5Object.h"

namespace QPT {

class H5Group : public H5Object {
 public:
  H5Group(hid_t hid);

  bool HasSubgroup(const std::string& name);
  std::optional<H5Group> OpenSubgroup(const std::string name);

  void EnumerateSubgroups(std::function<void(const std::string&)> callback);
  void EnumerateDatasets(std::function<void(const std::string&)> callback);
};

}  // namespace QPT

#endif  // !QPT_HDF5_H5GROUP_H_