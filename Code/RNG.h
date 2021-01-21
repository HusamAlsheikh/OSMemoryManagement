//	Program Name:		Memory Simulator
//	Programmers Name:	Husam Alsheikh, Judy Anani
//	Description:		Program Simulates allocation and deallocation of memory
//	Date Created:		09/27/2020

#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct Job {
	string type;
	int jobType;
	int jobNum;
	
	int arrivalT;
	int runningT;
	int codeS;
	int stackS;
	int heapS;

	vector<int> heapElements;
	vector<int> elementsLifetime;
};

class RNG {
private:
	struct Job temp;
	int jobNumber;

	int smallNum;
	int mediumNum;
	int largeNum;

public:
	RNG();

	int getArrivalTime();

	int getRunningTime();

	int getCodeSize();

	int getStackSize();

	int getHeapSize();

	void setPercentages(int smallNum, int mediumNum, int largeNum);

	Job generateNumbers( int simTime);
};