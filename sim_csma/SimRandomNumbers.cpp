//
//  SimRandomNumbers.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//
//#define GCC_4_2

#include "SimRandomNumbers.h"

#ifdef GCC_4_2
#include <time.h>
#include <stdlib.h>
#include <iostream>
#else
#include <random>
#include <stdlib.h>
#endif


int random_distro::seed(){
#ifdef GCC_4_2
	static bool firstPass = true;
	if (firstPass) {
		srand(time(NULL));
		firstPass= false;
	}
	return 0;
#else
    return (int)std::chrono::system_clock::now().time_since_epoch().count();
#endif
	
}

float random_distro::exponential(uint32_t freqHz,precision precisionUs)
{
#ifdef GCC_4_2	
    seed();
	double newNum = -( log( 1.0f - (float)rand()/(float)RAND_MAX ) )/freqHz;
#else
	std::default_random_engine generator (seed());
    std::exponential_distribution<double> distribution(freqHz);
    double newNum = distribution(generator);
#endif
    uint32_t precNum = (uint32_t)(1/(precisionUs*pow(10,-6)));
    return ( roundf(newNum * precNum)/precNum );
}

int random_distro::rand_int_max(uint32_t maximum){
    seed();
    return rand() % maximum;
}
