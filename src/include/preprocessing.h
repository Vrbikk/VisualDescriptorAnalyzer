//
// Created by vrbik on 3.10.16.
//

#ifndef PLAYGROUND_PREPROCESSING_H
#define PLAYGROUND_PREPROCESSING_H

#include "stdim.h"
#include "Configuration.h"

void gaussian_blur_preprocessing(Mat &a);
void equalize_hist_preprocessing(Mat &a);
void preprocess(vector<_image> &train, vector<_image> &test);
void iterate_preprocess(vector<_image> &images, string name_of_set);

#endif
