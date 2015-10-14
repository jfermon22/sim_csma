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

void TxNode::executeSend(){
	if(isHiddenNode)
	{
		if( ! m_channel->hiddenTransmittingNode || m_channel->owner == identity ) 
		{
#ifdef VERBOSE
			cout << m_sim->GetTime() << ",free to transmit" << endl;
#endif
			//channel is free
			m_channel->isIdle = false;
			m_channel->owner = identity;
			m_channel->hiddenTransmittingNode = this;
		    scheduleEndSend(sendDuration);
		}
		else
		{
			//collision occured
#ifdef VERBOSE
			cout <<m_sim->GetTime() << ",Hidden Transmitter:"<< m_channel->hiddenTransmittingNode->id()<< endl;
#endif
			handleCollision();
		}
	} 
	else 
	{
		m_channel->isIdle = false;
		m_channel->owner = identity;
		scheduleEndSend(sendDuration);
	}
	
	
}

void TxNode::handleCollision(){
	consecutiveCollissions++;
	totalCollisions++;
	
	if(isHiddenNode && m_channel->hiddenTransmittingNode )
	{
		if ( m_channel->hiddenTransmittingNode != this  ) //collision occured on initial send
		{
#ifdef VERBOSE
			cout << m_sim->GetTime()<<"," << identity << ",Collision" << endl;
#endif
			m_sim->clearQueue();
			m_channel->hiddenTransmittingNode->handleCollision();
		} 
		else 
		{
			if( m_channel->isIdle )
			{
				m_channel->hiddenTransmittingNode =  NULL;
				m_channel->owner = 999;
			}
		}
	}
	
	sim_time nextSend = getBackoff(consecutiveCollissions + 2);
	if(m_usesVcs)
		scheduleRts(nextSend);
	else
		schedulePacketSend(nextSend);
}

void TxNode::handleBusy(){
    sim_time channelFreeTime(0),backoff(0);
    if(m_usesVcs){
		sim_time lastCntrlTime(0);
		if(isHiddenNode)
			lastCntrlTime = m_channel->lastCTSTime;
		else
			lastCntrlTime = (m_channel->lastCTSTime > m_channel->lastRTSTime) ?
									 m_channel->lastCTSTime : m_channel->lastRTSTime;
		
        channelFreeTime =  roundf(((lastCntrlTime + NAV)- m_sim->GetTime()) * 100000.0f)/100000.0f;
		
#ifdef VERBOSE
        cout <<m_sim->GetTime()<<","<<identity <<",channel free in: " <<  channelFreeTime << endl;
#endif
    } else {
        channelFreeTime = m_sim->GetNextEventTime() - m_sim->GetTime();
        backoff = getBackoff(0);
    }
    scheduleDifs(new DIFS(this,(channelFreeTime+backoff),difs,slotDuration,true));
}

void TxNode::endTransmit(){
    rxNode->ScheduleAck(this,sifs);
}
void TxNode::executeAck(){
	consecutiveCollissions = 0;
    successfulSends++;
	scheduleChannelFree(ack_send_dur);
}

void TxNode::executeChannelFree(){
	schedulePacketReady(ack_send_dur + random_distro::exponential(sendFreq,random_distro::TEN_USECS));
}

void TxNode::executeRts()
{
	if(isHiddenNode)
	{
		if( ! m_channel->hiddenTransmittingNode ) 
		{
			//No hidden node transmitting, attempt to capture line
			m_channel->hiddenTransmittingNode = this;
			rxNode->ScheduleCTS(this,ack_send_dur + sifs);
		}
		else
		{
			//Hidden node transmitting, collision occured
			handleCollision();
		}
	} 
	else 
	{
		m_channel->isIdle = false;
		m_channel->owner = identity;
		rxNode->ScheduleCTS(this,ack_send_dur + sifs);
	}
    m_channel->lastRTSTime = m_sim->GetTime();
    
}


bool TxNode::executeCts()
{
	if ( isHiddenNode )
	{
		m_channel->isIdle = false;
		m_channel->owner = identity;
	}
	
	if ( ! m_channel->isIdle && m_channel->owner == identity )
	{
		m_channel->lastCTSTime = getSimTime();
		schedulePacketSend(ack_send_dur);
		return true;
	} else
		return false;
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
    m_sim->ScheduleEvent(new FreeChannel(this,m_channel,m_sim,freeChannelTime));
}

/////////////////////////* NODE */////////////////////////////

sim_time Node::getBackoff(uint32_t nCollisions)
{
	uint32_t MAX_COLL_VAL = 15;
    if ( nCollisions > MAX_COLL_VAL)
		nCollisions = MAX_COLL_VAL;

    uint32_t nBackOffSlots = (uint32_t)pow(2,nCollisions);

    if  ( nBackOffSlots < CWo )
        nBackOffSlots = CWo;
    else if  ( nBackOffSlots > CWmax )
        nBackOffSlots = CWmax;
    
    uint randSlot = random_distro::rand_int_max(nBackOffSlots);
#ifdef VERBOSE
    cout << getSimTime() << ","<< identity <<",Backoff [0," << nBackOffSlots << "] - " << randSlot << endl;
#endif
    
    return slotDuration + slotDuration * (float)randSlot ;
}


