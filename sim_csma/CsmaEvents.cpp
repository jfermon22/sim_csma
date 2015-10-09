//
//  CsmaEvents.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "CsmaEvents.h"


Send::Send(Node *sNode,sim_time newTime,sim_time sendDuration):
    Event(newTime,VERY_LOW)
{
    sendingNode = sNode;
}


void Send::execute(){
    if (sendingNode->channel()->isFree){
        sendingNode->sendSuccess();
        sendingNode->channel()->isFree = false;
        sendingNode->channel()->owner = sendingNode->id();
        //std::cout << time << "," << sendingNode->id() << ",Send" <<  endl;
    } else {
        sendingNode->handleBusy(this);
       // std::cout << time << "," << sendingNode->id() <<  ",RescheduleSend,owner:"<<sendingNode->channel()->owner << endl;
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
    sendingNode->channel()->isFree = true;
    //std::cout << time << "," << sendingNode->id() << ",End" <<  endl;
}

