
#ifndef PLAYGROUND_LBPA_H
#define PLAYGROUND_LBPA_H

#include "Method.h"
#include "GUI.h"
#include "stdim.h"

/**
 * Tato třída reprezentuje deskriptor E-LBP
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
