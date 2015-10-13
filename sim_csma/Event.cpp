//
//  Event.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#include "Event.h"

bool operator<(const Event &a, const Event &b) {
    if (a.time == b.time)
        return( a.priority < b.priority );
    else
        return ( a.time < b.time ) ;
}

bool operator>(const Event &a, const Event &b) {
    if (a.time == b.time)
        return( a.priority > b.priority );
    else
        return ( a.time > b.time ) ;
}

ostream& operator<<(std::ostream& os, const Event& obj)
{
    os << obj.time << "," << obj.priority;
    return os;
}
