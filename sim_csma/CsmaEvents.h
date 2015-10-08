//
//  CsmaEvents.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#ifndef __sim_csma__CsmaEvents__
#define __sim_csma__CsmaEvents__

#include <stdio.h>
#include "Event.h"


class Send : public Event {
    uint32_t nodeId;
public:
    Send(uint32_t Id,sim_time newTime = 0.0f);
    ~Send(){};
    void execute();
    
};

#endif /* defined(__sim_csma__CsmaEvents__) */
