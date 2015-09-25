//
//  CsmaSim.h
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//

#ifndef __sim_csma__CsmaSim__
#define __sim_csma__CsmaSim__

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "Node.h"

using namespace std;

#define DATA_PACKET_SZ_BYTES 1500
#define SLOT_DUR_US 20
#define SIFS_DUR 10
#define DIFS_DUR 40
#define ACK_SZ 8
#define RTS_SZ 8
#define CTS_SZ 8
#define TRANSM_RATE_Mbps 6
#define CWo 8
#define CWmax 1024
#define SIM_TIME_SECS 10

struct ScenarioMetrics {
    uint16_t lambda;
    float throughput; //The aggregate and individual throughput as a function of lambda
    float collisions; //The number of collisions (data and RTS/CTS) as a function of lambda.
    float link_utilization; //U(%): The percent fraction of time that the medium is used for the transmission of useful data as a function of lambda
    float fairness_index; // The fraction of time that the channel is occupied by pair A->B over the fraction of time that the channel is occupied by pair C->D as a function of lambda
};

class CsmaSim {
   
protected:
    Node A;
    Node B;
    Node C;
    Node D;
    sem_t
    vector<uint16_t> m_vecLambda;
    ScenarioMetrics m_strMetrics;
    
public:
    CsmaSim();
};

#endif /* defined(__sim_csma__CsmaSim__) */
