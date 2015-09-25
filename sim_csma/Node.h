//
//  Node.h
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//

#ifndef __sim_csma__Node__
#define __sim_csma__Node__

#include <stdio.h>

class Node {
    bool m_bTransmits;
    
public:
    Node(bool transmitter):m_bTransmits(transmitter){}
    bool DidTransmit(Channel c);
};

#endif /* defined(__sim_csma__Node__) */
