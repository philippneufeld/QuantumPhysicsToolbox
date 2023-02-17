// Philipp Neufeld, 2023

#ifndef QPT_HDF5_H5TYPETRAITS_H_
#define QPT_HDF5_H5TYPETRAITS_H_

// Includes
#include <hdf5.h>

#include <type_traits>
#include <vector>

#include "../TypeList.h"

namespace QPT {

template <typename T, typename = void>
struct H5TypeHandler;

template <typename T>
struct H5TypeHandler<T&, void> : H5TypeHandler<T> {};
template <typename T>
struct H5TypeHandler<const T, void> : H5TypeHandler<T> {};

// List of fundamental types that are supported by HDF5 as-is
using H5TrivialNativeTypes_t =
    Typelist<std::int8_t, std::uint8_t, std::int16_t, std::uint16_t,
             std::int32_t, std::uint32_t, std::int64_t, std::uint64_t, float,
             double>;

template <typename T>
struct H5TypeHandler<
    T, std::enable_if_t<TypelistContains_v<H5TrivialNativeTypes_t, T>>> {
  using Native_t = T;
  using Storage_t = Native_t;

  constexpr static std::size_t GetRank() { return 0; }
  constexpr static std::size_t GetSize(const T&) { return 1; }
  static void GetShape(const T&, std::size_t* dims) {}
  static std::vector<std::size_t> GetShape(const T&) { return {}; }

  void Serialize(const Native_t& val, Storage_t* buffer) { *buffer = val; }
  void Deserialize(Native_t& val, Storage_t* buffer) { val = *buffer; }
};

// declare H5TypeTraitsArray that can be specialized later
template <typename T>
struct H5TypeHandlerArray;

// H5Type traits for array types (specialize H5TypeTraitsArray)
template <typename T>
struct H5TypeHandler<T, std::void_t<decltype(H5TypeHandlerArray<T>{})>>
    : H5TypeHandler<decltype(std::declval<T>()[0])> {
  using Parent_t = H5TypeHandler<decltype(std::declval<T>()[0])>;

  constexpr static std::size_t GetRank() { return 1 + Parent_t::GetRank(); }
  static std::size_t GetSize(const T& val) {
    std::size_t size = H5TypeHandlerArray<T>::GetSize(val);
    if (size != 0) size *= Parent_t::GetSize(val[0]);
    return size;
  }
  void GetShape(const T& val, std::size_t* dims) const {
    dims[0] = H5TypeHandlerArray<T>::GetSize(val);
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
struct H5TypeHandlerArray<std::vector<T>> {
  static std::size_t GetSize(const std::vector<T>& val) { return val.size(); }
};

template <typename T, std::size_t N>
struct H5TypeHandlerArray<T[N]> {
  static constexpr std::size_t GetSize(const T (&val)[N]) { return N; }
};

}  // namespace QPT

#endif  // !QPT_HDF5_H5TYPETRAITS_H_
