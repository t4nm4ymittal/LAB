#include <bits/stdc++.h>
using namespace std;

class Process
{
public:
    int pid;
    int arrival;
    int priority;
    vector<pair<string, int>> burst;
};

void PriorityPreEmptive(int n,int q,map<int,Process> mp){
    vector<pair<int,int>> v;
    for(auto it : mp)
        v.push_back({it.second.arrival,it.first});
    sort(v.begin(),v.end());

    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<tuple<int, int, int, int>>> pq;
    map<int, vector<int>> arr;
    for (auto it : mp)
    {
        int arrival = it.second.arrival;
        int pid = it.first;
        arr[pid].push_back(arrival);
    }

    Process P = mp[v[0].second];
    pq.push({P.priority,P.pid,0,P.arrival});

    int t = 0,i = 1;
    while (!pq.empty())
    {
        int priority = get<0>(pq.top());
        int pid = get<1>(pq.top());
        int ind = get<2>(pq.top());
        int arrival = get<3>(pq.top());
        pq.pop();
        if(ind>=mp[pid].burst.size()) continue;
        int burst = mp[pid].burst[ind].second;
        if (t == 0)
            t = arrival;
        arr[pid].push_back(t);
        t += burst;
        pq.push({priority, pid, ++ind, t});
        arr[pid].push_back(t);
        while(i<v.size()){
            if(v[i].first>t)break;
            P = mp[v[i].second];
            pq.push({P.priority,P.pid,0,P.arrival});
            i++;
        }
    }

    cout << "Priority (Non Pre-Emptive)" << "\n";
    int t_turnaround = 0, t_waiting = 0, t_response = 0;
    for(auto it:arr){
        int response = it.second[1] - it.second[0];
        int size = it.second.size();
        int turnaround = it.second[size-1] - it.second[0];
        int totalBurst = 0;
        for(auto i : mp[it.first].burst)
            totalBurst += i.second;
        int waiting = turnaround - totalBurst;
        cout << it.first << "->" << " Response time: " << response << "\n";
        cout << it.first << "->" << " Turnaround time: " << turnaround << "\n";
        cout << it.first << "->" << " Waiting time: " << waiting << "\n";

        t_response += response;
        t_waiting += waiting;
        t_turnaround += turnaround;
        cout<< "\n";
    }
    cout << "Average reponse time: " << t_response/n << "\n";
    cout << "Average turnaround time: " << t_turnaround/n << "\n";
    cout << "Average waiting time: " << t_waiting/n << "\n";
}

void PriorityNonPreEmptive(int n,map<int,Process> mp){
    vector<pair<int,int>> v;
    for(auto it : mp)
        v.push_back({it.second.arrival,it.first});
    sort(v.begin(),v.end());
    
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<tuple<int, int, int, int>>> pq;
    map<int, vector<int>> arr;
    for (auto it : mp)
    {
        int arrival = it.second.arrival;
        int pid = it.first;
        arr[pid].push_back(arrival);
    }
    
    Process P = mp[v[0].second];
    pq.push({P.priority,P.pid,0,P.arrival});

    int t = 0,i = 1;
    while (!pq.empty())
    {
        int priority = get<0>(pq.top());
        int pid = get<1>(pq.top());
        int ind = get<2>(pq.top());
        int arrival = get<3>(pq.top());
        pq.pop();
        if(ind>=mp[pid].burst.size()) continue;
        int burst = mp[pid].burst[ind].second;
        if (t == 0)
            t = arrival;
        arr[pid].push_back(t);
        t += burst;
        pq.push({priority, pid, ++ind, t});
        arr[pid].push_back(t);
        while(i<v.size()){
            if(v[i].first>t)break;
            P = mp[v[i].second];
            pq.push({P.priority,P.pid,0,P.arrival});
            i++;
        }
    }

    cout << "Priority (Non Pre-Emptive)" << "\n";
    int t_turnaround = 0, t_waiting = 0, t_response = 0;
    for(auto it:arr){
        int response = it.second[1] - it.second[0];
        int size = it.second.size();
        int turnaround = it.second[size-1] - it.second[0];
        int totalBurst = 0;
        for(auto i : mp[it.first].burst)
            totalBurst += i.second;
        int waiting = turnaround - totalBurst;
        cout << it.first << "->" << " Response time: " << response << "\n";
        cout << it.first << "->" << " Turnaround time: " << turnaround << "\n";
        cout << it.first << "->" << " Waiting time: " << waiting << "\n";

        t_response += response;
        t_waiting += waiting;
        t_turnaround += turnaround;
        cout<< "\n";
    }
    cout << "Average reponse time: " << t_response/n << "\n";
    cout << "Average turnaround time: " << t_turnaround/n << "\n";
    cout << "Average waiting time: " << t_waiting/n << "\n";
}

void rr(int n, int q, map<int, Process> mp)
{
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<tuple<int, int, int, int>>> pq;
    map<int, vector<int>> arr;
    for (auto it : mp)
    {
        int arrival = it.second.arrival;
        int pid = it.first;
        int ind = 0;
        int burst = it.second.burst[0].second;
        pq.push({arrival, pid, ind, burst});
        arr[pid].push_back(arrival);
    }

    int t = 0;
    while (!pq.empty())
    {
        int arrival = get<0>(pq.top());
        int pid = get<1>(pq.top());
        int ind = get<2>(pq.top());
        int burst = get<3>(pq.top());
        pq.pop();
        if(ind>=mp[pid].burst.size()) continue;
        if (t == 0)
            t = arrival;
        if (burst > q)
        {
            arr[pid].push_back(t);
            burst -= q;
            arrival += q;
            t += q;
            pq.push({arrival, pid, ind, burst});
            arr[pid].push_back(t);
        }
        else
        {
            arr[pid].push_back(t);
            t += burst;
            arr[pid].push_back(t);
            burst = mp[pid].burst[++ind].second;
            pq.push({t, pid, ind, burst});
        }
    }

    cout << "RR" << "\n";
    int t_turnaround = 0, t_waiting = 0, t_response = 0;
    for(auto it:arr){
        int response = it.second[1] - it.second[0];
        int size = it.second.size();
        int turnaround = it.second[size-1] - it.second[0];
        int totalBurst = 0;
        for(auto i : mp[it.first].burst)
            totalBurst += i.second;
        int waiting = turnaround - totalBurst;
        cout << it.first << "->" << " Response time: " << response << "\n";
        cout << it.first << "->" << " Turnaround time: " << turnaround << "\n";
        cout << it.first << "->" << " Waiting time: " << waiting << "\n";

        t_response += response;
        t_waiting += waiting;
        t_turnaround += turnaround;
        cout<< "\n";
    }
    cout << "Average reponse time: " << t_response/n << "\n";
    cout << "Average turnaround time: " << t_turnaround/n << "\n";
    cout << "Average waiting time: " << t_waiting/n << "\n";
}


int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n;
    cin >> n;
    int q;
    cin >> q;

    map<int, Process> mp;
    for (int i = 0; i < n; i++)
    {
        int pid, arrival, priority;
        cin >> pid >> arrival >> priority;
        Process P;
        P.pid = pid;
        P.arrival = arrival;
        P.priority = priority;
        while (1)
        {
            string s;
            cin >> s;
            if (s == "-1")
                break;
            int a;
            cin >> a;
            P.burst.push_back({s, a});
        }
        // cout << P.pid << "\n";
        mp[pid] = P;
    }
    rr(n,q,mp);
    PriorityNonPreEmptive(n,mp);
    PriorityPreEmptive(n,q,mp);
    return 0;
}

