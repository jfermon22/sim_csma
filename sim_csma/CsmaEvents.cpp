//
//  CsmaEvents.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "CsmaEvents.h"

DIFS::DIFS(Node *sNode,sim_time execTime,sim_time duration,sim_time slotDur,bool shouldPause):
Event(execTime,VERY_LOW),timeLeft(duration),slotDuration(slotDur),m_shouldPause(shouldPause)
{
    sendingNode = sNode;
}

void DIFS::execute()
{
    if (sendingNode->channel()->isIdle || ! m_shouldPause ) {
        timeLeft -= slotDuration;
         //std::cout << sendingNode->getSimTime() << "," << sendingNode->id() <<  ",DIFS,senseTimeLeft: "<<timeLeft << endl;
    }
    time = slotDuration;
    sendingNode->scheduleDifs(this);
}

bool DIFS::isComplete()
{
    return timeLeft <= 0;
}


/*  PACKET READY EVENT  */

PacketReady::PacketReady(Node *sNode,sim_time newTime,sim_time newDifs, sim_time slotDur,bool isretry):
Event(newTime,VERY_LOW),difs(newDifs),slotDuration(slotDur),isRetry(isretry)
{
    sendingNode = sNode;
}

void PacketReady::execute(){
        sendingNode->scheduleDifs(new DIFS(sendingNode,0,difs,slotDuration,false));
       // std::cout << time << "," << sendingNode->id() << ",PacketReady" <<  endl;
}

/*void PacketReady::scheduleSend(sim_time sendTime)
{
    sendingNode->scheduleSend(sendTime);
}*/

void PacketReady::scheduleBackoff()
{
    sim_time backoff = sendingNode->getBackoff(0);
    sendingNode->scheduleDifs(new DIFS(sendingNode,backoff,difs,slotDuration,true));
}



                                  
/*  SEND EVENT  */

Send::Send(Node *sNode,sim_time newTime,sim_time sendDuration):
    Event(newTime,VERY_LOW,true)
{
    sendingNode = sNode;
}


void Send::execute()
{
    if (sendingNode->channel()->isIdle){
        sendingNode->sendSuccess();
        sendingNode->channel()->isIdle = false;
        sendingNode->channel()->owner = sendingNode->id();
        //std::cout << time << "," << sendingNode->id() << ",Send" <<  endl;
    } else {
        sendingNode->handleBusy(this);
       // std::cout << time << "," << sendingNode->id() <<  ",AbortSend_ChanBusy"<< endl;
    }
}

void Send::executeDuplicate(){
	sendingNode->handleCollision();
}

EndSend::EndSend(Node *sNode,sim_time newTime):
    Event(newTime,VERY_LOW)
{
    sendingNode = sNode;
}


void EndSend::execute(){
	sendingNode->endTransmit();
    sendingNode->channel()->isIdle = true;
    //std::cout << time << "," << sendingNode->id() << ",End" <<  endl;
}

