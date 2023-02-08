// Philipp Neufeld, 2023

#define H5_USE_EIGEN 1

#include <iostream>
#include <Eigen/Dense>
#include <highfive/H5File.hpp>

namespace HF = HighFive;

int main(int argc, char* argv[])
{
    // generate matrix
    const int nrows = 10;
    const int ncols = 3;
    Eigen::MatrixXd mat(nrows, ncols);
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            mat(i, j) = double(j + i * 100);
        }
    }

    Eigen::Map<Eigen::MatrixXd> mat2( mat.data(), ncols, nrows);
    

    HF::File file("eigen_mat.h5", HF::File::ReadWrite | HF::File::Create | HF::File::Truncate);
    HF::DataSet dset = file.createDataSet("mat", mat2);
    dset.write(mat);

    std::cout << "Hello world" << std::endl;
    return 0;
}
