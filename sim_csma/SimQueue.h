//
//  SimQueue.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#ifndef __sim_csma__SimQueue__
#define __sim_csma__SimQueue__

#include <stdio.h>
#include <queue>
#include <vector>
#include "Event.h"

using namespace std;

class SimQueue {
    priority_queue< Event*, vector<Event*>, ptr_greater<Event*> > myQueue;
public:
    
    void insert(Event *event){ myQueue.push(event); }
    vector<Event*> getNext();
    bool empty() const      { return myQueue.empty();}
    void clear()            { while ( ! myQueue.empty() ) myQueue.pop();}
    uint64_t size() const   { return myQueue.size();}
    Event* top() const      { return myQueue.top();}
};

#endif /* defined(__sim_csma__SimQueue__) */
