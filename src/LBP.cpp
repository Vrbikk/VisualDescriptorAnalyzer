
#include "include/LBP.h"
#include "include/Configuration.h"

LBP::LBP() {

}

LBP::~LBP() {

}

void LBP::Process(_image &img) {
   Mat cropped;
   croppedImage(img.working_img, cropped, config.lbp_params.range + config.lbp_params.shape_safe_offset);
   Mat lbp = Mat::zeros(img.working_img.rows, img.working_img.cols, CV_32SC1);
   extractLBP(cropped, lbp, config);
   img.exctracted_vector = globalHistogram(lbp, config.hist.grid_size, config.hist.uniform);

}

void LBP::setUp(void *_param) {
   config = *(struct _LBP_config*)_param;
   initUniformTable(config.lbp_params.neighbours);
   optimize_LBP_pls(config);
}



