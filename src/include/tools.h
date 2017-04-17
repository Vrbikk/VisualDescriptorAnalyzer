//
// Created by Antonín Vrba on 22.9.2016
//

#ifndef PLAYGROUND_TOOLS_H
#define PLAYGROUND_TOOLS_H

#include <string>
#include <sys/stat.h>
#include <algorithm>

using namespace std;

bool file_exists(const string file_name);
void clear_string(string &str);
bool is_number(const string &s);

// string split function returns vector of strings
std::vector<std::string> split(std::string message, std::string delim);
#endif
