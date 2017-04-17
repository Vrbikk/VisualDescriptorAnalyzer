//
// Created by Antonín Vrba on 1.10.2016
//

#ifndef PLAYGROUND_STRUCTS_H
#define PLAYGROUND_STRUCTS_H

#include <string>
#include <opencv2/opencv.hpp>
#include "Logger.h"

using namespace std;
using namespace cv;

/*
 * Main data structure
 */
struct _image{
    // id for every person
    int id;

    // variables that can be used for tracking invalid picture in DB
    string file_name;
    string path;

    // this image is never modified once is loaded
    Mat original_img;

    // to this image are saved all preprocessing features and is used as source for everything
    Mat working_img;

    // vector of vector for storing grid histograms
    vector<vector<int>> exctracted_vector;

    // vector of individualy positioned histograms
    vector<vector<int>> gabor_exctracted_vector;

    // vector of gabor points
    vector<Point> points;

    // this image contains filtered image after applying gabor filters
    Mat gabor_filter;

    // debugging image for gabors
    Mat points_from_gabor;
};

/*
 * universal histogram grid configuration
 */
struct _hist{
    int grid_size;
    bool uniform;

    string print(){
        return string("HIST config - grid_size:" + to_string(grid_size) + " uniform:" + to_string(uniform));
    }
};

// S-LBP, LBP params
struct _LBP_params{
    int range;
    int neighbours;
    int neighbour_shape;
    int shape_evaluation = 1;
    int shape_safe_offset = 2;

    string print(){
        return string("S-LBP config - range:" + to_string(range) + " neighbours:" + to_string(neighbours) + " neighbour_shape:" +
                              to_string(neighbour_shape));
    }
};

// E-LBP params
struct _LBPa_params {
    int center_size;
    int neighbour_shape;
    int range;
    int shape_evaluation = 1;
    int shape_safe_offset = 2;

    string print() {
        return string("E-LBP config - center_size:" + to_string(center_size) + " neighbour_shape:" +
                      to_string(neighbour_shape) + " range:" +
                      to_string(range));
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
        return string(lbpa_params.print() + " " + hist.print());
    }
};

// gabor settings
struct _Gabor_config{
    int gabor_setting;
    int gabor_points;
    int gabor_histogram_size;
    int gabor_border_size;
};

#endif
