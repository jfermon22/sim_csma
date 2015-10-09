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

const uint msgFreq = 50;
const sim_time simDuration = 10;
const random_distro::precision PRECISION = random_distro::TEN_USECS;


int main(int argc, const char * argv[]) {
    
    Simulation *sim = new Simulation(simDuration);
    if (! sim) {
        cout << "failed to allocate memory for simulation"<< endl;
        exit(1);
    }
    sim_time eventTime = 0;
    for (uint eventNum(0); eventNum < 5000; eventNum++ ){
        eventTime += random_distro::exponential(msgFreq,PRECISION);
        sim->ScheduleEvent(new Send(0,eventTime));
    }
    
    sim->Run();
    
    delete sim;
    
    return 0;
}
