//
//  CsmaEvents.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//


#include "CsmaEvents.h"

///////////////////////*  DIFS  *////////////////////////

void DIFS::execute()
{
    if (sendingNode->channel()->isIdle || ! m_shouldPause ) {
        timeLeft -= slotDuration;
#ifdef VERBOSE
         //std::cout << sendingNode->getSimTime() << "," << sendingNode->id() <<  ",DIFS,senseTimeLeft: "<< timeLeft << endl;
#endif
    }
    time = slotDuration;
    sendingNode->scheduleDifs(this);
}


////////////////////////*  PACKET READY EVENT  *////////////////////////

void PacketReady::execute()
{
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

////////////////////////*  RTS EVENT  *////////////////////////
void RTS::execute()
{
    if (sendingNode->channel()->isIdle){
        sendingNode->RtsSuccess();
        sendingNode->channel()->isIdle = false;
        sendingNode->channel()->owner = sendingNode->id();        sendingNode->channel()->lastRTSTime = sendingNode->getSimTime();
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() << ",RTS" <<  endl;
#endif
    } else {
        sendingNode->handleBusy(this);
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() <<  ",AbortRTS_ChanBusy,owner:"<<sendingNode->channel()->owner << endl;
#endif
    }
}

void RTS::executeDuplicate()
{
    if (sendingNode->channel()->isIdle)
        sendingNode->handleCollision();
    else
        sendingNode->handleBusy(this);
}

////////////////////////*  CTS EVENT  *////////////////////////

void CTS::execute()
{
    if (! sendingNode->channel()->isIdle && sendingNode->channel()->owner ==sendingNode->id() ){		sendingNode->channel()->lastCTSTime = sendingNode->getSimTime();
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

////////////////////////*  ACK EVENT *////////////////////////

void Ack::execute()
{
    if ( ! sendingNode->channel()->isIdle && sendingNode->channel()->owner == sendingNode->id() ){
		sendingNode->scheduleChannelFree(duration);
        sendingNode->schedulePacketReady(duration + random_distro::exponential(sendingNode->SendFreq(),random_distro::TEN_USECS));
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() << ",ACK" <<  endl;
#endif
    } else {
#ifdef VERBOSE
        std::cout << time << "," << sendingNode->id() <<  ",AbortAck_ChanNotIdle or not owned by sender"<< endl;
#endif
    }
}

////////////////////////*  SEND EVENT  *////////////////////////

void Send::execute()
{
    if (sendingNode->channel()->isIdle || sendingNode->channel()->owner == sendingNode->id()){
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

void Send::executeDuplicate()
{
	sendingNode->handleCollision();
}

////////////////////////*  END SEND EVENT  *////////////////////////

void EndSend::execute()
{
	sendingNode->endTransmit();
   // sendingNode->channel()->isIdle = true;
#ifdef VERBOSE
    std::cout << time << "," << sendingNode->id() << ",End" <<  endl;
#endif
}

////////////////////////*  FREE CHANNEL EVENT  *////////////////////////

void FreeChannel::execute()
{
#ifdef VERBOSE
	cout <<sim->GetTime()<< ",Channel freed" << endl;
#endif
	channel->isIdle = true;
	channel->owner = 999;
}

