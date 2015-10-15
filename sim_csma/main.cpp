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
#include <iomanip>
#include "DataLogger.h"
#include <sstream>

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

bool g_bShowEnhancedStats = false;
uint32_t g_uSimNum = 10;

void parseCommandLineArgs(int argc, const char * argv[])
{
    for (int iii = 0; iii < argc; iii++)
    {
        if ( 	strcmp(argv[iii], "-s") == 0 ||
            strcmp(argv[iii], "--show-stats") == 0 ||
            strcmp(argv[iii], "--stats") == 0 )
        {
            g_bShowEnhancedStats = true;
        }
        if ( 	strcmp(argv[iii], "-i") == 0 ||
            strcmp(argv[iii], "--iterations") == 0 ||
            strcmp(argv[iii], "--tests") == 0 )
        {
            g_uSimNum = atoi(argv[++iii]);
        }
    }
    
}

int main(int argc, const char * argv[]) {
    
    parseCommandLineArgs(argc,argv);
    
    vector<uint32_t> msgFreqVec;
    msgFreqVec.push_back(50);
    msgFreqVec.push_back(100);
    msgFreqVec.push_back(200);
    msgFreqVec.push_back(300);
    msgFreqVec.push_back(400);
    msgFreqVec.push_back(500);
    
    
    /*cout << "------------------------------------------------------------" << endl;
     cout << " KEY"<< endl;
     cout << "Sim Num,Node,Attribute,Value"<< endl;
     cout << "Node: 0 = A, 1 = C, 2 = Total"<< endl;
     cout << "------------------------------------------------------------" << endl;*/
    
    map <uint,float> fairnessIndexMap;
    map <uint,uint32_t> collisionsIndexMap;
    map <uint,float> utilizationMap;
    map <uint,uint32_t> thruputMap;
    
    DataVec dataMap;
    for (uint32_t testNum =1; testNum < g_uSimNum; testNum++)
    {
        uint jjj(1);
        cout << "Test: " << testNum << endl;
        for (uint hiddenNode = 0; hiddenNode <= 1; hiddenNode++ )
        {
            for (uint useVcs = 0; useVcs <= 1; useVcs++ )
            {
                for (uint nodeAFreqScale = 1; nodeAFreqScale <= 2; nodeAFreqScale++)
                {
                    for (vector<uint32_t>::iterator it = msgFreqVec.begin();
                         it != msgFreqVec.end(); ++it )
                    {
                        
                        //cout << "sim: " << jjj << endl;
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
                        TxNode *nodeA = new TxNode(0,sim,channel,nodeB,nodeAFreq,DIFS,SIFS,PACKET_SEND_DUR,ACK_RTS_CTS_SND_DUR,SLOT_DUR,useVcs,hiddenNode);
                        TxNode *nodeC = new TxNode(1,sim,channel,nodeD,nodeCFreq,DIFS,SIFS,PACKET_SEND_DUR,ACK_RTS_CTS_SND_DUR,SLOT_DUR,useVcs,hiddenNode);
                        
                        if (! nodeA ||!nodeB || ! nodeC || !nodeD) {
                            cout << "failed to allocate memory for nodes"<< endl;
                            exit(1);
                        }
                        
                        //seed starting events
                        nodeA->schedulePacketReady(random_distro::exponential(nodeAFreq,random_distro::TEN_USECS));
                        nodeC->schedulePacketReady(random_distro::exponential(nodeCFreq,random_distro::TEN_USECS));
                        
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
                        
                        
                        
                        /*
                         printf("%u,%u,%s,%u\n",jjj,nodeA->id(),"Lambda",nodeAFreq);
                         printf("%u,%u,%s,%u\n",jjj,nodeA->id(),"Throughput",aThruput);
                         printf("%u,%u,%s,%.2f\n",jjj,nodeA->id(),"Utilization",aUtil);
                         printf("%u,%u,%s,%u\n",jjj,nodeC->id(),"Lambda",nodeCFreq);
                         printf("%u,%u,%s,%u\n",jjj,nodeC->id(),"Throughput",cThruput);
                         printf("%u,%u,%s,%.2f\n",jjj,nodeC->id(),"Utilization",cUtil);
                         printf("%u,%u,%s,%u\n",jjj,2,"Collisions",nodeC->TotalCollisions());
                         printf("%u,%u,%s,%u\n",jjj,2,"HiddenNode",nodeC->IsHiddenNode());
                         printf("%u,%u,%s,%u\n",jjj,2,"VCS",nodeC->UsesVCS());
                         printf("%u,%u,%s,%u\n",jjj,2,"Throughput",aThruput + cThruput);
                         printf("%u,%u,%s,%.2f\n",jjj,2,"Utilization",aUtil + cUtil);
                         printf("%u,%u,%s,%.2f\n",jjj,2,"Fairnessindex",aUtil/cUtil);
                         if ( ! isnormal( aUtil/cUtil ) || 5 > (aUtil + cUtil) )
                         {
                         cout <<"BREAK"<<endl;
                         if (! isnormal( aUtil/cUtil ))
                         cout <<"not normal"<<endl;
                         else
                         cout <<"Utilization under 5"<<endl;
                         hiddenNode = 999;
                         useVcs = 999;
                         break;
                         }
                         //cout << endl;
                         */
                         //fairnessIndexMap.insert(std::pair<uint32_t, float>(jjj,aUtil/cUtil));
                         //collisionsIndexMap.insert(std::pair<uint32_t, uint32_t>(jjj,nodeC->TotalCollisions()));
                         //thruputMap.insert(std::pair<uint32_t, uint32_t>(jjj,aThruput + cThruput));
                         //utilizationMap.insert(std::pair<uint32_t, float>(jjj,aUtil+cUtil));
                         
                        dataMap.insert(Data(jjj, nodeAFreq, nodeCFreq, aThruput, cThruput, aUtil, cUtil, nodeC->TotalCollisions(), nodeC->IsHiddenNode(), nodeC->UsesVCS(), aThruput + cThruput, aUtil + cUtil, aUtil/cUtil));
                        
                        
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
                    } // for lamdas
                } //for nodeA Freq scale
            } //for use Vcs
        } // hiddenNode
    } // testNum
    dataMap.print();
    
    stringstream ss;
    ss<< g_uSimNum;
    string strNumOfTests("");
    ss >>strNumOfTests;
    dataMap.OutputToFile("data_"+ strNumOfTests+".csv");
    
    if (g_bShowEnhancedStats)
    {
        cout <<endl;
        cout << "Row 1: Parallel, No VCS" << endl;
        cout << "Row 2: Parallel, VCS" << endl;
        cout << "Row 3: Hidden Node, No VCS" << endl;
        cout << "Row 4: Hidden Node, VCS" << endl;
        
        
        cout << endl<<"Throughput:" << endl;
        
        for (  map<uint32_t, Data*>::iterator it2 = dataMap.begin();
             it2 != dataMap.end(); ++it2 )
        {
            cout << left << setw(2) <<it2->first << ":"
            <<setw(5) << std::setprecision( 3 ) << it2->second->thruput_tot << "\t";
            if (it2->first && it2->first % 12 == 0)
                cout << endl;
        }
        
        cout <<endl<< "Utilization:" << endl;
        for ( map <uint32_t, Data*>::iterator it2 = dataMap.begin();
             it2 != dataMap.end(); ++it2 )
        {
            cout << left << setw(2) <<it2->first << ":"
            <<setw(5) << std::setprecision( 3 ) << it2->second->util_tot << "\t";
            if (it2->first && it2->first % 12 == 0)
                cout << endl;
        }
        
        cout <<endl<< "Fairness Indeces:" << endl;
        for ( map <uint32_t, Data*>::iterator it2 = dataMap.begin();
             it2 != dataMap.end(); ++it2 )
        {
                cout << left << setw(2) <<it2->first << ":"
                <<setw(5) << std::setprecision( 3 ) << it2->second->fairness<< "\t";
                if (it2->first && it2->first % 12 == 0)
                    cout << endl;
        }
        
        cout << endl<<"Collisions:" << endl;
        for ( map<uint32_t, Data*>::iterator it2 = dataMap.begin();
             it2 != dataMap.end(); ++it2 )
        {
            cout << left << setw(2) <<it2->first << ":"
            <<setw(5) << std::setprecision( 3 ) << it2->second->colls<< "\t";
            if (it2->first && it2->first % 12 == 0)
                cout << endl;
        }
        
        /*cout << endl<<"Collisions:" << endl;
        for ( map<uint32_t, Data*>::iterator it2 = dataMap.begin();
             it2 != dataMap.end(); ++it2 )
        {
            cout << left << setw(2) <<it2->first << ":"
            <<setw(5) << std::setprecision( 3 ) << it2->second->colls<< "\t";
            if (it2->first && it2->first % 12 == 0)
                cout << endl;
        }
        
         cout << endl<<"Throughput:" << endl;
        for ( map <uint,uint32_t>::iterator it2 = thruputMap.begin();
             it2 != thruputMap.end(); ++it2 )
        {
            cout << left << setw(2) <<it2->first << ":"
            <<setw(5) << std::setprecision( 3 ) << it2->second<< "\t";
            if (it2->first && it2->first % 12 == 0)
                cout << endl;
        }
        
        cout <<endl<< "Utilization:" << endl;
        for ( map <uint,float>::iterator it2 = utilizationMap.begin();
             it2 != utilizationMap.end(); ++it2 )
        {
            cout << left << setw(2) <<it2->first << ":"
            <<setw(5) << std::setprecision( 3 ) << it2->second << "\t";
            if (it2->first && it2->first % 12 == 0)
                cout << endl;
        }
        
        cout <<endl<< "Fairness Indeces:" << endl;
        for ( map <uint,float>::iterator it2 = fairnessIndexMap.begin();
             it2 != fairnessIndexMap.end(); ++it2 )
        {
            if ( ! isnormal( it2->second ) )
                cout <<"\nERROR:"<<  it2->first << ":" << it2->second<<endl;
            else{
                cout << left << setw(2) <<it2->first << ":" 
                <<setw(5) << std::setprecision( 3 ) << it2->second<< "\t";
                if (it2->first && it2->first % 12 == 0)
                    cout << endl;
            }
        }
        
        cout << endl<<"Collisions:" << endl;
        for ( map <uint,uint32_t>::iterator it2 = collisionsIndexMap.begin();
             it2 != collisionsIndexMap.end(); ++it2 )
        {
            cout << left << setw(2) <<it2->first << ":" 
            <<setw(5) << std::setprecision( 3 ) << it2->second << "\t";
            if (it2->first && it2->first % 12 == 0)
                cout << endl;
        }
        */
    }
    return 0;
}
