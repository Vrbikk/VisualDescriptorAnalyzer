
#include "include/preprocessing.h"

void preprocess_textures(vector<_image> &train, vector<_image> &test) {
    texture_scaling(train);
    texture_scaling(test);
}

void texture_scaling(vector<_image> &images) {
    for(auto &a : images){
        Size size = Size(494,450); // resizing for all textures
        resize(a.original_img, a.working_img, size);
    }
}


