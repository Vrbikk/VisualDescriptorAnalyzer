#include "include/loader.h"
#include "include/extraction.h"
#include "include/classification.h"
#include "include/preprocessing.h"

void cleanup(void){
    LOGGER->destroyInstance();
    CONFIG->destroyInstance();
}

int main(int argc, char *argv[]) {

    if(argc == 2 && CONFIG->setUp(argv[1])){

        LOGGER->setUp(CONFIG->getLoggingFile());
        LOGGER->Info("Application started...");

        vector<_image> train = vector<_image>();
        vector<_image> test = vector<_image>();

        load_images(train, test);
        preprocess(train, test);
/*
        for(int i = 0; i < test.size(); i++){
            show_two_images(test[i].original_img, test[i].working_img);
            waitKey(0);
        }*/

        extract(train, test);
        classificate(train, test);
    }

    LOGGER->Info("Application ended correctly...");

    atexit(cleanup);

    return 0;
}