//
//  Event.h
//  sim_csma
//
//  Created by Jeff Fermon on 10/6/15.
//
//

#ifndef __sim_csma__Event__
#define __sim_csma__Event__

#include <stdio.h>
#include <iostream>
#include "SimRandomNumbers.h"

using namespace std;

typedef float sim_time;

enum Priority {
    VERY_HIGH,
    HIGH,
    LOW,
    VERY_LOW,
};

class Event
{
protected :
    Event(sim_time newTime = 0, Priority qpri=VERY_LOW,bool attemptedSend = false);  // queuing pri
    
public :
    virtual ~Event();                   // destructor.
    
    virtual void SetTime(sim_time newTime);
    virtual void execute() = 0;
	virtual void executeDuplicate(){execute();}       //special execute to call when events scheduled at same time and priority.
	                                                  //if not overridden calls execute
    friend bool operator<(const Event &a,const Event &b);
    friend bool operator>(const Event &a,const Event &b);
    friend ostream& operator<<(std::ostream& os, const Event& obj);
    
    //virtual int verify();               // returns OK
    //int report(histogram* h,double interval = 0);  // create report
    
    //void stamp();                       // add time stamp
    //double timespent();                 // time since the stamp
    bool isActive;
    bool isPending;
    bool isClosed;
    bool isQueued;
    bool isSendAttempt;
    Priority priority;
    sim_time time;
    
};

template<class T>
struct ptr_less : public binary_function<T, T, bool>
{
    bool operator()(const T& left, const T& right) const{
        return ((*left) <( *right));
    }
};

template<class T>
struct ptr_greater : public binary_function<T, T, bool>
{
    bool operator()(const T& left, const T& right) const{
        return ((*left) > ( *right));
    }
};


#endif /* defined(__sim_csma__Event__) */
