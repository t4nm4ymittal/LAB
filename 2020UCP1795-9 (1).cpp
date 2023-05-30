#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int MAX_RESOURCES = 100;

// Define the number of resources and processes
int numResources;
int numProcesses;

// Define the available resources and maximum needs of each process
int available[MAX_RESOURCES];
int maximum[MAX_RESOURCES][MAX_RESOURCES];

// Define the allocation and need of each process
int allocation[MAX_RESOURCES][MAX_RESOURCES];
int need[MAX_RESOURCES][MAX_RESOURCES];

// Define a vector to keep track of which processes are finished
vector<bool> finished;

// Define a function to check if a process can be safely executed
bool isSafe(int process) {
    // Check if all resources needed by this process are available
    for (int i = 0; i < numResources; i++) {
        if (need[process][i] > available[i]) {
            return false;
        }
    }

    // If the process can be executed, simulate the allocation of resources
    for (int i = 0; i < numResources; i++) {
        available[i] += allocation[process][i];
    }

    // Mark the process as finished
    finished[process] = true;

    return true;
}

// Define a function to check for deadlock
bool isDeadlock() {
    // Initialize a vector to keep track of which processes can be executed
    vector<bool> canExecute(numProcesses, false);

    // Initialize the available resources vector
    for (int i = 0; i < numResources; i++) {
        available[i] = 0;
        for (int j = 0; j < numProcesses; j++) {
            available[i] += allocation[j][i];
        }
    }

    // Check if any processes can be executed
    bool canExecuteProcesses = true;
    while (canExecuteProcesses) {
        canExecuteProcesses = false;
        for (int i = 0; i < numProcesses; i++) {
            if (!finished[i] && isSafe(i)) {
                canExecute[i] = true;
                canExecuteProcesses = true;
            }
        }
    }

    // Check if there are any processes that cannot be executed
    for (int i = 0; i < numProcesses; i++) {
        if (!finished[i] && !canExecute[i]) {
            return true;
        }
    }

    return false;
}

int main() {
    // Read the input from the file
    // ifstream infile("input.txt");
    cin >> numResources;

    // Read the number of instances of each resource
    for (int i = 0; i < numResources; i++) {
        int numInstances;
        cin >> numInstances;
        available[i] = numInstances;
    }

    // Read the number of processes
    cin >> numProcesses;

    // Initialize the finished vector
    finished.resize(numProcesses, false);

    // Read the resource requirements and initial resource allocation of each process
    for (int i = 0; i < numProcesses; i++) {
        int processID;
        cin >> processID;

        for (int j = 0; j < numResources; j++) {
            int requirement;
            cin >> requirement;
            maximum[processID][j] = requirement;
            need[processID][j] = requirement;
        }

        for (int j = 0; j < numResources; j++) {
            int allocationAmount;
            cin >> allocationAmount;
            allocation[processID][j] = allocationAmount;
            available[j] -= allocationAmount;
            need[processID][j] -= allocationAmount;
    }
}

// Check for deadlock
if (isDeadlock()) {
    cout << "Deadlock detected\n";
} else {
    cout << "No deadlock detected\n";
}

return 0;
}
