//
// Created by Antonín Vrba on 3.10.2016
//

#ifndef PLAYGROUND_PREPROCESSING_H
#define PLAYGROUND_PREPROCESSING_H

#include "stdim.h"
#include "Configuration.h"

// two basic preprocessing functions which are here only for testing purposes
void gaussian_blur_preprocessing(Mat &a);
void equalize_hist_preprocessing(Mat &a);

// applying filters to working image
void iterate_preprocess(vector<_image> &images, string name_of_set);

// main preprocess function
void preprocess(vector<_image> &train, vector<_image> &test);

#endif
