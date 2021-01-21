//	Program Name:		Memory Simulator
//	Programmers Name:	Husam Alsheikh, Judy Anani
//	Description:		Program Simulates allocation and deallocation of memory
//	Date Created:		09/27/2020

#include <iostream>
#include <vector>
#include <fstream>
#include "PriorityQueue.h"
#include "RNG.h"

using namespace std;

PriorityQueue::PriorityQueue() {
	size = 0;
}

void PriorityQueue::enqueue(struct Job job) {
	struct Node temp;

	priorityQueue.resize(size + 1);

	priorityQueue[size].type = job.type;
	priorityQueue[size].jobType = job.jobType;
	priorityQueue[size].jobNum = job.jobNum;

	priorityQueue[size].arrivalT = job.arrivalT;
	priorityQueue[size].runningT = job.runningT;
	priorityQueue[size].codeS = job.codeS;
	priorityQueue[size].stackS = job.stackS;
	priorityQueue[size].heapS = job.heapS;

	priorityQueue[size].heapElements = job.heapElements;
	priorityQueue[size].elementsLifetime = job.elementsLifetime;

	priorityQueue[size].processing = 0;
	priorityQueue[size].heapIncrement = 0;


	priorityQueue[size].heapIndexFF.resize(job.heapS);
	priorityQueue[size].heapIndexNF.resize(job.heapS);
	priorityQueue[size].heapIndexBF.resize(job.heapS);
	priorityQueue[size].heapIndexLF.resize(job.heapS);

	for (int x = 0; x < priorityQueue[size].heapS; x++) {
		priorityQueue[size].elementsDepart.push_back(-1);

		priorityQueue[size].elementCompleteFF.push_back(-1);
		priorityQueue[size].elementCompleteNF.push_back(-1);
		priorityQueue[size].elementCompleteBF.push_back(-1);
		priorityQueue[size].elementCompleteLF.push_back(-1);

		priorityQueue[size].memoryUnitsAllocateFF.push_back(-1);
		priorityQueue[size].memoryUnitsAllocateNF.push_back(-1);
		priorityQueue[size].memoryUnitsAllocateBF.push_back(-1);
		priorityQueue[size].memoryUnitsAllocateLF.push_back(-1);
	}

	priorityQueue[size].departureTime = -1;

	int x = size;
	while (x >= 1) {
		if (priorityQueue[x / 2].arrivalT > priorityQueue[x].arrivalT) {
			temp = priorityQueue[x / 2];
			priorityQueue[x / 2] = priorityQueue[x];
			priorityQueue[x] = temp;

			x = x / 2;
		}
		else {
			break;
		}
	}

	size++;
}

void PriorityQueue::heapify(int index) {
	struct Node temp;

	int l = 2 * index + 1;
	int r = 2 * index + 2;
	int smallest = index;
	if (l < size && priorityQueue[l].arrivalT < priorityQueue[index].arrivalT)
		smallest = l;
	if (r < size && priorityQueue[r].arrivalT < priorityQueue[smallest].arrivalT)
		smallest = r;
	if (smallest != index)
	{
		temp = priorityQueue[index];
		priorityQueue[index] = priorityQueue[smallest];
		priorityQueue[smallest] = temp;

		heapify(smallest);
	}
}

void PriorityQueue::deleteNode() {
	priorityQueue.erase(priorityQueue.begin());

	size--;

	heapify(0);
}

Node PriorityQueue::extractMin() {
	struct Node min = priorityQueue[0];

	deleteNode();

	return min;
}

int PriorityQueue::peek() {
	return priorityQueue[0].arrivalT;
}

bool PriorityQueue::isEmpty() {
	if (size == 0) {
		return true;
	}

	return false;
}