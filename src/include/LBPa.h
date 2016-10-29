//
// Created by vrbik on 20.10.16.
//

#ifndef PLAYGROUND_LBPA_H
#define PLAYGROUND_LBPA_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

class LBPa : public Method{
private:
    int BORDER_OFFSET = 2;
    _LBPa_config config;
    int BASE_SIZE = 2;

public:
    LBPa();
    ~LBPa();

    virtual void extract(Mat &src, Mat &dst) override;
    virtual void setUp(void *_param) override;
    virtual void Process(_image &img) override;
};


#endif //PLAYGROUND_LBPA_H
