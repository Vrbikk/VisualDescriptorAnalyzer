//
// Created by vrbik on 27.9.16.
//

#ifndef PLAYGROUND_METHOD_H
#define PLAYGROUND_METHOD_H


#include "stdim.h"

typedef unsigned char byte;

class Method {
protected:
    vector<int> uniform_table;
    bool bitAtPosition(unsigned num, unsigned pos);
    void initUniformTable();
    void croppedImage(Mat src, Mat &dst, int crop_size);
    void localHistogram(int x, int y, int xsize, int ysize, Mat &src, vector<int> &local_histogram, bool uniform);
    vector<vector<int>> globalHistogram(Mat &src, int grid_size, bool uniform);
public:
    Method();
    ~Method();
    virtual void setUp(void *param){};
    virtual void Process(_image &img){};
};

#endif //PLAYGROUND_METHOD_H
