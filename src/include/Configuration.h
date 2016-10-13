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
    __LBP, __LBPsuper
};

enum __comparison_method{
    __EUCLIDEAN, __INTERSECTION
};

struct _data_config{
    string data_path;
    string test_folder;
    string train_folder;
};

struct _preprocessing_config{
    bool equalize_hist;
    bool gaussian_blur;
};

class Configuration {
private:
    static Configuration *configuration_instance;
    Configuration();
    Configuration(const Configuration&); //protects copying

    string DELIMETER = ":";
    string logging_file;
    _data_config data_config;
    _preprocessing_config preprocessing_config;
    __extraction_method extraction_method;
    _LBP_config LBP_config;
    __comparison_method comparison_method;
    int classification_threads;
    bool horatio_caine_mode;

    void setExtractionMethod(string a);
    void setComparisonMethod(string a);
    bool isCommentOrEmpty(string line);
    void setTypeAndValue(string line, string &type, string &value);
    void setBoolValue(string a, bool &target, string target_name);
    void setIntegerValue(string a, int &target,  string target_name);

public:
    void destroyInstance();
    static Configuration *getConfiguration();
    bool setUp(const string path);
    _data_config getDataConfig();
    string getLoggingFile();
    Method *getExtractionMethod();
    __comparison_method getComparisonMethod();
    int getClassificationThreads();
    bool getHoratioCaineMode();
    _preprocessing_config getPreprocessingConfig();

};

#endif //PLAYGROUND_CONFIGURATION_H
