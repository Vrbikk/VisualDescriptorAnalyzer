//
// Created by Antonín Vrba on 21.9.2016
//

#ifndef PLAYGROUND_LOADER_H
#define PLAYGROUND_LOADER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <dirent.h>
#include "tools.h"
#include "Logger.h"
#include "Configuration.h"

using namespace std;
using namespace cv;

// set folder items into &files vector
int get_files(string path, vector<string> &files);
// function for loading all files from DB folder structure
void file_miner(string path, vector<_image> &images);
// main loading function loads training and testing data
bool load_images(vector<_image> &train, vector<_image> &test);

#endif
