#include "libKriging/utils/lk_armadillo.hpp"
#include <libKriging/Kriging.hpp>
#include <libKriging/KrigingLoader.hpp>
#include <libKriging/Trend.hpp>


// model_file is a path to a stored JSON containing a libKriging surrogate model
// incoming_frcs = [fx, fy, fz, wave_elev]; read from OpenFresco + analytical linear wave
// output_x_xd should be sent to MoorDyn S-Function
arma::vec predict_motions(char *model_file, const arma::vec incoming_frcs) {
    Kriging model = Kriging::load(&model_file);
    arma::vec output_x_cd = model.predict(incoming_frcs);

    return output_x_cd
}