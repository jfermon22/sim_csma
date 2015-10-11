//
//  Simulation.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "Simulation.h"

Simulation::Simulation( sim_time stop, sim_time start):
	startTime(start),stopTime(stop),nEvents(0),collisions(0)
{
}

Simulation::~Simulation(){
    event_q.clear();
}


void Simulation::SetStartTime(sim_time newTime){
    startTime = newTime;
}

void Simulation::SetStopTime(sim_time newTime){
    stopTime = newTime;
}

sim_time Simulation::GetTime(){
    return curTime;
}

void Simulation::ScheduleEvent(Event *event){
    event->time += curTime;
    event_q.insert(event);
}
sim_time Simulation::GetNextEventTime(){
    return (event_q.top())->time;
}

uint64_t Simulation::queuedEvents(){
    return event_q.size();
}

void Simulation::Run(){
    
    while( curTime < stopTime && !event_q.empty() )
    {
        vector<Event*> curEvents = event_q.getNext();
        vector<Event*>::iterator it = curEvents.begin();
        curTime = (*it)->time;
        if( curEvents.size() > 1 ) {
            //cout << "-------------------------------------------" << endl;
            //cout << "Got "<< curEvents.size() <<" events with same time:" << (*(curEvents.begin()))->time << endl;
            //cout << "-------------------------------------------" << endl;
            uint sendEvents(0);
            for (vector<Event*>::iterator it = curEvents.begin(); it != curEvents.end(); ++it){
                if ((*it)->isSendAttempt)
                    sendEvents++;
            }
            
            if ( sendEvents > 1) {
                std::cout << (*it)->time << ",Collision" <<  endl;
                collisions++;
            }
            
            for ( vector<Event*>::iterator it = curEvents.begin(); it != curEvents.end(); ++it) {
                (*it)->executeDuplicate();
                nEvents++;
            }
            
        }
        else {
            (*it)->execute();
            nEvents++;
        }
    }
    
}

void Simulation::PrintData(){
    cout << endl;
    cout << "-------------------------------------------" << endl;
    cout << "Final time:    " << curTime << endl;
    cout << "Event Data: " << endl;
    cout << " - In Queue:   " << event_q.size()<< endl;
    cout << " - Executed:   " << nEvents << endl;
    cout << " - Collisions: " << collisions << endl;
    
}
