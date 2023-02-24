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
  std::vector<float[4]> val(5);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 4; j++) val[i][j] = (1.25 + i * j);
  }

  std::list<float> val2(8);
  std::iota(val2.begin(), val2.end(), 1.1f);

  float val3 = 3.141592654;

  auto file = H5File::Open("test.h5", H5File_TRUNCATE);

  std::cout << file->HasSubgroup("subA") << std::endl;
  auto subA = file->OpenSubgroup("subA");
  auto subB = file->OpenSubgroup("subB");
  auto callback = [](auto c) { std::cout << c << std::endl; };
  file->EnumerateSubgroups(callback);

  if (subA->CreateDataset("test", val))
    std::cout << "Write successful (val)" << std::endl;
  if (subA->CreateDataset("test2", val2))
    std::cout << "Write successful (val2)" << std::endl;
  if (subA->CreateDataset("test3", val3))
    std::cout << "Write successful (val3)" << std::endl;

  if (auto ds = subA->OpenExistingDataset("test")) {
    std::cout << "std::vector<std::vector<float>>" << std::endl;
    if (auto ret = ds->Get<std::vector<std::vector<float>>>()) Print(*ret);
    std::cout << "std::list<std::list<float>>" << std::endl;
    if (auto ret = ds->Get<std::list<std::list<float>>>()) Print(*ret);
    std::cout << "std::vector<std::vector<int>>" << std::endl;
    if (auto ret = ds->Get<std::vector<std::vector<int>>>()) Print(*ret);
  }

  if (auto ds = subA->OpenExistingDataset("test2")) {
    std::cout << "std::vector<float>" << std::endl;
    if (auto ret = ds->Get<std::vector<float>>()) Print(*ret);
    std::cout << "std::list<float>" << std::endl;
    if (auto ret = ds->Get<std::list<float>>()) Print(*ret);
    std::cout << "std::vector<int>" << std::endl;
    if (auto ret = ds->Get<std::vector<int>>()) Print(*ret);
  }

  if (auto ds = subA->OpenExistingDataset("test3")) {
    std::cout << "float" << std::endl;
    if (auto ret = ds->Get<float>()) std::cout << *ret << std::endl;
    std::cout << "int" << std::endl;
    if (auto ret = ds->Get<int>()) std::cout << *ret << std::endl;
  }

  return 0;
}
