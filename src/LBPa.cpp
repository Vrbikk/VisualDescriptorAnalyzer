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

    //show_image(img.original_img);
    //show_image(lbpa);

    switch(CONFIG->getGaborSetting()){
        case 0:{
            img.exctracted_vector = globalHistogram(lbpa, config.hist.grid_size, config.hist.uniform);
            break;
        }
        case 1:{
            img.gabor_exctracted_vector = extractGaborPointsHistograms(lbpa, img.points, CONFIG->getGaborHistogramSize(), config.hist.uniform);
            break;
        }
        case 2:{
            img.exctracted_vector = globalHistogram(lbpa, config.hist.grid_size, config.hist.uniform);
            img.gabor_exctracted_vector = extractGaborPointsHistograms(lbpa, img.points, CONFIG->getGaborHistogramSize(), config.hist.uniform);
            break;
        }
        default:{
            LOGGER->Error("default branch called in LBPa");
            break;
        }
    }
}

void LBPa::setUp(void *_param) {
    config = *(struct _LBPa_config*)_param;
    initUniformTable(8);

    LOGGER->Info(config.lbpa_params.print());
    LOGGER->Info(config.hist.print());
    CONFIG->printGaborConfiguration();
}

LBPa::LBPa() {

}

LBPa::~LBPa() {

}
