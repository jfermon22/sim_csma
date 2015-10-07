//
//  Event.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "Event.h"

Event::Event(sim_time newTime, Priority qpri):
priority(qpri),time(newTime)
{
    
}
Event::~Event()
{
}

bool operator<(Event &a, Event &b) {
    if (a.time == b.time)
        return( a.priority < b.priority );
    else
        return ( a.time < b.time ) ;
}
