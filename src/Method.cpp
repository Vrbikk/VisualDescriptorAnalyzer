//
// Created by vrbik on 27.9.16.
//

#include "include/Method.h"

Method::Method() {

}

Method::~Method() {
}

void Method::initUniformTable(int neighbours) {
    number_of_all_patterns = 1 << neighbours;
    uniform_table.assign(number_of_all_patterns, 0);

    int transitions;

    for(int i = 0; i < number_of_all_patterns; i++){
        transitions = 0;

        for(int j = 0; j < neighbours -1; j++){
            transitions += bitAtPosition(i, j) != bitAtPosition(i, j + 1);
        }
        transitions += bitAtPosition(i, neighbours - 1) != bitAtPosition(i, 0); //first and last check

        if(transitions <= 2){
            uniform_table[i] = number_of_uniform_patterns++;

        }else{
            uniform_table[i] = UINT32_MAX;
        }
    }

    for(int i = 0; i < number_of_all_patterns; i++) {
        if(uniform_table[i] == UINT32_MAX){
            uniform_table[i] = number_of_uniform_patterns;
        }
    }
}

//false on 0, true on 1
bool Method::bitAtPosition(unsigned int num, unsigned int pos) {
    return ((num & (1 << pos)) != 0);
}

void Method::croppedImage(Mat src, Mat &dst, int crop_size) {
    copyMakeBorder(src, dst, crop_size, crop_size, crop_size, crop_size, BORDER_DEFAULT);
}

void Method::localHistogram(int x, int y, int xsize, int ysize, Mat &src, vector<int> &local_histogram, bool uniform) {
    if(uniform){
        local_histogram.assign(number_of_uniform_patterns + 1, 0);
        for(int local_x = x; local_x < x + xsize; local_x++){
            for(int local_y = y; local_y < y + ysize; local_y++) {
                local_histogram[uniform_table[src.at<int>(local_x, local_y)]] += 1;
            }
        }

    }else{
        local_histogram.assign(number_of_all_patterns, 0);
        for(int local_x = x; local_x < x + xsize; local_x++){
            for(int local_y = y; local_y < y + ysize; local_y++) {
                local_histogram[src.at<int>(local_x, local_y)] += 1;
            }
        }
    }
}

vector<vector<int>> Method::globalHistogram(std::vector<Mat*> src, int grid_size, bool uniform) {

    vector<vector<int>> global_histogram;

    int frame_width = src.at(0)->size().width / grid_size;
    int frame_height = src.at(0)->size().height / grid_size;
    int x_offset = (src.at(0)->size().width % grid_size) / 2;
    int y_offset = (src.at(0)->size().height % grid_size) / 2;



    for(int x = x_offset; x < grid_size * frame_width; x += frame_width){
        for(int y = y_offset; y < grid_size * frame_height; y += frame_height){

            vector<int> full_local_histogram;

            for(int i = 0; i < src.size(); i++){
                vector<int> local_histogram;
                localHistogram(x, y, frame_width, frame_height, *src.at(i), local_histogram, uniform);
                full_local_histogram.insert(full_local_histogram.end(), local_histogram.begin(), local_histogram.end());
            }


            global_histogram.push_back(full_local_histogram);
        }
    }
    return global_histogram;
}

byte Method::average_maximum_value(int x_, int y_, Mat &src, bool comparison, int size) {
    if(comparison) {
        byte maximum = 0;
        for (int x = x_; x < x_ + size; x++) {
            for (int y = y_; y < y_ + size; y++) {
                if (src.at<byte>(x, y) > maximum) {
                    maximum = src.at<byte>(x, y);
                }
            }
        }
        return maximum;
    }else{
        int sum = 0;

        for(int x = x_; x < x_ + size; x++){
            for(int y = y_; y < y_ + size; y++){
                sum += src.at<byte>(x, y);
            }
        }
        byte average = (byte)(sum / (size * size));
        return average;
    }
}

void Method::extractLBPa(Mat &src, Mat &dst, _LBPa_config config) {
    int dst_x, dst_y;
    for(int x = config.lbpa_params.BORDER_OFFSET; x < src.size().height - config.lbpa_params.BORDER_OFFSET; x += config.lbpa_params.center_size){
        for(int y = config.lbpa_params.BORDER_OFFSET; y < src.size().width - config.lbpa_params.BORDER_OFFSET; y += config.lbpa_params.center_size){

            dst_x = x - config.lbpa_params.BORDER_OFFSET;
            dst_y = y - config.lbpa_params.BORDER_OFFSET;

            byte LBP_code = 0;
            byte center;

            if(config.lbpa_params.center_size == 2){
                center = average_maximum_value(x, y, src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE);
            }else{
                center = src.at<byte>(x, y);
            }

            LBP_code |= (average_maximum_value(x-2, y-2, src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE) > center) << 7;
            LBP_code |= (average_maximum_value(x-2, y  , src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE) > center) << 6;
            LBP_code |= (average_maximum_value(x-2, y+2, src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE) > center) << 5;
            LBP_code |= (average_maximum_value(x  , y+2, src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE) > center) << 4;
            LBP_code |= (average_maximum_value(x+2, y+2, src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE) > center) << 3;
            LBP_code |= (average_maximum_value(x+2, y  , src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE) > center) << 2;
            LBP_code |= (average_maximum_value(x+2, y-2, src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE) > center) << 1;
            LBP_code |= (average_maximum_value(x  , y-2, src, config.lbpa_params.comparison, config.lbpa_params.BASE_SIZE) > center) << 0;

            if(config.lbpa_params.center_size == 2){
                for (int x_pos = dst_x; x_pos < (dst_x + config.lbpa_params.BASE_SIZE); x_pos++) {
                    for (int y_pos = dst_y; y_pos < (dst_y + config.lbpa_params.BASE_SIZE); y_pos++) {
                        dst.at<int>(x_pos, y_pos) = LBP_code;
                    }
                }
            }else{
                dst.at<int>(dst_x, dst_y) = LBP_code;
            }
        }
    }
}

void Method::extractLBP(Mat &src, Mat &dst, _LBP_config config) {

    int dst_x, dst_y;
    for(int x = config.lbp_params.range; x < src.size().height - config.lbp_params.range; x++){
        for(int y = config.lbp_params.range; y < src.size().width - config.lbp_params.range; y++){

            dst_x = x - config.lbp_params.range;
            dst_y = y - config.lbp_params.range;

            int LBP_code = 0;

            byte center = src.at<byte>(x, y);

            for(int i = 0; i < config.lbp_params.neighbours ; i++){
                double x_circle = x + optimized_LBP[i][0];
                double y_circle = y + optimized_LBP[i][1];
                LBP_code |= (src.at<byte>((int)round(x_circle), (int)round(y_circle)) > center) << i;
            }

            dst.at<int>(dst_x, dst_y) = LBP_code;
        }
    }
}

void Method::optimize_LBP_pls(_LBP_config config) {
    double angle = 0;
    double step = (M_PI * 2) / config.lbp_params.neighbours;
    optimized_LBP.resize(config.lbp_params.neighbours);
    for(int i = 0; i < config.lbp_params.neighbours ; i++){
        optimized_LBP[i].resize(2);
        optimized_LBP[i][0] = config.lbp_params.range * cos(angle);
        optimized_LBP[i][1] =  config.lbp_params.range * sin(angle);
        angle += step;
    }
}

