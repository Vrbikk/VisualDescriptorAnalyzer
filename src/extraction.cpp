//
// Created by vrbik on 27.9.16.
//

#include "include/extraction.h"


void iterate_extract(vector<_image> &images, string name_of_set) {

    Method *SelectedMethod = CONFIG->getExtractionMethod();

    for(int i = 0; i < images.size(); i++){
        SelectedMethod->Process(images[i]);
        draw_progress_to_console(name_of_set, i + 1, (int)images.size());
    }
}

void extract(vector<_image> &train, vector<_image> &test) {
    LOGGER->Info("Extracting vectors");
    iterate_extract(train, "Training data extraction");
    iterate_extract(test, "Testing data extraction");
}

