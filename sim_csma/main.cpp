//
//  main.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//
#include "Simulation.h"
#include "CsmaEvents.h"

using namespace std;

const uint msgFreq = 100;
const sim_time simDuration = 10;
const sim_time SEND_DUR = .002;
const sim_time SLOT_DUR = .00001;
//const random_distro::precision PRECISION = random_distro::TEN_USECS;


int main(int argc, const char * argv[]) {
    vector<uint32_t> msgFreqVec;
    msgFreqVec.push_back(50);
    msgFreqVec.push_back(100);
    msgFreqVec.push_back(200);
    msgFreqVec.push_back(300);
    msgFreqVec.push_back(400);
    msgFreqVec.push_back(500);
    
    for (uint nodeBFreqScale = 1; nodeBFreqScale <= 2; i++)
    {
        for (vector<uint32_t>::iterator it = msgFreqVec.begin();
             it != msgFreqVec.end(); ++it )
        {
            //init simulation
            Simulation *sim = new Simulation(simDuration);
            Channel *channel = new Channel();
            if (! sim || ! channel) {
                cout << "failed to allocate memory for simulation and channel"<< endl;
                exit(1);
            }
            
            //init nodes
            uint32_t nodeAFreq = *it;
            uint32_t nodeBFreq = *it;
            Node *nodeA = new Node(0,sim,channel,nodeAFreq,SEND_DUR);
            Node *nodeB = new Node(1,sim,channel,nodeBFreq,SEND_DUR);
            if (! nodeA  || ! nodeB ) {
                cout << "failed to allocate memory for nodes"<< endl;
                exit(1);
            }
            
            //seed starting events
            sim->ScheduleEvent(new Send(nodeA,0,SEND_DUR));
            sim->ScheduleEvent(new Send(nodeB,0,SEND_DUR));
            
            //run simulation
            sim->Run();
            
            cout << endl;
            cout << " Sim " :
            cout << "-------------------------------------------" << endl;
            cout << "Node A Lambda: " <<nodeAFreq <<endl;
            cout << "Node B Lambda: " <<nodeBFreq <<endl;
            
            sim->PrintData();
            
            
            // destroy objects
            nodeA = NULL;
            nodeB = NULL;
            channel = NULL;
            sim = NULL;
            delete nodeA;
            delete nodeB;
            delete channel;
            delete sim;
        }
    }
    
    return 0;
}
