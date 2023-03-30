#ifndef MLFQ_H
#define MLFQ_H

#include<fstream>
#include<queue>
#include<iostream>
#include<string>
#include<iomanip>
#include<vector>

#include<ctime>
#include<cmath>
#include<cstdlib>

using namespace std;

//All details are also mentioned in Readme file
//this Class will be used for queue, queue will store details of process
struct Process
{
	int processId;
	int arrivalTime;
	int burstTime;
	Process(int pid,int at,int bt)
	{
		processId = pid;
		burstTime = bt;
		arrivalTime = at;
	}
};

//this class will be used in for vector, grant chart will used this class 
struct Node
{
	int pid;
	int start;
	int end;
	Node(int p,int s,int e)
	{
		pid = p;
		start = s;
		end = e;
	}
};


class MLFQ
{
	int * pid;
	int * arrival_time;
	int * burst_time;
	int * waitingTime;
	int * responseTime;
	int * turnAroundTime;
	double avgWaitingTime;
	double avgTurnAroundTime;
	double avgResponseTime;
	int totalPids = 0;
	double QUANTAM0 = 50;
	double QUANTAM1 = 100;
	queue<Process*> queue0;
	queue<Process*> queue1;
	queue<Process*> queue2;
	queue<Process*> tempQueue;
	vector<Node> granttChart;
	int counter;
	int currentQueue;
	const int queue1WaitTime = 200;
	const int queue2WaitTime = 300;
	bool stop;
	string fileName;
private:
	//this method will calculte total process
	int findTotalPids(const char* fName)
	{
		ifstream ifs(fName);
		int count = 0;
		int num = 0;
		while ((ifs >> num))
		{
			count++;
		}
		ifs.close();
		return count / 3;
	}
	//this method will initalize all the data from file 
	void initalizeData(const char * fName)
	{
		stop = false;
		counter = 0;
		if (totalPids != 0)
		{
			this->~MLFQ();
		}
		totalPids = findTotalPids(fName);
		if (totalPids > 0)
		{
			pid = new int[totalPids];
			arrival_time = new int[totalPids];
			burst_time = new int[totalPids];
			waitingTime = new int[totalPids];
			responseTime = new int[totalPids];
			turnAroundTime = new int[totalPids];
			ifstream ifs(fileName);
			for (int i = 0; i < totalPids; i++)
			{
				ifs >> pid[i];
				ifs >> arrival_time[i];
				ifs >> burst_time[i];
				waitingTime[i] = 0;
				responseTime[i] = 0;
				turnAroundTime[i] = 0;
			}
			ifs.close();
		}
		else
		{
			pid = NULL;
			arrival_time = NULL;
			burst_time = NULL;
			waitingTime = NULL;
			responseTime = NULL;
			turnAroundTime = NULL;
		}
	}
	// this method will display all queues, arrivat time, burst time, 
	// waiting time, response time, and grant chartt
	void display()
	{
		system("cls");		//clear screen
		//display table of all processes
		cout << "Process ID" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Waiting Time" << setw(20) << "Turn-Around Time" << setw(15) << "Response Time";
		for (int i = 0; i < totalPids; i++)
		{
			cout << '\n' << setw(5) << pid[i] << setw(15) << arrival_time[i] << setw(15) << burst_time[i];
			if (stop == true)
			{
				cout << setw(15) << waitingTime[i] << setw(18) << turnAroundTime[i] << setw(15) << responseTime[i];
			}
			else
			{
				cout << setw(15) << "-" << setw(18) << "-" << setw(15) << "-";
			}
		}
		//display counter, current queue, and current operation
		cout << '\n';
		cout << "\n " << right << setw(50) << "Counter : " << counter;
		cout << "\n " << right << setw(50) << "Current Queue : " << currentQueue;
		if (stop == true)
		{
			cout << " (Last Used)";
		}
		if (currentQueue == 0)
		{
			cout << "\n " << right << setw(50) << "Current Algorithm : " << "RR";
			if (stop == true)
			{
				cout << " (Last Used)";
			}
			cout << "\n " << right << setw(50) << "Current Quantum : " << QUANTAM0;
			if (stop == true)
			{
				cout << " (Last Used)";
			}
		}
		else if (currentQueue == 1)
		{
			cout << "\n " << right << setw(50) << "Current Algorithm : " << "RR";
			if (stop == true)
			{
				cout << " (Last Used)";
			}
			cout << "\n " << right << setw(50) << "Current Quantum : " << QUANTAM1;
			if (stop == true)
			{
				cout << " (Last Used)";
			}
		}
		else
		{
			cout << "\n " << right << setw(50) << "Current Algorithm : " << "SRTF";
			if (stop == true)
			{
				cout << " (Last Used)";
			}
		}
		cout << '\n';
		//display all queues states
		cout << "\n Queue (0) : ";
		if (queue0.empty() == true)
		{
			cout << "\tEmpty";
		}
		queue<Process* > temp;
		while (queue0.empty() == false)
		{
			Process* p = queue0.front();
			cout << " | P" << p->processId;
			temp.push(queue0.front());
			queue0.pop();
		}
		while (temp.empty() == false)
		{
			queue0.push(temp.front());
			temp.pop();
		}
		cout << '\n';
		cout << "\n Queue (1) : ";
		if (queue1.empty() == true)
		{
			cout << "\tEmpty";
		}
		while (queue1.empty() == false)
		{
			Process* p = queue1.front();
			cout << " | P" << p->processId;
			temp.push(queue1.front());
			queue1.pop();
		}
		while (temp.empty() == false)
		{
			queue1.push(temp.front());
			temp.pop();
		}
		cout << '\n';
		cout << "\n Queue (2) : ";
		if (queue2.empty() == true)
		{
			cout << "\tEmpty";
		}
		while (queue2.empty() == false)
		{
			Process* p = queue2.front();
			cout << " | P" << p->processId;
			temp.push(queue2.front());
			queue2.pop();
		}
		while (temp.empty() == false)
		{
			queue2.push(temp.front());
			temp.pop();
		}
		cout << '\n';
		//display grantt chart
		cout << "\n Grannt Chart:- \n\n";
		
		int n = granttChart.size();
		int step = 15;						//display max 15 process in one line
		int size = ceil(n / (double)step);	// total no of lines of grantt chart
		int i = 0;
		for (int j = 0; j < size; j++)
		{
			//display process id
			cout << " ";
			for (i = (j)*step ;i < (j+1) * step && i < n; i++)
			{
				int  s = granttChart[i].start;
				if ( s >= 10 && s< 100)
				{
					cout << " ";
				}
				else if (s >= 100 && s < 1000)
				{
					cout << "  ";
				}
				else if (s >= 1000)
				{
					cout << "   ";
				}
				if (granttChart[i].pid == -1)
				{
					cout << "  " << "i" << " ";
				}
				else
				{
					cout << " P" << granttChart[i].pid << " ";
				}
			}
			//display line with use of - and | 
			cout << '\n';
			for (i = (j)*step; i < (j + 1) * step && i < n; i++)
			{
				if (granttChart[i].start >= 10)
				{
					int  s = granttChart[i].start;
					if (s >= 10 && s < 100)
					{
						cout << "-|---";
					}
					else if (s >= 100 && s < 1000)
					{
						cout << "--|---";
					}
					else if (s >= 1000)
					{
						cout << "--|---";
					}
					
				}
				else if (granttChart[i].pid == -1)
				{
					cout << "-|-" << "-";
				}
				else
				{
					cout << "-|-" << "-";
				}
			}
			cout << "-|-";
			cout << '\n';
			cout << " ";
			
			// display the values of starting and position of process in grantt chart
			for (i = (j)*step; i < (j + 1) * step && i < n; i++)
			{
				cout << granttChart[i].start << "   ";
			}
			cout << granttChart[i-1].end << "   ";
			if (j != size)
			{
				cout << "\n\n";
			}
		}
		cout << '\n';
		//Sleep(2000);		//for delay of 0.5 sec
	}
	//check all queues are empty or not
	bool isEmptyAllQueue()
	{
		if (queue0.empty()==true && queue1.empty()==true && queue2.empty()==true && tempQueue.empty()==true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//sort the process acording to arrival time
	void sortProcesses(Process** p)
	{
		int n = totalPids;
		for (int i = 0; i < n; i++)
		{
			int min = i;
			for (int j = i + 1;j<n; j++)
			{
				if (p[j]->arrivalTime < p[min]->arrivalTime)
				{
					min = j;
				}
			}
			Process* temp = p[min];
			p[min] = p[i];
			p[i] = temp;
		}
	}
	//calculate the waiting time of all process
	void calcWaitingTime()
	{
		
		for (int i = 0; i < totalPids; i++)
		{
			int finishTime = 0;
			int n = granttChart.size();
			for (int j = 0; j < n; j++)
			{
				if (granttChart[j].pid == pid[i])
				{
					finishTime = granttChart[j].end;
				}
			}
			waitingTime[i] = finishTime - arrival_time[i] - burst_time[i] ;
		}
	}
	//calculte the turnaround time of all processes
	void calcTurnAroundTime()
	{
		for (int i = 0; i < totalPids; i++)
		{
			int finishTime = 0;
			int n = granttChart.size();
			for (int j = 0; j < n; j++)
			{
				if (granttChart[j].pid == pid[i])
				{
					finishTime = granttChart[j].end;
				}
			}
			turnAroundTime[i] = finishTime - arrival_time[i];
		}
	}
	//calculate the response time of all processes
	void calcResponseTime()
	{
		for (int i = 0; i < totalPids; i++)
		{
			int n = granttChart.size();
			int j = 0;
			while (j < n && granttChart[j].pid!=pid[i])
			{
				j++;
			}
			responseTime[i] = granttChart[j].start - arrival_time[i];
		}
	}
	// calculate average waiting time
	void calcAvgWaitingTime()
	{
		avgWaitingTime = 0;
		for (int i = 0; i < totalPids; i++)
		{
			avgWaitingTime = avgWaitingTime+waitingTime[i];
		}
		avgWaitingTime = avgWaitingTime / totalPids;
	}
	// calculate average turn around time
	void calcAvgTurnAroundTime()
	{
		avgTurnAroundTime = 0;
		for (int i = 0; i < totalPids; i++)
		{
			avgTurnAroundTime = avgTurnAroundTime + turnAroundTime[i];
		}
		avgTurnAroundTime = avgTurnAroundTime / totalPids;
	}
	// calculate average response time
	void calcAvgResponseTime()
	{
		avgResponseTime = 0;
		for (int i = 0; i < totalPids; i++)
		{
			avgResponseTime = avgResponseTime + responseTime[i];
		}
		avgResponseTime = avgResponseTime / totalPids;
	}
public:
	//constructor of MLFQ
	MLFQ()
	{
		stop = false;
		totalPids = 0;
		pid = NULL;
		arrival_time = NULL;
		burst_time = NULL;
		waitingTime = NULL;
		responseTime = NULL;
		turnAroundTime = NULL;
		avgResponseTime = 0;
		avgWaitingTime = 0;
		avgTurnAroundTime = 0;
		counter = 0;
		currentQueue = 0;
	}
	//Destructor of MLFQ
	~MLFQ()
	{
		if (totalPids != 0)
		{
			delete[] pid;
			delete[] arrival_time;
			delete[] burst_time;
			delete[] waitingTime;
			delete[] responseTime;
			pid = NULL;
			arrival_time = NULL;
			burst_time = NULL;
			waitingTime = NULL;
			responseTime = NULL;
			totalPids = 0;
		}
		else
		{
			totalPids = 0;
			pid = NULL;
			arrival_time = NULL;
			burst_time = NULL;
			waitingTime = NULL;
			responseTime = NULL;
		}
	}
	//set file name for input data
	void setFileName(string fName)
	{
		fileName = fName;
	}
	//change quantum for round robbin for queue0 or queue1 if necessary 
	void setQuantum(double quant0, double quant1)
	{
		QUANTAM0 = quant0;
		QUANTAM1 = quant1;
	}
	//start which will handle all mlfq algortihm , 
	// updates queus , and display grantt chart
	// at the end display avg waiting time, avg response time, and avg turn arround time
	void start()
	{
		initalizeData(fileName.data());
		if (totalPids == 0)
		{
			cout << "\nNo Process";
			exit(0);
		}
		currentQueue = 0;
		Process ** proc = new Process * [totalPids];
		for (int i = 0; i < totalPids; i++)
		{
			proc[i] = new Process(pid[i],arrival_time[i],burst_time[i]);
		}
		sortProcesses(proc);
		for (int i = 0; i < totalPids; i++)
		{
			tempQueue.push(proc[i]);
		}
		bool stopMLFQ = isEmptyAllQueue();	
		//int bTime = 0;
		int idleTime = 0;
		cout << '\n';
		int waitTimer = 0;
		while (stopMLFQ!=true)
		{
			while (tempQueue.empty()==false && tempQueue.front()->arrivalTime<=counter)
			{
				queue0.push(tempQueue.front());
				tempQueue.pop();
			}
			display();
			// run round robin on Queue0 with quantum QUANT0
			if (queue0.empty() == false)
			{
				Process * p = queue0.front();
				if (counter >= p->arrivalTime)
				{
					if (p->burstTime > QUANTAM0)
					{
						queue0.front()->burstTime = queue0.front()->burstTime - QUANTAM0;
						granttChart.push_back(Node(queue0.front()->processId, counter, counter+QUANTAM0));
						queue1.push(queue0.front());
						queue0.pop();
						counter = counter + QUANTAM0;
					}
					else if (p->burstTime == QUANTAM0)
					{
						granttChart.push_back(Node(queue0.front()->processId, counter, counter + QUANTAM0));
						counter = counter + QUANTAM0;
						queue0.pop();
					}
					else
					{
						granttChart.push_back(Node(queue0.front()->processId, counter, counter + queue0.front()->burstTime));
						counter = counter + queue0.front()->burstTime;
						queue0.pop();
					}
				}
				else	//idle time
				{
					granttChart.push_back(Node(-1, counter, counter + 1));
					counter++;
				}
				if (queue0.empty() == true)
				{
					if (queue1.empty() == false)
					{
						waitTimer = queue1WaitTime;
						currentQueue = 1;
					}
					else if (queue2.empty() == false)
					{
						waitTimer = queue2WaitTime;
						currentQueue = 2;
					}
				}
			}
			// run round robin on Queue1 with quantum QUANT1
			else if (queue1.empty() == false)
			{
				Process* p = queue1.front();
				if (counter >= p->arrivalTime)
				{
					if (p->burstTime > QUANTAM1)
					{
						queue1.front()->burstTime = queue1.front()->burstTime - QUANTAM1;
						granttChart.push_back(Node(queue1.front()->processId, counter, counter + QUANTAM1));
						queue2.push(queue1.front());
						queue1.pop();
						waitTimer = waitTimer - QUANTAM1;
						counter = counter + QUANTAM1;
					}
					else if (p->burstTime == QUANTAM1)
					{	
						granttChart.push_back(Node(queue1.front()->processId, counter, counter + QUANTAM1));
						counter = counter + QUANTAM1;
						waitTimer = waitTimer - QUANTAM1;
						queue1.pop();
					}
					else
					{
						granttChart.push_back(Node(queue1.front()->processId, counter, counter + queue1.front()->burstTime));
						counter = counter + queue1.front()->burstTime;
						waitTimer = waitTimer - queue1.front()->burstTime;
						queue1.pop();
					}
					if (waitTimer < 1)
					{
						if (queue1.empty() == false)
						{
							queue0.push(queue1.front());
							queue1.pop();
							currentQueue = 0;
							waitTimer = 0;
						}
						if (queue0.empty())
						{
							currentQueue = 2;
							waitTimer = queue2WaitTime;
						}
					}
					else if (queue1.empty() == true)
					{
						if (queue0.empty() == true)
						{
							currentQueue = 2;
							waitTimer = queue2WaitTime;
						}
						else
						{
							currentQueue = 1;
							waitTimer = 0;
						}
					}
				}
				else
				{
					granttChart.push_back(Node(-1, counter, counter + 1));
					counter++;
				}
			}
			// run SRTF on Queue2
			else if (queue2.empty() == false)
			{
				vector<Process*> temp;
				while (queue2.empty()==false)
				{
					temp.push_back(queue2.front());
					queue2.pop();
				}
				int n = temp.size();
				int min = 0;
				for (int i = 1; i < n; i++)
				{
					if (temp[i]->burstTime < temp[min]->burstTime)
					{
						min = i;
					}
				}
				if (temp[min]->burstTime > 20)
				{
					temp[min]->burstTime = temp[min]->burstTime- 20;
					waitTimer = waitTimer - 20;
					granttChart.push_back(Node(temp[min]->processId, counter, counter + 20));
					counter = counter + 20;
				}
				else
				{
					temp[min]->burstTime = temp[min]->burstTime - 1;
					waitTimer = waitTimer - 1;
					granttChart.push_back(Node(temp[min]->processId, counter, counter + 1));
					counter = counter + 1;
				}				
				for (int i = 0; i < n; i++)
				{
					if (temp[i]->burstTime > 0)
					{
						queue2.push(temp[i]);
					}
				}
				temp.clear();
				if (waitTimer < 1)
				{
					if (queue2.empty()==false)
					{
						if (queue2.empty() == false)
						{
							queue1.push(queue2.front());
							queue2.pop();
						}
						currentQueue = 1;
						waitTimer = queue1WaitTime;
					}
				}
			}
			else
			{
				granttChart.push_back(Node(-1, counter, counter + 1));
				counter++;
			}
			stopMLFQ = isEmptyAllQueue();
		}
		stop = true;
		calcWaitingTime();
		calcAvgWaitingTime();
		calcTurnAroundTime();
		calcResponseTime();
		calcAvgTurnAroundTime();
		calcAvgResponseTime();
		display();
		cout << "\n"<<setw(50)<<"Avg Waiting Time :" << setw(10) << avgWaitingTime;
		cout << "\n" << setw(50) << "Avg Turn Around TIme :" << setw(10) << avgTurnAroundTime;
		cout << "\n" << setw(50) << "Avg Response Time :"<<setw(10)<< avgResponseTime<<'\n';
	}
	
};

#endif
