// Philipp Neufeld, 2023

#include "H5Object.h"

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

}  // namespace QPT
