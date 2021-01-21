//	Program Name:		Memory Simulator
//	Programmers Name:	Husam Alsheikh, Judy Anani
//	Description:		Program Simulates allocation and deallocation of memory
//	Date Created:		09/27/2020

#include <iostream>
#include <time.h>
#include <iomanip>
#include <fstream>
#include "RNG.h"

RNG::RNG() {
	srand(time(NULL));
	jobNumber = 0;
}

int RNG::getArrivalTime() {
	return temp.arrivalT;
}

int RNG::getRunningTime() {
	return temp.runningT;
}

int RNG::getCodeSize() {
	return temp.codeS;
}

int RNG::getStackSize() {
	return temp.stackS;
}

int RNG::getHeapSize() {
	return temp.heapS;
}

void RNG::setPercentages(int smallNum, int mediumNum, int largeNum) {
	this->smallNum = smallNum;
	this->mediumNum = mediumNum;
	this->largeNum = largeNum;
}

Job RNG::generateNumbers(int simTime) {
	//	Random Job Type
	temp.jobType = rand() % 100 + 1;

	jobNumber++;
	temp.jobNum = jobNumber;

	if (temp.jobType <= smallNum) {	//	Small Job
		temp.type = "Small";
		temp.jobType = 1;

		//	Random arrival time
		temp.arrivalT = simTime + rand() % 5 + 1;

		//	Random running time
		temp.runningT = rand() % 3 + 4;

		//	Random code size
		temp.codeS = rand() % 41 + 40;

		//	Random stack size
		temp.stackS = rand() % 21 + 20;

		//	Heap size
		temp.heapS = temp.runningT * 50;

		for (int x = 0; x < temp.heapS; x++) {
			//	Random heap elements
			temp.heapElements.push_back(rand() % 31 + 20);

			temp.elementsLifetime.push_back(rand() % temp.runningT + 1);
		}
	}
	else if (temp.jobType <= (smallNum + mediumNum)) {	//	Medium Job
		temp.type = "Medium";
		temp.jobType = 2;

		//	Random arrival time
		temp.arrivalT = simTime + rand() % 10 + 1;

		//	Random running time
		temp.runningT = rand() % 3 + 9;

		//	Random code size
		temp.codeS = rand() % 61 + 60;

		//	Random stack size
		temp.stackS = rand() % 41 + 40;

		//	Heap size
		temp.heapS = temp.runningT * 100;

		for (int x = 0; x < temp.heapS; x++) {
			temp.heapElements.push_back(rand() % 31 + 20);

			temp.elementsLifetime.push_back(rand() % temp.runningT + 1);
		}
	}
	else {	//	Large Job
		temp.type = "Large";
		temp.jobType = 3;

		//	Random arrival time
		temp.arrivalT = simTime + rand() % 3 + 24;

		//	Random running time
		temp.runningT = rand() % 25 + 1;

		//	Random code size
		temp.codeS = rand() % 101 + 120;

		//	Random stack size
		temp.stackS = rand() % 61 + 60;

		//	Heap size
		temp.heapS = temp.runningT * 250;


		for (int x = 0; x < temp.heapS; x++) {
			temp.heapElements.push_back(rand() % 31 + 20);

			temp.elementsLifetime.push_back(rand() % temp.runningT + 1);
		}
	}

	return temp;
}