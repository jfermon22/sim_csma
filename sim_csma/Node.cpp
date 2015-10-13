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

/////////////////////////* RX NODE */////////////////////////////

void RxNode::ScheduleAck(TxNode *txNode,sim_time newTime){
    m_sim->ScheduleEvent(new Ack(txNode,this,newTime,sendDuration));
}

void RxNode::ScheduleCTS(TxNode *txNode,sim_time newTime){
    m_sim->ScheduleEvent(new CTS(txNode,this,newTime,sendDuration));
}

/////////////////////////* TX NODE */////////////////////////////

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
    if(m_usesVcs)
        scheduleRts(nextSend);
    else
        schedulePacketSend(nextSend);
}

void TxNode::handleBusy(Event *event){
    sim_time channelFreeTime(0),backoff(0);
    if(m_usesVcs){
        sim_time lastCntrlTime = (m_channel->lastCTSTime > m_channel->lastRTSTime) ?
        m_channel->lastCTSTime : m_channel->lastRTSTime;
        channelFreeTime = (lastCntrlTime + NAV)- m_sim->GetTime();
        //cout << "channel free in: " <<  channelFreeTime << endl;
    } else {
        channelFreeTime = m_sim->GetNextEventTime() - m_sim->GetTime();
        backoff = getBackoff(0);
    }
    scheduleDifs(new DIFS(this,(channelFreeTime+backoff),difs,slotDuration,true));
}

void TxNode::endTransmit(){
    rxNode->ScheduleAck(this,sifs);
    //channel->isIdle = true;
    //schedulePacketReady(random_distro::exponential(sendFreq,random_distro::TEN_USECS));
}

void TxNode::RtsSuccess(){
    rxNode->ScheduleCTS(this,ack_send_dur +sifs);
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
    if ( difsEvent->isComplete()){
        if ( !m_usesVcs )
            schedulePacketSend(0);
        else
            scheduleRts(0);
    } else
        m_sim->ScheduleEvent(difsEvent);
}

void TxNode::scheduleRts(sim_time sendTime){
    m_sim->ScheduleEvent(new RTS(this,rxNode,sendTime,ack_send_dur));
}

void TxNode::scheduleEndSend(sim_time endTime){
    m_sim->ScheduleEvent(new EndSend(this,endTime));
}


void TxNode::scheduleChannelFree(sim_time freeChannelTime){
    m_sim->ScheduleEvent(new FreeChannel(m_channel,m_sim,freeChannelTime));
}

/////////////////////////* NODE */////////////////////////////

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


