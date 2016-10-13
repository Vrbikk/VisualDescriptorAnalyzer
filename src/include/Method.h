//
// Created by vrbik on 27.9.16.
//

#ifndef PLAYGROUND_METHOD_H
#define PLAYGROUND_METHOD_H


#include "stdim.h"

class Method {
protected:
    vector<int> uniform_table;
    bool bitAtPosition(unsigned num, unsigned pos);
    void initUniformTable();
    void croppedImage(Mat src, Mat &dst, int crop_size);
public:
    Method();
    ~Method();
    virtual void setUp(void *param){};
    virtual void Process(_image &img){};
};

#endif //PLAYGROUND_METHOD_H
