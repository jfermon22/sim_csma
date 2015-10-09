//
//  Simulation.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "Simulation.h"

Simulation::Simulation( sim_time stop, sim_time start):
	startTime(start),stopTime(stop),nEvents(0),dupEvents(0)
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
		vector<Event*>::iterator it = curEvents.begin();
		curTime = (*it)->time;
        if( curEvents.size() > 1 ) {
			cout << "-------------------------------------------" << endl;
            cout << "Got "<< curEvents.size() <<" events with same time:" << (*(curEvents.begin()))->time << endl;
			dupEvents++;
			for (; it != curEvents.end(); ++it) {
				(*it)->executeDuplicate();
				nEvents++;
			}
		}
		else {
			(*it)->execute();
			nEvents++;
		}
	}
	cout << "Events left in queue: " << event_q.size()<< endl;
	cout << "Finished sim at time: " << curTime << endl;
	cout << "Events Executed: " << nEvents << endl;
	cout << "Duplicate Events: " << dupEvents << endl;
}
