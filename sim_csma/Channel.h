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

struct Channel {
    bool isFree;
    uint16_t owner;
    Channel():isFree(true){}
     
};

#endif /* defined(__sim_csma__Channel__) */
