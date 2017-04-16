//
// Created by vrbik on 27.9.16.
//

#ifndef PLAYGROUND_LBP_H
#define PLAYGROUND_LBP_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

/**
 * This class stands for LBP and S-LBP descriptors
 */

class LBP: public Method{
private:
    _LBP_config config;

public:
    LBP();
    ~LBP();

    virtual void setUp(void *_param) override;
    virtual void Process(_image &img) override;
};


#endif
