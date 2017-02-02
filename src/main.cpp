#include "include/loader.h"
#include "include/extraction.h"
#include "include/classification.h"
#include "include/preprocessing.h"
#include "include/gabor.h"

void cleanup(void){
    LOGGER->destroyInstance();
    CONFIG->destroyInstance();
}

int main(int argc, char *argv[]) {

    vector<_image> train = vector<_image>();
    vector<_image> test = vector<_image>();

    if(argc == 3 && LOGGER->setUp(argv[1]) && CONFIG->setUp(argv[2]) && load_images(train, test)){

        if(CONFIG->getJobMode()) {
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
            LOGGER->Info("Application started in [NORMAL MODE]");
            LOGGER->Info(CONFIG->configurationDump());

            preprocess(train, test);

            /*std::multimap<unsigned int, unsigned int> kek;

            int i = 0;
            for(auto a : train){
                kek.insert(make_pair(a.id, i++));
            }

            for(auto const &x : kek){
                Gabor_editor(train[x.second].working_img);
            }*/

            if(CONFIG->getGaborSetting() > 0){
                gabor(train, test);
            }

            extract(train, test);
            classificate(train, test);
        }
    }

    LOGGER->Info("Application ended correctly...");

    atexit(cleanup);

    return 0;
}