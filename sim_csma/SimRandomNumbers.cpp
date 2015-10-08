//
//  SimRandomNumbers.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "SimRandomNumbers.h"


double random_distro::exponential(uint32_t freqHz)
{
    uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::exponential_distribution<double> distribution(freqHz);
    
    return distribution(generator);
}