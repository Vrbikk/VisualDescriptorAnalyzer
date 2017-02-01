//
// Created by vrbik on 23.1.17.
//

#ifndef PLAYGROUND_GABOR_H
#define PLAYGROUND_GABOR_H

#include "stdim.h"
#include "Configuration.h"

void assign_points(_image &img, int count, int border);
void gabor(vector<_image> &train, vector<_image> &test);

#endif //PLAYGROUND_GABOR_H

