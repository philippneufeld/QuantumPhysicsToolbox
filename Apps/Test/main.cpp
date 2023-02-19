// Philipp Neufeld, 2023

#include <QPT/Serialization.h>

#include <Eigen/Dense>
#include <iostream>

using namespace QPT;

template <typename T>
void Print(const T* data, std::size_t n) {
  for (int i = 0; i < n; i++) std::cout << data[i] << " ";
  std::cout << std::endl << std::endl;
}

template <typename T>
std::enable_if_t<std::is_fundamental_v<T>> Print(const T& data) {
  std::cout << data << " ";
}

template <typename T>
std::enable_if_t<!std::is_fundamental_v<T>> Print(const T& data) {
  for (int i = 0; i < SerializationTraits<T>::GetShape(data)[0]; i++)
    Print(data[i]);
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  // prepare data
  std::vector<std::vector<float>> val(5);
  for (int i = 0; i < val.size(); i++) {
    val[i].resize(4);
    for (int j = 0; j < val[i].size(); j++) val[i][j] = i * j;
  }

  Print(val);
  auto serialized = Serialize(val);
  Print(serialized.GetData(), serialized.GetSize());
  Print(Deserialize<decltype(val)>(serialized.GetData(), {5, 4}));

  auto val2 = val[1];

  Print(val2);
  auto serialized2 = Serialize(val2);
  Print(serialized2.GetData(), serialized2.GetSize());
  Print(Deserialize<decltype(val2)>(serialized2.GetData(), {4}));

  return 0;
}
