//
// Created by vrbik on 21.9.16.
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

int get_files(string path, vector<string> &files);
bool load_images(vector<_image> &train, vector<_image> &test);
void file_miner(string path, vector<_image> &images);
void texture_miner(string label_path, string texture_path, vector<_image> &images);
void load_textures(vector<_image> &train, vector<_image> &test, int set);

#endif
