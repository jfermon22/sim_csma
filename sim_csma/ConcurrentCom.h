//
//  ConcurrentCom.h
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//

#ifndef __sim_csma__ConcurrentCom__
#define __sim_csma__ConcurrentCom__

#include <stdio.h>
#include "CsmaSimExpBackoff.h"
#include "CsmaSimVcs.h"

class ConcurrentCom {
    CsmaSimExpBackoff expBackOff;
    CsmsSimVcs vcs;
    
public:
    <#member functions#>
};

#endif /* defined(__sim_csma__ConcurrentCom__) */
