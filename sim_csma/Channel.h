//
//  Channel.h
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//

#ifndef __sim_csma__Channel__
#define __sim_csma__Channel__
#include <stdint.h>
#include <stdio.h>
enum ChannelState {
    BUSY,
    IDLE,
};


class Channel {
    
public:
    bool isBusy;
};

#endif /* defined(__sim_csma__Channel__) */
