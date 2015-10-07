//
//  Node.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//

#include "Node.h"


Node::Node(bool transmitter):
m_bTransmits(transmitter)
{
    //channel = newChannel;
}

//uint32_t Node::GetCw(){
//    CWo * pow(2,retransmitAttempt);
//    return ;
//}

//float GetNextTransmitTime(float rateParameter){
//        return -logf(1.0f - (float)random() / (RAND_MAX + 1)) / rateParameter;
//}