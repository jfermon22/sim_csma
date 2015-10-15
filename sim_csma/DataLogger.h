//
//  DataLogger.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/14/15.
//
//

#ifndef __sim_csma__DataLogger__
#define __sim_csma__DataLogger__

#include <stdio.h>
#include <stdint.h> 
#include <map>
#include <string>

using namespace std;

struct Data {
    uint32_t index;
    uint32_t freq_a;
    uint32_t freq_c;
    uint32_t thruput_a;
    uint32_t thruput_c;
    float util_a;
    float util_c;
    uint32_t colls;
    uint32_t isHidden;
    uint32_t useVcs;
    uint32_t thruput_tot;
    float util_tot;
    float fairness;
    
    Data(uint32_t idx, uint32_t fa, uint32_t fc,
         uint32_t thrua, uint32_t thruc,
         float utila, float utilc, uint32_t cols,
         uint32_t hide, uint32_t vcs, uint32_t thrutot,
         float utiltot, float fair ):
    index(idx),freq_a(fa),freq_c(fc),thruput_a(thrua),
    thruput_c(thruc),util_a(utila),util_c(utilc),
    colls(cols),isHidden(hide),useVcs(vcs),
    thruput_tot(thrutot),util_tot(utiltot),fairness(fair)
    {}
    
};

class DataVec
{
    map<uint32_t,Data*> myMap;
    float  avg(float, float);
    uint32_t avg(uint32_t,uint32_t);
public:
    DataVec(){}
    void insert(Data d);
    void print();
    void OutputToFile(string filename = "data.csv");
    map<uint32_t, Data*>::iterator begin(){return myMap.begin();}
    map<uint32_t, Data*>::iterator end(){return myMap.end();}

    
};

#endif /* defined(__sim_csma__DataLogger__) */
