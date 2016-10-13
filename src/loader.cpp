#include "include/loader.h"

int get_files(string path, vector<string> &files) {
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(path.c_str())) == NULL) {
        LOGGER->Error("get_files failed, folder was not found");
        return 1;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if(string(dirp->d_name).compare(".") == 0 || string(dirp->d_name).compare("..") == 0|| string(dirp->d_name).compare("info.txt") == 0){
        }else{
            files.push_back(string(dirp->d_name));
        }
    }
    closedir(dp);
    return 0;
}

void file_miner(string path, vector<_image> &images){
    vector<string> folder_paths = vector<string>();
    get_files(path, folder_paths);

    for(int i = 0; i < folder_paths.size(); i++){
        string files_path = path + folder_paths[i];
        vector<string> files = vector<string>();
        get_files(files_path, files);

        for(int j = 0; j < files.size(); j++){
            _image loaded_image;
            loaded_image.id = stoi(folder_paths[i].substr(1, folder_paths[i].size()));
            loaded_image.file_name = files[j];
            loaded_image.path = files_path + "/" + files[j];

            //loading picture
            loaded_image.original_img = imread(files_path + "/" + files[j], IMREAD_GRAYSCALE);

            images.push_back(loaded_image);
        }
    }
}

int load_images(vector<_image> &train, vector<_image> &test){
    LOGGER->Info("Collecting images");

    _data_config config = CONFIG->getDataConfig();

    string train_path = config.data_path + config.train_folder;
    string test_path = config.data_path + config.test_folder;

    if(file_exists(config.data_path) && file_exists(train_path) && file_exists(test_path)){

        file_miner(train_path, train);
        file_miner(test_path, test);

        LOGGER->Info("Collected training data: " + to_string(train.size()));
        LOGGER->Info("Collected testing data: " + to_string(test.size()));

        return 0;
    }else{
        LOGGER->Error("load_images failed, some folders in #data_config section were not found");
        return 1;
    }



}
