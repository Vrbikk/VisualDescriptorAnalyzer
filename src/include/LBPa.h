//
// Created by vrbik on 20.10.16.
//

#ifndef PLAYGROUND_LBPA_H
#define PLAYGROUND_LBPA_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

typedef unsigned char byte;

class LBPa : public Method{
private:
    int BORDER_OFFSET = 2;
    _LBPa_config config;
    int BASE_SIZE = 2;

    void extractLBPa(Mat src, Mat &dst);
    void localHistogram(int x, int y, int xsize, int ysize, Mat &src, vector<int> &local_histogram);
    vector<vector<int>> globalHistogram(Mat &src);
    byte average_maximum_value(int x_, int y_, Mat src);

public:
    virtual void setUp(void *_param) override;
    virtual void Process(_image &img) override;
};


#endif //PLAYGROUND_LBPA_H
