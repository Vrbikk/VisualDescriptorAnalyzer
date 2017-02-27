//
// Created by vrbik on 24.9.16.
//

#ifndef PLAYGROUND_LOGGER_H
#define PLAYGROUND_LOGGER_H
#define LOGGER Logger::getLogger()

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Logger {
private:
    static Logger *logger_instance;
    Logger();
    Logger(const Logger&);
    static ofstream log_stream;
    void Log(string log_message);
    string logging_file;

public:
    void destroyInstance();
    bool setUp(string path);
    void Info(string message);
    void Error(string message);
    static Logger *getLogger();
    string get_current_date_time();
};

#endif //PLAYGROUND_LOGGER_H
