// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5OBJECT_H_
#define QPT_HDF5_H5OBJECT_H_

#include <hdf5.h>

#include <cstdint>
#include <utility>

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

 protected:
  hid_t GetHandle() const { return m_hid; }

 private:
  hid_t m_hid;
};

}  // namespace QPT

#endif  // !QPT_HDF5_H5OBJECT_H_