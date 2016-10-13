//
// Created by vrbik on 22.9.16.
//

#include "include/Configuration.h"

Configuration *Configuration::configuration_instance = NULL;

Configuration::Configuration() {

        //default configuration

    logging_file = "log.log";
    data_config.data_path = "ufi-cropped";
    data_config.train_folder = "train";
    data_config.test_folder = "test";
    extraction_method = __LBP;
    LBP_config.grid_size = 10;
    LBP_config.uniform = true;
    comparison_method = __EUCLIDEAN;
    classification_threads = 1;
    horatio_caine_mode = false;
    preprocessing_config.equalize_hist = false;
    preprocessing_config.gaussian_blur = false;
}

bool Configuration::isCommentOrEmpty(string line) {
    return line.substr(0, 1).compare("#") == 0 || line.compare("") == 0;
}

void Configuration::setTypeAndValue(string line, string &type, string &value) {
    type = line.substr(0, line.find(DELIMETER));
    value = line.substr(line.find(DELIMETER) + 1, line.size());
}

void Configuration::setExtractionMethod(string a){
    if(is_number(a)){
        switch (stoi(a)) {
            case 0: {
                extraction_method = __LBP;
                break;
            }
            case 1: {
                extraction_method = __LBPsuper;
                break;
            }
            default: {
                LOGGER->Error("configuration file corrupted - extraction_method");
                break;
            }
        }
    }else{
        LOGGER->Error("configuration file corrupted - extraction_method");
    }
}

void Configuration::setComparisonMethod(string a) {
    if(is_number(a)) {
        switch (stoi(a)) {
            case 0: {
                comparison_method = __EUCLIDEAN;
                break;
            }
            case 1: {
                comparison_method = __INTERSECTION;
                break;
            }
            default: {
                LOGGER->Error("configuration file corrupted - comparison_method");
                break;
            }
        }
    }else{
        LOGGER->Error("configuration file corrupted - comparison_method");
    }
}

bool Configuration::setUp(const string path) {
    if(file_exists(path)){
        ifstream file(path);
        string line;

        while(getline(file, line)){
            clear_string(line);

            if(!isCommentOrEmpty(line)){
                string type, value;
                setTypeAndValue(line, type, value);
                        // c++ does not support switching for string so... :(
                if(!type.compare("data_path")){data_config.data_path = value;}
                if(!type.compare("train_folder")){data_config.train_folder = value;}
                if(!type.compare("test_folder")){data_config.test_folder = value;}
                if(!type.compare("logging_file")){logging_file = value;}
                if(!type.compare("extraction_method")){ setExtractionMethod(value);}
                if(!type.compare("lbp_grid_size")){setIntegerValue(value, LBP_config.grid_size, "lbp_grid_size");}
                if(!type.compare("lbp_uniform")){ setBoolValue(value, LBP_config.uniform, "lbp_uniform");}
                if(!type.compare("comparison_method")){setComparisonMethod(value);}
                if(!type.compare("classification_threads")){setIntegerValue(value, classification_threads, "classification_threads");}
                if(!type.compare("horatio_caine_mode")){setBoolValue(value, horatio_caine_mode, "horatio_caine_mode");}
                if(!type.compare("equalize_hist")){setBoolValue(value, preprocessing_config.equalize_hist, "equalize_hist");}
                if(!type.compare("gaussian_blur")){setBoolValue(value, preprocessing_config.gaussian_blur, "gaussian_blur");}
            }
        }
        return true;
    }else{
        LOGGER->Error("Config file was not found");
        return false;
    }
}

_data_config Configuration::getDataConfig() {
    return data_config;
}

string Configuration::getLoggingFile() {
    return logging_file;
}

Method *Configuration::getExtractionMethod() {

    Method *SelectedMethod;

    switch(extraction_method){
        case __LBP:{
            SelectedMethod = new LBP();
            SelectedMethod->setUp(&LBP_config);
            break;
        }
        case __LBPsuper:{
            SelectedMethod = NULL;
            break;
        }
        default:{
            SelectedMethod = NULL;
            break;
        }

    }

    return SelectedMethod;
}

__comparison_method Configuration::getComparisonMethod() {
    return comparison_method;
}

Configuration *Configuration::getConfiguration() {
    if(configuration_instance == NULL){
        configuration_instance = new Configuration();
    }

    return configuration_instance;
}

int Configuration::getClassificationThreads() {
    return classification_threads;
}

bool Configuration::getHoratioCaineMode() {
    return horatio_caine_mode;
}

void Configuration::setBoolValue(string a, bool &target, string target_name) {
    if(is_number(a) && stoi(a) >= 0 && stoi(a) <= 1) {
        target = (stoi(a) == 1);
    }else{
        LOGGER->Error("configuration file corrupted - " + target_name);
    }
}

void Configuration::setIntegerValue(string a, int &target, string target_name) {
    if(is_number(a) && stoi(a) > 0) {
        target = stoi(a);
    }else{
        LOGGER->Error("configuration file corrupted - " + target_name);
    }
}

_preprocessing_config Configuration::getPreprocessingConfig() {
    return preprocessing_config;
}

void Configuration::destroyInstance() {
    delete configuration_instance;
    configuration_instance = NULL;
}
