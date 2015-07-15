#include "Timeslot.h"

/*
 * Functions to insert a node in one of the lists
 */
void Timeslot::addNode(advNode a)
{
	listNode.push_back(a);
	//cout<<"added node"<<a.getNodeID()<<"\n";
}

/*
 * From the list of all the nodes extract only the active nodes in that timeslot
 */
void Timeslot::insertActive(int asn)
{
	int channelUsed;
	
	for( list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it  )
	{
		//cout<<"ListNode: "<<it->getNodeID()<<": "<<it->getUsedChannel(asn)<<"\n";
		//if node active then add a record in the list
		if((channelUsed = it -> getUsedChannel(asn)) != -1) 
		{
			
			/*
			 * if there is a certain ploss of losing the message, a node can be inserted in 
			 * the list of active nodes according to that probability 
			 * i.e. a node is active with probability 1 - ploss
			 */
			if(method == PLOSS_SCENARIO)
			{
				double pActive = rand.getNumber01();
				//cout<<pActive<<" VS "<<ploss<<endl;
				if(pActive > ploss)
					activeNode.push_back(*it);
			}
			
			else 
				activeNode.push_back(*it);
			
			
			//cout<<"Active: "<<it->getNodeID()<<": "<<it->getUsedChannel(asn)<<"\n";
		}
	}
}

/*
 * adds the listener channel
 * @params: listener channel without ChannelStart
 */
void Timeslot::addListener(int l)
{
	listener = l + CHSTART;
}

//searches for a match throughout the list of advertisers
bool Timeslot::compareChannel(int asn)
{
	//scan all the list looking for a match
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		if(it->getUsedChannel(asn) == listener)
			return true;
	}
	return false;
}

void Timeslot::print()
{
	cout<<listener<<endl;
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		cout<<'\t'<<it->getNodeID()<<' '<<it->getAbsoluteChannel();
	}
	cout<<endl<<"**************************************"<<endl;
}

//delete all the list
void Timeslot::erase()
{
	listNode.erase(listNode.begin(), listNode.end());
	eraseActive();
}
void Timeslot::eraseActive()
{
	activeNode.erase(activeNode.begin(), activeNode.end());
}

//find if the max value available (size-1) is present in the list 
int findMax(int* v, int size) 
{
	int max = size - 1;
	int totMax = 0;
	for(int i = 0; i < size; i++) {
		if(max == v[i])
			totMax++;
	}
	if(totMax == 1)
		return true;
	else
		return false;
}

//finds int t in list l
bool find(list<int> l, int t) {
	if(l.empty())
		return false;
	for( list<int>::iterator it = l.begin(); it != l.end(); ++it  )
	{
		//cout<<"list scanning"<<endl;
		if(*it == t)
			return true;
	}
	return false;
}

/*
 * Try to solve a collision in the same timeslot if there are more thn one node that want to transmitt
 * @return: true -> beacon received, false beacon not received
 */
bool Timeslot::beaconReceived(int asn)
{
	int transmittingNodes = 0;
	int genNumb = 0; 
	list<int> chOff = list<int>();
	//vector<int> usedChannels;
	for(list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it)
	{
		genNumb = rand.getNumber(energyFactor);
		//cout << it -> getNodeID() << ": " << genNumb<<'\t'<<energyFactor<<endl;
		if(genNumb == (int)TRANSMISSIONFLAG)
		{
			chOff.push_back(it -> getUsedChannel(asn));
		}
	}
	
	if(chOff.size() >= 1)
	{
		for(list<int>::iterator it = chOff.begin(); it != chOff.end(); ++it)
		{
			if(*it == listener)
				transmittingNodes = transmittingNodes + 1;
		}
	}
	
	if(transmittingNodes == 1)
		return true;
	else 
	{
		return false;
	}
}	

/*
 * Function that operates the most operations on a timeslot:
 * 1) arranges links in the timeslot
 * 2) checks wether or not channels are equal 
 * 3) operates on a timeslot Base
 * 4) if method is fixed returns the number of frameslot elapsed
 * 
 * @params: method used
 * @return: framselot elapsed
 */
int Timeslot::timeslotManager(int m)
{
	method = m;
	bool matchFound = false;
	char t;
	
	int timeslotOn = 0;
	//cout<<listNode.size()<<endl;
	/*
	* In the STAGGERED schema and in all other schemas, a node becomes active at a certain point 
	* and it waits until the first EB is received correctly ALWAYS ON and
	* ON THE SAME CHANNEL 
	*/
	if(method == STAGGERED || method == PLOSS_SCENARIO)
	{
		/*
		* N is the number of timeslots, listenerChannels is the number of channels
		* available to the listener
		*/
		timeslotOn = rand.getNumber( N * listenerChannels ) + 1;
		
		//cout<<"Staggered"<<endl;
		
		asn = timeslotOn;
	}
	
	//until a match hasn't been found increment absolute sequence number and look for a match
	while(!matchFound) {
		
		/*
		 * insert active nodes in the list of active nodes
		 * handles the ploss. In this case ploss is intended as the probability of 
		 * having a certain node active in its turn
		 */
		insertActive(asn);
		
		//print the list of active channels
		//cout<<"\t****AbsoluteSequenceNumber: "<<asn<<"****"<<endl;
		//if(activeNode.size() > 0)
		//	print();
		
		/*
		 * In the fixed schema the listener becomes active at the first slot
		 * and remain active until a match is found
		 */
		bool receivedBeacon = beaconReceived(asn);
		
		/*
		* if listener and advertiser use the same channel and 
		* there are no collisions then the match is found
		*/
		if(receivedBeacon)  
		{
			matchFound = true;
		}
		eraseActive();
		asn++;
		
	}
	
	//if the method is fixed then the measure is performed in slotframe 
	if(method == FIXEDSCHEMA) 
	{
		int ratio = (asn / N) + 1;
		return ratio;
	}
		
	//otherwise return the number of timeslot elapsed
	else
	{
		int ret = asn - timeslotOn;
		//cout<<"return value: "<<ret<<endl; 
		return ret;
	}
}

/*
 * The probability declares how much is the packet loss that we have in our transmission
 * By default it is 0
 * @param: probability of having a loss
 */
void Timeslot::setProbability(double p)
{
	ploss = p;
	//cout<<p<<":  "<<ploss<<endl;
}
