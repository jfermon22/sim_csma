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

class TxNode;

struct Channel {
    bool isIdle;
    TxNode *hiddenTransmittingNode;
    uint16_t owner;
    sim_time lastRTSTime;
	sim_time lastCTSTime;
    Channel():
		isIdle(true),owner(999),lastRTSTime(0),lastCTSTime(0)
	{
		hiddenTransmittingNode = NULL;
	}
};

#endif /* defined(__sim_csma__Channel__) */
