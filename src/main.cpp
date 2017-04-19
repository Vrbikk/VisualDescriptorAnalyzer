#include "include/loader.h"
#include "include/extraction.h"
#include "include/classification.h"
#include "include/preprocessing.h"

void cleanup(void){
    LOGGER->destroyInstance();
    CONFIG->destroyInstance();
}

int main(int argc, char *argv[]) {

    if(CONFIG->setUp(argv[2]) && LOGGER->setUp(argv[1])){

            vector<_image> train;
            vector<_image> test;

            if(CONFIG->getJobMode()){
                LOGGER->Info("Application started in [JOB TEXTURE MODE] with " + to_string(CONFIG->getJobs().size()) + " jobs");
                LOGGER->Info(CONFIG->configurationDump());

                for(auto job : CONFIG->getJobs()){
                    CONFIG->setActualJob(job);
                    if(!job.gabor) {
                        train = vector<_image>();
                        test = vector<_image>();

                        vector<double> results;
                        for(int i = 1; i <= 10; i++){
                            train = vector<_image>();
                            test = vector<_image>();

                            load_textures(train, test,i);
                            preprocess_textures(train, test);
                            extract(train, test);
                            results.push_back(classificate(train, test));
                            if(i == 1) CONFIG->print_config = false;
                        }

                        CONFIG->print_config = true;

                        int i = 1;
                        double sum = 0;

                        for(auto a : results){
                            sum += a;
                            LOGGER->Info("set " + to_string(i++) + ": <" + to_string(a) + ">");
                        }
                        LOGGER->Info("↑ accuracy: <" + to_string(sum/10) + "> ↑\n");
                    }
                }

            }else{

                LOGGER->Info("Application started in [NORMAL TEXTURE MODE]");
                LOGGER->Info(CONFIG->configurationDump());

                train = vector<_image>();
                test = vector<_image>();

                vector<double> results;
                for(int i = 1; i <= 10; i++){
                    train = vector<_image>();
                    test = vector<_image>();

                    load_textures(train, test,i);
                    preprocess_textures(train, test);
                    extract(train, test);
                    results.push_back(classificate(train, test));
                }

                int i = 1;
                double sum = 0;

                for(auto a : results){
                    sum += a;
                    LOGGER->Info(to_string(i++) + ": <" + to_string(a) + ">");
                }
                LOGGER->Info("↑ accuracy: <" + to_string(sum/10) + "> ↑\n");
            }
        }

    LOGGER->Info("Application ended correctly...");

    atexit(cleanup);
    return 0;
}