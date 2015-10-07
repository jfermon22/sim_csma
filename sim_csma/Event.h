//
//  Event.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#ifndef __sim_csma__Event__
#define __sim_csma__Event__

#include <stdio.h>
#include <iostream>
#include "SimRandomNumbers.h"

typedef double sim_time;

enum Priority {
    VERY_HIGH,
    HIGH,
    LOW,
    VERY_LOW,
};

class Event
{
protected :
    Event(sim_time newTime = 0, Priority qpri=VERY_LOW);  // queuing pri
    
public :
    virtual ~Event();                   // destructor.
    
    virtual int operator()() = 0;       // empty
    virtual void execute() = 0;
    friend bool operator<(const Event &a,const Event &b);
    //virtual int verify();               // returns OK
    
    //int report(histogram* h,double interval = 0);  // create report
    
    //void stamp();                       // add time stamp
    //double timespent();                 // time since the stamp
    
    bool isActive;
    bool isPending;
    bool isClosed;
    bool isQueued;
    Priority priority;
    sim_time time;
};

#endif /* defined(__sim_csma__Event__) */
