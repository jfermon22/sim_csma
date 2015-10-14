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
    if (sendingNode->channel()->isIdle){
#ifdef VERBOSE
		std::cout << time << "," << sendingNode->id() <<  "," << action << endl;
#endif
        sendingNode->executeRts();
    } else {
		if( sendingNode->channel()->owner  == sendingNode->id() )//already owns channel. This event is garbage.
		   return;
	   
		action = "AbortRTS_ChanBusy";
#ifdef VERBOSE
		//std::cout << time << "," << sendingNode->id() <<  "," << action << ",owner:"<< sendingNode->channel()->owner << endl;
#endif
        sendingNode->handleBusy();
        
    }
#ifdef VERBOSE
	// std::cout << time << "," << sendingNode->id() <<  "," << action << endl;
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
	if (! sendingNode->executeCts() )
		action = "AbortCTS_ChanNotIdle or not owned by sender";
	
#ifdef VERBOSE
    std::cout << time << "," << sendingNode->id() <<  "," << action << endl;
#endif
}

void CTS::executeDuplicate()
{
	if ( sendingNode->IsHiddenNode() && sendingNode->UsesVCS() && 
			sendingNode->channel()->isIdle && 
			sendingNode->channel()->hiddenTransmittingNode )
	{
		if ( sendingNode->channel()->hiddenTransmittingNode == sendingNode )
			sendingNode->handleCollision();
    } else
        execute();
}

////////////////////////*  SEND EVENT  *////////////////////////

void Send::execute()
{
    string action("Send");

	if (sendingNode->channel()->isIdle || sendingNode->channel()->owner == sendingNode->id() || sendingNode->IsHiddenNode() ){
#ifdef VERBOSE
		std::cout << time << "," << sendingNode->id() << "," << action << endl;
#endif
		sendingNode->executeSend();
	}else {
		action = "AbortSend_ChanBusy";
#ifdef VERBOSE
		std::cout << time << "," << sendingNode->id() << "," << action << ",owner:"<< sendingNode->channel()->owner << endl;
#endif
		sendingNode->handleBusy();
	}

#ifdef VERBOSE
	//std::cout << time << "," << sendingNode->id() << "," << action << endl;
#endif

}

void Send::executeDuplicate()
{
	if ( sendingNode->UsesVCS() && sendingNode->channel()->owner == sendingNode->id())
		execute();
	else
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
    if ( ! sendingNode->channel()->isIdle && sendingNode->channel()->owner == sendingNode->id() || sendingNode->IsHiddenNode() )
    {
		sendingNode->executeAck();
		//sendingNode->scheduleChannelFree(duration);
        //sendingNode->schedulePacketReady(duration + random_distro::exponential(sendingNode->SendFreq(),random_distro::TEN_USECS));
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
	channel->hiddenTransmittingNode = NULL;
	node->executeChannelFree();
#ifdef VERBOSE
    cout <<sim->GetTime()<< "," <<node->id() << ",Channel freed" << endl;
#endif
}

void Collision::execute(){
	//do some stuff
}

