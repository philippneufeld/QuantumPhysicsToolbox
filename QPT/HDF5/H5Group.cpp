// Philipp Neufeld, 2023

#include "H5Group.h"

#include <hdf5.h>

namespace QPT {

// Helpers
herr_t EnumGroupsHelper(hid_t parent, const char* name, const H5L_info_t*,
                        void* data) {
  const auto& callback =
      *static_cast<const std::function<void(const std::string&)>*>(data);

  bool valid = false;
  H5E_BEGIN_TRY
  H5Object obj(H5Gopen2(parent, name, H5P_DEFAULT));
  if (obj.IsValid()) valid = true;
  H5E_END_TRY;

  if (valid) callback(name);
  return 0;
};

herr_t EnumDatasetsHelper(hid_t parent, const char* name, const H5L_info_t*,
                          void* data) {
  const auto& callback =
      *static_cast<const std::function<void(const std::string&)>*>(data);

  bool valid = false;
  H5E_BEGIN_TRY
  H5Object obj(H5Dopen2(parent, name, H5P_DEFAULT));
  if (obj.IsValid()) valid = true;
  H5E_END_TRY;

  if (valid) callback(name);
  return 0;
};

// H5Group
H5Group::H5Group(hid_t hid) : H5Object(hid) {}

bool H5Group::HasSubgroup(const std::string& name) {
  // check if link exists
  if (H5Lexists(GetHandle(), name.c_str(), H5P_DEFAULT) <= 0) return false;

  bool valid = false;
  H5E_BEGIN_TRY { valid = OpenSubgroup(name).has_value(); }
  H5E_END_TRY;
  return valid;
}

std::optional<H5Group> H5Group::OpenSubgroup(const std::string name) {
  hid_t handle = H5I_INVALID_HID;

  H5E_BEGIN_TRY
  handle = H5Gopen2(GetHandle(), name.c_str(), H5P_DEFAULT);
  if (handle < 0)
    handle = H5Gcreate2(GetHandle(), name.c_str(), H5P_DEFAULT, H5P_DEFAULT,
                        H5P_DEFAULT);
  H5E_END_TRY;

  return (handle >= H5I_INVALID_HID) ? std::make_optional(H5Group(handle))
                                     : std::nullopt;
}

void H5Group::EnumerateSubgroups(
    std::function<void(const std::string&)> callback) {
  H5Literate(GetHandle(), H5_INDEX_NAME, H5_ITER_NATIVE, nullptr,
             &EnumGroupsHelper, &callback);
}

void H5Group::EnumerateDatasets(
    std::function<void(const std::string&)> callback) {
  H5Literate(GetHandle(), H5_INDEX_NAME, H5_ITER_NATIVE, nullptr,
             &EnumDatasetsHelper, &callback);
}

}  // namespace QPT
