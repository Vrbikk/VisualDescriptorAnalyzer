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
   croppedImage(img.working_img, cropped, config.lbp_params.range + config.lbp_params.shape_safe_offset);
   Mat lbp = Mat::zeros(img.working_img.rows, img.working_img.cols, CV_32SC1);
   extractLBP(cropped, lbp, config);

   //show_image(lbp);
   //Making of histogram
   img.exctracted_vector = globalHistogram(lbp, config.hist.grid_size, config.hist.uniform);
}

void LBP::setUp(void *_param) {
   config = *(struct _LBP_config*)_param;
   initUniformTable(config.lbp_params.neighbours);
   optimize_LBP_pls(config);

   LOGGER->Info(config.lbp_params.print());
   LOGGER->Info(config.hist.print());
}



