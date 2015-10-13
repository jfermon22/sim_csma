//
//  Channel.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/8/15.
//
//

#ifndef __sim_csma__Channel__
#define __sim_csma__Channel__

#include <stdio.h>
#include <stdint.h>
#include "Event.h"

struct Channel {
    bool isIdle;
    bool hiddenNodeTransmitting;
    uint16_t owner;
    sim_time lastRTSTime;
	sim_time lastCTSTime;
    Channel():isIdle(true),hiddenNodeTransmitting(false){}
};

#endif /* defined(__sim_csma__Channel__) */
