//
// Created by vrbik on 27.9.16.
//

#include "include/Method.h"

Method::Method() {
    initUniformTable();
}

Method::~Method() {

}

void Method::initUniformTable() {
    int num_uniforms = 0;
    int neighbours = 8;
    int combinations = 1 << neighbours;

    uniform_table = vector<int>(combinations);

    int transitions;
    for(int i = 0; i < combinations; i++){
        transitions = 0;

        for(int j = 0; j < neighbours -1; j++){
            transitions += bitAtPosition(i, j) != bitAtPosition(i, j + 1);
        }
        //extra transition test for first and last element
        transitions += bitAtPosition(i, neighbours - 1) != bitAtPosition(i, 0);

        if(transitions <= 2){
            uniform_table[i] = num_uniforms++;
        }else{
            uniform_table[i] = 58; //used for trash pixels, also there is only 58 uniform numbers from 0 to 255
        }

    }
}

//false on 0, true on 1
bool Method::bitAtPosition(unsigned int num, unsigned int pos) {
    return ((num & (1 << pos)) != 0);
}

void Method::croppedImage(Mat src, Mat &dst, int crop_size) {
    copyMakeBorder(src, dst, crop_size, crop_size, crop_size, crop_size, BORDER_DEFAULT);
}
