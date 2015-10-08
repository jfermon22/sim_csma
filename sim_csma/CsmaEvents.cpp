//
//  CsmaEvents.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "CsmaEvents.h"


Send::Send(uint32_t Id,sim_time newTime):
    Event(newTime,VERY_LOW),nodeId(Id)
{
    
}


void Send::execute(){
    std::cout << "Executing " << *this << std::endl;
}