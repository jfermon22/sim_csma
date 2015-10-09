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


class Send : public Event 
{
    Node *sendingNode;
	sim_time duration;
public:
    Send(Node *sNode,sim_time newTime = 0.0f,sim_time sendDuration = 0.00001f);
    ~Send(){};
    void execute();
	void executeDuplicate();
    
};

class EndSend : public Event 
{
    Node *sendingNode;
public:
    EndSend(Node *sNode,sim_time newTime = 0.0f);
    ~EndSend(){};
    void execute();
	//void executeDuplicate();
    
};

#endif /* defined(__sim_csma__CsmaEvents__) */
