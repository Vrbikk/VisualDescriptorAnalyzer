
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

/**
 * Progression status
 */

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
            distance = get_histogram_distance(test[i].exctracted_vector, train[j].exctracted_vector, CONFIG->getComparisonMethod());
            if(distance < min_distance){
                min_distance = distance;
                nearest_image = &train[j];
            }
        }
        progression();
        candidates[i] = nearest_image;
    }
}

double result_calculation(vector<_image> &test, vector<_image *> &candidates) {
    int hits = 0;
    for(int i = 0; i < test.size(); i++){
        if(test[i].id == (*candidates[i]).id && !test[i].texture_class.compare((*candidates[i]).texture_class)){
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
    return accuracy;
}

double classificate(vector<_image> &train, vector<_image> &test) {

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

    return result_calculation(test, candidates);
}
