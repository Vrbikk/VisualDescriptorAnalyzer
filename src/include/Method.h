

#ifndef PLAYGROUND_METHOD_H
#define PLAYGROUND_METHOD_H

#include "stdim.h"

typedef unsigned char byte;

class Method {
protected:
    unsigned int number_of_uniform_patterns = 0;
    unsigned int number_of_all_patterns = 0;
    vector<unsigned int> uniform_table;

    vector<vector<double>> optimized_LBP;
    byte getShapeValue(Mat &src, int x, int y, int type, int shape_evaluation);

    bool bitAtPosition(unsigned num, unsigned pos);
    void initUniformTable(int neighbours);
    void croppedImage(Mat src, Mat &dst, int crop_size);
    void localHistogram(int x, int y, int xsize, int ysize, Mat &src, vector<int> &local_histogram, bool uniform);
    vector<vector<int>> globalHistogram(Mat &src, int grid_size, bool uniform);
    void extractLBPa(Mat &src, Mat &dst, _LBPa_config config);
    void extractLBP(Mat &src, Mat &dst, _LBP_config config);
    void optimize_LBP(_LBP_config config);

public:
    Method();
    ~Method();
    //virtual void extract(Mat &src, Mat&dst){};
    virtual void setUp(void *param){};
    virtual void Process(_image &img){};
};

#endif
