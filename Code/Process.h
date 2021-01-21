//	Program Name:		Memory Simulator
//	Programmers Name:	Husam Alsheikh, Judy Anani
//	Description:		Program Simulates allocation and deallocation of memory
//	Date Created:		09/27/2020

#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "PriorityQueue.h"

using namespace std;

class Process {
private:
	ofstream outputSummary;

	vector<Node> jobs;

	vector<int> memory;

	vector<int> memoryIndexFF;
	vector<int> memoryIndexNF;
	vector<int> memoryIndexBF;
	vector<int> memoryIndexLF;

	int totalMemory;

	int totalMemoryNumFF;
	int totalMemoryNumNF;
	int totalMemoryNumBF;
	int totalMemoryNumLF;

	//	Metrics
	int smallJobsNum, mediumJobsNum, largeJobsNum;
	int memoryAllocatedFF, memoryAllocatedNF, memoryAllocatedBF, memoryAllocatedLF;
	int requiredMemoryFF, requiredMemoryNF, requiredMemoryBF, requiredMemoryLF;
	int internalFragFF, internalFragNF, internalFragBF, internalFragLF;
	int memFreeFF, memFreeNF, memFreeBF, memFreeLF;
	int externalFragFF, externalFragNF, externalFragBF, externalFragLF;
	int largestSpace, smallestSpace;
	int heapAFF, heapANF, heapABF, heapALF;

	int numberOfAllocationsFF, numberOfAllocationsNF, numberOfAllocationsBF, numberOfAllocationsLF;
	int allocationOpFF, allocationOpNF, allocationOpBF, allocationOpLF;
	int averageOpFF, averageOpNF, averageOpBF,  averageOpLF;

public:
	Process(vector<int> memoryUnitSize, vector<int> memoryNum);

	void addJob(Node job, ofstream& outputSummary);

	void deleteJob(int index, ofstream& outputSummary);

	void run(int simTime, ofstream& outputSummary, ofstream& logFF, ofstream& logNF, ofstream& logBF, ofstream& logLF);

	vector<int> mallocFF(int size, int& units, ofstream& logFF);

	vector<int> mallocNF(int size, int& units, ofstream& logNF);

	vector<int> mallocBF(int size, int& units, ofstream& logBF);

	vector<int> mallocLF(int size, int& units, ofstream& logLF);

	void freeFF(vector<int> memoryLoc, int memoryNum);

	void freeNF(vector<int> memoryLoc, int memoryNum);

	void freeBF(vector<int> memoryLoc, int memoryNum);

	void freeLF(vector<int> memoryLoc, int memoryNum);

	bool isEmpty();

	void printMetrics(ofstream& outputSummary);
};