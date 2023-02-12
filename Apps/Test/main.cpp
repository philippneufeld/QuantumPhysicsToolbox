// Philipp Neufeld, 2023

// #include <QPT/HDF5/H5Traits.h>
#include <QPT/HDF5/H5TypeTraits.h>

#include <Eigen/Dense>
#include <iostream>

using namespace QPT;

template <typename T>
void test(const T& val) {
  std::cout << H5TypeTraits<T>().GetNativeType() << std::endl;
  std::cout << H5TypeTraits<T>().GetRank() << std::endl;
  std::cout << H5TypeTraits<T>().GetSize(val) << std::endl;
  for (auto d : H5TypeTraits<T>().GetShape(val)) std::cout << d << ", ";
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  std::vector<std::vector<float>> val(5);
  for (auto& v : val) v.resize(9);
  test(val);

  double val2[8][3][12];
  test(val2);

  std::vector<int> val3[8][3][12];
  val3[0][0][0].resize(4);
  test(val3);

  return 0;
}
