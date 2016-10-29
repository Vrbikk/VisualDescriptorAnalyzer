//
// Created by vrbik on 27.9.16.
//

#include "include/Method.h"

Method::Method() {
    initUniformTable();
}

Method::~Method() {

}

void Method::initUniformTable() {
    int num_uniforms = 0;
    int neighbours = 8;
    int combinations = 1 << neighbours;

    uniform_table = vector<int>(combinations);

    int transitions;
    for(int i = 0; i < combinations; i++){
        transitions = 0;

        for(int j = 0; j < neighbours -1; j++){
            transitions += bitAtPosition(i, j) != bitAtPosition(i, j + 1);
        }
        //extra transition test for first and last element
        transitions += bitAtPosition(i, neighbours - 1) != bitAtPosition(i, 0);

        if(transitions <= 2){
            uniform_table[i] = num_uniforms++;
        }else{
            uniform_table[i] = 58; //used for trash pixels, also there is only 58 uniform numbers from 0 to 255
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
