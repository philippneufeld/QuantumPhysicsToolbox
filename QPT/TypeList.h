// Philipp Neufeld, 2023

#ifndef QPT_TYPELIST_H_
#define QPT_TYPELIST_H_

namespace QPT {

template <typename... Ts>
struct Typelist;

// TypelistSize
template <typename TL>
struct TypelistSize;
template <typename... Ts>
struct TypelistSize<Typelist<Ts...>> {
  constexpr static auto value = sizeof...(Ts);
};
template <typename TL>
constexpr static bool TypelistSize_v = TypelistSize<TL, T>::value;

// TypelistContains
template <typename TL, typename T>
struct TypelistContains;
template <typename Head, typename... Tail, typename T>
struct TypelistContains<Typelist<Head, Tail...>, T> {
  constexpr static auto value = TypelistContains<Typelist<Tail...>, T>::value;
};
template <typename... Tail, typename T>
struct TypelistContains<Typelist<T, Tail...>, T> {
  constexpr static auto value = true;
};
template <typename T>
struct TypelistContains<Typelist<>, T> {
  constexpr static auto value = false;
};
template <typename TL, typename T>
constexpr static bool TypelistContains_v = TypelistContains<TL, T>::value;

}  // namespace QPT

#endif
