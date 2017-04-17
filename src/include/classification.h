//
// Created by vrbik on 1.10.16.
//

#ifndef PLAYGROUND_CLASSIFICATION_H
#define PLAYGROUND_CLASSIFICATION_H

#include "Configuration.h"
#include "stdim.h"
#include <thread>
#include <mutex>

// This function divides classification work between threads
void classificate(vector<_image> &train, vector<_image> &test);

// Return distance between two nearest histograms, Intersection is default
double get_gabor_intersection_distance(vector<int> a, vector<int> b);
double get_gabor_euclidean_distance(vector<int> a, vector<int> b);

// Return index of nearest point from vector &points
int get_nearest_gabor_histogram_index(Point &a, vector<Point> &points, double &local_distance);

// Return distance based on gabor points between two images
double get_gabor_distance(_image &a, _image &b);

// Return distance based on symetrical grid of histograms
double get_histogram_distance(vector<vector<int>> a, vector<vector<int>> b, __comparison_method comparison_method);

// Main comparing methods for symetrical grid
double get_euclidean_distance(vector<vector<int>> a, vector<vector<int>> b);
double get_intersection_distance(vector<vector<int>> a, vector<vector<int>> b);
double get_bhattacharyya_distance(vector<vector<int>> a, vector<vector<int>> b); //experimental

// Function for every thread that partialy classificate dataset
void semi_classificate(vector<_image> &train, vector<_image> &test, vector<_image*> &candidates, int from, int to);

// Console progress
void progression();

// Final check for recognition rate
void result_calculation(vector<_image> &test, vector<_image*> &candidates);

#endif
