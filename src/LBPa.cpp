//
// Created by vrbik on 20.10.16.
//

#include "include/LBPa.h"
#include "include/Configuration.h"

void LBPa::Process(_image &img) {
    //Extraction phase

    Mat cropped;
    croppedImage(img.working_img, cropped, config.lbpa_params.shape_safe_offset + config.lbpa_params.range);
    Mat lbpa = Mat(img.working_img.rows, img.working_img.cols, CV_32SC1);
    extractLBPa(cropped, lbpa, config);

    img.exctracted_vector = globalHistogram(lbpa, config.hist.grid_size, config.hist.uniform);

}

void LBPa::setUp(void *_param) {
    config = *(struct _LBPa_config*)_param;
    initUniformTable(8);
}

LBPa::LBPa() {

}

LBPa::~LBPa() {

}
