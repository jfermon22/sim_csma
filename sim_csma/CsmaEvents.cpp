//
//  CsmaEvents.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//


#include "CsmaEvents.h"

/*  DIFS  */

DIFS::DIFS(TxNode *sNode, sim_time execTime,sim_time duration,sim_time slotDur,bool shouldPause):
Event(execTime,VERY_LOW),timeLeft(duration),slotDuration(slotDur),m_shouldPause(shouldPause)
{
    sendingNode = sNode;
}

void DIFS::execute()
{
    if (sendingNode->channel()->isIdle || ! m_shouldPause ) {
        timeLeft -= slotDuration;
#ifdef VERBOSE
         std::cout << sendingNode->getSimTime() << "," << sendingNode->id() <<  ",DIFS,senseTimeLeft: "<< timeLeft << endl;
#endif
    }
    time = slotDuration;
    sendingNode->scheduleDifs(this);
}

bool DIFS::isComplete()
{
    return timeLeft <= 0;
}


/*  PACKET READY EVENT  */

PacketReady::PacketReady(TxNode *sNode,RxNode *rnode,sim_time newTime,sim_time newDifs, sim_time slotDur,bool isretry):
Event(newTime,VERY_LOW),difs(newDifs),slotDuration(slotDur),isRetry(isretry)
{
    sendingNode = sNode;
    rxNode = rnode;
}

void PacketReady::execute(){
        sendingNode->scheduleDifs(new DIFS(sendingNode,0,difs,slotDuration,false));
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() << ",PacketReady" <<  endl;
#endif
}

void PacketReady::scheduleBackoff()
{
    sim_time backoff = sendingNode->getBackoff(0);
    sendingNode->scheduleDifs(new DIFS(sendingNode,backoff,difs,slotDuration,true));
}

/*  RTS EVENT  */
RTS::RTS(TxNode *sNode,RxNode *rnode, sim_time newTime, sim_time sendDuration):
Send(sNode,rnode,newTime,sendDuration)
{
    
}

void RTS::execute()
{
    if (sendingNode->channel()->isIdle){
        sendingNode->RtsSuccess();
        sendingNode->channel()->isIdle = false;
        sendingNode->channel()->owner = sendingNode->id();
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() << ",RTS" <<  endl;
#endif
    } else {
        sendingNode->handleBusy(this);
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() <<  ",AbortSend_ChanBusy"<< endl;
#endif
    }
}

void RTS::executeDuplicate(){
    sendingNode->handleCollision();
}


/*  CTS EVENT  */
CTS::CTS(TxNode *sNode, RxNode *rnode, sim_time newTime, sim_time sendDuration):
Send(sNode,rnode,newTime,sendDuration)
{
    
}

void CTS::execute()
{
    if (! sendingNode->channel()->isIdle && sendingNode->channel()->owner ==sendingNode->id() ){
        sendingNode->schedulePacketSend(duration);
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() << ",CTS" <<  endl;
#endif
    } else {
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() <<  ",AbortCTS_ChanNotIdle or not owned by sender"<< endl;
#endif
    }
}


/*  PACKET SEND EVENT  */
PacketSend::PacketSend( TxNode *sNode, RxNode *rNode, sim_time newTime, sim_time sendDuration):
Send(sNode,rNode,newTime,sendDuration)
{
    
}
                                  
/*  SEND EVENT  */

Send::Send(TxNode *sNode, RxNode *rnode,sim_time newTime,sim_time sendDuration):
    Event(newTime,VERY_LOW,true)
{
    sendingNode = sNode;
    receivingNode = rnode;
}


void Send::execute()
{
    if (sendingNode->channel()->isIdle){
        sendingNode->sendSuccess();
        sendingNode->channel()->isIdle = false;
        sendingNode->channel()->owner = sendingNode->id();
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() << ",Send" <<  endl;
#endif
    } else {
        sendingNode->handleBusy(this);
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() <<  ",AbortSend_ChanBusy"<< endl;
#endif
    }
}

void Send::executeDuplicate(){
	sendingNode->handleCollision();
}

EndSend::EndSend(TxNode *sNode,sim_time newTime):
    Event(newTime,VERY_LOW)
{
    sendingNode = sNode;
}


void EndSend::execute(){
	sendingNode->endTransmit();
    sendingNode->channel()->isIdle = true;
#ifdef VERBOSE
    std::cout << time << "," << sendingNode->id() << ",End" <<  endl;
#endif
}

