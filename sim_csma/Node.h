//
//  Node.h
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//

#ifndef __sim_csma__Node__
#define __sim_csma__Node__

#include <stdio.h>
#include "Channel.h"

enum NodeState {
    SENDING,
    WAITING,
    
};


class Node {
    bool m_bTransmits;
    Channel *channel;
    uint32_t retransmitAttempt;
    
    
public:
    Node(bool transmitter);
    bool DidTransmit();
    float GetNextTransmitTime();
    uint32_t GetCw();
};

#endif /* defined(__sim_csma__Node__) */
