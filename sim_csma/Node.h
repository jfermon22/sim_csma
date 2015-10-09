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
#include "Simulation.h"


class Node: public Entity {
    uint32_t consecutiveCollissions;
    Simulation *m_sim;
	//bool lastSendSuccess;
	sim_time getBackoff();
	uint32_t sendFreq;
	
	void scheduleSend(sim_time sendTime);
	void scheduleEndSend(sim_time endTime);
public:
    Node(uint newId, Simulation *sim, uint32_t sendMsgFreq = 1);
	~Node(){}
   
    void collisionOccured();
	uint32_t id();
	void sendSuccess(sim_time duration);
	void sendFailure();
	void endTransmit();
};

#endif /* defined(__sim_csma__Node__) */
