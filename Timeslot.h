#include <cstdlib>
#include <iostream>
#include <list>
#include "Node.h"

class Timeslot{
	list<advNode> listNode; //list of nodes
	list<advNode> activeNode;	//list of active nodes in that timeslot
	int listener;	//listener Channel
	int asn;	//absolute Sequence Number
	Random rand;
	int listenerChannels;
	double ploss;	//probability of having a loss
	int method;		//method used to compute the timeslot
	
	int energyFactor;
	bool beaconReceived(int);
	void eraseActive();	//erase the list of active nodes
	void insertActive(int); //inserts active nodes in the list
public:
	Timeslot(Random r, int lc)  { asn = 0; rand = r; listenerChannels = lc; ploss = 0;energyFactor = ENERGY;};
	void addNode(advNode a);
	bool compareChannel(int);
	void print();
	void addListener(int l);
	void erase();
	int timeslotManager(int);
	void setProbability(double);
};
