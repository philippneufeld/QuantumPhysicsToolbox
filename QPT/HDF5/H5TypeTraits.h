// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5TYPETRAITS_H_
#define QPT_HDF5_H5TYPETRAITS_H_

// Includes
#include <hdf5.h>

#include <type_traits>
#include <vector>

#include "../Platform.h"

namespace QPT {

namespace Detail {
template <typename T, typename = void>
struct H5TypeTraitsHelper;
}  // namespace Detail

template <typename T>
struct H5TypeTraits
    : Detail::H5TypeTraitsHelper<std::remove_cv_t<std::remove_reference_t<T>>> {
};

namespace Detail {

// Helper for "fundamental" types
template <typename T, typename = void>
struct H5FundamentalTypeTraits;
#define QPT_DEF_H5TTH(type, ntype, stype)           \
  template <>                                       \
  struct H5FundamentalTypeTraits<type> {            \
    using Native_t = type;                          \
    using H5Type_t = type;                          \
    static hid_t GetStorageType() { return stype; } \
    static hid_t GetNativeType() { return ntype; }  \
  };
QPT_DEF_H5TTH(float, H5T_NATIVE_FLOAT, H5T_IEEE_F32LE);
QPT_DEF_H5TTH(double, H5T_NATIVE_DOUBLE, H5T_IEEE_F64LE);
QPT_DEF_H5TTH(std::int8_t, H5T_NATIVE_INT8, H5T_STD_I8LE);
QPT_DEF_H5TTH(std::uint8_t, H5T_NATIVE_UINT8, H5T_STD_U8LE);
QPT_DEF_H5TTH(std::int16_t, H5T_NATIVE_INT16, H5T_STD_I16LE);
QPT_DEF_H5TTH(std::uint16_t, H5T_NATIVE_UINT16, H5T_STD_U16LE);
QPT_DEF_H5TTH(std::int32_t, H5T_NATIVE_INT32, H5T_STD_I32LE);
QPT_DEF_H5TTH(std::uint32_t, H5T_NATIVE_UINT32, H5T_STD_U32LE);
QPT_DEF_H5TTH(std::int64_t, H5T_NATIVE_INT64, H5T_STD_I64LE);
QPT_DEF_H5TTH(std::uint64_t, H5T_NATIVE_UINT64, H5T_STD_U64LE);
#undef QPT_DEF_H5TTH

// H5TypeTraits for fundamental types
template <typename T>
struct H5TypeTraitsHelper<T, std::enable_if_t<std::is_fundamental_v<T>>> {
 private:
  using Reduced_t = std::remove_cv_t<std::remove_reference_t<T>>;
  using Helper_t = Detail::H5FundamentalTypeTraits<Reduced_t>;

 public:
  // Define types that are required by the traits policy
  using Base_t = typename Helper_t::Native_t;
  using H5BaseType_t = typename Helper_t::H5Type_t;

  // Get H5 type ids
  hid_t GetStorageType() const { return Helper_t::GetNativeType(); }
  hid_t GetNativeType() const { return Helper_t::GetNativeType(); }

  constexpr static std::size_t GetRank() { return 0; }
  constexpr static std::size_t GetSize(const T&) { return 1; }
  static void GetShape(const T&, std::size_t* dims) {}
  static std::vector<std::size_t> GetShape(const T&) { return {}; }
};

// declare H5TypeTraitsArray that can be specialized later
template <typename T>
struct H5TypeTraitsArray;

// H5Type traits for array types (specialize H5TypeTraitsArray)
template <typename T>
struct H5TypeTraitsHelper<T, std::void_t<decltype(H5TypeTraitsArray<T>{})>>
    : H5TypeTraits<decltype(std::declval<T>()[0])> {
  using Parent_t = H5TypeTraits<decltype(std::declval<T>()[0])>;

  constexpr static std::size_t GetRank() { return 1 + Parent_t::GetRank(); }
  static std::size_t GetSize(const T& val) {
    std::size_t size = H5TypeTraitsArray<T>::GetSize(val);
    if (size != 0) size *= Parent_t::GetSize(val[0]);
    return size;
  }
  void GetShape(const T& val, std::size_t* dims) const {
    dims[0] = H5TypeTraitsArray<T>::GetSize(val);
    if (dims[0] != 0)
      Parent_t::GetShape(val[0], dims + 1);
    else
      std::fill(dims + 1, dims + GetRank(), 0);
  }
  std::vector<std::size_t> GetShape(const T& val) const {
    std::vector<std::size_t> dims(GetRank());
    GetShape(val, dims.data());
    return dims;
  }
};

template <typename T>
struct H5TypeTraitsArray<std::vector<T>> {
  static std::size_t GetSize(const std::vector<T>& val) { return val.size(); }
};

template <typename T, std::size_t N>
struct H5TypeTraitsArray<T[N]> {
  static constexpr std::size_t GetSize(const T (&val)[N]) { return N; }
};

}  // namespace Detail
}  // namespace QPT

#endif  // !QPT_HDF5_H5TYPETRAITS_H_
