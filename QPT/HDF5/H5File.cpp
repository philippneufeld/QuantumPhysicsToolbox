// Philipp Neufeld, 2023

#include "H5File.h"

namespace QPT {

H5File::H5File(hid_t file, hid_t root) : H5Group(root), m_file(file) {}

H5File::H5File(H5File&& rhs) : H5Group(std::move(rhs)), m_file(rhs.m_file) {
  rhs.m_file = H5I_INVALID_HID;
}

H5File& H5File::operator=(H5File&& rhs) {
  H5Group::operator=(std::move(rhs));
  std::swap(m_file, rhs.m_file);
  return *this;
}

H5File::~H5File() {
  if (m_file >= 0) H5Fclose(m_file);
  m_file = H5I_INVALID_HID;
}

std::optional<H5File> H5File::Open(const std::string& name,
                                   H5FileOpenFlag flag) {
  hid_t file = H5I_INVALID_HID;
  hid_t root = H5I_INVALID_HID;

  H5E_BEGIN_TRY
  if (flag & (H5File_MUST_NOT_EXIST | H5File_TRUNCATE)) {
    // create empty file
    const auto creationFlags =
        (flag & H5File_MUST_NOT_EXIST) ? H5F_ACC_EXCL : H5F_ACC_TRUNC;
    file = H5Fcreate(name.c_str(), creationFlags, H5P_DEFAULT, H5P_DEFAULT);
  } else {
    // default open mode
    // open exisiting file -> if it does not exist create new file
    file = H5Fopen(name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

    if (file < 0 && !(flag & H5File_MUST_EXIST))
      file = H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  }

  if (file < 0) return std::nullopt;
  root = H5Gopen2(file, "/", H5P_DEFAULT);
  if (root < 0) return std::nullopt;

  H5E_END_TRY

  return std::make_optional(H5File(file, root));
}

}  // namespace QPT
