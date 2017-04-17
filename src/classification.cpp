//
// Created by vrbik on 1.10.16.
//

#include "include/classification.h"

double get_euclidean_distance(vector<vector<int>> a, vector<vector<int>> b) {
    double distance = 0;

    int outer_size = a.size();
    int inner_size = a[0].size();

    for(int i = 0; i < outer_size; i++){
        double pom = 0;
        for(int j = 0; j < inner_size; j++){
            pom += (a[i][j] - b[i][j]) * (a[i][j] - b[i][j]);
        }
        distance += sqrt(pom);
    }
    return distance;
}

double get_intersection_distance(vector<vector<int>> a, vector<vector<int>> b) {

    double distance = 0;

    int outer_size = a.size();
    int inner_size = a[0].size();

    for(int i = 0; i < outer_size; i++){

        double sum = 0;
        double sub_distance = 0;

        for(int j = 0; j < inner_size; j++){
            /*distance_a += a[i][j];
            if(a[i][j] < b[i][j]){  //min
                distance += a[i][j];
            }else{
                distance += b[i][j];
            }*/
            sum += a[i][j];
            if(a[i][j] < b[i][j]){  //min
                sub_distance += a[i][j];
            }else{
                sub_distance += b[i][j];
            }
        }
        distance += (sum - sub_distance);
    }
    //return distance_a - distance;
    return distance;
}

double get_histogram_distance(vector<vector<int>> a, vector<vector<int>> b, __comparison_method comparison_method) {


    switch(comparison_method){
        case __EUCLIDEAN:{
            return get_euclidean_distance(a, b);
        }
        case __INTERSECTION:{
            return get_intersection_distance(a, b);
        }
        case __BHATTACHARYYA:{
            return get_bhattacharyya_distance(a, b);
        }
        default:{
            LOGGER->Error("default switch branch called in get_histogram_distance function - this should never happend");
            return get_euclidean_distance(a, b);
        }
    }
}

double get_bhattacharyya_distance(vector<vector<int>> a, vector<vector<int>> b){

    double coefficient = 0;
    double distance = 0;

    double a_avg = 0;
    double b_avg = 0;

    int outer_size = a.size();
    int inner_size = a[0].size();


    for(int i = 0; i < outer_size; i++){
        for(int j = 0; j < inner_size; j++){
            a_avg += a[i][j];
            b_avg += b[i][j];
            coefficient += sqrt(a[i][j] * b[i][j]);
        }
    }

    int all = outer_size * inner_size;

    a_avg = a_avg / all;
    b_avg = b_avg / all;

    long double s = std::sqrt(static_cast<long double>(a_avg*b_avg*all*all)); //force long double to sqrt, there is really big number
    distance = sqrt((1 - (1.0/s) * coefficient));
    return distance;
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
            double distance;

                switch(CONFIG->getGaborSetting()){
                    case 0:{
                        distance = get_histogram_distance(test[i].exctracted_vector, train[j].exctracted_vector, CONFIG->getComparisonMethod());
                        break;
                    }
                    case 1:{
                        distance = get_gabor_distance(test[i], train[j]);
                        break;
                    }
                    case 2:{
                        double a = get_gabor_distance(test[i], train[j]);
                        double b = get_histogram_distance(test[i].exctracted_vector, train[j].exctracted_vector, CONFIG->getComparisonMethod());
                        distance = a + b;
                        break;
                    }
                    default:{
                        LOGGER->Error("default branch called in semi_classificate method");
                        break;
                    }

                }

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
            if(CONFIG->getResultMode()) {
                result_mode_show_two_images(test[i], *candidates[i], true);
                waitKey(0);
            }
        }else{
            if(CONFIG->getResultMode()) {
                result_mode_show_two_images(test[i], *candidates[i], false);
                waitKey(0);
            }
        }
    }

    double accuracy = hits/(test.size() * 1.0);
    LOGGER->Info("↑ accuracy: <" + to_string(accuracy) + "> ↑\n");
}

void classificate(vector<_image> &train, vector<_image> &test) {
    if(!CONFIG->getJobMode()) {
        LOGGER->Info("Classification started");
    }

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

    // waiting for threads to finish their work
    for_each(threads.begin(), threads.end(), [](thread &t){t.join();});

    result_calculation(test, candidates);
}

double get_gabor_distance(_image &a, _image &b) {
    double distance = 0;
    for(int i = 0; i < a.points.size(); i++){
        double local_distance = 1;
        int index = get_nearest_gabor_histogram_index(a.points[i], b.points, local_distance);
        distance += local_distance * get_gabor_intersection_distance(a.gabor_exctracted_vector[i], b.gabor_exctracted_vector[index]);
    }

    return distance;
}

int get_nearest_gabor_histogram_index(Point &a, vector<Point> &points, double &local_distance) {
    double min_distance = DBL_MAX;
    int index = 0;
    for(int i = 0; i < points.size(); i++){
        double distance = sqrt((a.x - points[i].x)*(a.x - points[i].x) + (a.y - points[i].y)*(a.y - points[i].y));
        if(distance < min_distance){
            min_distance = distance;
            index = i;
        }
    }
    //local_distance += min_distance / 10; //experimental
    return index;
}

double get_gabor_intersection_distance(vector<int> a, vector<int> b){
    double distance = 0;
        double sum = 0;
        double sub_distance = 0;
        for(int j = 0; j < a.size(); j++){
            sum += a[j];
            if(a[j] < b[j]){  //min
                sub_distance += a[j];
            }else{
                sub_distance += b[j];
            }
        }
    return (sum - sub_distance);
}

double get_gabor_euclidean_distance(vector<int> a, vector<int> b) {

    double distance = 0;
    for(int j = 0; j < a.size(); j++){
        distance += (a[j] - b[j]) * (a[j] - b[j]);
    }

    return sqrt(distance);
}

