#include "include/loader.h"
#include "include/extraction.h"
#include "include/classification.h"
#include "include/preprocessing.h"
#include "include/gabor.h"

/*
 * Singleton classes removal
 */
void cleanup(void){
    LOGGER->destroyInstance();
    CONFIG->destroyInstance();
}

int main(int argc, char *argv[]) {

    if(CONFIG->setUp(argv[2])){

        // TEXTURE MODE
        if(CONFIG->isTexture_mode() && LOGGER->setUp(argv[1])){

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
        }else{
            vector<_image> train = vector<_image>();
            vector<_image> test = vector<_image>();

            if(argc == 3 && LOGGER->setUp(argv[1]) && load_images(train, test)){

                if(CONFIG->getJobMode()) {

                    // JOB mode for iterative run over all configurations

                    LOGGER->Info("Application started in [JOB MODE] with " + to_string(CONFIG->getJobs().size()) + " jobs");
                    LOGGER->Info(CONFIG->configurationDump());

                    for(auto job : CONFIG->getJobs()){
                        CONFIG->setActualJob(job);
                        if(!job.gabor) {

                            preprocess(train, test);

                            if (CONFIG->getGaborSetting() > 0) {
                                gabor(train, test);
                            }
                            extract(train, test);
                            classificate(train, test);
                        }
                    }

                }else{

                    // NORMAL mode for testing purposes and graphical features

                    LOGGER->Info("Application started in [NORMAL MODE]");
                    LOGGER->Info(CONFIG->configurationDump());

                    preprocess(train, test);
                    if(CONFIG->getGaborEditorMode()) {
                        std::multimap<unsigned int, unsigned int> kek;
                        int i = 0;
                        for (auto a : train) {
                            kek.insert(make_pair(a.id, i++));
                        }
                        for (auto const &x : kek) {
                            Gabor_editor(train[x.second].working_img);
                        }
                    }

                    if (CONFIG->getGaborSetting() > 0) {
                        gabor(train, test);
                    }

                    extract(train, test);
                    classificate(train, test);
                }
            }
        }
    }


    // initializing main data containers


    LOGGER->Info("Application ended correctly...");

    atexit(cleanup);
    return 0;
}