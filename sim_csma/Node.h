//
//  Node.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/7/15.
//
//

#ifndef __sim_csma__Node__
#define __sim_csma__Node__

#include <stdio.h>
#include "Entity.h"
#include "CsmaEvents.h"
#include "Simulation.h"


class Node: public Entity {
    uint32_t consecutiveCollissions;
    Simulation *m_sim;
public:
    Node(uint newId,Simulation *sim);
    void scheduleSend();
    void collisionOccured();
    
};

#endif /* defined(__sim_csma__Node__) */
