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
    std::priority_queue<Event*> myQueue;
public:
    
    void insert(Event *event);
    vector<Event*> getNext();
    bool empty();
};

#endif /* defined(__sim_csma__SimQueue__) */
