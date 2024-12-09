import pandas as pd
import numpy as np
from numpy.typing import ArrayLike

import pylibkriging as lk

def predict_motions(model_file:str, incoming_frcs:ArrayLike):
    """model_file is a path to a stored JSON file containing a libKriging
        surrogate model
    incoming_frcs = arma::vec = {fx, fy, fz, wave_elev};
    read from OpenFresco + analytical linear wave
    output_x_xd should be sent to MoorDyn S-Function
    """
    frcs = np.atleast_2d(incoming_frcs)
    model = lk.Kriging('gauss').load(model_file)
    output_x_xd, _, _, _, _ = model.predict(frcs, False, False, False)
    return output_x_xd

if __name__ == '__main__':
    file1 = 'models/model_l1px.h5'
    file2 = 'models/model_l2vz.h5'
    inputs1 = np.array([[-22403700.0, 500.0, -23290000.0, 0.5]])
    inputs2 = np.array([[-22600000.0, 200.0, -23550000.0, -0.5]])
    motions1 = predict_motions(file1, inputs1)
    motions2 = predict_motions(file2, inputs2)
    print(motions1)
    print(motions2)