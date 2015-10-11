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

Node::Node(uint32_t newID,Simulation *sim,Channel *channel, uint32_t msgFreq,
           sim_time difsTime,sim_time sendDur,sim_time slotDur ):Entity(newID ),
consecutiveCollissions(0),totalCollisions(0),successfulSends(0),
sendFreq(msgFreq),sendDuration(sendDur),
slotDuration(slotDur),difs(difsTime)
{
    m_sim = sim;
    m_channel = channel;
}
uint32_t Node::id(){
	return identity;
}

void Node::sendSuccess(){
	consecutiveCollissions = 0;
    successfulSends++;
	scheduleEndSend(sendDuration);
}

void Node::handleCollision(){
    consecutiveCollissions++;
    totalCollisions++;
	sim_time nextSend = getBackoff(consecutiveCollissions + 2);
	//cout << m_sim->GetTime() << "," << identity <<",Collision, " << totalCollisions << endl;
	
    scheduleSend(nextSend);
}

void Node::handleBusy(Event *event){
    sim_time channelFreeTime = m_sim->GetNextEventTime() - m_sim->GetTime();
    sim_time backoff = getBackoff(0);
    scheduleDifs(new DIFS(this,(channelFreeTime+backoff),difs,slotDuration,true));
    //sim_time nextSendTime = rescheduleTime - m_sim->GetTime();
    //event->SetTime(nextSendTime);
    //m_sim->ScheduleEvent(event);
}

void Node::endTransmit(){
	schedulePacketReady(random_distro::exponential(sendFreq,random_distro::TEN_USECS));
}

void Node::schedulePacketReady(sim_time eventTime){
    m_sim->ScheduleEvent(new PacketReady(this,eventTime,difs,slotDuration));
}

/*void Node::reschedulePacketReady(Event *packReadyEvent){
    sim_time packetBackoff = packReadyEvent->time + difs;
    sim_time channelFreeTime = m_sim->GetNextEventTime();
    sim_time rescheduleTime = channelFreeTime + difs;

    sim_time nextSendTime = rescheduleTime - m_sim->GetTime();
    sim_time
    if (packetBackoff < nextSendTime ) {
       lafn
    }
   
    event->SetTime(nextSendTime);
    
   
    m_sim->ScheduleEvent(packReadyEvent);
}*/

void Node::scheduleSend(sim_time nextSendTime){
    m_sim->ScheduleEvent(new Send(this,nextSendTime,sendDuration));
}

void Node::scheduleEndSend(sim_time endTime){
    m_sim->ScheduleEvent(new EndSend(this,endTime));
}

void Node::scheduleDifs(Event *event)
{
    DIFS *difsEvent = static_cast<DIFS*>(event);
    if ( difsEvent->isComplete())
        scheduleSend(0);
    else
        m_sim->ScheduleEvent(difsEvent);
}

sim_time Node::getBackoff(uint32_t nCollisions)
{
    uint16_t nBackOffSlots = (uint16_t)pow(2,nCollisions);
    
    if  ( nBackOffSlots < CWo )
        nBackOffSlots = CWo;
    else if  ( nBackOffSlots > CWmax )
        nBackOffSlots = CWmax;
    
	uint randSlot = random_distro::rand_int_max(nBackOffSlots);
	//cout << identity <<" Backoff [0," << nBackOffSlots << "] - " << randSlot << endl;
    
	return slotDuration * (float)randSlot;
}


Channel* Node::channel(){
    return m_channel;
}

sim_time Node::getSimTime(){
    
    return m_sim->GetTime();
}

