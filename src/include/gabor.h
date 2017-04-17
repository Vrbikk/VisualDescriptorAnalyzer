//
// Created by Antonín Vrba on 23.1.2017
//

#ifndef PLAYGROUND_GABOR_H
#define PLAYGROUND_GABOR_H

#include "stdim.h"
#include "Configuration.h"

// searching for points in image
void assign_points(_image &img, int count, int border);

// iterates over train and test data and for every image are calculated face specific gabor points
void gabor(vector<_image> &train, vector<_image> &test);

#endif

