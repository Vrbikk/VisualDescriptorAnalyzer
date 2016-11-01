//
// Created by vrbik on 22.9.16.
//

#include <cstring>
#include "include/tools.h"

bool file_exists(const string file_name){
    struct stat buffer;
    return (stat (file_name.c_str(), &buffer) == 0);
}

void clear_string(string &str) {
    str.erase(std::remove(str.begin(),str.end(),' '),str.end());
}

string get_vector_view(vector<vector<int>> vec) {
    string content = "";
    int size_a = vec.size();
    int size_b = vec[0].size();

    content.append("global_size: " + to_string(size_a) + "\n");
    content.append("local_size: " + to_string(size_b) + "\n");

    for(int i = 0; i < size_a; i++){

        content.append("hist_" + to_string(i) + ": (");
        for(int j = 0; j < size_b; j++){
            content.append(to_string(vec[i][j]) + ", ");
        }
        content.append(")\n");
    }

    return content;
}

bool is_number(const string &s) {
    return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

std::vector<std::string> split(std::string message, std::string delim) {

    std::vector<std::string> tmp_items;

    char arr[message.size()];
    strcpy(arr, message.c_str());
    char *tokens = strtok(arr, delim.c_str());

    while(tokens != nullptr){
        tmp_items.push_back(tokens);
        tokens = strtok(nullptr, delim.c_str());
    }

    return tmp_items;
}



