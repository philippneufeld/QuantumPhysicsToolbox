// Philipp Neufeld, 2023

#include "H5Object.h"

#include "../ScopeGuard.h"

namespace QPT {

H5Object::H5Object(hid_t hid) : m_hid(hid) {}

H5Object::~H5Object() {
  if (IsValid()) H5Idec_ref(m_hid);
  m_hid = H5I_INVALID_HID;
}

H5Object::H5Object(const H5Object& rhs) : m_hid(rhs.m_hid) {
  if (IsValid()) H5Iinc_ref(m_hid);
}

H5Object::H5Object(H5Object&& rhs) : m_hid(rhs.m_hid) {
  rhs.m_hid = H5I_INVALID_HID;
}

H5Object& H5Object::operator=(const H5Object& rhs) {
  return this->operator=(std::move(H5Object(rhs)));
}

H5Object& H5Object::operator=(H5Object&& rhs) {
  std::swap(m_hid, rhs.m_hid);
  return *this;
}

bool H5Object::IsValid() const { return m_hid >= 0; }

bool H5Object::HasAttribute(const std::string& name) {
  return (H5Aexists(m_hid, name.c_str()) > 0);
}

std::optional<std::vector<std::size_t>> H5Object::GetAttributeShape(
    const std::string& name) {
  std::optional<std::vector<std::size_t>> res = std::nullopt;
  H5E_BEGIN_TRY
  hid_t attr = H5Aopen(m_hid, name.c_str(), H5P_DEFAULT);
  if (attr >= 0) {
    res = GetAttributeShape(attr);
    H5Aclose(attr);
  }
  H5E_END_TRY
  return res;
}

std::optional<std::vector<std::size_t>> H5Object::GetAttributeShape(
    hid_t attr) {
  if (attr < 0) return std::nullopt;

  hid_t dspace = H5Aget_space(attr);
  if (dspace < 0) return std::nullopt;
  auto dspaceGuard = CreateScopeGuard([=]() { H5Sclose(dspace); });

  int ndims = H5Sget_simple_extent_ndims(dspace);
  if (ndims <= 0) return std::nullopt;

  std::vector<hsize_t> dims(ndims);
  if (H5Sget_simple_extent_dims(dspace, &dims[0], nullptr) < 0)
    return std::nullopt;

  return std::make_optional(std::vector<std::size_t>(dims.begin(), dims.end()));
}

bool H5Object::GetAttributeRaw(const std::string& name, hid_t nType,
                               std::size_t rank, void* data) {
  if (!HasAttribute(name)) return false;
  hid_t attr = H5Aopen(m_hid, name.c_str(), H5P_DEFAULT);
  if (attr < 0) return false;
  auto attrGuard = CreateScopeGuard([=]() { H5Aclose(attr); });

  // validate rank
  if (auto shape = GetAttributeShape(attr)) {
    if (shape->size() != rank) return false;
  } else
    return false;

  return H5Aread(attr, nType, data) >= 0;
}

bool H5Object::SetAttributeRaw(const std::string& name, hid_t nType,
                               hid_t sType,
                               const std::vector<std::size_t>& shape,
                               const void* data) {
  hid_t attr = H5I_INVALID_HID;

  if (HasAttribute(name)) {
    attr = H5Aopen(m_hid, name.c_str(), H5P_DEFAULT);
    if (GetAttributeShape(attr) != shape) return false;
  } else {
    // create dataspace
    std::vector<hsize_t> dims(shape.begin(), shape.end());
    hid_t dspace = H5Screate_simple(dims.size(), dims.data(), dims.data());
    if (dspace < 0) return false;
    auto dspaceGuard = CreateScopeGuard([=]() { H5Sclose(dspace); });

    // create the actual attribute
    attr = H5Acreate2(m_hid, name.c_str(), sType, dspace, H5P_DEFAULT,
                      H5P_DEFAULT);
    if (attr < 0) return false;
  }

  // write data
  return H5Awrite(attr, nType, data) >= 0;
}

}  // namespace QPT
