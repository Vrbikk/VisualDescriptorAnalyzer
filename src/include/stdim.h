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
        return string("grid_size:" + to_string(grid_size) + " uniform:" + to_string(uniform));
    }
};

struct _LBP_params{
    int range;
    int neighbours;

    string print(){
        return string("range:" + to_string(range) + " neighbours:" + to_string(neighbours));
    }
};

struct _LBPa_params{
    int BORDER_OFFSET = 2;
    int BASE_SIZE = 2;
    int center_size;
    bool comparison;
    string print(){
        return string("center_size:" + to_string(center_size) + " comparison:" + to_string(comparison));
    }

};

struct _LBP_config{
    _LBP_params lbp_params;
    _hist hist;

    string print(){
        return string("LBP config - " + lbp_params.print() + " " + hist.print());
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
