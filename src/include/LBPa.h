//
// Created by Antonín Vrba on 20.10.2016
//

#ifndef PLAYGROUND_LBPA_H
#define PLAYGROUND_LBPA_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

/*
 * This class stands for E-LBP descriptor
 */
class LBPa : public Method{
private:
    _LBPa_config config;

public:
    LBPa();
    ~LBPa();

    // overrided methods
    virtual void setUp(void *_param) override;
    virtual void Process(_image &img) override;
};


#endif
