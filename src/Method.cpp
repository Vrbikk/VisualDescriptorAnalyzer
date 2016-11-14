//
// Created by vrbik on 27.9.16.
//

#include "include/Method.h"
#include "include/GUI.h"

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

    //LOGGER->Info("number of all patterns:" + to_string(number_of_all_patterns));
    //LOGGER->Info("number of uniform patterns:" + to_string(number_of_uniform_patterns));
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

vector<vector<int>> Method::globalHistogram(Mat &src, int grid_size, bool uniform) {

    vector<vector<int>> global_histogram;

    int frame_width = src.size().width / grid_size;
    int frame_height = src.size().height / grid_size;
    int x_offset = (src.size().width % grid_size) / 2;
    int y_offset = (src.size().height % grid_size) / 2;


    for(int x = x_offset; x < grid_size * frame_width; x += frame_width){
        for(int y = y_offset; y < grid_size * frame_height; y += frame_height){

            vector<int> local_histogram;
            localHistogram(x, y, frame_width, frame_height, src, local_histogram, uniform);
            global_histogram.push_back(local_histogram);
        }
    }
    return global_histogram;
}

void Method::extractLBPa(Mat &src, Mat &dst, _LBPa_config config) {

    int offset = config.lbpa_params.range + config.lbpa_params.shape_safe_offset;

    int dst_x, dst_y;
    for(int x = offset; x < src.rows - offset; x += config.lbpa_params.center_size){
        for(int y = offset; y < src.cols - offset; y += config.lbpa_params.center_size){

            dst_x = x - offset;
            dst_y = y - offset;

            byte LBP_code = 0;
            byte center;

            if(config.lbpa_params.center_size == 1){
                center = src.at<byte>(x, y);
            }else{
                center = getShapeValue(src, x, y, config.lbpa_params.center_size, config.lbpa_params.shape_evaluation);
            }

            byte k = config.lbpa_params.range;

            LBP_code |= (getShapeValue(src, x-k, y-k, config.lbpa_params.neighbour_shape, config.lbpa_params.shape_evaluation) > center) << 7;
            LBP_code |= (getShapeValue(src, x-k, y  , config.lbpa_params.neighbour_shape, config.lbpa_params.shape_evaluation) > center) << 6;
            LBP_code |= (getShapeValue(src, x-k, y+k, config.lbpa_params.neighbour_shape, config.lbpa_params.shape_evaluation) > center) << 5;
            LBP_code |= (getShapeValue(src, x  , y+k, config.lbpa_params.neighbour_shape, config.lbpa_params.shape_evaluation) > center) << 4;
            LBP_code |= (getShapeValue(src, x+k, y+k, config.lbpa_params.neighbour_shape, config.lbpa_params.shape_evaluation) > center) << 3;
            LBP_code |= (getShapeValue(src, x+k, y  , config.lbpa_params.neighbour_shape, config.lbpa_params.shape_evaluation) > center) << 2;
            LBP_code |= (getShapeValue(src, x+k, y-k, config.lbpa_params.neighbour_shape, config.lbpa_params.shape_evaluation) > center) << 1;
            LBP_code |= (getShapeValue(src, x  , y-k, config.lbpa_params.neighbour_shape, config.lbpa_params.shape_evaluation) > center) << 0;

            if(config.lbpa_params.center_size == 1){
                dst.at<int>(dst_x, dst_y) = LBP_code;
            }else{
                for (int x_pos = dst_x; x_pos < (dst_x + config.lbpa_params.center_size) && x_pos < dst.rows; x_pos++) {
                    for (int y_pos = dst_y; y_pos < (dst_y + config.lbpa_params.center_size) && y_pos < dst.cols; y_pos++) {
                        dst.at<int>(x_pos, y_pos) = LBP_code;
                    }
                }
            }
        }
    }
}

void Method::extractLBP(Mat &src, Mat &dst, _LBP_config config) {

    int offset = config.lbp_params.range + config.lbp_params.shape_safe_offset;

    int dst_x, dst_y;

    for(int x = offset; x < src.rows - offset; x++){
        for(int y = offset; y < src.cols - offset; y++){
            dst_x = x - offset;
            dst_y = y - offset;

            int LBP_code = 0;
            byte center = src.at<byte>(x, y); //selecting center point

            for(int i = 0; i < config.lbp_params.neighbours ; i++){  // iterating circle neighbours

                int x_circle = (int) round(x + optimized_LBP[i][0]);
                int y_circle = (int) round(y + optimized_LBP[i][1]);

                byte neighbour_value = getShapeValue(src, x_circle, y_circle, config.lbp_params.shape, config.lbp_params.shape_evaluation);

                LBP_code |= (neighbour_value > center) << i; // shifting LBP buffer
            }

            dst.at<int>(dst_x, dst_y) = LBP_code; // writting LBP value to LBP Mat
        }
    }
}

void Method::optimize_LBP_pls(_LBP_config config) {

    // precalculating circular positions

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

/*
 * shape_evaluation 0 = Min, 1 = Avg, 2 = Max
 */

byte Method::getShapeValue(Mat &src, int x, int y, int type, int shape_evaluation) {

    byte arr[10];
    byte count;

    switch(type){

        // X
        case 1: {

            /*(src.at<byte>(x,y)) = 0;
             show_image(src);*/

             return src.at<byte>(x, y);
        }

        // XO
        // OO
        case 2:{
            count = 4;

            arr[0] = (src.at<byte>(x,y));
            arr[1] = (src.at<byte>(x,y + 1));
            arr[2] = (src.at<byte>(x + 1,y + 1));
            arr[3] = (src.at<byte>(x + 1,y));

            /*src.at<byte>(x,y) = 0;
            src.at<byte>(x,y + 1) = 0;
            src.at<byte>(x + 1,y + 1) = 0;
            src.at<byte>(x + 1,y) = 0;

            show__image(src);*/
            break;
        }

        // XOO
        // OOO
        // OOO
        case 3:{
            count = 9;
            byte index = 0;

            for(int x_ = x; x_ < x + 3; x_++){
                for(int y_ = y; y_ < y + 3; y_++){
                    arr[index++] = src.at<byte>(x_, y_);
                    //src.at<byte>(x_, y_) = 0;
                }
            }

            //show_image(src);
            break;
        }

        // OXO
        case 4:{
            count = 3;

            arr[0] = (src.at<byte>(x,y));
            arr[1] = (src.at<byte>(x,y + 1));
            arr[2] = (src.at<byte>(x,y - 1));

            /*(src.at<byte>(x,y)) = 0;
            (src.at<byte>(x,y + 1)) = 0;
            (src.at<byte>(x,y - 1)) = 0;
            show_image(src);*/

            break;
        }

            //  O
            // OXO
            //  O
        case 5: {

            count = 5;

            arr[0] = (src.at<byte>(x, y));
            arr[1] = (src.at<byte>(x - 1, y));
            arr[2] = (src.at<byte>(x + 1, y));
            arr[3] = (src.at<byte>(x, y - 1));
            arr[4] = (src.at<byte>(x, y + 1));

            /*(src.at<byte>(x,y)) = 0;
            (src.at<byte>(x - 1,y)) = 0;
            (src.at<byte>(x + 1,y)) = 0;
            (src.at<byte>(x,y - 1)) = 0;
            (src.at<byte>(x,y + 1)) = 0;
             show_two_images(src, src);*/
            break;
        }


        default:{
            LOGGER->Error("default switch branch in getShapeValue - should not happend");
            break;
        }
    }

    switch(shape_evaluation){
        case 0:{
            byte min = 255;

            for(int i = 0; i < count; i++){
                if(arr[i] < min){
                    min = arr[i];
                }
            }

            return min;
        }

        case 1:{
            int average = 0;

            for(int i = 0; i < count; i++){
                average += arr[i];
            }

            return (byte)(average / count);
        }

        case 2:{
            byte max = 0;

            for(int i = 0; i < count; i++){
                if(arr[i] > max){
                    max = arr[i];
                }
            }

            return max;
        }

        default:{
            LOGGER->Error("Default switch branch in minAvgmax - should not happend");
            break;
        }
    }
}

