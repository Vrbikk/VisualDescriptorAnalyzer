//
// Created by vrbik on 20.10.16.
//

#include "include/LBPa.h"

void LBPa::Process(_image &img) {
    //Extraction phase

    Mat cropped;
    croppedImage(img.working_img, cropped, config.lbpa_params.shape_safe_offset + config.lbpa_params.range);
    Mat lbpa = Mat(img.working_img.rows, img.working_img.cols, CV_32SC1);
    extractLBPa(cropped, lbpa, config);

    //show_image(lbpa);
    img.exctracted_vector = globalHistogram(lbpa, config.hist.grid_size, config.hist.uniform);
}

void LBPa::setUp(void *_param) {
    config = *(struct _LBPa_config*)_param;
    initUniformTable(8);

    LOGGER->Info(config.lbpa_params.print());
    LOGGER->Info(config.hist.print());
}

LBPa::LBPa() {

}

LBPa::~LBPa() {

}
