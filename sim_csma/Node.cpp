//
//  Node.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/7/15.
//
//

#include "Node.h"
#include "SimRandomNumbers.h"
#include "CsmaEvents.h"

const float SLOT_DUR = .00002;

Node::Node(uint32_t newID,Simulation *sim,uint32_t msgFreq):Entity(newID),
	consecutiveCollissions(0),sendFreq(msgFreq)
{
    m_sim = sim;
}
uint32_t Node::id(){
	return identity;
}

void Node::sendSuccess(sim_time duration){
	//lastSendSuccess = true;
	consecutiveCollissions = 0;
	scheduleEndSend(duration);
}

void Node::sendFailure(){
	if (consecutiveCollissions < 10)
        consecutiveCollissions++;
	
	sim_time nextSend = getBackoff();
	cout << "retry " << consecutiveCollissions << ": " << nextSend << endl;
	
    scheduleSend(nextSend);
}

void Node::endTransmit(){
	scheduleSend(random_distro::exponential(sendFreq,random_distro::TEN_USECS));
}

void Node::scheduleSend(sim_time nextSendTime){
    m_sim->ScheduleEvent(new Send(this,nextSendTime));
}

void Node::scheduleEndSend(sim_time endTime){
    m_sim->ScheduleEvent(new EndSend(this,endTime));
}

sim_time Node::getBackoff(){
	uint randSlot = random_distro::rand_int_max((uint16_t)pow(2, consecutiveCollissions));
	cout << "getting rand between [0," << (uint16_t)pow(2, consecutiveCollissions) << "] - " << randSlot << endl;
	return SLOT_DUR * (float)randSlot;
}
