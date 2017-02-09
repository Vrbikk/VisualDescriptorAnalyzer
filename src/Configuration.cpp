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

    LBP_config.hist.grid_size = 10;
    LBP_config.hist.uniform = true;
    LBP_config.lbp_params.range = 1;
    LBP_config.lbp_params.neighbours = 8;
    LBP_config.lbp_params.neighbour_shape = 1;

    LBPa_config.hist.grid_size = 10;
    LBPa_config.hist.uniform = true;
    LBPa_config.lbpa_params.center_size = 2;
    LBPa_config.lbpa_params.neighbour_shape = 2;
    LBPa_config.lbpa_params.range = 2;

    comparison_method = __EUCLIDEAN;
    classification_threads = 1;
    horatio_caine_mode = false;
    preprocessing_config.equalize_hist = false;
    preprocessing_config.gaussian_blur = false;
    job_mode = false;

    Gabor_config.gabor_setting = 2;
    Gabor_config.gabor_border_size = 20;
    Gabor_config.gabor_points = 64;
    Gabor_config.gabor_histogram_size = 15;

    GFS.TMPkernel_size = 13;
    GFS.TMPpos_sigma = 5;
    GFS.TMPpos_lambda = 44;
    GFS.TMPpos_theta = 15;
    GFS.TMPpos_gamma = 3;
    GFS.TMPpos_psi = 104;
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
            case 2: {
                comparison_method = __BHATTACHARYYA;
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

                if(!type.compare("data_path")){data_config.data_path = value;}
                else if(!type.compare("train_folder")){data_config.train_folder = value;}
                else if(!type.compare("test_folder")){data_config.test_folder = value;}
                else if(!type.compare("extraction_method")){ setExtractionMethod(value);}

                    //Gabor
                else if(!type.compare("gabor_setting")){ setIntegerValue(value, Gabor_config.gabor_setting, "gebor_setting");}
                else if(!type.compare("gabor_points")){ setIntegerValue(value, Gabor_config.gabor_points, "gabor_points");}
                else if(!type.compare("gabor_histogram_size")){ setIntegerValue(value, Gabor_config.gabor_histogram_size, "gabor_histogram_size");}
                else if(!type.compare("gabor_border_size")){ setIntegerValue(value, Gabor_config.gabor_border_size, "gabor_border_size");}

                    //LBP
                else if(!type.compare("lbp_grid_size")){setIntegerValue(value, LBP_config.hist.grid_size, "lbp_grid_size");}
                else if(!type.compare("lbp_uniform")){ setBoolValue(value, LBP_config.hist.uniform, "lbp_uniform");}
                else if(!type.compare("lbp_range")){ setIntegerValue(value, LBP_config.lbp_params.range, "lbp_range");}
                else if(!type.compare("lbp_neighbours")){ setIntegerValue(value, LBP_config.lbp_params.neighbours, "lbp_neighbours");}
                else if(!type.compare("lbp_neighbour_shape")){ setIntegerValue(value, LBP_config.lbp_params.neighbour_shape, "lbp_neighbour_shape");}
                    //LBPa
                else if(!type.compare("lbpa_grid_size")){setIntegerValue(value, LBPa_config.hist.grid_size, "lbpa_grid_size");}
                else if(!type.compare("lbpa_uniform")){ setBoolValue(value, LBPa_config.hist.uniform, "lbpa_uniform");}
                else if(!type.compare("lbpa_center_size")){ setIntegerValue(value, LBPa_config.lbpa_params.center_size, "lbpa_center_size");}
                else if(!type.compare("lbpa_neighbour_shape")){ setIntegerValue(value, LBPa_config.lbpa_params.neighbour_shape, "lbpa_neighbour_shape");}
                else if(!type.compare("lbpa_range")){ setIntegerValue(value, LBPa_config.lbpa_params.range, "lbpa_range");}

                else if(!type.compare("comparison_method")){setComparisonMethod(value);}
                else if(!type.compare("classification_threads")){setIntegerValue(value, classification_threads, "classification_threads");}
                else if(!type.compare("horatio_caine_mode")){setBoolValue(value, horatio_caine_mode, "horatio_caine_mode");}
                else if(!type.compare("equalize_hist")){setBoolValue(value, preprocessing_config.equalize_hist, "equalize_hist");}
                else if(!type.compare("gaussian_blur")){setBoolValue(value, preprocessing_config.gaussian_blur, "gaussian_blur");}
                else if(!type.compare("job_mode")){setBoolValue(value, job_mode, "job_mode");}
                else if(!type.compare("job")){addJob(value);}
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
    if(is_number(a) && stoi(a) >= 0) {
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

    if(method == 2){
        _Gabor_config conf;
        setIntegerValue(params[0], conf.gabor_setting, "job_gabor_setting");
        setIntegerValue(params[1], conf.gabor_points, "job_gabor_points");
        setIntegerValue(params[2], conf.gabor_histogram_size, "job_gabor_histogram_size");
        setIntegerValue(params[3], conf.gabor_border_size, "job_gabor_border_size");

        _job job;
        job.gabor = true;
        job.gabor_conf = conf;
        jobs.push_back(job);

        return;
    }

    switch(method){
        case __LBP:{
            _LBP_config conf;
            setIntegerValue(params[0], conf.hist.grid_size, "job_lbp_grid_size");
            setBoolValue(params[1], conf.hist.uniform, "job_lbp_uniform");
            setIntegerValue(params[2], conf.lbp_params.range, "job_lbp_range");
            setIntegerValue(params[3], conf.lbp_params.neighbours, "job_lbp_neighbours");
            setIntegerValue(params[4], conf.lbp_params.neighbour_shape, "job_lbp_neighbour_shape");

            _job job;
            job.method = __LBP;
            job.lbp_conf = conf;
            jobs.push_back(job);
            break;
        }
        case __LBPa:{
            _LBPa_config conf;
            setIntegerValue(params[0], conf.hist.grid_size, "job_lbpa_grid_size");
            setBoolValue(params[1], conf.hist.uniform, "job_lbpa_uniform");
            setIntegerValue(params[2], conf.lbpa_params.center_size, "job_lbpa_center_size");
            setIntegerValue(params[3], conf.lbpa_params.neighbour_shape, "job_lbpa_neighbour_shape");
            setIntegerValue(params[4], conf.lbpa_params.range, "job_lbpa_range");

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

    if(job.gabor){
        Gabor_config = job.gabor_conf;
        LOGGER->Info("Updating Gabor settings\n");
        return;
    }



    switch (job.method){
        case __LBP:{
            extraction_method = __LBP;
            LBP_config = job.lbp_conf;
            break;
        }
        case __LBPa:{
            extraction_method = __LBPa;
            LBPa_config = job.lbpa_conf;
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
    return string(        "------ config ------\n" + space +
                          data_config.print() + space +
                          preprocessing_config.print() + space +
                          "classification_threads:" + to_string(classification_threads) + space +
                          "comparison_method:" + comparidon_method_string[comparison_method] + space +
                          "gabor_filter_setting: [kernel_size:" + to_string(GFS.TMPkernel_size) + " Sigma:" +
                          to_string(GFS.TMPpos_sigma) + " Lambda:" + to_string(GFS.TMPpos_lambda) + " Theta:" + to_string(GFS.TMPpos_theta) +
                          " Gamma:"+ to_string(GFS.TMPpos_gamma) + " Psi:" + to_string(GFS.TMPpos_psi) + "]\n"
    );
}

int Configuration::getGaborSetting() {
    return Gabor_config.gabor_setting;
}

int Configuration::getGaborHistogramSize() {
    return Gabor_config.gabor_histogram_size;
}

int Configuration::getGaborPoints() {
    return Gabor_config.gabor_points;
}

int Configuration::getGaborBorderSize() {
    return Gabor_config.gabor_border_size;
}

void Configuration::printGaborConfiguration() {
    if(Gabor_config.gabor_setting == 0){
        LOGGER->Info("Gabor config - setting:" + gabor_settings[Gabor_config.gabor_setting]);
    }else{
        LOGGER->Info("Gabor config - setting:" + gabor_settings[Gabor_config.gabor_setting] + " points:" + to_string(Gabor_config.gabor_points) +
                        " histogram_size:" + to_string(Gabor_config.gabor_histogram_size) + " border_size:" + to_string(Gabor_config.gabor_border_size));
    }
}
