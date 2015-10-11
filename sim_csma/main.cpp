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

//const uint msgFreq = 100;
const sim_time simDuration = 10;
const sim_time SEND_DUR = 0.002;
const sim_time SLOT_DUR = 0.00001;
const sim_time DIFS = 0.00004;
const uint32_t PACKETS_PER_BYTE = 1500;
//const random_distro::precision PRECISION = random_distro::TEN_USECS;


int main(int argc, const char * argv[]) {
    vector<uint32_t> msgFreqVec;
    msgFreqVec.push_back(50);
    msgFreqVec.push_back(100);
    msgFreqVec.push_back(200);
    msgFreqVec.push_back(300);
    msgFreqVec.push_back(400);
    msgFreqVec.push_back(500);
    
    uint jjj(0);
    for (uint nodeAFreqScale = 1; nodeAFreqScale <= 2; nodeAFreqScale++)
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
            uint32_t nodeAFreq = *it*nodeAFreqScale;
            uint32_t nodeCFreq = (*it);
            
            //init nodes
            Node *nodeA = new Node(0,sim,channel,nodeAFreq,DIFS,SEND_DUR,SLOT_DUR);
            Node *nodeC = new Node(1,sim,channel,nodeCFreq,DIFS,SEND_DUR,SLOT_DUR);
            if (! nodeA  || ! nodeC ) {
                cout << "failed to allocate memory for nodes"<< endl;
                exit(1);
            }
            
            //seed starting events
            sim->ScheduleEvent(new Send(nodeA,DIFS,SEND_DUR));
            sim->ScheduleEvent(new Send(nodeC,DIFS,SEND_DUR));
            
            //run simulation
            sim->Run();
            
            
            
            uint32_t aThruput = nodeA->SuccessfulSends() * PACKETS_PER_BYTE;
            float aUtil = (((float)nodeA->SuccessfulSends()*SEND_DUR)/(float)simDuration) * 100.0f;
            uint32_t cThruput = nodeC->SuccessfulSends() * PACKETS_PER_BYTE;
            float cUtil = (((float)nodeC->SuccessfulSends()*SEND_DUR)/(float)simDuration) * 100.0f;
            
            cout << "------------------------------------------------------------" << endl;
            cout << " Simulation: " << jjj << endl;
            cout << "Node A :" << endl;
            cout << "   Lambda: " <<nodeAFreq <<endl;
            //cout << "   Sends: " << nodeA->SuccessfulSends()<< endl;
            cout << "   Throughput: " << aThruput << " Bytes" << endl;
            cout << "   Collisions: " << nodeA->TotalCollisions()<< endl;
            cout << "   Utilization: " <<aUtil << "%"<< endl;
            cout << "Node C :" << endl;
            cout << "   Lambda: " <<nodeCFreq <<endl;
            //cout << "   Sends: " << nodeC->SuccessfulSends()<< endl;
            cout << "   Throughput: " << cThruput << " Bytes" << endl;
            cout << "   Collisions: " << nodeC->TotalCollisions()<< endl;
            cout << "   Utilization: " << cUtil<< "%"<< endl;
            cout << "Total:"<< endl;
            cout << "   Throughput: " << aThruput + cThruput << " Bytes" << endl;
            cout << "   FairnessIndex (A:C): " << aUtil/cUtil << endl;
            
            sim->PrintData();
            cout << endl << endl;
            
            // destroy objects
            nodeA = NULL;
            nodeC = NULL;
            channel = NULL;
            sim = NULL;
            delete nodeA;
            delete nodeC;
            delete channel;
            delete sim;
            jjj++;
        }
    }
    
    return 0;
}
