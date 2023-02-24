// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5TYPES_H_
#define QPT_HDF5_H5TYPES_H_

#include <hdf5.h>

#include <cstdint>
#include <type_traits>

#include "../Serialization.h"

namespace QPT {

template <typename T>
struct H5TypeTraits;

// check if type is serializable and can be stored in a HDF5 file
template <typename T, typename = void>
struct H5TypeIsSerializable : std::false_type {};
template <typename T>
struct H5TypeIsSerializable<
    T, std::void_t<decltype(H5TypeTraits<
                            typename SerializationTraits<T>::Storage_t>{})>>
    : std::true_type {};
template <typename T>
constexpr static bool H5TypeIsSerializable_v = H5TypeIsSerializable<T>::value;

template <>
class H5TypeTraits<float> {
 public:
  static hid_t GetStorageType() { return H5T_IEEE_F32LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_FLOAT; }
};

template <>
class H5TypeTraits<double> {
 public:
  static hid_t GetStorageType() { return H5T_IEEE_F64LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_DOUBLE; }
};

template <>
class H5TypeTraits<std::int8_t> {
 public:
  static hid_t GetStorageType() { return H5T_STD_I8LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_INT8; }
};

template <>
class H5TypeTraits<std::uint8_t> {
 public:
  static hid_t GetStorageType() { return H5T_STD_U8LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_UINT8; }
};

template <>
class H5TypeTraits<std::int16_t> {
 public:
  static hid_t GetStorageType() { return H5T_STD_I16LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_INT16; }
};

template <>
class H5TypeTraits<std::uint16_t> {
 public:
  static hid_t GetStorageType() { return H5T_STD_U16LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_UINT16; }
};

template <>
class H5TypeTraits<std::int32_t> {
 public:
  static hid_t GetStorageType() { return H5T_STD_I32LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_INT32; }
};

template <>
class H5TypeTraits<std::uint32_t> {
 public:
  static hid_t GetStorageType() { return H5T_STD_U32LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_UINT32; }
};

template <>
class H5TypeTraits<std::int64_t> {
 public:
  static hid_t GetStorageType() { return H5T_STD_I64LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_INT64; }
};

template <>
class H5TypeTraits<std::uint64_t> {
 public:
  static hid_t GetStorageType() { return H5T_STD_U64LE; }
  static hid_t GetNativeType() { return H5T_NATIVE_UINT64; }
};

}  // namespace QPT

#endif  // !QPT_HDF5_H5TYPES_H_