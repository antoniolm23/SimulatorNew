#include "Stats.h"

/*
 * Insert, if necessary, a statistic referred to a new iteration
 * @params: frequency, timeslotnumber
 */
void Stat::statInsert(int tsn)
{
	//scan the list of statistics and if necessary increment the frequeny
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it) 
	{
		if(it->timeslotNumber == tsn) 
		{
			it->frequency++;
			return;
		}
	}
	
	//if the scan wasn't successful add a new record in the list
	statElem st;
	st.frequency = 1;
	st.timeslotNumber = tsn;
	statsList.push_back(st);
}

/*
 * Function that computes the statistics on the list and prints it on the file
 * @return: the average number of steps required 
 */
double Stat::computeStatistic() {
	
	double sum = 0;
	double ret;
	
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		sum += (it -> timeslotNumber * it->frequency);
		//cout<<it -> timeslotNumber<<' '<<it->frequency<<' '<<sum<<endl;
	}
	
	ret = sum / iterations; 
	return ret;
}

//prints the list of statistics on file
void Stat::print()
{
	ofstream myfile;
	myfile.open("statistics.txt", ios::app);
	
	computeStatistic();
	
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		myfile<<"FrameslotNumber\t"<<it->timeslotNumber<<"\tfrequency:\t"<<it->frequency<<endl; 
	}
	
	
	myfile.close();
}

//print the statistic on the file, each statistic is preceeded by the string t
void Stat::print(string t)
{
	//cout<<"othermethodCalled"<<endl;
	ofstream myfile;
	myfile.open("statistics.txt", ios::app);
	
	double mean = computeStatistic();
	double CI = computeConfidenceInterval(mean);
	myfile<<t<<":\tAvg:\t"<<mean<<"\tCI:\t"<<mean - CI<<'\t'<<mean + CI<<endl; 
	
	myfile.close();
}

//set the number of iterations performed
void Stat::setIterations(int it)
{
	iterations = it;
}

/**
 * Computes the 95% confidence interval for the average slot number required
 * @param: method used to compute the average joining time, average slot number required
 * @return: 95% confidence interval
 */
double Stat::computeConfidenceInterval(double mean) 
{
	double stDevSum = 0;
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		double tmp = (mean - it -> timeslotNumber) * it ->frequency; 
		stDevSum += pow(((tmp)), 2);
			//cout<<it -> timeslotNumber<<' '<<it->frequency<<' '<<sum<<endl;
	}
	double stDev = sqrt(stDevSum / iterations);
	double confidenceInterval = CONFIDENCE95 * (stDev / sqrt(iterations));
	
	return confidenceInterval;
}


