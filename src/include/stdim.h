//
// Created by vrbik on 1.10.16.
//

#ifndef PLAYGROUND_STRUCTS_H
#define PLAYGROUND_STRUCTS_H

#include <string>
#include <opencv2/opencv.hpp>
#include "Logger.h"

using namespace std;
using namespace cv;

struct _image{
    int id;
    string file_name;
    string path;
    Mat original_img;
    Mat working_img;
    vector<vector<int>> exctracted_vector;
};

struct _LBP_config{
    int grid_size;
    bool uniform;

    string print(){
        return string("LBP config - grid_size:" + to_string(grid_size) + " uniform:" + to_string(uniform));
    }
};

struct _LBPa_config{
    int grid_size;
    bool uniform;
    int center_size;
    bool comparison;  // true - max, false - avg

    string print(){
        return string("LBPa config - grid_size:" + to_string(grid_size) + " uniform:" + to_string(uniform) + " center_size:" +
        to_string(center_size) + " comparison:" + to_string(comparison));
    }

};

#endif //PLAYGROUND_STRUCTS_H
