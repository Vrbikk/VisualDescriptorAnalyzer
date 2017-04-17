//
// Created by Antonín Vrba on 27.9.2016
//

#ifndef PLAYGROUND_LBP_H
#define PLAYGROUND_LBP_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

/*
 * This class stands for LBP and S-LBP descriptors
 */

class LBP: public Method{
private:
    _LBP_config config;

public:
    LBP();
    ~LBP();

    // overrided methods
    virtual void setUp(void *_param) override;
    virtual void Process(_image &img) override;
};


#endif
