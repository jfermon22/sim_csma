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

class Simulation {
protected:
    sim_time startTime;
    sim_time stopTime;
    sim_time curTime;
    SimQueue event_q;
    uint64_t nEvents;
	uint32_t collisions;
    
public:
    Simulation(sim_time stop, sim_time start = 0);
    ~Simulation();
    void Run();
    void Stop();
    void PrintData();
    void ScheduleEvent(Event *event);
    //void TerminateEvent(Event *event);
    
    void SetStartTime(sim_time newTime);
    void SetStopTime(sim_time newTime);
    sim_time GetTime();
    sim_time GetNextEventTime();
    uint64_t queuedEvents();
    
};

#endif /* defined(__sim_csma__Simulation__) */
