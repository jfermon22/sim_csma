//
//  CsmaSimVcs.h
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//

#ifndef __sim_csma__CsmaSimVcs__
#define __sim_csma__CsmaSimVcs__

#include <stdio.h>
#include "CsmaSim.h"

//  CSMA/CA with virtual carrier sensing enabled:
//  RTS and CTS frames are exchanged before the transmission
//  of a frame. If RTS transmissions collide, nodes interpret this as a collision
//  and invoke the exponential backoff mechanism. Otherwise,
//  nodes that overhear an RTS/CTS message defer from transmission for the time
//  indicated in the NAV vector.

class CsmaSimVcs: public CsmaSim {
    
    
public:

};


#endif /* defined(__sim_csma__CsmaSimVcs__) */
