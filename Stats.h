#include <iostream>
#include <cstdlib>
#include <fstream>
#include <list>
#include <cmath>

#define CONFIDENCE95 1.96
using namespace std;

//an element of the statistics
struct statElem
{
	int timeslotNumber;	//denotes the timeslot nomber at which a "join" occurs
	int frequency;	//denotes how many times a node takes this amount of steps 
};

class Stat
{
	list<statElem> statsList;
	double computeStatistic();
	
	int iterations;
public:
	Stat() {};
	void statInsert(int tsn);
	void setIterations(int);
	double computeConfidenceInterval(double);
	void print();
	void print(string t);
};