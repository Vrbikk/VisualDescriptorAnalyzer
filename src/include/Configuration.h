//
// Created by vrbik on 22.9.16.
//

#ifndef PLAYGROUND_CONFIGURATION_H
#define PLAYGROUND_CONFIGURATION_H

#define CONFIG Configuration::getConfiguration()

#include <string>
#include "stdim.h"
#include "tools.h"
#include <iostream>
#include <fstream>
#include "LBP.h"

enum __extraction_method{
    __LBP, __LBPa
};

enum __comparison_method{
    __EUCLIDEAN, __INTERSECTION
};

struct _data_config{
    string data_path;
    string test_folder;
    string train_folder;

    string print(){
        return string("data_config:[test_folder:" + test_folder +", train_folder:" + train_folder + "]");
    }
};

struct _preprocessing_config{
    bool equalize_hist;
    bool gaussian_blur;

    string print(){
        return string("preprocessing_config:[equalize_hist:" + to_string(equalize_hist) + ", gaussina_blur:" + to_string(gaussian_blur) + "]");
    }
};

struct _job{
    __extraction_method method;
    _LBP_config lbp_conf;
    _LBPa_config lbpa_conf;
};

class Configuration {
private:
    static Configuration *configuration_instance;
    Configuration();
    Configuration(const Configuration&); //protects copying

    string DELIMETER = ":";
    _data_config data_config;
    _preprocessing_config preprocessing_config;
    __extraction_method extraction_method;
    __comparison_method comparison_method;
    int classification_threads;
    bool horatio_caine_mode;
    vector<_job> jobs;
    bool job_mode;
    _LBP_config LBP_config;
    _LBPa_config LBPa_config;

    void setExtractionMethod(string a);
    void setComparisonMethod(string a);
    bool isCommentOrEmpty(string line);
    void setTypeAndValue(string line, string &type, string &value);
    void setBoolValue(string a, bool &target, string target_name);
    void setIntegerValue(string a, int &target,  string target_name);
    void addJob(string line_job);

public:
    void destroyInstance();
    static Configuration *getConfiguration();
    bool setUp(const string path);
    _data_config getDataConfig();
    Method *getExtractionMethod();
    __comparison_method getComparisonMethod();
    int getClassificationThreads();
    bool getHoratioCaineMode();
    _preprocessing_config getPreprocessingConfig();
    vector<_job> getJobs();
    bool getJobMode();
    void setActualJob(_job job);
    string configurationDump();

};

#endif //PLAYGROUND_CONFIGURATION_H
