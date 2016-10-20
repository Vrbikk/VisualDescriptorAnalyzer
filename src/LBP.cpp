//
// Created by vrbik on 27.9.16.
//

#include "include/LBP.h"

LBP::LBP() {

}

LBP::~LBP() {

}

void LBP::localHistogram(int x, int y, int xsize, int ysize, Mat &src, vector<int> &local_histogram) {
    if(config.uniform){
        local_histogram.assign(59, 0);
        for(int local_x = x; local_x < x + xsize; local_x++){
            for(int local_y = y; local_y < y + ysize; local_y++) {
                local_histogram[uniform_table[src.at<byte>(local_x, local_y)]] += 1;
            }
        }

    }else{
        local_histogram.assign(256, 0);
        for(int local_x = x; local_x < x + xsize; local_x++){
            for(int local_y = y; local_y < y + ysize; local_y++) {
                local_histogram[src.at<byte>(local_x, local_y)] += 1;
            }
        }
    }
}

vector<vector<int>> LBP::globalHistogram(Mat &src) {

    vector<vector<int>> global_histogram;

    int frame_width = src.size().width / config.grid_size;
    int frame_height = src.size().height / config.grid_size;
    int x_offset = (src.size().width % config.grid_size) / 2;
    int y_offset = (src.size().height % config.grid_size) / 2;

    for(int x = x_offset; x < config.grid_size * frame_width; x += frame_width){
        for(int y = y_offset; y < config.grid_size * frame_height; y += frame_height){
            vector<int> local_histogram;
            localHistogram(x, y, frame_width, frame_height, src, local_histogram);
            global_histogram.push_back(local_histogram);
        }
    }
    return global_histogram;
}

void LBP::extractLBP(Mat src, Mat &dst) {
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
    img.exctracted_vector = globalHistogram(lbp);
}

void LBP::setUp(void *_param) {
    config = *(struct _LBP_config*)_param;
}



