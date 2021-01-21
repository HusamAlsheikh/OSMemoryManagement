//	Program Name:		Memory Simulator
//	Programmers Name:	Husam Alsheikh, Judy Anani
//	Description:		Program Simulates allocation and deallocation of memory
//	Date Created:		09/27/2020

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include "PriorityQueue.h"
#include "RNG.h"
#include "Process.h"

using namespace std;

Process::Process(vector<int> memoryUnitSize, vector<int> memoryNum) {
	for (int x = 0; x < memoryNum.size(); x++) {
		totalMemoryNumFF += (memoryNum[x] * memoryUnitSize[x]);
	}

	totalMemoryNumNF = totalMemoryNumFF;
	totalMemoryNumBF = totalMemoryNumFF;
	totalMemoryNumLF = totalMemoryNumFF;
	totalMemory = totalMemoryNumFF;

	for (int x = 0; x < memoryUnitSize.size(); x++) {
		for (int z = 0; z < memoryNum[x]; z++) {
			memory.push_back(memoryUnitSize[x]);
			memoryIndexFF.push_back(1);
			memoryIndexNF.push_back(1);
			memoryIndexBF.push_back(1);
			memoryIndexLF.push_back(1);
		}
	}

	//	Metrics
	smallJobsNum = 0;
	mediumJobsNum = 0;
	largeJobsNum = 0;
	memoryAllocatedFF = 0;
	memoryAllocatedNF = 0;
	memoryAllocatedBF = 0;
	memoryAllocatedLF = 0;
	requiredMemoryFF = 0;
	requiredMemoryNF = 0;
	requiredMemoryBF = 0;
	requiredMemoryLF = 0;
	smallestSpace = memoryUnitSize[0];
	largestSpace = memoryUnitSize[memoryUnitSize.size() - 1];

	numberOfAllocationsFF = 0;
	numberOfAllocationsNF = 0;
	numberOfAllocationsBF = 0;
	numberOfAllocationsLF = 0;
	allocationOpFF = 0;
	allocationOpNF = 0;
	allocationOpBF = 0;
	allocationOpLF = 0;
}

void Process::addJob(Node job, ofstream& outputSummary) {
	jobs.push_back(job);

	if (job.jobType == 1) {
		smallJobsNum++;
	}
	else if (job.jobType == 2) {
		mediumJobsNum++;
	}
	else {
		largeJobsNum++;
	}

	//	Job Arrival
	cout << "\n" << setw(20) << "Job Type" << setw(20) << "Arrival Time" << setw(20) << "Running Time" << setw(20) << "Code Size" << setw(20) << "Stack Size" << setw(20) << "Heap Size" << endl;
	cout << setw(20) << job.type << setw(20) << job.arrivalT << setw(20) << job.runningT << setw(20) << job.codeS << setw(20) << job.stackS << setw(20) << job.heapS << endl << endl;

	outputSummary << "\nJob Type" << setw(20) << "Arrival Time" << setw(20) << "Running Time" << setw(20) << "Code Size" << setw(20) << "Stack Size" << setw(20) << "Heap Size" << endl;
	outputSummary << job.type << setw(20) << job.arrivalT << setw(20) << job.runningT << setw(20) << job.codeS << setw(20) << job.stackS << setw(20) << job.heapS << endl << endl;
}

void Process::deleteJob(int index, ofstream& outputSummary) {
	//	Job departure
	Node job;
	for (int x = 0; x < jobs.size(); x++) {
		if (x == index) {
			job = jobs[x];
		}
	}

	cout << "\nJob Departure: ";
	outputSummary << "\nJob Departure: ";

	cout << "\nJob Type" << setw(20) << "Arrival Time" << setw(20) << "Running Time" << setw(20) << "Code Size" << setw(20) << "Stack Size" << setw(20) << "Heap Size" << endl;
	cout << "\n" << job.type << setw(20) << job.arrivalT << setw(20) << job.runningT << setw(20) << job.codeS << setw(20) << job.stackS << setw(20) << job.heapS << endl << endl;

	outputSummary << "\nJob Type" << setw(20) << "Arrival Time" << setw(20) << "Running Time" << setw(20) << "Code Size" << setw(20) << "Stack Size" << setw(20) << "Heap Size" << endl;
	outputSummary << "\n" << job.type << setw(20) << job.arrivalT << setw(20) << job.runningT << setw(20) << job.codeS << setw(20) << job.stackS << setw(20) << job.heapS << endl << endl;

	jobs.erase(jobs.begin() + index);
}

void Process::run(int simTime, ofstream& outputSummary, ofstream& logFF, ofstream& logNF, ofstream& logBF, ofstream& logLF) {
	vector<int> index;
	int units = -1;
	int memorySize = memory.size();

	//	Run for every job in queue
	for (int y = 0; y < jobs.size(); y++) {
		index.resize(memorySize);

		//	If new job
		if (jobs[y].processing == 0) {
			jobs[y].processing = 1;			//	Job is now processing

			jobs[y].departureTime = simTime + jobs[y].runningT;			//	Set departure time of job
			for (int x = 0; x < jobs[y].elementsDepart.size(); x++) {	//	Set departure time of elements
				jobs[y].elementsDepart[x] = simTime + jobs[y].elementsLifetime[x];
			}

			//	Code and stack size
			index = mallocFF(jobs[y].codeS, units, logFF);			//	Allocate memory
			if (index[0] != -1) {
				jobs[y].codeIndexFF = index;
				jobs[y].codeMemoryUnitsFF = units;			//	Number of memory units used

				allocationOpFF++;

				logFF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			}
			index = mallocFF(jobs[y].stackS, units, logFF);
			if (index[0] != -1) {
				jobs[y].stackIndexFF = index;
				jobs[y].stackMemoryUnitsFF = units;

				allocationOpFF++;

				logFF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			}

			index = mallocNF(jobs[y].codeS, units, logNF);			//	Allocate memory
			if (index[0] != -1) {
				jobs[y].codeIndexNF = index;
				jobs[y].codeMemoryUnitsNF = units;			//	Number of memory units used

				allocationOpNF++;

				logNF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			}
			index = mallocNF(jobs[y].stackS, units, logNF);
			if (index[0] != -1) {
				jobs[y].stackIndexNF = index;
				jobs[y].stackMemoryUnitsNF = units;

				allocationOpNF++;

				logNF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			}

			index = mallocBF(jobs[y].codeS, units, logBF);			//	Allocate memory
			if (index[0] != -1) {
				jobs[y].codeIndexBF = index;
				jobs[y].codeMemoryUnitsBF = units;			//	Number of memory units used

				allocationOpBF++;

				logBF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			}
			index = mallocBF(jobs[y].stackS, units, logBF);
			if (index[0] != -1) {
				jobs[y].stackIndexBF = index;
				jobs[y].stackMemoryUnitsBF = units;

				allocationOpBF++;

				logBF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			}

			index = mallocLF(jobs[y].codeS, units, logBF);			//	Allocate memory
			if (index[0] != -1) {
				jobs[y].codeIndexLF = index;
				jobs[y].codeMemoryUnitsLF = units;			//	Number of memory units used

				allocationOpLF++;

				logLF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			}
			index = mallocLF(jobs[y].stackS, units, logBF);
			if (index[0] != -1) {
				jobs[y].stackIndexLF = index;
				jobs[y].stackMemoryUnitsLF = units;

				allocationOpLF++;

				logLF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			}

			//	Heap elements
			for (int x = 0; x < (jobs[y].heapS / jobs[y].runningT); x++) {
				//	Allocate memory, set units allocated, and get initial index of memory unit
				index = mallocFF(jobs[y].heapElements[x], jobs[y].memoryUnitsAllocateFF[x], logFF);
				if (index[0] != -1) {
					jobs[y].heapIndexFF[x] = index;
					jobs[y].elementCompleteFF[x] = 0;

					heapAFF++;
					allocationOpFF++;
				}

				index = mallocNF(jobs[y].heapElements[x], jobs[y].memoryUnitsAllocateNF[x], logNF);
				if (index[0] != -1) {
					jobs[y].heapIndexNF[x] = index;
					jobs[y].elementCompleteNF[x] = 0;

					heapANF++;
					allocationOpNF++;
				}

				index = mallocBF(jobs[y].heapElements[x], jobs[y].memoryUnitsAllocateBF[x], logBF);
				if (index[0] != -1) {
					jobs[y].heapIndexBF[x] = index;
					jobs[y].elementCompleteBF[x] = 0;

					heapABF++;
					allocationOpBF++;
				}

				index = mallocLF(jobs[y].heapElements[x], jobs[y].memoryUnitsAllocateLF[x], logLF);
				if (index[0] != -1) {
					jobs[y].heapIndexLF[x] = index;
					jobs[y].elementCompleteLF[x] = 0;

					heapALF++;
					allocationOpLF++;
				}
			}
		}

		//	If current job
		if (jobs[y].heapIncrement < jobs[y].heapS) {
			jobs[y].heapIncrement += (jobs[y].heapS / jobs[y].runningT);
		}
		for (int x = 0; x < jobs[y].heapIncrement; x++) {
			//	In if statements for better readability
			if (1) {	//	First Fit
				//	If element has had memory allocated for it
				if (jobs[y].elementCompleteFF[x] == 0) {
					//	If element lifetime over
					if (jobs[y].elementsDepart[x] <= simTime) {
						freeFF(jobs[y].heapIndexFF[x], jobs[y].memoryUnitsAllocateFF[x]);
						jobs[y].elementCompleteFF[x] = 1;

						logFF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
					}
				}

				//	If element has not had memory allocated for it yet
				if (jobs[y].elementCompleteFF[x] == -1) {
					index = mallocFF(jobs[y].heapElements[x], jobs[y].memoryUnitsAllocateFF[x], logFF);
					if (index[0] != -1) {
						jobs[y].heapIndexFF[x] = index;
						jobs[y].elementCompleteFF[x] = 0;

						heapAFF++;

						logFF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
					}
				}
			}
			if (1) {	//	Next Fit
				//	If element has had memory allocated for it
				if (jobs[y].elementCompleteNF[x] == 0) {
					//	If element lifetime over
					if (jobs[y].elementsDepart[x] <= simTime) {
						freeNF(jobs[y].heapIndexNF[x], jobs[y].memoryUnitsAllocateNF[x]);
						jobs[y].elementCompleteNF[x] = 1;

						logNF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
					}
				}

				//	If element has not had memory allocated for it yet
				if (jobs[y].elementCompleteNF[x] == -1) {
					index = mallocNF(jobs[y].heapElements[x], jobs[y].memoryUnitsAllocateNF[x], logNF);
					if (index[0] != -1) {
						jobs[y].heapIndexNF[x] = index;
						jobs[y].elementCompleteNF[x] = 0;

						heapANF++;

						logNF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
					}
				}
			}
			if (1) {	//	Best Fit
				//	If element has had memory allocated for it
				if (jobs[y].elementCompleteBF[x] == 0) {
					//	If element lifetime over
					if (jobs[y].elementsDepart[x] <= simTime) {
						freeBF(jobs[y].heapIndexBF[x], jobs[y].memoryUnitsAllocateBF[x]);
						jobs[y].elementCompleteBF[x] = 1;

						logBF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
					}
				}

				//	If element has not had memory allocated for it yet
				if (jobs[y].elementCompleteBF[x] == -1) {
					index = mallocBF(jobs[y].heapElements[x], jobs[y].memoryUnitsAllocateBF[x], logBF);
					if (index[0] != -1) {
						jobs[y].heapIndexBF[x] = index;
						jobs[y].elementCompleteBF[x] = 0;

						heapABF++;

						logBF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
					}
				}
			}
			if (1) {	//	Last Fit
				//	If element has had memory allocated for it
				if (jobs[y].elementCompleteLF[x] == 0) {
					//	If element lifetime over
					if (jobs[y].elementsDepart[x] <= simTime) {
						freeLF(jobs[y].heapIndexLF[x], jobs[y].memoryUnitsAllocateLF[x]);
						jobs[y].elementCompleteLF[x] = 1;

						logLF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
					}
				}

				//	If element has not had memory allocated for it yet
				if (jobs[y].elementCompleteLF[x] == -1) {
					index = mallocLF(jobs[y].heapElements[x], jobs[y].memoryUnitsAllocateLF[x], logLF);
					if (index[0] != -1) {
						jobs[y].heapIndexLF[x] = index;
						jobs[y].elementCompleteLF[x] = 0;

						heapALF++;

						logLF << "\n\n Allocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
					}
				}
			}
		}

		//	If job finished, free memory and delete job
		if (simTime >= jobs[y].departureTime) {
			//	Free code and stack
			freeFF(jobs[y].codeIndexFF, jobs[y].codeMemoryUnitsFF);
			freeFF(jobs[y].stackIndexFF, jobs[y].stackMemoryUnitsFF);
			freeNF(jobs[y].codeIndexNF, jobs[y].codeMemoryUnitsNF);
			freeNF(jobs[y].stackIndexNF, jobs[y].stackMemoryUnitsNF);
			freeBF(jobs[y].codeIndexBF, jobs[y].codeMemoryUnitsBF);
			freeBF(jobs[y].stackIndexBF, jobs[y].stackMemoryUnitsBF);
			freeLF(jobs[y].codeIndexLF, jobs[y].codeMemoryUnitsLF);
			freeLF(jobs[y].stackIndexLF, jobs[y].stackMemoryUnitsLF);

			logFF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			logNF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			logBF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			logLF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << y;
			//	Free any elements that still have memory allocated but not finished
			for (int x = 0; x < jobs[y].elementCompleteFF.size(); x++) {
				if (jobs[y].elementCompleteFF[x] == 0) {
					freeFF(jobs[y].heapIndexFF[x], jobs[y].memoryUnitsAllocateFF[x]);

					logFF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
				}
			}

			//	Free any elements that still have memory allocated but not finished
			for (int x = 0; x < jobs[y].elementCompleteNF.size(); x++) {
				if (jobs[y].elementCompleteNF[x] == 0) {
					freeNF(jobs[y].heapIndexNF[x], jobs[y].memoryUnitsAllocateNF[x]);

					logNF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
				}
			}

			//	Free any elements that still have memory allocated but not finished
			for (int x = 0; x < jobs[y].elementCompleteBF.size(); x++) {
				if (jobs[y].elementCompleteBF[x] == 0) {
					freeBF(jobs[y].heapIndexBF[x], jobs[y].memoryUnitsAllocateBF[x]);

					logBF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
				}
			}

			//	Free any elements that still have memory allocated but not finished
			for (int x = 0; x < jobs[y].elementCompleteLF.size(); x++) {
				if (jobs[y].elementCompleteLF[x] == 0) {
					freeLF(jobs[y].heapIndexLF[x], jobs[y].memoryUnitsAllocateLF[x]);

					logLF << "\n\n Deallocation-> Job #: " << jobs[y].jobNum << "\tTime: " << simTime << "\t Location: " << x;
				}
			}

			deleteJob(y, outputSummary);
		}
	}
}

vector<int> Process::mallocFF(int size, int& units, ofstream& logFF) {
	vector<int> index;
	int tempSize = size;
	int z = 0;

	index.resize(memoryIndexFF.size());

	//	Return -1 if not enough memory available
	if (size > totalMemoryNumFF) {
		index[0] = -1;
		return index;
	}

	requiredMemoryFF += size;

	fill(index.begin(), index.end(), -1);

	//	Find initial memory index
	for (int x = 0; x < memoryIndexFF.size(); x++) {
		if (memoryIndexFF[x] == 1) {
			index[z] = x;
			break;
		}
	}

	units = 0;
	z = 0;

	//	Allocate memory
	for (int x = index[0]; x < memoryIndexFF.size(); x++) {	//	Loop through index
		if (memoryIndexFF[x] == 1) {			//	If memory unit is available
			tempSize -= memory[x];			//	Get remaining memory
			totalMemoryNumFF -= memory[x];	//	Decrement total memory available
			memoryIndexFF[x] = 0;				//	Set location as used
			units++;						//	Increment units

			memoryAllocatedFF += memory[x];
			numberOfAllocationsFF++;

			index[z] = x;
			z++;

			if (tempSize <= 0) {
				return index;		//	return location of memory where allocated
			}
		}
	}

	return index;
}

vector<int> Process::mallocNF(int size, int& units, ofstream& logNF) {
	vector<int> index;
	int tempSize = size;
	int z = 0;

	index.resize(memoryIndexNF.size());

	//	Return -1 if not enough memory available
	if ((size * 2) > totalMemoryNumNF) {
		index[0] = -1;
		return index;
	}

	requiredMemoryNF += size;

	fill(index.begin(), index.end(), -1);

	for (int x = 0; x < memoryIndexNF.size() - 1; x++) {
		if (memoryIndexNF[x] == 1) {
			index[z] = x;
			break;
		}
	}

	units = 0;
	z = 0;

	//	Allocate memory
	int y = 0;
	for (int x = index[0]; x < memoryIndexNF.size(); x++) {
		if (y == 0) {
			int temp = size;
			for (int i = index[0]; i < memoryIndexNF.size() - 1; i++) {
				if (memoryIndexNF[i] == 1) {
					temp -= memory[x];

					if (temp <= 0) {
						x = i + 1;
						y = 1;
						break;
					}
				}
			}
		}

		if (memoryIndexNF[x] == 1 && y == 1) {
			tempSize -= memory[x];
			totalMemoryNumNF -= memory[x];
			memoryIndexNF[x] = 0;
			units++;

			memoryAllocatedNF += memory[x];
			numberOfAllocationsNF++;

			index[z] = x;
			z++;

			if (tempSize <= 0) {
				return index;
			}
		}
	}

	return index;
}

vector<int> Process::mallocBF(int size, int& units, ofstream& logBF) {
	vector<int> index;
	int tempSize = size;
	int z = 0;

	index.resize(memoryIndexBF.size());

	//	Return -1 if not enough memory available
	if (size > totalMemoryNumBF) {
		index[0] = -1;
		return index;
	}

	requiredMemoryBF += size;

	fill(index.begin(), index.end(), -1);

	//	Find initial memory index
	for (int x = 0; x < memoryIndexBF.size(); x++) {
		if (memoryIndexBF[x] == 1) {
			index[z] = x;
			break;
		}
	}

	units = 0;
	z = 0;

	//	Allocate memory
	for (int x = index[0]; x < memoryIndexBF.size(); x++) {
		if (memoryIndexBF[x] == 1) {
			tempSize -= memory[x];
			totalMemoryNumBF -= memory[x];
			memoryIndexBF[x] = 0;
			units++;

			memoryAllocatedBF += memory[x];
			numberOfAllocationsBF++;

			index[z] = x;
			z++;

			if (tempSize <= 0) {
				return index;
			}
		}

		return index;
	}
}

vector<int> Process::mallocLF(int size, int& units, ofstream& logLF) {
	vector<int> index;
	int tempSize = size;
	int z = 0;

	index.resize(memoryIndexLF.size());

	//	Return -1 if not enough memory available
	if (size > totalMemoryNumLF) {
		index[0] = -1;
		return index;
	}

	requiredMemoryLF += size;

	fill(index.begin(), index.end(), -1);

	for (int x = memoryIndexLF.size() - 1; x > 0; x--) {
		if (memoryIndexLF[x] == 1) {
			index[z] = x;
			break;
		}
	}

	units = 0;
	z = 0;

	//	Allocate memory
	for (int x = index[0]; x > 0; x--) {
		if (memoryIndexLF[x] == 1) {
			tempSize -= memory[x];
			totalMemoryNumLF -= memory[x];
			memoryIndexLF[x] = 0;
			units++;

			memoryAllocatedLF += memory[x];
			numberOfAllocationsLF++;

			index[z] = x;
			z++;

			if (tempSize <= 0) {
				return index;
			}
		}
	}

	return index;
}

void Process::freeFF(vector<int> memoryLoc, int memoryNum) {
	for (int x = 0; x < memoryNum; x++) {
		memoryIndexFF[memoryLoc[x]] = 1;
		totalMemoryNumFF += memory[memoryLoc[x]];

		memoryAllocatedFF -= memory[memoryLoc[x]];
	}
}

void Process::freeNF(vector<int> memoryLoc, int memoryNum) {
	for (int x = 0; x < memoryNum; x++) {
		memoryIndexNF[memoryLoc[x]] = 1;
		totalMemoryNumNF += memory[memoryLoc[x]];

		memoryAllocatedNF -= memory[memoryLoc[x]];
	}
}

void Process::freeBF(vector<int> memoryLoc, int memoryNum) {
	for (int x = 0; x < memoryNum; x++) {
		memoryIndexBF[memoryLoc[x]] = 1;
		totalMemoryNumBF += memory[memoryLoc[x]];

		memoryAllocatedBF -= memory[memoryLoc[x]];
	}
}

void Process::freeLF(vector<int> memoryLoc, int memoryNum) {
	for (int x = memoryNum - 1; x > 0; x--) {
		memoryIndexLF[memoryLoc[x]] = 1;
		totalMemoryNumLF += memory[memoryLoc[x]];

		memoryAllocatedLF -= memory[memoryLoc[x]];
	}
}

bool Process::isEmpty() {
	if (jobs.size() == 0) {
		return true;
	}

	return false;
}

void Process::printMetrics(ofstream& outputSummary) {
	cout << endl << endl << right << setw(45) << setfill(' ') << "First Fit" << setw(20) << "Next Fit" << setw(20) << "Best Fit" << setw(20) << "Last Fit" << endl;
	outputSummary << endl << right << setw(45) << setfill(' ') << "First Fit" << setw(20) << "Next Fit" << setw(20) << "Best Fit" << setw(20) << "Last Fit" << endl;

	int tempFF = 0, tempNF = 0, tempBF = 0, tempLF = 0;
	for (int x = 0; x < memoryIndexFF.size(); x++) {
		if (memoryIndexFF[x] == 0) {
			tempFF++;
		}
		else {
			externalFragFF++;
		}

		if (memoryIndexNF[x] == 0) {
			tempNF++;
		}
		else {
			externalFragNF++;
		}

		if (memoryIndexBF[x] == 0) {
			tempBF++;
		}
		else {
			externalFragBF++;
		}

		if (memoryIndexLF[x] == 0) {
			tempLF++;
		}
		else {
			externalFragLF++;
		}
	}

	//	Number of small Jobs
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Number of small Jobs: " << setw(20) << smallJobsNum << setw(20) << smallJobsNum << setw(20) << smallJobsNum << setw(20) << smallJobsNum << endl;
	outputSummary << "Number of small Jobs: " << setw(20) << smallJobsNum << setw(20) << smallJobsNum << setw(20) << smallJobsNum << setw(20) << smallJobsNum << endl;

	//	Number of medium Jobs
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Number of medium Jobs: " << setw(20) << mediumJobsNum << setw(20) << mediumJobsNum << setw(20) << mediumJobsNum << setw(20) << mediumJobsNum << endl;
	outputSummary << "Number of medium Jobs: " << setw(20) << mediumJobsNum << setw(20) << mediumJobsNum << setw(20) << mediumJobsNum << setw(20) << mediumJobsNum << endl;

	//	Number of large Jobs
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Number of large Jobs: " << setw(20) << largeJobsNum << setw(20) << largeJobsNum << setw(20) << largeJobsNum << setw(20) << largeJobsNum << endl;
	outputSummary << "Number of large Jobs: " << setw(20) << largeJobsNum << setw(20) << largeJobsNum << setw(20) << largeJobsNum << setw(20) << largeJobsNum << endl;

	//	Total memory Defined
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Total Memory Defined: " << setw(20) << totalMemoryNumFF << setw(20) << totalMemoryNumNF << setw(20) << totalMemoryNumBF << setw(20) << totalMemoryNumLF << endl;
	outputSummary << "Total Memory Defined: " << setw(20) << totalMemoryNumFF << setw(20) << totalMemoryNumNF << setw(20) << totalMemoryNumBF << setw(20) << totalMemoryNumLF << endl;

	//	Amount of memory Allocated
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Total Memory Allocated: " << setw(20) << memoryAllocatedFF << setw(20) << memoryAllocatedNF << setw(20) << memoryAllocatedBF << setw(20) << memoryAllocatedLF << endl;
	outputSummary << "Total Memory Allocated: " << setw(20) << memoryAllocatedFF << setw(20) << memoryAllocatedNF << setw(20) << memoryAllocatedBF << setw(20) << memoryAllocatedLF << endl;

	//	% memory in use
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	if (totalMemoryNumLF == 0 || totalMemoryNumFF == 0 || totalMemoryNumBF == 0 || totalMemoryNumNF == 0) {
		totalMemoryNumBF++;
		totalMemoryNumFF++;
		totalMemoryNumLF++;
		totalMemoryNumNF++;
	}
	cout << "% memory in use: " << setw(20) << (memoryAllocatedFF / totalMemoryNumFF) << setw(20) << (memoryAllocatedNF / totalMemoryNumNF) << setw(20) << (memoryAllocatedBF / totalMemoryNumBF) << setw(20) << (memoryAllocatedLF / totalMemoryNumLF) << endl;
	outputSummary << "% memory in use: " << setw(20) << (memoryAllocatedFF / totalMemoryNumFF) << setw(20) << (memoryAllocatedNF / totalMemoryNumNF) << setw(20) << (memoryAllocatedBF / totalMemoryNumBF) << setw(20) << (memoryAllocatedLF / totalMemoryNumLF) << endl;

	//	Required amount of memory
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Required Memory: " << setw(20) << requiredMemoryFF << setw(20) << requiredMemoryNF << setw(20) << requiredMemoryBF << setw(20) << requiredMemoryLF << endl;
	outputSummary << "Required Memory: " << setw(20) << requiredMemoryFF << setw(20) << requiredMemoryNF << setw(20) << requiredMemoryBF << setw(20) << requiredMemoryLF << endl;

	//	% Internal fragmentation
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	if(tempFF != 0 && tempNF != 0 && tempBF != 0 && tempLF != 0){
		internalFragFF = (memoryAllocatedFF - requiredMemoryFF) / tempFF;
		internalFragNF = (memoryAllocatedNF - requiredMemoryNF) / tempNF;
		internalFragBF = (memoryAllocatedBF - requiredMemoryBF) / tempBF;
		internalFragLF = (memoryAllocatedLF - requiredMemoryLF) / tempLF;
	}
	else {
		internalFragFF = 0;
		internalFragNF = 0;
		internalFragBF = 0;
		internalFragLF = 0;
	}
	cout << "Internal Fragmentation: " << setw(20) << internalFragFF << setw(20) << internalFragNF << setw(20) << internalFragBF << setw(20) << internalFragLF << endl;
	outputSummary << "Internal Fragmentation: " << setw(20) << internalFragFF << setw(20) << internalFragNF << setw(20) << internalFragBF << setw(20) << internalFragLF << endl;

	//	% memory free
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	memFreeFF = (totalMemory - memoryAllocatedFF) / totalMemoryNumFF;
	memFreeNF = (totalMemory - memoryAllocatedNF) / totalMemoryNumNF;
	memFreeBF = (totalMemory - memoryAllocatedBF) / totalMemoryNumBF;
	memFreeLF = (totalMemory - memoryAllocatedLF) / totalMemoryNumLF;
	cout << "% Memory Free: " << setw(20) << memFreeFF << setw(20) << memFreeNF << setw(20) << memFreeBF << setw(20) << memFreeLF << endl;
	outputSummary << "% Memory Free: " << setw(20) << memFreeFF << setw(20) << memFreeNF << setw(20) << memFreeBF << setw(20) << memFreeLF << endl;

	//	External Fragmentation
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "External Fragmentation: " << setw(20) << externalFragFF << setw(20) << externalFragNF << setw(20) << externalFragBF << setw(20) << externalFragLF << endl;
	outputSummary << "External Fragmentation: " << setw(20) << externalFragFF << setw(20) << externalFragNF << setw(20) << externalFragBF << setw(20) << externalFragLF << endl;

	//	Smallest Free Space
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Smallest Space: " << setw(20) << smallestSpace << setw(20) << smallestSpace << setw(20) << smallestSpace << setw(20) << smallestSpace << endl;
	outputSummary << "Smallest Space: " << setw(20) << smallestSpace << setw(20) << smallestSpace << setw(20) << smallestSpace << setw(20) << smallestSpace << endl;

	//	 Largest Free Space
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Largest Space: " << setw(20) << largestSpace << setw(20) << largestSpace << setw(20) << largestSpace << setw(20) << largestSpace << endl;
	outputSummary << "Largest Space: " << setw(20) << largestSpace << setw(20) << largestSpace << setw(20) << largestSpace << setw(20) << largestSpace << endl;

	//	Number of heap allocations
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Heap Allocations: " << setw(20) << heapAFF << setw(20) << heapAFF << setw(20) << heapAFF << setw(20) << heapAFF << endl;
	outputSummary << "Heap Allocations: " << setw(20) << heapAFF << setw(20) << heapAFF << setw(20) << heapAFF << setw(20) << heapAFF << endl;

	//	

		//	Efficiency Metrics
	//	Number of allocations
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Number allocations: " << setw(20) << numberOfAllocationsFF << setw(20) << numberOfAllocationsNF << setw(20) << numberOfAllocationsBF << setw(20) << numberOfAllocationsLF << endl;
	outputSummary << "Number allocations: " << setw(20) << numberOfAllocationsFF << setw(20) << numberOfAllocationsNF << setw(20) << numberOfAllocationsBF << setw(20) << numberOfAllocationsLF << endl;

	//	Number of allocation operations
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	cout << "Allocation operations: " << setw(20) << allocationOpFF << setw(20) << allocationOpNF << setw(20) << allocationOpBF << setw(20) << allocationOpLF << endl;
	outputSummary << "Allocation operations: " << setw(20) << allocationOpFF << setw(20) << allocationOpNF << setw(20) << allocationOpBF << setw(20) << allocationOpLF << endl;

	//	Average number of allocations operations
	cout << left << setw(40) << setfill(' ');
	outputSummary << left << setw(40) << setfill(' ');
	if (allocationOpFF != 0 && allocationOpNF != 0 && allocationOpBF != 0 && allocationOpLF != 0) {

		averageOpFF = numberOfAllocationsFF / allocationOpFF;
		averageOpNF = numberOfAllocationsNF / allocationOpNF;
		averageOpBF = numberOfAllocationsBF / allocationOpBF;
		averageOpLF = numberOfAllocationsLF / allocationOpLF;
	}
	else {
		averageOpFF = 0;
		averageOpNF = 0;
		averageOpBF = 0;
		averageOpLF = 0;
	}
	cout << "Average allocation operations: " << setw(20) << averageOpFF << setw(20) << averageOpLF << setw(20) << averageOpBF << setw(20) << averageOpLF << endl;
	outputSummary << "Average allocation operations: " << setw(20) << averageOpFF << setw(20) << averageOpLF << setw(20) << averageOpBF << setw(20) << averageOpLF << endl;

	//	
}