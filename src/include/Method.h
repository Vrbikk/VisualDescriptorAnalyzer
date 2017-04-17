//
// Created by Antonín Vrba on 27.9.2016
//

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
    //false on 0, true on 1
    bool bitAtPosition(unsigned num, unsigned pos);
    void initUniformTable(int neighbours);
    void borderedImage(Mat src, Mat &dst, int crop_size);

    // Saves histogram data into &local_histogram varibale which is partial histogram of global histogram
    void localHistogram(int x, int y, int xsize, int ysize, Mat &src, vector<int> &local_histogram, bool uniform);

    // Return global histogram
    vector<vector<int>> globalHistogram(Mat &src, int grid_size, bool uniform);

    // Methods for extracting LBP, E-LBP, S-LBP
    void extractLBPa(Mat &src, Mat &dst, _LBPa_config config);
    void extractLBP(Mat &src, Mat &dst, _LBP_config config);

    // Function to precalculate circular neighbour positions to achieve significant better performance
    void optimize_LBP(_LBP_config config);

    // Calculates local histogram on specific position
    void localGaborHistogram(Mat &src, Point &point, vector<int> &local_histogram, int size, bool uniform);

    // Iterates all gabor points and returns vector of local histograms, there are accessed individualy
    vector<vector<int>> extractGaborPointsHistograms(Mat &src, vector<Point> &points, int size, bool uniform);

public:
    Method();
    ~Method();

    /*
     * Passing configuration parameters to descriptor class
     */
    virtual void setUp(void *param){};
    virtual void Process(_image &img){};
};

#endif
