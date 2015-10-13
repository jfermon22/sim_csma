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


/* NODE */
class Node: public Entity {
protected:
    uint32_t consecutiveCollissions;
    uint32_t totalCollisions;
    uint32_t successfulSends;
    Simulation *m_sim;
    Channel *m_channel;
	uint32_t sendFreq;
    sim_time sendDuration;
    sim_time slotDuration;
    sim_time difs;
    sim_time ack_send_dur;
	
public:
    Node(uint newId, Simulation *sim, Channel *channel,
         uint32_t sendMsgFreq = 1,sim_time difsTime=.00004,
         sim_time sendDur = .00001,sim_time acksenddur=.00004,sim_time slotDur = .00001 ):
    Entity(newId ),consecutiveCollissions(0),totalCollisions(0),successfulSends(0),
    sendFreq(sendMsgFreq),sendDuration(sendDur),slotDuration(slotDur),difs(difsTime),
    ack_send_dur(acksenddur)
    {
        m_sim = sim;
        m_channel = channel;
    }

	~Node(){}
    
    void collisionOccured();
    uint32_t id(){return identity;}
    void scheduleSend(sim_time sendTime);
    sim_time getBackoff(uint32_t nCollisions = 1);
    sim_time getSimTime(){return m_sim->GetTime();}
    uint32_t SuccessfulSends() const {return successfulSends;}
    uint32_t TotalCollisions() const {return totalCollisions;}
    Channel* channel() const {return m_channel;}
	uint32_t SendFreq() const{return sendFreq;}
};

class TxNode ;

/* RX NODE */

class RxNode :public Node
{
    
public:
    RxNode(uint newId, Simulation *sim, Channel *channel,
           sim_time sendDur = .00001,sim_time slotDur = .00001):
    Node(newId,sim,channel, 1,0,sendDur,slotDur ){};
    void ScheduleAck(TxNode *txNode,sim_time newTime);
    void ScheduleCTS(TxNode *txNode,sim_time newTime);
};

/* TX NODE */

class TxNode :public Node
{
    bool m_usesVcs;
    sim_time sifs;
    RxNode* rxNode;
    sim_time NAV;
    bool isHiddenNode;
public:
    TxNode(uint newId, Simulation *sim, Channel *channel, RxNode *rNode,
           uint32_t sendMsgFreq = 1,sim_time difsTime=.00004,sim_time sifsTime=.00001,
           sim_time sendDur = .00001,sim_time ackRtsCtsDur = 0.00004f,sim_time slotDur = .00001,
           bool useVcs = false, bool isHidden = false):
    Node(newId,sim,channel,sendMsgFreq,difsTime,sendDur,ackRtsCtsDur,slotDur ),
    m_usesVcs(useVcs),sifs(sifsTime),NAV(sendDuration + sifs + 2 * ack_send_dur),isHiddenNode(isHidden)
    { rxNode = rNode; }
    
    void executeSend();
    void executeRts();
    void handleCollision();
    void handleBusy();
    void endTransmit();
    void schedulePacketSend(sim_time sendTime);
    void schedulePacketReady(sim_time eventTime);
    void scheduleDifs(Event *event);
    void scheduleEndSend(sim_time endTime);
    void scheduleChannelFree(sim_time freeChannelTime);
    void scheduleRts(sim_time sendTime);
    bool UsesVCS() const {return m_usesVcs;}
    bool IsHiddenNode() const {return isHiddenNode;}
};



#endif /* defined(__sim_csma__Node__) */
