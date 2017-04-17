//
// Created by vrbik on 27.9.16.
//

#include "include/LBP.h"
#include "include/Configuration.h"

LBP::LBP() {

}

LBP::~LBP() {

}

void LBP::Process(_image &img) {

   //Extraction phase
   Mat cropped;
   borderedImage(img.working_img, cropped, config.lbp_params.range + config.lbp_params.shape_safe_offset);
   Mat lbp = Mat::zeros(img.working_img.rows, img.working_img.cols, CV_32SC1);
   extractLBP(cropped, lbp, config);

   //show_image(img.original_img);
   //Making of histogram

   switch(CONFIG->getGaborSetting()){
      case 0:{
         img.exctracted_vector = globalHistogram(lbp, config.hist.grid_size, config.hist.uniform);
         break;
      }
      case 1:{
         img.gabor_exctracted_vector = extractGaborPointsHistograms(lbp, img.points, CONFIG->getGaborHistogramSize(), config.hist.uniform);
         break;
      }
      case 2:{
         img.exctracted_vector = globalHistogram(lbp, config.hist.grid_size, config.hist.uniform);
         img.gabor_exctracted_vector = extractGaborPointsHistograms(lbp, img.points, CONFIG->getGaborHistogramSize(), config.hist.uniform);
         break;
      }
      default:{
         LOGGER->Error("default branch called in LBP");
         break;
      }
   }
}

void LBP::setUp(void *_param) {
   config = *(struct _LBP_config*)_param;
   initUniformTable(config.lbp_params.neighbours);
   optimize_LBP(config);

   LOGGER->Info(config.lbp_params.print());
   LOGGER->Info(config.hist.print());
   CONFIG->printGaborConfiguration();
}



