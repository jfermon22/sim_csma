//
//  main.cpp
//  sim_csma
//
//  Created by Jeff Fermon on 9/24/15.
//
//
#include "Simulation.h"
#include "CsmaEvents.h"
#include <map>

using namespace std;

const sim_time simDuration = 10;
const sim_time PACKET_SEND_DUR = 0.002;
const sim_time ACK_RTS_CTS_SND_DUR = 0.00004;
const sim_time SLOT_DUR = 0.00001;
const sim_time DIFS = 0.00004;
const sim_time SIFS = 0.00001;
//const uint32_t VCS_OVERHEAD_BYTES = 60;
const uint32_t PACKET_SIZE_BYTES = 1500;
const uint32_t ACK_SIZE_BYTES = 30;
const sim_time VCS_RTT = (PACKET_SEND_DUR + SIFS + 3 * ACK_RTS_CTS_SND_DUR);
const sim_time RTT = (PACKET_SEND_DUR + SIFS + ACK_RTS_CTS_SND_DUR);

int main(int argc, const char * argv[]) {
    argc = 0;
    argv = NULL;
    
    vector<uint32_t> msgFreqVec;
    msgFreqVec.push_back(50);
    msgFreqVec.push_back(100);
    msgFreqVec.push_back(200);
    msgFreqVec.push_back(300);
    msgFreqVec.push_back(400);
    msgFreqVec.push_back(500);
    
    uint jjj(0);
    cout << "------------------------------------------------------------" << endl;
    cout << " KEY"<< endl;
    cout << "Sim Num,Node,Attribute,Value"<< endl;
    cout << "Node: 0 = A, 1 = C, 2 = Total"<< endl;
    cout << "------------------------------------------------------------" << endl;
    
    map <uint,float> fairnessIndexMap;
//    for (uint hiddenNode = 0; hiddenNode <= 1; hiddenNode++ )
//    {
        for (uint useVcs = 0; useVcs <= 1; useVcs++ )
        {
            for (uint nodeAFreqScale = 1; nodeAFreqScale <= 2; nodeAFreqScale++)
            {
                for (vector<uint32_t>::iterator it = msgFreqVec.begin();
                     it != msgFreqVec.end(); ++it )
                {
                    //init simulation
                    Simulation *sim = new Simulation(simDuration);
                    Channel *channel = new Channel();
                    if (! sim || ! channel) {
                        cout << "failed to allocate memory for simulation and channel"<< endl;
                        exit(1);
                    }
                    
                    //init nodes
                    uint32_t nodeAFreq = *it*nodeAFreqScale;
                    uint32_t nodeCFreq = (*it);
                    
                    //init nodes
                    RxNode *nodeB = new RxNode(2,sim,channel,ACK_RTS_CTS_SND_DUR,SLOT_DUR);
                    RxNode *nodeD = new RxNode(4,sim,channel,ACK_RTS_CTS_SND_DUR,SLOT_DUR);
                    TxNode *nodeA = new TxNode(0,sim,channel,nodeB,nodeAFreq,DIFS,SIFS,PACKET_SEND_DUR,ACK_RTS_CTS_SND_DUR,SLOT_DUR,useVcs);
                    TxNode *nodeC = new TxNode(1,sim,channel,nodeD,nodeCFreq,DIFS,SIFS,PACKET_SEND_DUR,ACK_RTS_CTS_SND_DUR,SLOT_DUR,useVcs);
                    
                    if (! nodeA ||!nodeB || ! nodeC || !nodeD) {
                        cout << "failed to allocate memory for nodes"<< endl;
                        exit(1);
                    }
                    
                    //seed starting events
                    nodeA->schedulePacketReady(0);
                    nodeC->schedulePacketReady(0);
                    
                    //run simulation
                    sim->Run();
                    
                    uint32_t aThruput(0),cThruput(0);
                    float aUtil(0.0f),cUtil(0.0f);
                    if ( useVcs)
                    {
                        aThruput = nodeA->SuccessfulSends() * (PACKET_SIZE_BYTES + 3 * ACK_SIZE_BYTES);
                        aUtil = (((float)nodeA->SuccessfulSends()*VCS_RTT)/(float)simDuration) * 100.0f;
                        cThruput = nodeC->SuccessfulSends() * (PACKET_SIZE_BYTES + 3* ACK_SIZE_BYTES);
                        cUtil = (((float)nodeC->SuccessfulSends()*VCS_RTT)/(float)simDuration) * 100.0f;
                    }
                    else
                    {
                        aThruput = nodeA->SuccessfulSends() * (PACKET_SIZE_BYTES + ACK_SIZE_BYTES);
                        aUtil = (((float)nodeA->SuccessfulSends()*RTT)/(float)simDuration) * 100.0f;
                        cThruput = nodeC->SuccessfulSends() * (PACKET_SIZE_BYTES + ACK_SIZE_BYTES);
                        cUtil = (((float)nodeC->SuccessfulSends()*RTT)/(float)simDuration) * 100.0f;
                    }
                    
                    cout << endl<< endl;
                    
                    printf("%u,%u,%s,%u\n",jjj,nodeA->id(),"Lambda",nodeAFreq);
                    printf("%u,%u,%s,%u\n",jjj,nodeA->id(),"Throughput",aThruput);
                    printf("%u,%u,%s,%.2f\n",jjj,nodeA->id(),"Utilization",aUtil);
                    printf("%u,%u,%s,%u\n",jjj,nodeC->id(),"Lambda",nodeCFreq);
                    printf("%u,%u,%s,%u\n",jjj,nodeC->id(),"Throughput",cThruput);
                    printf("%u,%u,%s,%.2f\n",jjj,nodeC->id(),"Utilization",cUtil);
                    printf("%u,%u,%s,%u\n",jjj,2,"Collisions",nodeC->TotalCollisions());
                    printf("%u,%u,%s,%u\n",jjj,2,"HiddenNode",nodeC->UsesVCS());
                    printf("%u,%u,%s,%u\n",jjj,2,"VCS",nodeC->UsesVCS());
                    printf("%u,%u,%s,%u\n",jjj,2,"Throughput",aThruput + cThruput);
                    printf("%u,%u,%s,%.2f\n",jjj,2,"Utilization",aUtil + cUtil);
                    printf("%u,%u,%s,%.2f\n",jjj,2,"Fairnessindex",aUtil/cUtil);
                    
                    fairnessIndexMap.insert(std::pair<uint32_t, float>(jjj,aUtil/cUtil));
                    
                    //cout << " Simulation: " << jjj << endl;
                    //cout << "Node A :" << endl;
                    //cout << "   Lambda: " <<nodeAFreq <<endl;
                    //cout << "   Sends: " << nodeA->SuccessfulSends()<< endl;
                    //cout << "   Throughput: " << aThruput << " Bytes" << endl;
                    //cout << "   Collisions: " << nodeA->TotalCollisions()<< endl;
                    //cout << "   Utilization: " << aUtil << "%"<< endl;
                    //cout << "Node C :" << endl;
                    //cout << "   Lambda: " <<nodeCFreq <<endl;
                    //cout << "   Sends: " << nodeC->SuccessfulSends()<< endl;
                    //cout << "   Throughput: " << cThruput << " Bytes" << endl;
                    //cout << "   Collisions: " << nodeC->TotalCollisions()<< endl;
                    //cout << "   Utilization: " << cUtil<< "%"<< endl;
                    //cout << "Total:"<< endl;
                    //cout << "   Throughput: " << aThruput + cThruput << " Bytes" << endl;
                    //cout << "   Utilization: " << aUtil +cUtil << "%" << endl;
                    //cout << "   FairnessIndex (A:C): " << aUtil/cUtil << endl;
                    
                    //sim->PrintData();
                    
                    // destroy objects
                    nodeA = NULL;
                    nodeC = NULL;
                    channel = NULL;
                    sim = NULL;
                    delete nodeA;
                    delete nodeC;
                    delete channel;
                    delete sim;
                    jjj++;
                }//for lamdas
            } //for nodeA Freq scale
        } //for use Vcs
 //   }//hiddenNode
    
    for ( map <uint,float>::iterator it2 = fairnessIndexMap.begin();
         it2 != fairnessIndexMap.end(); ++it2 )
    {
        if (it2->second ==NAN || it2->second == INFINITY ||
            it2->second < .5  || it2->second > 30 )
            cout <<"\nERROR:"<<  it2->first << ":" << it2->second<<endl;
    }

    
    return 0;
}
