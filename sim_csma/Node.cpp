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

Node::Node(uint32_t newID,Simulation *sim,Channel *channel, uint32_t msgFreq,sim_time sendDur,sim_time slotDur ):Entity(newID),
	consecutiveCollissions(0),sendFreq(msgFreq),sendDuration(sendDur),slotDuration(slotDur)
{
    m_sim = sim;
    m_channel = channel;
}
uint32_t Node::id(){
	return identity;
}

void Node::sendSuccess(){
	consecutiveCollissions = 0;
	scheduleEndSend(sendDuration);
}

void Node::handleCollision(){
	if (consecutiveCollissions < 10)
        consecutiveCollissions++;
	
	sim_time nextSend = getBackoff();
	//cout << "retry " << consecutiveCollissions << ": " << nextSend << endl;
	
    scheduleSend(nextSend);
}

void Node::handleBusy(Event *event){
    sim_time channelFreeTime = m_sim->GetNextEventTime();
    sim_time rescheduleTime = channelFreeTime + slotDuration;
    sim_time nextSendTime = rescheduleTime - m_sim->GetTime();
    event->SetTime(nextSendTime);
    m_sim->ScheduleEvent(event);
}


void Node::endTransmit(){
	scheduleSend(random_distro::exponential(sendFreq,random_distro::TEN_USECS));
}

void Node::scheduleSend(sim_time nextSendTime){
    m_sim->ScheduleEvent(new Send(this,nextSendTime,sendDuration));
}

void Node::scheduleEndSend(sim_time endTime){
    m_sim->ScheduleEvent(new EndSend(this,endTime));
}

sim_time Node::getBackoff(){
	uint randSlot = random_distro::rand_int_max((uint16_t)pow(2, consecutiveCollissions));
	//cout << "getting rand between [0," << (uint16_t)pow(2, consecutiveCollissions) << "] - " << randSlot << endl;
	return slotDuration * (float)randSlot;
}


Channel* Node::channel(){
    return m_channel;
}
