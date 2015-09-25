//
//  CsmaSimExpBackoff.h
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//

#ifndef __sim_csma__CsmaSimExpBackoff__
#define __sim_csma__CsmaSimExpBackoff__

#include <stdio.h>
#include "CsmaSim.h"

// CSMA with Collision Avoidance (CSMA/CA): In this protocol, a node ready to transmit (when a packet has
// arrived for transmission from the upper layers of the network stack), senses the channel for an initial period
// of DIFS time.
//  If the channel is idle, it starts to transmit.
//  If the channel is busy, it waits until the medium becomes idle. Once idle, it selects a random back
//value in [1;CW0]: It then waits for DIFS time and starts the countdown of its timer to zero, pausing
//any time a frame transmission occurs. When the counter reaches zero, it attempts to transmit.
// If the packet is successfully received (no collision), the node repeats the transmission process by selecting
//a random backoff value in [1;CW0]: For successive transmissions, the node has to sense for DIFS time
//before starting the countdown.
// If a collision occurs, it doubles its contention window CW and
//repeats the backoffprocess by selecting
//a backoff value in [1;CW]: The CW value cannot become larger than a threshold value CWmax.

class CsmaSimExpBackoff : public CsmaSim {

    
public:
 
};

#endif /* defined(__sim_csma__CsmaSimExpBackoff__) */
