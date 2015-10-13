//
//  SimQueue.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "SimQueue.h"

vector<Event*> SimQueue::getNext()
{
    vector<Event*> curEventsVec;
    do
    {
        curEventsVec.push_back(myQueue.top());
        myQueue.pop();
        
    } while ( ! myQueue.empty() &&
             (*(curEventsVec.begin()))->time == (myQueue.top())->time &&
             (*(curEventsVec.begin()))->priority == (myQueue.top())->priority );
    
    return curEventsVec;
}

