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


//const uint32_t nIntervals = 10000;
//const uint msgFreq = 500;
const sim_time simDuration = 10;


int main(int argc, const char * argv[]) {
    
    Simulation *sim = new Simulation(simDuration);
    if (! sim) {
        cout << "failed to allocate memory for simulation"<< endl;
        exit(1);
    }

    sim->ScheduleEvent(new Send(0,1.2f));
    sim->ScheduleEvent(new Send(0,5.2f));
    sim->ScheduleEvent(new Send(0,0.2f));
    sim->ScheduleEvent(new Send(0,1.2f));
    sim->ScheduleEvent(new Send(0,0.2f));
    sim->ScheduleEvent(new Send(0,4.2f));
    sim->ScheduleEvent(new Send(0,2.2f));
    sim->ScheduleEvent(new Send(0,3.2f));

    sim->Run();
    
    /*float sum(0);
    for (int intervalNum(0);intervalNum < nIntervals; intervalNum++){
        double num = random_distro::exponential(msgFreq);
        //cout << intervalNum <<", " << num << endl;
        sum+=num;
    }
    
    cout << "average: " <<sum/nIntervals << endl;
    cout << "freq*ave: " << (sum/nIntervals)*msgFreq << endl;
    cout << "msg/sec: " << 1.0/(sum/nIntervals) << endl;*/
    
    delete sim;
    
    return 0;
}
