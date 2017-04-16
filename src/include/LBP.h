//
// Created by vrbik on 27.9.16.
//

#ifndef PLAYGROUND_LBP_H
#define PLAYGROUND_LBP_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

/**
 * Tato třída reprezentuje deskriptor LBP a S-LBP
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
