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
    
    double exponential(uint32_t hertz)
    {
        double r = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
        return -log(1.0f-r)/hertz;
    }
    
}

#endif /* defined(__sim_csma__SimRandomNumbers__) */
