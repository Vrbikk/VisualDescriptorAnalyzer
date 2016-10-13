//
// Created by vrbik on 1.10.16.
//

#include "include/classification.h"

double get_euclidean_distance(vector<vector<int>> a, vector<vector<int>> b) {
    double distance = 0;

    int outer_size = a.size();
    int inner_size = a[0].size();

    for(int i = 0; i < outer_size; i++){
        for(int j = 0; j < inner_size; j++){
            distance += (a[i][j] - b[i][j]) * (a[i][j] - b[i][j]);
        }
    }

    return sqrt(distance);
}

double get_intersection_distance(vector<vector<int>> a, vector<vector<int>> b) {

    double distance = 0;
    double distance_a = 0;

    int outer_size = a.size();
    int inner_size = a[0].size();

    for(int i = 0; i < outer_size; i++){
        for(int j = 0; j < inner_size; j++){
            distance_a += a[i][j];

            if(a[i][j] < b[i][j]){
                distance += a[i][j];
            }else{
                distance += b[i][j];
            }
        }
    }

    return distance_a - distance;
}

double get_histogram_distance(vector<vector<int>> a, vector<vector<int>> b, __comparison_method comparison_method) {
    switch(comparison_method){

        case __EUCLIDEAN:{
            return get_euclidean_distance(a, b);
        }
        case __INTERSECTION:{
            return get_intersection_distance(a, b);
        }
        default:{
            LOGGER->Error("default switch branch called in get_histogram_distance function - this should never happend");
            return get_euclidean_distance(a, b);
        }
    }
}

mutex mu;
int progress;
int test_size;

void progression(){
    lock_guard<mutex> lock(mu);
    draw_progress_to_console("Classification",  progress++ + 1, test_size);
}

void semi_classificate(vector<_image> &train, vector<_image> &test, vector<_image*> &candidates, int from, int to){

    for(int i = from; i < to; i++){

        double min_distance = DBL_MAX;
        _image *nearest_image = NULL;

        for(int j = 0; j < train.size(); j++){
            double distance = get_histogram_distance(
                    test[i].exctracted_vector, train[j].exctracted_vector, CONFIG->getComparisonMethod());

            if(distance < min_distance){
                min_distance = distance;
                nearest_image = &train[j];
            }
        }
        progression();
        candidates[i] = nearest_image;
    }
}

void result_calculation(vector<_image> &test, vector<_image*> &candidates) {
    int hits = 0;
    for(int i = 0; i < test.size(); i++){
        if(test[i].id == (*candidates[i]).id){
            hits++;
            if(CONFIG->getHoratioCaineMode()) {
                csi_show_two_images(test[i], *candidates[i], true);
                waitKey(0);
            }
        }else{
            if(CONFIG->getHoratioCaineMode()) {
                csi_show_two_images(test[i], *candidates[i], false);
                waitKey(0);
            }
        }
    }

    double accuracy = hits/(test.size() * 1.0);
    LOGGER->Info("Accuracy: <" + to_string(accuracy) + ">");
}

void classificate(vector<_image> &train, vector<_image> &test) {

    LOGGER->Info("Classification started");

    test_size = (int)test.size();
    progress = 0;
    vector<_image*> candidates;
    candidates.resize((unsigned long)test_size);

    int number_of_threads = CONFIG->getClassificationThreads();

    if(number_of_threads > test_size){
        LOGGER->Error("number of threads is bigger than number of test images");
        number_of_threads = test_size;
    }

    vector<thread> threads((unsigned long)number_of_threads);

    int thread_data_portion = (int)test.size() / number_of_threads;

    for(int i = 0; i < number_of_threads; i++) {
        int position = i * thread_data_portion;
        int overload = 0;

        if (i == (number_of_threads - 1)) {
            overload = (int)test.size() % number_of_threads;
        }

        threads.at(i) = thread(semi_classificate, ref(train), ref(test), ref(candidates),
                               position, position + thread_data_portion + overload);
    }

    for_each(threads.begin(), threads.end(), [](thread &t){t.join();});

    result_calculation(test, candidates);
}


