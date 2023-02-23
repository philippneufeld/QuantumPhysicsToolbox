// Philipp Neufeld, 2023

#ifndef QPT_SERIALIZATION_H_
#define QPT_SERIALIZATION_H_

// Includes
#include <array>
#include <cassert>
#include <cstdint>
#include <list>
#include <type_traits>
#include <vector>

#include "TypeList.h"

namespace QPT {

//
// SerializationTraits
//

template <typename T, typename = void>
struct SerializationTraits;

// remove const, volatile and reference
template <typename T>
struct SerializationTraits<T&, void> : SerializationTraits<T> {};
template <typename T>
struct SerializationTraits<const T, void> : SerializationTraits<T> {};
template <typename T>
struct SerializationTraits<volatile T, void> : SerializationTraits<T> {};

// List of fundamental types that are supported by HDF5 as-is
using SerializationTrivialNatives_t =
    Typelist<std::int8_t, std::uint8_t, std::int16_t, std::uint16_t,
             std::int32_t, std::uint32_t, std::int64_t, std::uint64_t, float,
             double>;
template <typename T>
constexpr static bool IsSerializationTrivialNative_v =
    TypelistContains_v<SerializationTrivialNatives_t, T>;

// SerializationTraits for fundamental types
template <typename T>
struct SerializationTraits<
    T, std::enable_if_t<IsSerializationTrivialNative_v<T>>> {
  using Native_t = T;
  using Storage_t = Native_t;

  constexpr static std::size_t GetRank() { return 0; }
  constexpr static std::size_t GetSize(const T&) { return 1; }
  static void GetShape(const T&, std::size_t* shape) {}
  static std::vector<std::size_t> GetShape(const T&) { return {}; }

  static const Storage_t* SerializeTrivial(const Native_t& val) { return &val; }
  static void Prepare(Native_t& val, const std::size_t* shape) {}
  static void Serialize(const Native_t& val, Storage_t* buffer) {
    *buffer = val;
  }
  static void Deserialize(Native_t& val, const Storage_t* buffer) {
    val = *buffer;
  }
};

// SerializationTraitsHelper (can later be specialized later)
template <typename T>
struct SerializationTraitsHelper;

// Serialization traits for array types (specialize SerializationTraitsHelper)
template <typename T>
struct SerializationTraits<
    T, std::void_t<decltype(SerializationTraitsHelper<T>{})>>
    : SerializationTraits<decltype(*std::begin(std::declval<T&>()))> {
  using Inner_t =
      SerializationTraits<decltype(*std::begin(std::declval<T&>()))>;

  using Native_t = typename Inner_t::Native_t;
  using Storage_t = typename Inner_t::Storage_t;

  constexpr static std::size_t GetRank() { return 1 + Inner_t::GetRank(); }
  static std::size_t GetSize(const T& val) {
    std::size_t size = SerializationTraitsHelper<T>::GetSize(val);
    const auto it = std::begin(val);
    if (size != 0) size *= Inner_t::GetSize(*it);
    return size;
  }
  static void GetShape(const T& val, std::size_t* shape) {
    shape[0] = SerializationTraitsHelper<T>::GetSize(val);
    if (shape[0] != 0)
      Inner_t::GetShape(val[0], shape + 1);
    else
      std::fill(shape + 1, shape + GetRank(), 0);
  }
  static std::vector<std::size_t> GetShape(const T& val) {
    std::vector<std::size_t> shape(GetRank());
    GetShape(val, shape.data());
    return shape;
  }

  template <typename Dummy = T>
  static std::enable_if_t<
      std::is_same_v<
          void, std::void_t<
                    decltype(SerializationTraitsHelper<Dummy>::SerializeTrivial(
                        std::declval<Dummy>()))>> &&
          std::is_same_v<T, Dummy>,
      const Storage_t*>
  SerializeTrivial(const Dummy& val) {
    return SerializationTraitsHelper<Dummy>::SerializeTrivial(val);
  }
  static void Prepare(T& val, const std::size_t* shape) {
    std::size_t n = shape[0];
    SerializationTraitsHelper<T>::Prepare(val, n);
    for (auto it = std::begin(val); it != std::end(val); it++)
      Inner_t::Prepare(*it, shape + 1);
  }
  static void Serialize(const T& val, Storage_t* buffer) {
    std::size_t outerSize = SerializationTraitsHelper<T>::GetSize(val);
    if (outerSize != 0) {
      auto it = std::begin(val);
      std::size_t stride = Inner_t::GetSize(*it);
      for (; it != std::end(val); it++, buffer += stride)
        Inner_t::Serialize(*it, buffer);
    }
  }

  static void Deserialize(T& val, const Storage_t* buffer) {
    std::size_t outerSize = SerializationTraitsHelper<T>::GetSize(val);
    if (outerSize != 0) {
      auto it = std::begin(val);
      std::size_t stride = Inner_t::GetSize(*it);
      for (; it != std::end(val); it++, buffer += stride)
        Inner_t::Deserialize(*it, buffer);
    }
  }
};

//
// Specialization of the SerializationTraitsHelper for various array types
//

template <typename T, std::size_t N>
struct SerializationTraitsHelper<T[N]> {
  using Ref_t = T (&)[N];
  using CRef_t = const T (&)[N];
  template <typename Dummy = T>
  static std::enable_if_t<IsSerializationTrivialNative_v<Dummy>, const T*>
  SerializeTrivial(const T (&val)[N]) {
    return val;
  }
  static void Prepare(T (&val)[N], std::size_t n) { assert(n == N); }
  static constexpr std::size_t GetSize(const T (&val)[N]) { return N; }
};

template <typename T, std::size_t N>
struct SerializationTraitsHelper<std::array<T, N>> {
  template <typename Dummy = T>
  static std::enable_if_t<IsSerializationTrivialNative_v<Dummy>, const T*>
  SerializeTrivial(const std::array<T, N>& val) {
    return val.data();
  }
  static void Prepare(std::array<T, N>& val, std::size_t n) { assert(n == N); }
  static constexpr std::size_t GetSize(const std::array<T, N>& val) {
    return N;
  }
};

template <typename T>
struct SerializationTraitsHelper<std::vector<T>> {
  template <typename Dummy = T>
  static std::enable_if_t<IsSerializationTrivialNative_v<Dummy>, const T*>
  SerializeTrivial(const std::vector<T>& val) {
    return val.data();
  }
  static void Prepare(std::vector<T>& val, std::size_t n) { val.resize(n); }
  static std::size_t GetSize(const std::vector<T>& val) { return val.size(); }
};

template <typename T>
struct SerializationTraitsHelper<std::list<T>> {
  static void Prepare(std::list<T>& val, std::size_t n) { val.resize(n); }
  static std::size_t GetSize(const std::list<T>& val) { return val.size(); }
};

//
// Serializer
//

template <typename T, typename = void>
class Serializer {
  using Storage_t = typename SerializationTraits<T>::Storage_t;

 public:
  Serializer(const T& val) : m_data(SerializationTraits<T>::GetSize(val)) {
    SerializationTraits<T>::Serialize(val, m_data.data());
  }

  std::size_t GetSize() const { return m_data.size(); }
  const Storage_t* GetData() const { return m_data.data(); }

 private:
  std::vector<Storage_t> m_data;
};
template <typename T>
class Serializer<T,
                 std::void_t<decltype(SerializationTraits<T>::SerializeTrivial(
                     std::declval<T>()))>> {
  using Storage_t = typename SerializationTraits<T>::Storage_t;

 public:
  Serializer(const T& val) : m_val(val) {}
  std::size_t GetSize() const { return SerializationTraits<T>::GetSize(m_val); }
  const Storage_t* GetData() const {
    return SerializationTraits<T>::SerializeTrivial(m_val);
  }

 private:
  const T& m_val;
};
template <typename T, typename Dummy>
class Serializer<T&, Dummy> : public Serializer<T> {};
template <typename T, typename Dummy>
class Serializer<const T, Dummy> : public Serializer<T> {};
template <typename T, typename Dummy>
class Serializer<volatile T, Dummy> : public Serializer<T> {};
template <typename T>
Serializer<T> Serialize(const T& val) {
  return Serializer<T>(val);
}

//
// Deserialize
//

template <typename T>
T Deserialize(const typename SerializationTraits<T>::Storage_t* ptr,
              const std::vector<std::size_t>& shape) {
  T val;
  SerializationTraits<T>::Prepare(val, shape.data());
  SerializationTraits<T>::Deserialize(val, ptr);
  return val;
}

}  // namespace QPT

#endif  // !QPT_SERIALIZATION_H_
