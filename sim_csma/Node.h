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
#include "Channel.h"


class Node: public Entity {
    uint32_t consecutiveCollissions;
    Simulation *m_sim;
    Channel *m_channel;
	sim_time getBackoff();
	uint32_t sendFreq;
    sim_time sendDuration;
    sim_time slotDuration;
	
	void scheduleSend(sim_time sendTime);
	void scheduleEndSend(sim_time endTime);
public:
    Node(uint newId, Simulation *sim, Channel *channel, uint32_t sendMsgFreq = 1,sim_time sendDur = .00001,sim_time slotDur = .00001);
	~Node(){}
   
    void collisionOccured();
	uint32_t id();
	void sendSuccess();
	void handleCollision();
    void handleBusy(Event *e);
	void endTransmit();
    Channel* channel();
};

#endif /* defined(__sim_csma__Node__) */
