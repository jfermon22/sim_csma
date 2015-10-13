//
//  SimQueue.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "SimQueue.h"

void SimQueue::insert(Event *event)
{
    myQueue.push(event);
}

uint64_t SimQueue::size()
{
   return myQueue.size();
}

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

Event* SimQueue::top(){
    return myQueue.top();
}

bool SimQueue::empty(){
    return myQueue.empty();
}


void SimQueue::clear(){
    while ( ! myQueue.empty())
        myQueue.pop();

}
