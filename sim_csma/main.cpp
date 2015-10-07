//
//  main.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//
#include "Simulation.h"

using namespace std;

uint32_t intervalNum = 1;
const uint32_t nIntervals = 1000;


int main(int argc, const char * argv[]) {
    float sum(0);
    cout << "Event Num     TImeline Val " << endl;
    
    srand(time(NULL));
    for (;intervalNum < nIntervals; intervalNum ++){
        double num = random_distro::exponential(50);
        cout << intervalNum <<", " << num << endl;
        sum+=num;
    }
    
    cout << "average: " <<sum/nIntervals << endl;
    return 0;
}
