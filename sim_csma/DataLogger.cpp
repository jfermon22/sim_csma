//
//  DataLogger.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/14/15.
//
//

#include "DataLogger.h"
#include <math.h>
#include <fstream>

void DataVec::insert(Data d)
{
    
    if ( ! myMap[d.index] )
    {
        myMap[d.index] = new Data(d.index,d.freq_a,d.freq_c,d.thruput_a,d.thruput_c,d.util_a,d.util_c,d.colls,d.isHidden,d.useVcs,d.thruput_tot,d.util_tot,d.fairness);
    }
                                  
    Data *curData = myMap[d.index];
    //uint32_t freq_a= avg(curData->freq_a,d.freq_a);
    //uint32_t freq_c= avg(curData->freq_c,d.freq_c);
    float thruput_a = avg(curData->thruput_a,d.thruput_a);
    float thruput_c= avg(curData->thruput_c,d.thruput_c);
    float util_a= avg(curData->util_a,d.util_a);
    float util_c= avg(curData->util_c,d.util_c);
    float colls= avg(curData->colls,d.colls);
    float thruput_tot= avg(curData->thruput_tot,d.thruput_tot);
    float util_tot= avg(curData->util_tot,d.util_tot);
    float fairness= avg(curData->fairness,d.fairness);
    
    Data *newData = new Data(d.index,d.freq_a,d.freq_c,thruput_a,thruput_c,util_a,util_c,colls,d.isHidden,d.useVcs,thruput_tot,util_tot,fairness);
    
    myMap[d.index] = newData;
}


float  DataVec::avg(float num1, float num2){
    return ((num1+num2)/2.0f);
}

uint32_t DataVec::avg(uint32_t num1, uint32_t num2){
    return ceilf(((static_cast<float>(num1)+static_cast<float>(num2))/2.0f));
}

void DataVec::print()
{
    for ( map<uint32_t,Data*>::iterator it = myMap.begin(); it != myMap.end(); it++)
    {
        uint32_t index = it->first;
        printf("%u,%u,%s,%u\n",index,0,"Lambda",it->second->freq_a);
        printf("%u,%u,%s,%.2f\n",index,0,"Throughput",it->second->thruput_a);
        printf("%u,%u,%s,%.2f\n",index,0,"Utilization",it->second->util_a);
        printf("%u,%u,%s,%u\n",index,1,"Lambda",it->second->freq_c);
        printf("%u,%u,%s,%.2f\n",index,1,"Throughput",it->second->thruput_c);
        printf("%u,%u,%s,%.2f\n",index,1,"Utilization",it->second->util_c);
        printf("%u,%u,%s,%.2f\n",index,2,"Collisions",it->second->colls);
        printf("%u,%u,%s,%u\n",index,2,"HiddenNode",it->second->isHidden);
        printf("%u,%u,%s,%u\n",index,2,"VCS",it->second->useVcs);
        printf("%u,%u,%s,%.2f\n",index,2,"Throughput",it->second->thruput_tot);
        printf("%u,%u,%s,%.2f\n",index,2,"Utilization",it->second->util_tot);
        printf("%u,%u,%s,%.2f\n",index,2,"Fairnessindex",it->second->fairness);
    }
}

void DataVec::OutputToFile(string filename){
    
    ofstream myfile;
    myfile.open (filename);
    for ( map<uint32_t,Data*>::iterator it = myMap.begin(); it != myMap.end(); it++)
    {
        char buffer [50];
        uint32_t index = it->first;
        sprintf(buffer,"%u,%u,%s,%u\n",index,0,"Lambda",it->second->freq_a);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%.2f\n",index,0,"Throughput",it->second->thruput_a);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%.2f\n",index,0,"Utilization",it->second->util_a);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%u\n",index,1,"Lambda",it->second->freq_c);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%.2f\n",index,1,"Throughput",it->second->thruput_c);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%.2f\n",index,1,"Utilization",it->second->util_c);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%.2f\n",index,2,"Collisions",it->second->colls);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%u\n",index,2,"HiddenNode",it->second->isHidden);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%u\n",index,2,"VCS",it->second->useVcs);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%.2f\n",index,2,"Throughput",it->second->thruput_tot);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%.2f\n",index,2,"Utilization",it->second->util_tot);
        myfile << buffer;
        sprintf(buffer,"%u,%u,%s,%.2f\n",index,2,"Fairnessindex",it->second->fairness);
        myfile << buffer;
    }
    
    myfile.close();
}
