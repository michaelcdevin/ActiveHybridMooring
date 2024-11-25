// MCD: it seems like libKriging sets these in CMakeLists.txt, but it doesn't seem to propagate correctly, so I'm hardcoding these here
#define ARMA_ALIEN_MEM_ALLOC_FUNCTION lkalloc::malloc
#define ARMA_ALIEN_MEM_FREE_FUNCTION lkalloc::free

#include <iostream>
#include <string>
#include "libKriging/utils/lk_armadillo.hpp"
#include <libKriging/Kriging.hpp>
#include <libKriging/KrigingLoader.hpp>
#include <libKriging/Trend.hpp>



// model_file is a path to a stored JSON file containing a libKriging surrogate model
// incoming_frcs = arma::vec = {fx, fy, fz, wave_elev}; read from OpenFresco + analytical linear wave
// output_x_xd should be sent to MoorDyn S-Function
arma::vec predict_motions(const std::string model_file, const arma::mat incoming_frcs) {
    auto model = Kriging::load(model_file);
    auto [output_x_xd, stdev, cov, deriv, _] = model.predict(incoming_frcs, false, false, false);

    return output_x_xd;
}

int main() {
    std::string file1 = "../../../models/model_l1px.json";
    std::string file2 = "../../../models/model_l2vz.json";
    arma::mat inputs1 = {-22403700.0, 500.0, -23290000.0, 0.5};
    arma::mat inputs2 = {-22600000.0, 200.0, -23550000.0, -0.5};
    auto motions1 = predict_motions(file1, inputs1);
    auto motions2 = predict_motions(file2, inputs2);
    std::cout << motions1 << std::endl;
    std::cout << motions2 << std::endl;

    return 0;
}
