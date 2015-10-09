//
//  Node.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/7/15.
//
//

#include "Node.h"
#include "SimRandomNumbers.h"

const float SLOT_DUR = .00002;

Node::Node(uint32_t newID,Simulation *sim):Entity(newID),
 consecutiveCollissions(0)
{
    m_sim = sim;
}

void Node::collisionOccured(){
    if (consecutiveCollissions < 10) {
        consecutiveCollissions++;
    }
    scheduleSend();
}

void Node::scheduleSend(){
    if (consecutiveCollissions) {
        uint randSlot = random_distro::rand_int_max(pow(2, consecutiveCollissions));
        sim_time nextSend = SLOT_DUR *randSlot;
        scheduleSend();
    }
    
}