// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5TYPES_H_
#define QPT_HDF5_H5TYPES_H_

#include <hdf5.h>

#include <cstdint>

namespace QPT {

template <typename T>
struct H5TypeTraits;

template <>
class H5TypeTraits<float> {
 public:
  hid_t GetStorageType() const { return H5T_IEEE_F32LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_FLOAT; }
};

template <>
class H5TypeTraits<double> {
 public:
  hid_t GetStorageType() const { return H5T_IEEE_F64LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_DOUBLE; }
};

template <>
class H5TypeTraits<std::int8_t> {
 public:
  hid_t GetStorageType() const { return H5T_STD_I8LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_INT8; }
};

template <>
class H5TypeTraits<std::uint8_t> {
 public:
  hid_t GetStorageType() const { return H5T_STD_U8LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_UINT8; }
};

template <>
class H5TypeTraits<std::int16_t> {
 public:
  hid_t GetStorageType() const { return H5T_STD_I16LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_INT16; }
};

template <>
class H5TypeTraits<std::uint16_t> {
 public:
  hid_t GetStorageType() const { return H5T_STD_U16LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_UINT16; }
};

template <>
class H5TypeTraits<std::int32_t> {
 public:
  hid_t GetStorageType() const { return H5T_STD_I32LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_INT32; }
};

template <>
class H5TypeTraits<std::uint32_t> {
 public:
  hid_t GetStorageType() const { return H5T_STD_U32LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_UINT32; }
};

template <>
class H5TypeTraits<std::int64_t> {
 public:
  hid_t GetStorageType() const { return H5T_STD_I64LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_INT64; }
};

template <>
class H5TypeTraits<std::uint64_t> {
 public:
  hid_t GetStorageType() const { return H5T_STD_U64LE; }
  hid_t GetNativeType() const { return H5T_NATIVE_UINT64; }
};

}  // namespace QPT

#endif  // !QPT_HDF5_H5TYPES_H_