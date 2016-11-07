//
// Created by vrbik on 27.9.16.
//

#include "include/extraction.h"


void iterate_extract(Method &selectedMethod, vector<_image> &images, string name_of_set) {

    for(int i = 0; i < images.size(); i++){
        selectedMethod.Process(images[i]);
        draw_progress_to_console(name_of_set, i + 1, (int)images.size());
    }
}

void extract(vector<_image> &train, vector<_image> &test) {

    Method *selectedMethod = CONFIG->getExtractionMethod();

    if(!CONFIG->getJobMode()) {
        LOGGER->Info("Extracting vectors");
    }

    iterate_extract(*selectedMethod, train, "Training data extraction");
    iterate_extract(*selectedMethod, test, "Testing data extraction");

    delete(selectedMethod);
}


