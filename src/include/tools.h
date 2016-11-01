//
// Created by vrbik on 22.9.16.
//

#ifndef PLAYGROUND_TOOLS_H
#define PLAYGROUND_TOOLS_H

#include <string>
#include <sys/stat.h>
#include <algorithm>

using namespace std;

bool file_exists(const string file_name);
void clear_string(string &str);
string get_vector_view(vector<vector<int>> vec);
bool is_number(const string &s);
std::vector<std::string> split(std::string message, std::string delim);
#endif //PLAYGROUND_TOOLS_H
