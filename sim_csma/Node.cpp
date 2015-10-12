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

RxNode::RxNode(uint newId, Simulation *sim, Channel *channel,
               sim_time sendDur,sim_time slotDur ):
Node(newId,sim,channel, 1,0,sendDur,slotDur )
{
    
    
}

void RxNode::ScheduleAck(TxNode *txNode,sim_time newTime){
    txNode->schedulePacketReady(random_distro::exponential(sendFreq,random_distro::TEN_USECS));
    
}

void RxNode::ScheduleCTS(TxNode *txNode,sim_time newTime){
      m_sim->ScheduleEvent(new CTS(txNode,this,newTime,sendDuration));
}



TxNode::TxNode(uint newId, Simulation *sim, Channel *channel, RxNode *rNode,
       uint32_t sendMsgFreq,sim_time difsTime, sim_time sifstime,
               sim_time sendDur,sim_time ackRtsCtsDur, sim_time slotDur,bool useVcs ):
Node(newId,sim,channel,sendMsgFreq,difsTime,sendDur,slotDur ),
m_usesVcs(useVcs),sifs(sifstime),rxNode(rNode)
{
    
}
void TxNode::sendSuccess(){
    consecutiveCollissions = 0;
    successfulSends++;
    scheduleEndSend(sendDuration);
}

void TxNode::handleCollision(){
    consecutiveCollissions++;
    totalCollisions++;
    sim_time nextSend = getBackoff(consecutiveCollissions + 2);
    //cout << m_sim->GetTime() << "," << identity <<",Collision, " << totalCollisions << endl;
    
    schedulePacketSend(nextSend);
}

void TxNode::handleBusy(Event *event){
    sim_time channelFreeTime = m_sim->GetNextEventTime() - m_sim->GetTime();
    sim_time backoff = getBackoff(0);
    scheduleDifs(new DIFS(this,(channelFreeTime+backoff),difs,slotDuration,true));
}

void TxNode::endTransmit(){
    schedulePacketReady(random_distro::exponential(sendFreq,random_distro::TEN_USECS));
}

void TxNode::RtsSuccess(){
    rxNode->ScheduleCTS(this,ack_send_dur);
}

void TxNode::schedulePacketReady(sim_time eventTime){
    m_sim->ScheduleEvent(new PacketReady(this,rxNode,eventTime,difs,slotDuration));
}

void TxNode::schedulePacketSend(sim_time nextSendTime){
    m_sim->ScheduleEvent(new Send(this,rxNode,nextSendTime,sendDuration));
}

void TxNode::scheduleDifs(Event *event)
{
    DIFS *difsEvent = static_cast<DIFS*>(event);
    if ( difsEvent->isComplete())
        schedulePacketSend(0);
    else
        m_sim->ScheduleEvent(difsEvent);
}

void TxNode::scheduleEndSend(sim_time endTime){
    m_sim->ScheduleEvent(new EndSend(this,endTime));
}

Node::Node(uint32_t newID,Simulation *sim,Channel *channel, uint32_t msgFreq,
           sim_time difsTime,sim_time sendDur,sim_time acksenddur,sim_time slotDur ):Entity(newID ),
consecutiveCollissions(0),totalCollisions(0),successfulSends(0),
sendFreq(msgFreq),sendDuration(sendDur),ack_send_dur(acksenddur),
slotDuration(slotDur),difs(difsTime)
{
    m_sim = sim;
    m_channel = channel;
}
uint32_t Node::id(){
	return identity;
}








sim_time Node::getBackoff(uint32_t nCollisions)
{
    uint16_t nBackOffSlots = (uint16_t)pow(2,nCollisions);
    
    if  ( nBackOffSlots < CWo )
        nBackOffSlots = CWo;
    else if  ( nBackOffSlots > CWmax )
        nBackOffSlots = CWmax;
    
	uint randSlot = random_distro::rand_int_max(nBackOffSlots);
#ifdef VERBOSE
	cout << identity <<" Backoff [0," << nBackOffSlots << "] - " << randSlot << endl;
#endif
    
	return slotDuration * (float)randSlot;
}


Channel* Node::channel(){
    return m_channel;
}

sim_time Node::getSimTime(){
    
    return m_sim->GetTime();
}

