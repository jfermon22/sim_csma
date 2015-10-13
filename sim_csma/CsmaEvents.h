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
    FreeChannel( Channel *c, Simulation *nSim, sim_time newTime = 0.0f):
    Event(newTime,VERY_LOW)
    {
        channel =c;
        sim = nSim;
    }
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
    DIFS(TxNode *sNode, sim_time execTime,sim_time duration,sim_time slotDur,bool shouldPause):
    Event(execTime,VERY_LOW),timeLeft(duration),slotDuration(slotDur),m_shouldPause(shouldPause)
    { sendingNode = sNode; }
    ~DIFS(){};
    void execute();
    bool isComplete() const { return timeLeft <= 0;}
};


class PacketReady : public Event
{
    TxNode *sendingNode;
    RxNode *rxNode;
    sim_time difs;
    sim_time slotDuration;
public:
    PacketReady(TxNode *sNode,RxNode *rnode,sim_time execTime,sim_time difsTime, sim_time slotDur, bool isretry = false):
    Event(execTime,VERY_LOW),difs(difsTime),slotDuration(slotDur)    {
        sendingNode = sNode;
        rxNode = rnode;
    }
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
    Send(TxNode *sNode, RxNode *rNode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00001f):
    Event(newTime,VERY_LOW,true),duration(sendDuration)
    {
        sendingNode = sNode;
        receivingNode = rNode;
    }
    ~Send(){};
    void execute();
    void executeDuplicate();
};


class PacketSend : public Send
{
public:
    PacketSend(TxNode *sNode,RxNode *rNode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f):
    Send(sNode,rNode,newTime,sendDuration)
    {}
    ~PacketSend(){};
};


class RTS : public Send
{
public:
    RTS(TxNode *sNode,RxNode *rnode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f):
    Send(sNode,rnode,newTime,sendDuration)
    {}
    ~RTS(){};
    void execute();
    void executeDuplicate();
};


class CTS : public Send
{
public:
    CTS(TxNode *sNode, RxNode *rnode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f):
    Send(sNode,rnode,newTime,sendDuration)
    {}
    ~CTS(){};
    void execute();
    void executeDuplicate(){ execute();}
};


class Ack : public Send
{
public:
    Ack(TxNode *sNode, RxNode *rnode, sim_time newTime = 0.0f, sim_time sendDuration = 0.00004f):
    Send(sNode,rnode,newTime,sendDuration)
    {}
    ~Ack(){};
    void execute();
};


class EndSend : public Event
{
    TxNode *sendingNode;
public:
    EndSend(TxNode *sNode,sim_time newTime = 0.0f):
    Event(newTime,VERY_LOW)
    { sendingNode = sNode; }
    ~EndSend(){};
    void execute();
    
};


#endif /* defined(__sim_csma__CsmaEvents__) */
