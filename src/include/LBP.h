//
// Created by vrbik on 27.9.16.
//

#ifndef PLAYGROUND_LBP_H
#define PLAYGROUND_LBP_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

class LBP: public Method{
private:
    int BORDER_OFFSET = 1;
    _LBP_config config;

public:
    LBP();
    ~LBP();

    virtual void extract(Mat &src, Mat &dst) override;
    virtual void setUp(void *_param) override;
    virtual void Process(_image &img) override;
};


#endif //PLAYGROUND_LBP_H
