//
//  CsmaEvents.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#ifndef __sim_csma__CsmaEvents__
#define __sim_csma__CsmaEvents__

#include <stdio.h>
#include "Event.h"
#include "Node.h"

class FreeChannel : public Event
{
	Channel *channel;
	Simulation *sim;
public:
    FreeChannel( Channel *c, Simulation *nSim, sim_time newTime = 0.0f);
    ~FreeChannel(){};
    void execute();
};

class DIFS : public Event
{
    TxNode *sendingNode;
    sim_time timeLeft;
    sim_time slotDuration;
    bool m_shouldPause;
    void scheduleBackoff();
public:
    DIFS(TxNode *sNode, sim_time execTime,sim_time duration,sim_time slotDur,bool shouldPause);
    ~DIFS(){};
    void execute();
    bool isComplete();
};

class PacketReady : public Event
{
    TxNode *sendingNode;
    RxNode *rxNode;
    sim_time difs;
    sim_time slotDuration;
    bool isRetry;
    void scheduleSend(sim_time sendTime);
    void scheduleBackoff();
public:
    PacketReady(TxNode *sNode,RxNode *rnode,sim_time execTime,sim_time difsTime, sim_time slotDur, bool isretry = false);
    ~PacketReady(){};
    void execute();
};


class Send : public Event 
{
protected:
    TxNode *sendingNode;
    RxNode *receivingNode;
	sim_time duration;
public:
    Send(TxNode *sNode, RxNode *rNode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00001f);
    ~Send(){};
    void execute();
    void executeDuplicate();
    
};

class PacketSend : public Send
{
public:
    PacketSend(TxNode *sNode,RxNode *rNode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f);
    ~PacketSend(){};
};

class RTS : public Send
{
public:
    RTS(TxNode *sNode,RxNode *rnode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f);
    ~RTS(){};
    void execute();
    void executeDuplicate();
};

class CTS : public Send
{
public:
    CTS(TxNode *sNode, RxNode *rnode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f);
    ~CTS(){};
    void execute();
    void executeDuplicate(){ execute();}
};


class Ack : public Send
{
public:
    Ack(TxNode *sNode, RxNode *rnode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f);
    ~Ack(){};
    void execute();
};



class EndSend : public Event 
{
    TxNode *sendingNode;
public:
    EndSend(TxNode *sNode,sim_time newTime = 0.0f);
    ~EndSend(){};
    void execute();
	//void executeDuplicate();
    
};

#endif /* defined(__sim_csma__CsmaEvents__) */
