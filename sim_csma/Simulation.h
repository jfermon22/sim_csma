//
//  Simulation.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#ifndef __sim_csma__Simulation__
#define __sim_csma__Simulation__

#include <stdio.h>
#include "Event.h"
#include "SimQueue.h"

//#define VERBOSE

class Simulation {
protected:
    sim_time startTime;
    sim_time stopTime;
    sim_time curTime;
    SimQueue event_q;
    uint64_t nEvents;
	uint32_t collisions;
    
public:
    Simulation(sim_time stop, sim_time start = 0):
    startTime(start),stopTime(stop),nEvents(0),collisions(0)
    {}
    ~Simulation(){ event_q.clear(); }
    void Run();
    void Stop();
    void PrintData();
    void ScheduleEvent(Event *event){ event->time += curTime; event_q.insert(event); }
    void SetStartTime(sim_time newTime){startTime = newTime;}
    void SetStopTime(sim_time newTime) { stopTime = newTime;}
    sim_time GetTime()          const { return curTime;}
    sim_time GetNextEventTime() const { return (event_q.top())->time;}
    uint64_t queuedEvents()     const { return event_q.size(); }
	void clearQueue()                 { event_q.clear(); }
    
};

#endif /* defined(__sim_csma__Simulation__) */
