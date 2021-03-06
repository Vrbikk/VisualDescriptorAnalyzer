//
// Created by Antonín Vrba on 27.9.2016
//

#ifndef PLAYGROUND_EXTRACTION_H
#define PLAYGROUND_EXTRACTION_H

#include "stdim.h"
#include "Configuration.h"

void iterate_extract(Method &selectedMethod, vector<_image> &images, string name_of_set);
void extract(vector<_image> &train, vector<_image> &test);

#endif
