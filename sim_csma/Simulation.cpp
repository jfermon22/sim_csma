//
//  Simulation.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "Simulation.h"

Simulation::Simulation( sim_time stop, sim_time start):
startTime(start),stopTime(stop)
{
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

uint64_t Simulation::queuedEvents(){
    return event_q.size();
}

void Simulation::Run(){
    
    while( curTime < stopTime && !event_q.empty() )
    {
        vector<Event*> curEvents = event_q.getNext();
        if( curEvents.size() > 1 )
            cout << "Got two events with same time:" << (*(curEvents.begin()))->time << endl;
        
        for (vector<Event*>::iterator it = curEvents.begin(); it != curEvents.end(); ++it) {
            curTime = (*it)->time;
            (*it)->execute();
        }
    }
    cout << "Finished sim at time: " << curTime << endl;
}