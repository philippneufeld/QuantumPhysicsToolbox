// Philipp Neufeld, 2023

#include "H5Dataset.h"

#include <hdf5.h>

#include "../ScopeGuard.h"
#include "H5Group.h"

namespace QPT {

std::optional<H5Dataset> H5Dataset::Create(
    hid_t grp, hid_t sType, const std::string& name,
    const std::vector<std::size_t>& shape) {
  if (H5Lexists(grp, name.c_str(), H5P_DEFAULT) != 0) return std::nullopt;

  std::vector<hsize_t> dims(shape.begin(), shape.end());
  hid_t dspace = H5Screate_simple(dims.size(), dims.data(), dims.data());
  if (dspace < 0) return std::nullopt;
  auto dspaceGuard = CreateScopeGuard([=]() { H5Sclose(dspace); });

  hid_t dataset = H5I_INVALID_HID;
  H5E_BEGIN_TRY
  dataset = H5Dcreate2(grp, name.c_str(), sType, dspace, H5P_DEFAULT,
                       H5P_DEFAULT, H5P_DEFAULT);
  H5E_END_TRY

  return (dataset >= 0) ? std::make_optional(H5Dataset(dataset)) : std::nullopt;
}

H5Dataset::H5Dataset(hid_t hid) : H5Object(hid) {}

std::vector<std::size_t> H5Dataset::GetShape() {
  hid_t dspace = H5Dget_space(GetHandle());
  if (dspace < 0) return std::vector<std::size_t>{};
  auto dspaceGuard = CreateScopeGuard([=]() { H5Sclose(dspace); });

  int ndims = H5Sget_simple_extent_ndims(dspace);
  if (ndims <= 0) return std::vector<std::size_t>{};

  std::vector<hsize_t> dims(ndims);
  if (H5Sget_simple_extent_dims(dspace, &dims[0], nullptr) < 0)
    return std::vector<std::size_t>{};

  return std::vector<std::size_t>(dims.begin(), dims.end());
}

bool H5Dataset::GetRaw(hid_t nType, void* data) {
  if (H5Dread(GetHandle(), nType, H5S_ALL, H5S_ALL, H5P_DEFAULT, data) < 0)
    return false;
  return true;
}

bool H5Dataset::SetRaw(hid_t nType, const void* data) {
  if (H5Dwrite(GetHandle(), nType, H5S_ALL, H5S_ALL, H5P_DEFAULT, data) < 0)
    return false;
  if (H5Dflush(GetHandle()) < 0) return false;
  return true;
}

}  // namespace QPT
