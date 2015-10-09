//
//  CsmaEvents.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "CsmaEvents.h"


Send::Send(Node *sNode,sim_time newTime,sim_time sendDuration):
    Event(newTime,VERY_LOW),duration(sendDuration)
{
    sendingNode = sNode;
}


void Send::execute(){
	sendingNode->sendSuccess(duration);
    std::cout << "Executing Send: " << *this << " " << sendingNode->id() << "->" <<endl;
}

void Send::executeDuplicate(){
	std::cout << "Executing Send Duplicates: " << *this << " " << sendingNode->id() << "->" <<endl;
	sendingNode->sendFailure();
    
}

EndSend::EndSend(Node *sNode,sim_time newTime):
    Event(newTime,VERY_LOW)
{
    sendingNode = sNode;
}


void EndSend::execute(){
	sendingNode->endTransmit();
    std::cout << "Executing End: " << *this << " " << sendingNode->id() << "->" <<endl;
}

