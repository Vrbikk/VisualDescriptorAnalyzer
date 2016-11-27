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

struct _hist{
    int grid_size;
    bool uniform;

    string print(){
        return string("HIST config - grid_size:" + to_string(grid_size) + " uniform:" + to_string(uniform));
    }
};

struct _LBP_params{
    int range;
    int neighbours;
    int neighbour_shape;
    int shape_evaluation = 1;
    int shape_safe_offset = 2;

    string print(){
        return string("LBP config - range:" + to_string(range) + " neighbours:" + to_string(neighbours) + " neighbour_shape:" + to_string(neighbour_shape)
                      + " shape_evaluation:" + to_string(shape_evaluation));
    }
};

struct _LBPa_params{
    int center_size;
    int neighbour_shape;
    int range;
    int shape_evaluation = 1;
    int shape_safe_offset = 2;
    string print(){
        return string("LBPa config - center_size:" + to_string(center_size) + " neighbour_shape:" + to_string(neighbour_shape) + " range:" +
                              to_string(range) + " shape_evaluation:" + to_string(shape_evaluation));
    }
};

struct _LBP_config{
    _LBP_params lbp_params;
    _hist hist;

    string print(){
        return string(lbp_params.print() + " " + hist.print());
    }
};

struct _LBPa_config{
    _LBPa_params lbpa_params;
    _hist hist;

    string print(){
        return string("LBPa config - " + lbpa_params.print() + " " + hist.print());
    }
};

#endif //PLAYGROUND_STRUCTS_H
