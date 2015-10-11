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
const uint16_t CWo = 8;
const uint16_t CWmax = 1024;

class Node: public Entity {
    uint32_t consecutiveCollissions;
    uint32_t totalCollisions;
    uint32_t successfulSends;
    Simulation *m_sim;
    Channel *m_channel;
	uint32_t sendFreq;
    sim_time sendDuration;
    sim_time slotDuration;
    sim_time difs;
	
    void scheduleEndSend(sim_time endTime);
public:
    Node(uint newId, Simulation *sim, Channel *channel,
         uint32_t sendMsgFreq = 1,sim_time difsTime=.00004,
         sim_time sendDur = .00001,sim_time slotDur = .00001 );
	~Node(){}
   
    void collisionOccured();
	uint32_t id();
	void sendSuccess();
	void handleCollision();
    void handleBusy(Event *e);
	void endTransmit();
    void scheduleSend(sim_time sendTime);
    void schedulePacketReady(sim_time eventTime);
    //void reschedulePacketReady(Event *event);
    void scheduleDifs(Event *event);
    sim_time getBackoff(uint32_t nCollisions = 1);
    sim_time getSimTime();
    uint32_t SuccessfulSends() const {return successfulSends;}
    uint32_t TotalCollisions() const {return totalCollisions;}
    Channel* channel();
};

#endif /* defined(__sim_csma__Node__) */
