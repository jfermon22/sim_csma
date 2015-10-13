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

////////////////////////*  RTS EVENT  *////////////////////////

void RTS::execute()
{
    string action("RTS");
    if (sendingNode->channel()->isIdle)
        sendingNode->executeRts();
    else {
        sendingNode->handleBusy();
        action = "AbortRTS_ChanBusy";
    }
#ifdef VERBOSE
    std::cout << time << "," << sendingNode->id() <<  "," << action << endl;
#endif
}

void RTS::executeDuplicate()
{
    if (sendingNode->channel()->isIdle)
        sendingNode->handleCollision();
    else
        sendingNode->handleBusy();
}

////////////////////////*  CTS EVENT  *////////////////////////

void CTS::execute()
{
    string action("CTS");
    if (! sendingNode->channel()->isIdle && sendingNode->channel()->owner ==sendingNode->id() )
    {
        sendingNode->channel()->lastCTSTime = sendingNode->getSimTime();
        sendingNode->schedulePacketSend(duration);
    } else
        action = "AbortCTS_ChanNotIdle or not owned by sender";
    
#ifdef VERBOSE
    std::cout << time << "," << sendingNode->id() <<  "," << action << endl;
#endif
}

////////////////////////*  SEND EVENT  *////////////////////////

void Send::execute()
{
    string action("Send");
    if (sendingNode->channel()->isIdle || sendingNode->channel()->owner == sendingNode->id() )
        sendingNode->executeSend();
    else {
        sendingNode->handleBusy();
        action = "AbortSend_ChanBusy";
    }
    
#ifdef VERBOSE
    std::cout << time << "," << sendingNode->id() << "," << action << endl;
#endif
    
}

void Send::executeDuplicate()
{
    sendingNode->handleCollision();
}

////////////////////////*  END SEND EVENT  *////////////////////////

void EndSend::execute()
{
    sendingNode->endTransmit();
    
#ifdef VERBOSE
    std::cout << time << "," << sendingNode->id() << ",End" <<  endl;
#endif
}

////////////////////////*  ACK EVENT *////////////////////////

void Ack::execute()
{
    string action("ACK");
    if ( ! sendingNode->channel()->isIdle && sendingNode->channel()->owner == sendingNode->id() )
    {
		sendingNode->scheduleChannelFree(duration);
        sendingNode->schedulePacketReady(duration + random_distro::exponential(sendingNode->SendFreq(),random_distro::TEN_USECS));
    } else
        action = "AbortAck_ChanNotIdle or not owned by sender";
    
    
#ifdef VERBOSE
    std::cout << time << "," << sendingNode->id() <<  "," << action << endl;
#endif

}

////////////////////////*  FREE CHANNEL EVENT  *////////////////////////

void FreeChannel::execute()
{
	channel->isIdle = true;
	channel->owner = 999;
#ifdef VERBOSE
    cout <<sim->GetTime()<< ",Channel freed" << endl;
#endif
}

