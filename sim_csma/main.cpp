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

const uint msgFreq = 1;
const sim_time simDuration = 10;
const random_distro::precision PRECISION = random_distro::TEN_USECS;


int main(int argc, const char * argv[]) {
    
    Simulation *sim = new Simulation(simDuration);
    if (! sim) {
        cout << "failed to allocate memory for simulation"<< endl;
        exit(1);
    }
    
	Node *nodeA = new Node(0,sim,msgFreq);
	if (! nodeA ) {
        cout << "failed to allocate memory for nodeA"<< endl;
        exit(1);
    }
	
	Node *nodeB = new Node(1,sim,msgFreq);
	if (! nodeB ) {
        cout << "failed to allocate memory for nodeB"<< endl;
        exit(1);
    }

	sim->ScheduleEvent(new Send(nodeA,0));
    sim->ScheduleEvent(new Send(nodeB,0));
    sim->Run();
    
    delete sim;
    
    return 0;
}
