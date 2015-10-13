//
//  Entity.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/7/15.
//
//

#ifndef __sim_csma__Entity__
#define __sim_csma__Entity__

#include <stdio.h>
#include <stdint.h>

class Entity {
protected:
    uint32_t identity;
public:
    Entity(uint32_t newID):identity(newID)
    {}

    
};

#endif /* defined(__sim_csma__Entity__) */
