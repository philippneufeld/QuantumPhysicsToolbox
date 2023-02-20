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
  for (auto it = data.begin(); it != data.end(); it++) Print(*it);
  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  // prepare data
  std::list<std::vector<float>> val;
  for (int i = 0; i < 5; i++) {
    val.push_back({});
    for (int j = 0; j < 4; j++) val.back().push_back(1 + i * j);
  }

  Print(val);
  auto serialized = Serialize(val);
  Print(serialized.GetData(), serialized.GetSize());
  Print(Deserialize<decltype(val)>(serialized.GetData(), {5, 4}));

  auto val2 = val.front();
  Print(val2);
  auto serialized2 = Serialize(val2);
  Print(serialized2.GetData(), serialized2.GetSize());
  Print(Deserialize<decltype(val2)>(serialized2.GetData(), {4}));

  return 0;
}
