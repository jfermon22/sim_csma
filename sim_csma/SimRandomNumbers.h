//
//  SimRandomNumbers.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#ifndef __sim_csma__SimRandomNumbers__
#define __sim_csma__SimRandomNumbers__

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <random>

namespace random_distro
{
    enum precision {
        USECS       = 1,
        TEN_USECS   = 10,
        HUND_USECS  = 100,
        MILSECS     = 1000,
        TEN_MILSECS = 10000,
        HUN_MILSECS = 100000,
        SECS        = 1000000,
    };
    
    float exponential(uint32_t freqHz,precision precisionUs = USECS);
    int  rand_int_max(uint32_t maximum);
}

#endif /* defined(__sim_csma__SimRandomNumbers__) */
