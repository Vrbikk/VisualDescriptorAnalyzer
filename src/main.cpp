#include "include/loader.h"
#include "include/extraction.h"
#include "include/classification.h"
#include "include/preprocessing.h"

void cleanup(void){
    LOGGER->destroyInstance();
    CONFIG->destroyInstance();
}

int main(int argc, char *argv[]) {

    vector<_image> train = vector<_image>();
    vector<_image> test = vector<_image>();

    if(argc == 3 && LOGGER->setUp(argv[1]) && CONFIG->setUp(argv[2]) && load_images(train, test)){
        if(CONFIG->getJobMode()) {
            LOGGER->Info("Application started in [JOB MODE]");
            LOGGER->Info(CONFIG->configurationDump());

            for(auto job : CONFIG->getJobs()){
                CONFIG->setActualJob(job);
                preprocess(train, test);
                extract(train, test);
                classificate(train, test);
            }

        }else{
            LOGGER->Info("Application started in [NORMAL MODE]");
            LOGGER->Info(CONFIG->configurationDump());

            /*for(auto a : test){
                Gabor_editor(a.original_img);
            }*/

            preprocess(train, test);
            extract(train, test);
            classificate(train, test);
        }
    }

    LOGGER->Info("Application ended correctly...");

    atexit(cleanup);

    return 0;
}