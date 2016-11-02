//
// Created by vrbik on 22.9.16.
//

#include "include/Configuration.h"
#include "include/LBPa.h"

Configuration *Configuration::configuration_instance = NULL;

Configuration::Configuration() {

        //default configuration
    data_config.data_path = "ufi-cropped";
    data_config.train_folder = "train";
    data_config.test_folder = "test";
    extraction_method = __LBP;
    LBP_config.grid_size = 10;
    LBP_config.uniform = true;
    LBPa_config.grid_size = 10;
    LBPa_config.uniform = true;
    LBPa_config.comparison = true;
    LBPa_config.center_size = 2;
    comparison_method = __EUCLIDEAN;
    classification_threads = 1;
    horatio_caine_mode = false;
    preprocessing_config.equalize_hist = false;
    preprocessing_config.gaussian_blur = false;
    job_mode = false;
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
                extraction_method = __LBPa;
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
                else if(!type.compare("train_folder")){data_config.train_folder = value;}
                else if(!type.compare("test_folder")){data_config.test_folder = value;}
                else if(!type.compare("extraction_method")){ setExtractionMethod(value);}
                else if(!type.compare("lbp_grid_size")){setIntegerValue(value, LBP_config.grid_size, "lbp_grid_size");}
                else if(!type.compare("lbp_uniform")){ setBoolValue(value, LBP_config.uniform, "lbp_uniform");}
                else if(!type.compare("lbpa_grid_size")){setIntegerValue(value, LBPa_config.grid_size, "lbpa_grid_size");}
                else if(!type.compare("lbpa_uniform")){ setBoolValue(value, LBPa_config.uniform, "lbpa_uniform");}
                else if(!type.compare("lbpa_comparison")){ setBoolValue(value, LBPa_config.comparison, "lbpa_comparison");}
                else if(!type.compare("lbpa_center_size")){ setIntegerValue(value, LBPa_config.center_size, "lbpa_center_size");}
                else if(!type.compare("comparison_method")){setComparisonMethod(value);}
                else if(!type.compare("classification_threads")){setIntegerValue(value, classification_threads, "classification_threads");}
                else if(!type.compare("horatio_caine_mode")){setBoolValue(value, horatio_caine_mode, "horatio_caine_mode");}
                else if(!type.compare("equalize_hist")){setBoolValue(value, preprocessing_config.equalize_hist, "equalize_hist");}
                else if(!type.compare("gaussian_blur")){setBoolValue(value, preprocessing_config.gaussian_blur, "gaussian_blur");}
                else if(!type.compare("job_mode")){setBoolValue(value, job_mode, "job_mode");}
                else if(!type.compare("job")){addJob(value);}
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

Method *Configuration::getExtractionMethod() {

    Method *SelectedMethod;

    switch(extraction_method){
        case __LBP:{
            SelectedMethod = new LBP();
            SelectedMethod->setUp(&LBP_config);
            break;
        }
        case __LBPa:{
            SelectedMethod = new LBPa();
            SelectedMethod->setUp(&LBPa_config);
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
    if(is_number(a) && stoi(a) >= 0) { //TODO why > 0
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

void Configuration::addJob(string line_job) {
    vector<string> items = split(line_job, "-");
    int method;
    setIntegerValue(items[0], method, "job_method");
    vector<string> params = split(items[1], ",");

    switch(method){
        case __LBP:{
            _LBP_config conf;
            setIntegerValue(params[0], conf.grid_size, "job_lbp_grid_size");
            setBoolValue(params[1], conf.uniform, "job_lbp_uniform");

            _job job;
            job.method = __LBP;
            job.lbp_conf = conf;
            jobs.push_back(job);
            break;
        }
        case __LBPa:{
            _LBPa_config conf;
            setIntegerValue(params[0], conf.grid_size, "job_lbpa_grid_size");
            setBoolValue(params[1], conf.uniform, "job_lbpa_uniform");
            setIntegerValue(params[2], conf.center_size, "job_lbpa_center_size");
            setBoolValue(params[3], conf.comparison, "job_lbpa_comparison");

            _job job;
            job.method = __LBPa;
            job.lbpa_conf = conf;
            jobs.push_back(job);
            break;
        }
        default:{
            LOGGER->Error("Job config error");
            break;
        }
    }

}

vector<_job> Configuration::getJobs() {
    return jobs;
}

bool Configuration::getJobMode() {
    return job_mode;
}

void Configuration::setActualJob(_job job) {
    switch (job.method){
        case __LBP:{
            extraction_method = __LBP;
            LBP_config = job.lbp_conf;
            LOGGER->Info(LBP_config.print());
            break;
        }
        case __LBPa:{
            extraction_method = __LBPa;
            LBPa_config = job.lbpa_conf;
            LOGGER->Info(LBPa_config.print());
            break;
        }
        default:{
            LOGGER->Error("setActualJob default switch branch");
            break;
        }
    }
}

string Configuration::configurationDump() {

    string space = "\n\t\t\t\t";
    return string(space + "------ config ------" + space + data_config.print() + space + preprocessing_config.print() + space +
                          "classification_threads:" + to_string(classification_threads));
}
