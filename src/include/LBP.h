//
// Created by vrbik on 27.9.16.
//

#ifndef PLAYGROUND_LBP_H
#define PLAYGROUND_LBP_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

typedef unsigned char byte;

class LBP: public Method{
private:
    int BORDER_OFFSET = 1;
    _LBP_config config;
    void extractLBP(Mat src, Mat &dst);
    void localHistogram(int x, int y, int xsize, int ysize, Mat &src, vector<int> &local_histogram);
    vector<vector<int>> globalHistogram(Mat &src);
public:
    LBP();
    ~LBP();

    virtual void setUp(void *_param);
    virtual void Process(_image &img);
};


#endif //PLAYGROUND_LBP_H
