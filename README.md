# OSMemoryManagement
Program for Operating Systems Class. Program simulates memory allocation and deallocation in an OS.

The program will simulate the allocation of memory.  
The different memory sized jobs to be processed will be arriving into the system on a random basis based on user parameters with random processing time and memory units assigned to them.  
The user will be requested to enter the percentage of the three types of jobs to be created (must total 100%). What if it doesn’t?
Jobs will arrive every 3 +/- 2 time units.   
User will enter the memory unit size (multiple of 8). Memory units are all the same size. You must have a test with memory unit size of 8 and at least one other value. The user will also enter the number of memory units of that size that are available.   

  Job | Running Time | Code Size | Stack Size | Heap elements
--------|-------------|-----------|-----------|--------------
"Small" | 5 +/- 1 | 60 +/- 20 | 30 +/- 10| run time * 50
"Medium" | 10 +/- 1 | 90 +/- 30 | 60 +/- 20	| run time * 100
"Large" | 25 +/- 1 | 170 +/- 50	| 90 +/- 30 | run time * 250
 
Each heap element will have an amount of memory of 35 +/- 15.  
Assume the heap elements are allocated evenly across the run time. Heap elements lifetime will be randomly distributed between 1 and the end of the process.    
Memory must be allocated in units of memory unit size.  

### Example: (memory unit size of 8)
Small job  
Run Time: 5 time units  
Code Size: 60 memory                  - 8 memory units allocate for all 5 time units  
Stack Size: 20 memory units        - 3 memory units allocate for all 5 time units  
Heap elements: 5 * 50 = 250 heap elements    
50 heap elements arrive each time unit   

Heap element 1: 20 memory            3 memory units lifetime: 4 time units      
Heap element 2: 50 memory    	       7 memory units lifetime: 2 time units  
Heap element 3: 32 memory            4 memory units lifetime: 1 time unit  
Heap element 4: 45 memory            6 memory units lifetime: 5 time units  
….  

The user will also be able to have the option to create a simulation with lost objects.  
If user chooses to simulate lost objects  
Every 100th process job of each type (e.g. process arrival 100, 200, 300 … of “small” process type) will have no free operations on its heap allocations    
The program will determine arrival and departure time of jobs, stack and heap elements.  
At the appropiate time the memory units will allocate and deallocate calling methods malloc and free. You will perform all 4 memory assignment algorithms.  
int mallocFF(int size) where size represents amount of memory requested (NOT memory units), returns location for first fit algorithm, mallocNF for next fit, mallocBF for best fit, mallocWF for worst fit.  
void freeFF(int loc) where loc represents the initial memory unit location to return to unused status.  

The user will enter the name of the test and the name of the output summary and log files (one for each type of algorithm) where you will output events.  
Screen output and summary file will display test name, all user input parameters, then metrics (starting with end of prefill events – described below), arrival and departure process events and memory metrics every 20 time units.  
Log file will log every memory allocation and deallocation (time, process, location).     
You will also produce a count of operations performed for each algorithm.  

You should be counting and reporting operations for both allocation operations and free operations. Examples of operations would include: comparisons to find memory location, actions to update data structure after allocation or delete (e.g. adjustments to a linked list, adjustments to a heap/priority queue).  
You are to evaluate 2 additional program efficiency metrics of your choosing.   

The simulation should run for 2000 time units to “pre-fill” system memory with jobs, report pre-fill “steady state” metrics  then continue to run for an additional 10000 time units. Final test metrics will be summarized and analyzed.  
You are to submit a 1-2 page write up (tables of data are not include in page count) on your review of runs with various memory algorithms and memory unit size and your analysis and additional run(s) to determine how to run the memory management system most efficiently.  
