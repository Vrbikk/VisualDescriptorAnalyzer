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
    _LBPa_config config;

public:
    LBPa();
    ~LBPa();
    virtual void setUp(void *_param) override;
    virtual void Process(_image &img) override;
};


#endif //PLAYGROUND_LBPA_H
