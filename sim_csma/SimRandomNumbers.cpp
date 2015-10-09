//
//  SimRandomNumbers.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "SimRandomNumbers.h"


float random_distro::exponential(uint32_t freqHz,precision precisionUs)
{
    uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::exponential_distribution<double> distribution(freqHz);
    double newNum = distribution(generator);
    uint32_t precNum = 1/(precisionUs*pow(10,-6));
    return ( roundf(newNum * precNum)/precNum );
}

int random_distro::rand_int_max(uint32_t maximum){
    uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);
    return rand()%maximum;
}