//	Program Name:		Memory Simulator
//	Programmers Name:	Husam Alsheikh, Judy Anani
//	Description:		Program Simulates allocation and deallocation of memory
//	Date Created:		09/27/2020

#pragma once
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Node {
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

	vector<int> elementsDepart;

	int processing;
	int departureTime;
	int heapIncrement;

	//	Different algorithms result in different results
	int codeMemoryUnitsFF;
	int stackMemoryUnitsFF;
	int codeMemoryUnitsNF;
	int stackMemoryUnitsNF;
	int codeMemoryUnitsBF;
	int stackMemoryUnitsBF;
	int codeMemoryUnitsLF;
	int stackMemoryUnitsLF;

	vector<int> codeIndexFF;
	vector<int> stackIndexFF;
	vector<int> codeIndexNF;
	vector<int> stackIndexNF;
	vector<int> codeIndexBF;
	vector<int> stackIndexBF;
	vector<int> codeIndexLF;
	vector<int> stackIndexLF;

	vector<vector<int>> heapIndexFF;
	vector<vector<int>> heapIndexNF;
	vector<vector<int>> heapIndexBF;
	vector<vector<int>> heapIndexLF;

	vector<int> memoryUnitsAllocateFF;
	vector<int> memoryUnitsAllocateNF;
	vector<int> memoryUnitsAllocateBF;
	vector<int> memoryUnitsAllocateLF;

	vector<int> elementCompleteFF;
	vector<int> elementCompleteNF;
	vector<int> elementCompleteBF;
	vector<int> elementCompleteLF;
};

class PriorityQueue {
private:
	vector <Node> priorityQueue;
	int size;

public:
	PriorityQueue();

	void enqueue(struct Job edge);

	void heapify(int index);

	void deleteNode();

	Node extractMin();

	int peek();

	bool isEmpty();
};