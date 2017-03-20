//
// Created by vrbik on 3.10.16.
//

#include "include/preprocessing.h"

void gaussian_blur_preprocessing(Mat &a){
    GaussianBlur(a, a, Size(5, 5), 0);
}

void equalize_hist_preprocessing(Mat &a){
    equalizeHist(a , a);
}

void iterate_preprocess(vector<_image> &images, string name_of_set) {

    _preprocessing_config config = CONFIG->getPreprocessingConfig();

    for(int i = 0; i < images.size(); i++){
        images[i].original_img.copyTo(images[i].working_img);
        if(config.equalize_hist){equalize_hist_preprocessing(images[i].working_img);}
        if(config.gaussian_blur){gaussian_blur_preprocessing(images[i].working_img);}
        draw_progress_to_console(name_of_set, i + 1, (int)images.size());
    }
}

void preprocess(vector<_image> &train, vector<_image> &test) {
    if(!CONFIG->getJobMode()){
        LOGGER->Info("Preprocessing images");
    }

    iterate_preprocess(train, "Training data preprocessing");
    iterate_preprocess(test, "Testing data preprocessing");
}

void preprocess_textures(vector<_image> &train, vector<_image> &test) {

    if(!CONFIG->getJobMode()){
        LOGGER->Info("Preprocessing textures");
    }
    texture_scaling(train);
    texture_scaling(test);

}

void texture_scaling(vector<_image> &images) {
    for(auto &a : images){
        Size size = Size(494,450);
        resize(a.original_img, a.working_img, size);
    }
}


