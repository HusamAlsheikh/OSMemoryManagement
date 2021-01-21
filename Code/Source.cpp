//	Program Name:		Memory Simulator
//	Programmers Name:	Husam Alsheikh, Judy Anani
//	Description:		Program Simulates allocation and deallocation of memory
//	Date Created:		09/27/2020

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "RNG.h"
#include "PriorityQueue.h"
#include "Process.h"

using namespace std;

void main() {
	ofstream outputSummary;
	ofstream logFF;
	ofstream logNF;
	ofstream logBF;
	ofstream logLF;

	string testName;
	string outputName;
	string logFFName;
	string logNFName;
	string logBFName;
	string logLFName;

	cout << "Enter name of test: ";
	cin >> testName;

	cout << "\n\nEnter name of output summary file: ";
	cin >> outputName;

	cout << "\n\n Enter name of log file for first fit:	";
	cin >> logFFName;

	cout << "\n\n Enter name of log file for next fit:	";
	cin >> logNFName;

	cout << "\n\n Enter name of log file for best fit:	";
	cin >> logBFName;

	cout << "\n\n Enter name of log file for last fit:	";
	cin >> logLFName;

	outputSummary.open(outputName, ios::out);
	logFF.open(logFFName, ios::out);
	logNF.open(logNFName, ios::out);
	logBF.open(logBFName, ios::out);
	logLF.open(logLFName, ios::out);

	cout << setw(50) << testName;
	outputSummary << setw(50) << testName;

	//	Memory Unit sizes and number of each size
	int numMemorySizes;
	vector<int> memoryNum;
	vector<int> memorySize;

	int smallNum, mediumNum, largeNum;
	int simTime = 0;

	//	Ask user for percentage of each job type
	cout << "\n\nEnter the percentage of each of the three types of jobs to be created (must total 100%)" << endl;
	outputSummary << "\n\nEnter the percentage of each of the three types of jobs to be created (must total 100%)" << endl;

	cout << "Small Jobs: ";
	cin >> smallNum;
	cout << "Medium Jobs: ";
	cin >> mediumNum;
	cout << "Large Jobs: ";
	cin >> largeNum;

	//	Data validate job type percentages
	while (smallNum + mediumNum + largeNum != 100) {
		cout << "\nPercentage of each job does not equal to 100%, try again!" << endl;
		cout << "Small Jobs: ";
		cin >> smallNum;
		cout << "Medium Jobs: ";
		cin >> mediumNum;
		cout << "Large Jobs: ";
		cin >> largeNum;
	}

	outputSummary << "Small Jobs: " << smallNum << endl;
	outputSummary << "Medium Jobs: " << mediumNum << endl;
	outputSummary << "Large Jobs: " << largeNum << endl;

	//	Ask user for memory unit size
	cout << "\nEnter how many different memory unit sizes you would like to have: ";
	outputSummary << "\nEnter how many different memory unit sizes you would like to have: ";

	cin >> numMemorySizes;
	outputSummary << numMemorySizes;

	for (int x = 0; x < numMemorySizes; x++) {
		int temp = 0;
		cout << "\nEnter memory unit size: ";
		outputSummary << "\nEnter memory unit size: ";

		cin >> temp;
		while (temp % 8 != 0) {
			cout << "\nMemory size must be multiple of 8, please try again: ";
			cin >> temp;
		}
		outputSummary << temp;

		memorySize.push_back(temp);

		//	Ask user for number of memory units
		cout << "Enter number of memory units: ";
		outputSummary << "\nEnter number of memory units: ";

		cin >> temp;
		outputSummary << temp << endl;

		memoryNum.push_back(temp);
	}

	RNG rng;
	PriorityQueue pq;
	Process processes(memorySize, memoryNum);

	rng.setPercentages(smallNum, mediumNum, largeNum);

	//	Run for 2000 time units to prefill memory
	while(simTime < 2000) {
		pq.enqueue(rng.generateNumbers(simTime));
		
		simTime++;
	}

	//	Print metrics at end of prefill events
	processes.printMetrics(outputSummary);
	
	while (simTime < 10000) {
		while (!pq.isEmpty() && pq.peek() <= simTime) {
			//	Print arrival of job
			cout << "\nJob Arrival: ";
			outputSummary << "\nJob Arrival: ";

			processes.addJob(pq.extractMin(), outputSummary);
		}
		if (!processes.isEmpty()) {
			processes.run(simTime, outputSummary, logFF, logNF, logBF, logLF);
		}

		if (simTime % 20 == 0) {
			processes.printMetrics(outputSummary);
		}

		if (processes.isEmpty()) {
			break;
		}

		simTime++;
	}

	//	Final Metric print out
	processes.printMetrics(outputSummary);
	

	system("pause");
}