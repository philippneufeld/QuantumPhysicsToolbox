// Philipp Neufeld, 2023

#include <QPT/HDF5/H5File.h>
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
  // std::list<std::vector<float>> val;
  std::vector<float[4]> val(5);
  for (int i = 0; i < 5; i++) {
    // val.push_back({});
    // for (int j = 0; j < 4; j++) val.back().push_back(1 + i * j);
    for (int j = 0; j < 4; j++) val[i][j] = (1 + i * j);
  }

  // Print(val);
  auto serialized = Serialize(val);
  Print(serialized.GetData(), serialized.GetSize());
  Print(Deserialize<std::vector<std::array<float, 4>>>(serialized.GetData(),
                                                       {5, 4}));

  auto& val2 = val.front();
  auto serialized2 = Serialize(val2);
  Print(serialized2.GetData(), serialized2.GetSize());
  Print(Deserialize<std::list<float>>(serialized2.GetData(), {4}));

  auto file = H5File::Open("test.h5", H5File_TRUNCATE);

  std::cout << file->HasSubgroup("subA") << std::endl;
  auto subA = file->OpenSubgroup("subA");
  file->OpenSubgroup("subA");
  file->OpenSubgroup("subB");
  file->OpenSubgroup("subC");
  std::cout << subA.has_value() << " " << file->HasSubgroup("subA")
            << std::endl;

  auto callback = [](auto c) { std::cout << c << std::endl; };
  file->EnumerateSubgroups(callback);

  return 0;
}
