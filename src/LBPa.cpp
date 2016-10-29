//
// Created by vrbik on 20.10.16.
//

#include "include/LBPa.h"

void LBPa::setUp(void *_param) {
    config = *(struct _LBPa_config*)_param;
}

void LBPa::Process(_image &img) {
    //Extraction phase

    Mat cropped;
    croppedImage(img.working_img, cropped, BORDER_OFFSET);
    Mat lbpa = Mat(img.working_img.cols, img.working_img.rows, CV_8U);
    extract(cropped, lbpa);

    img.exctracted_vector = globalHistogram(lbpa, config.grid_size, config.uniform);
}

void LBPa::extract(Mat &src, Mat &dst) {
    int dst_x, dst_y;
    for(int x = BORDER_OFFSET; x < src.size().height - BORDER_OFFSET; x += config.center_size){
        for(int y = BORDER_OFFSET; y < src.size().width - BORDER_OFFSET; y += config.center_size){

            dst_x = x - BORDER_OFFSET;
            dst_y = y - BORDER_OFFSET;

            byte LBP_code = 0;
            byte center;

            if(config.center_size == 2){
                center = average_maximum_value(x, y, src, config.comparison, BASE_SIZE);
            }else{
                center = src.at<byte>(x, y);
            }

            LBP_code |= (average_maximum_value(x-2, y-2, src, config.comparison, BASE_SIZE) > center) << 7;
            LBP_code |= (average_maximum_value(x-2, y  , src, config.comparison, BASE_SIZE) > center) << 6;
            LBP_code |= (average_maximum_value(x-2, y+2, src, config.comparison, BASE_SIZE) > center) << 5;
            LBP_code |= (average_maximum_value(x  , y+2, src, config.comparison, BASE_SIZE) > center) << 4;
            LBP_code |= (average_maximum_value(x+2, y+2, src, config.comparison, BASE_SIZE) > center) << 3;
            LBP_code |= (average_maximum_value(x+2, y  , src, config.comparison, BASE_SIZE) > center) << 2;
            LBP_code |= (average_maximum_value(x+2, y-2, src, config.comparison, BASE_SIZE) > center) << 1;
            LBP_code |= (average_maximum_value(x  , y-2, src, config.comparison, BASE_SIZE) > center) << 0;

            if(config.center_size == 2){
                for (int x_pos = dst_x; x_pos < (dst_x + BASE_SIZE); x_pos++) {
                    for (int y_pos = dst_y; y_pos < (dst_y + BASE_SIZE); y_pos++) {
                        dst.at<byte>(x_pos, y_pos) = LBP_code;
                    }
                }
            }else{
                dst.at<byte>(dst_x, dst_y) = LBP_code;
            }
        }
    }
}

LBPa::LBPa() {

}

LBPa::~LBPa() {

}
