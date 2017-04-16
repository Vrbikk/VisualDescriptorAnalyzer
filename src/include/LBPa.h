//
// Created by vrbik on 20.10.16.
//

#ifndef PLAYGROUND_LBPA_H
#define PLAYGROUND_LBPA_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

/**
 * This class stands for E-LBP descriptor
 */
class LBPa : public Method{
private:
    _LBPa_config config;

public:
    LBPa();
    ~LBPa();
    virtual void setUp(void *_param) override;
    virtual void Process(_image &img) override;
};


#endif
