//
// Created by vrbik on 27.9.16.
//

#include "include/LBP.h"

LBP::LBP() {

}

LBP::~LBP() {

}

void LBP::extractLBP(Mat &src, Mat &dst) {
    int dst_x, dst_y;
    for(int x = BORDER_OFFSET; x < src.size().height - BORDER_OFFSET; x++){
        for(int y = BORDER_OFFSET; y < src.size().width - BORDER_OFFSET; y++){

            dst_x = x - BORDER_OFFSET;
            dst_y = y - BORDER_OFFSET;
            byte LBP_code = 0;
            byte center = src.at<byte>(x, y);

            LBP_code |= (src.at<byte>(x-1, y-1) > center) << 7;
            LBP_code |= (src.at<byte>(x-1, y) > center) << 6;
            LBP_code |= (src.at<byte>(x-1, y+1) > center) << 5;
            LBP_code |= (src.at<byte>(x, y+1) > center) << 4;
            LBP_code |= (src.at<byte>(x+1, y+1) > center) << 3;
            LBP_code |= (src.at<byte>(x+1, y) > center) << 2;
            LBP_code |= (src.at<byte>(x+1, y-1) > center) << 1;
            LBP_code |= (src.at<byte>(x, y-1) > center) << 0;

            dst.at<byte>(dst_x, dst_y) = LBP_code;
        }
    }
}

void LBP::Process(_image &img) {

    //Extraction phase
    Mat cropped;
    croppedImage(img.working_img, cropped, BORDER_OFFSET);
    Mat lbp = Mat(img.working_img.cols, img.working_img.rows, CV_8U);
    extractLBP(cropped, lbp);

    //Making of histogram
    img.exctracted_vector = globalHistogram(lbp, config.grid_size, config.uniform);
}

void LBP::setUp(void *_param) {
    config = *(struct _LBP_config*)_param;
}



