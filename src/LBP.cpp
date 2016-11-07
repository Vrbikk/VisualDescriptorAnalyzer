//
// Created by vrbik on 27.9.16.
//

#include "include/LBP.h"

LBP::LBP() {

}

LBP::~LBP() {

}

void LBP::Process(_image &img) {

   //Extraction phase
   Mat cropped;
   croppedImage(img.working_img, cropped, config.lbp_params.range);
   Mat lbp = Mat::zeros(img.working_img.cols, img.working_img.rows, CV_32SC1);
   extractLBP(cropped, lbp, config);

   //Making of histogram

   std::vector<Mat*> src;
   src.push_back(&lbp);
   img.exctracted_vector = globalHistogram(src, config.hist.grid_size, config.hist.uniform);
}

void LBP::setUp(void *_param) {
   config = *(struct _LBP_config*)_param;
   initUniformTable(config.lbp_params.neighbours);
   optimize_LBP_pls(config);
}



