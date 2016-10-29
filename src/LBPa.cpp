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
    extractLBPa(cropped, lbpa);
    img.exctracted_vector = globalHistogram(lbpa, config.grid_size, config.uniform);
}

void LBPa::extractLBPa(Mat &src, Mat &dst) {
    int dst_x, dst_y;
    for(int x = BORDER_OFFSET; x < src.size().height - BORDER_OFFSET; x += BASE_SIZE){
        for(int y = BORDER_OFFSET; y < src.size().width - BORDER_OFFSET; y += BASE_SIZE){

            dst_x = x - BORDER_OFFSET;
            dst_y = y - BORDER_OFFSET;

            byte LBP_code = 0;
            byte center;

            center = average_maximum_value(x, y, src);

            LBP_code |= (average_maximum_value(x-2, y-2, src) > center) << 7;
            LBP_code |= (average_maximum_value(x-2, y  , src) > center) << 6;
            LBP_code |= (average_maximum_value(x-2, y+2, src) > center) << 5;
            LBP_code |= (average_maximum_value(x  , y+2, src) > center) << 4;
            LBP_code |= (average_maximum_value(x+2, y+2, src) > center) << 3;
            LBP_code |= (average_maximum_value(x+2, y  , src) > center) << 2;
            LBP_code |= (average_maximum_value(x+2, y-2, src) > center) << 1;
            LBP_code |= (average_maximum_value(x  , y-2, src) > center) << 0;

            for (int x_pos = dst_x; x_pos < (dst_x + BASE_SIZE); x_pos++) {
                for (int y_pos = dst_y; y_pos < (dst_y + BASE_SIZE); y_pos++) {
                    dst.at<byte>(x_pos, y_pos) = LBP_code;
                }
            }
        }
    }
}

byte LBPa::average_maximum_value(int x_, int y_, Mat &src) {
    if(config.comparison) {
        byte maximum = 0;
        for (int x = x_; x < x_ + BASE_SIZE; x++) {
            for (int y = y_; y < y_ + BASE_SIZE; y++) {
                if (src.at<byte>(x, y) > maximum) {
                    maximum = src.at<byte>(x, y);
                }
            }
        }
        return maximum;
    }else{
        int sum = 0;

        for(int x = x_; x < x_ + BASE_SIZE; x++){
            for(int y = y_; y < y_ + BASE_SIZE; y++){
                sum += src.at<byte>(x, y);
            }
        }
        byte average = (byte)(sum / (BASE_SIZE * BASE_SIZE));
        return average;
    }
}
