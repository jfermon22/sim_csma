//
//  Simulation.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "Simulation.h"

void Simulation::Run()
{
    while( curTime < stopTime && !event_q.empty() )
    {
        vector<Event*> curEvents = event_q.getNext();
        vector<Event*>::iterator it = curEvents.begin();
        curTime = (*it)->time;
        uint sendEvents(0);
        if( curEvents.size() > 1 )
        {
            //cout << "-------------------------------------------" << endl;
            //cout << "Got "<< curEvents.size() <<" events with same time:" << (*(curEvents.begin()))->time << endl;
            //cout << "-------------------------------------------" << endl;
            
            for (it = curEvents.begin(); it != curEvents.end(); ++it)
            {
                if ((*it)->isSendAttempt)
                    sendEvents++;
            }
        }
        
        if ( sendEvents > 1){
#ifdef VERBOSE
            std::cout << curTime << ",Collision" <<  endl;
#endif
            collisions++;
        }
        for (it = curEvents.begin(); it != curEvents.end(); ++it)
        {
            if ( sendEvents > 1){
                (*it)->executeDuplicate();
            }else
                (*it)->execute();
            nEvents++;
        }
    }
}

void Simulation::PrintData(){
    cout << endl;
    cout << "-------------------------------------------" << endl;
    cout << "Final time:    " << curTime << endl;
    cout << "Event Data: " << endl;
    cout << " - In Queue:   " << event_q.size()<< endl;
    cout << " - Executed:   " << nEvents << endl;
    cout << " - Collisions: " << collisions << endl;
    
}
