#include "include/loader.h"

void load_textures(vector<_image> &train, vector<_image> &test, int set) {

    _data_config config = CONFIG->getDataConfig();

    string label_test_path = config.data_path + "labels/test" + to_string(set) + ".txt";
    string label_train_path = config.data_path + "labels/train" + to_string(set) + ".txt";
    string texture_path = config.data_path + "images/";

    texture_miner(label_train_path, texture_path, train);
    texture_miner(label_test_path, texture_path, test);
}

void texture_miner(string label_path, string texture_path, vector<_image> &images) {

    std::ifstream infile(label_path);

    string line;
    while(std::getline(infile, line)){
        _image img;
        img.original_img = imread(texture_path + line, IMREAD_GRAYSCALE);
        img.texture_class = split(line, "/").at(0);
        images.push_back(img);
    }
}
