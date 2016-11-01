//
// Created by vrbik on 1.10.16.
//

#ifndef PLAYGROUND_CLASSIFICATION_H
#define PLAYGROUND_CLASSIFICATION_H

#include "Configuration.h"
#include "stdim.h"
#include <thread>
#include <mutex>

using namespace std;

void classificate(vector<_image> &train, vector<_image> &test);
double get_histogram_distance(vector<vector<int>> a, vector<vector<int>> b, __comparison_method comparison_method);
double get_euclidean_distance(vector<vector<int>> a, vector<vector<int>> b);
double get_intersection_distance(vector<vector<int>> a, vector<vector<int>> b);
void semi_classificate(vector<_image> &train, vector<_image> &test, vector<_image*> &candidates, int from, int to);
void progression();
void result_calculation(vector<_image> &test, vector<_image*> &candidates);


#endif //PLAYGROUND_CLASSIFICATION_H
