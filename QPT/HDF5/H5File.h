// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5FILE_H_
#define QPT_HDF5_H5FILE_H_

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include "H5Object.h"

namespace QPT {

enum H5FileOpenFlag {
  H5File_DEFAULT = 0,
  H5File_MUST_EXIST = 1,
  H5File_MUST_NOT_EXIST = 2,
  H5File_TRUNCATE = 4,
};

class H5File {
 public:
  static std::optional<H5File> Open(const std::string& name,
                                    H5FileOpenFlag flag);

 protected:
  H5File(hid_t hid);

 public:
  virtual ~H5File();

  H5File(const H5File&) = delete;
  H5File(H5File&& rhs);
  H5File& operator=(const H5File&) = delete;
  H5File& operator=(H5File&& rhs);

  // H5FileGroup OpenRootGroup();

 private:
  hid_t m_file;
};

}  // namespace QPT

#endif  // !QPT_HDF5_H5FILE_H_