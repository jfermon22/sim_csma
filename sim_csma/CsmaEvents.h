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


class DIFS : public Event
{
    TxNode *sendingNode;
    sim_time timeLeft;
    sim_time slotDuration;
    bool m_shouldPause;
    void scheduleBackoff();
public:
    DIFS(TxNode *sNode,sim_time execTime,sim_time duration,sim_time slotDur,bool shouldPause);
    ~DIFS(){};
    void execute();
    bool isComplete();
};

class PacketReady : public Event
{
    TxNode *sendingNode;
    sim_time difs;
    sim_time slotDuration;
    bool isRetry;
    void scheduleSend(sim_time sendTime);
    void scheduleBackoff();
public:
    PacketReady(TxNode *sNode,sim_time execTime,sim_time difsTime, sim_time slotDur, bool isretry = false);
    ~PacketReady(){};
    void execute();
};


class Send : public Event 
{
protected:
    TxNode *sendingNode;
    RxNode *receiveingNode;
	sim_time duration;
public:
    Send(TxNode *sNode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00001f);
    ~Send(){};
    void execute();
    void executeDuplicate();
    
};

class Ack : public Send
{
public:
    Ack(Node *sNode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f);
    ~Ack(){};
    void execute();
    void executeDuplicate();
};

class RTS : public Send
{
public:
    RTS(Node *sNode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f);
    ~RTS(){};
    void execute();
    void executeDuplicate();
};

class CTS : public Event
{
public:
    CTS(Node *sNode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f);
    ~CTS(){};
    void execute();
    void executeDuplicate();
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
