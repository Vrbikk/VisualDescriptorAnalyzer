//
// Created by vrbik on 20.10.16.
//

#include "include/LBPa.h"



void LBPa::Process(_image &img) {
    //Extraction phase

    Mat cropped;
    croppedImage(img.working_img, cropped, config.lbpa_params.BORDER_OFFSET);
    Mat lbpa = Mat(img.working_img.cols, img.working_img.rows, CV_32SC1);
    extractLBPa(cropped, lbpa, config);

    std::vector<Mat*> src;
    src.push_back(&lbpa);

    img.exctracted_vector = globalHistogram(src, config.hist.grid_size, config.hist.uniform);
}

void LBPa::setUp(void *_param) {
    config = *(struct _LBPa_config*)_param;
    initUniformTable(8);

}

LBPa::LBPa() {

}

LBPa::~LBPa() {

}
