#pragma once
#include"engine.h"
#include<iomanip>
#include<iostream>
#include<ctime>

class GanttChart {
public:
    void print(vector<GE>&g){
        if(g.empty()){cout<<"No Gantt data.\n";return;}
        cout<<"\n--- Gantt Chart ---\n|";
        for(auto&x:g){
            if(x.pid==-1) cout<<"IDLE|";
            else cout<<"P"<<x.pid<<"|";
        }
        cout<<"\n"<<g[0].s;
        for(auto&x:g) cout<<"    "<<x.e;
        cout<<"\n";
    }
};

class StatisticsView {
public:
    void print(vector<Process>&p,CPUTracker&ct){
        float tw=0,tt=0;
        int n=p.size();
        cout<<"\n--- Results ---\n";
        cout<<left<<setw(5)<<"PID"<<setw(9)<<"Arrival"<<setw(7)<<"Burst"
            <<setw(12)<<"Completion"<<setw(12)<<"Turnaround"<<"Waiting\n";
        for(auto&x:p){
            cout<<setw(5)<<x.getPid()<<setw(9)<<x.getAt()<<setw(7)<<x.getBt()
                <<setw(12)<<x.getCt()<<setw(12)<<x.getTat()<<x.getWt()<<"\n";
            tw+=x.getWt();tt+=x.getTat();
        }
        cout<<fixed<<setprecision(2);
        cout<<"Average Waiting Time: "<<tw/n<<"\n";
        cout<<"Average Turnaround Time: "<<tt/n<<"\n";
        cout<<"CPU Idle Time: "<<ct.getIdle()<<"\n";
        cout<<"CPU Utilization: "<<ct.getUtil()<<"%\n";
    }
};

class TerminalUI {
    ProcessManager pm;
    SimulationEngine engine;
    GanttChart gc;
    StatisticsView sv;
    RandomProcessGenerator rpg;
    string lastAlgo;

    void setup(){
        cout<<"1. Enter number of processes manually\n2. Use random count\nChoice: ";
        int c;cin>>c;
        int n=0;
        if(c==1){cout<<"Number of processes: ";cin>>n;}
        else{srand(time(0));n=rand()%6+3;}
        if(n<=0)throw invalid_argument("Process count must be positive.");
        pm.clear();
        auto procs=rpg.generate(n);
        for(auto&p:procs) pm.add(p);
        cout<<"\nGenerated "<<n<<" processes.\n";
        printTable();
    }

    void printTable(){
        cout<<"\n--- Process Table ---\n";
        cout<<left<<setw(5)<<"PID"<<setw(9)<<"Arrival"<<setw(7)<<"Burst"<<"Priority\n";
        for(auto&p:pm.getAll())
            cout<<setw(5)<<p.getPid()<<setw(9)<<p.getAt()<<setw(7)<<p.getBt()<<p.getPr()<<"\n";
    }

    void runAlgo(Scheduler*s,bool cmpMode=false){
        lastAlgo=s->name();
        engine.run(s,cmpMode);
        if(!cmpMode){
            gc.print(s->getGantt());
            sv.print(pm.getAll(),engine.getTracker());
        }
    }

public:
    TerminalUI():engine(pm){}

    void start(){
        int ch;
        try{ setup(); }
        catch(exception&e){cout<<"Setup error: "<<e.what()<<"\n";return;}

        do {
            cout<<"\n=== CPU Scheduling Simulator ===\n";
            cout<<"1. FCFS\n2. SJF\n3. Priority Scheduling\n4. Round Robin\n";
            cout<<"5. Compare All Algorithms\n6. Save Last Simulation\n7. Load Simulation\n8. Exit\nChoice: ";
            cin>>ch;
            try{
                if(ch==1){FCFS s;runAlgo(&s);}
                else if(ch==2){SJF s;runAlgo(&s);}
                else if(ch==3){PriorityScheduler s;runAlgo(&s);}
                else if(ch==4){
                    int tq;cout<<"Time Quantum: ";cin>>tq;
                    RoundRobin s(tq);runAlgo(&s);
                }
                else if(ch==5){
                    engine.getComparator().results.clear();
                    int tq;cout<<"Time Quantum for RR: ";cin>>tq;
                    {FCFS s;runAlgo(&s,true);}
                    {SJF s;runAlgo(&s,true);}
                    {PriorityScheduler s;runAlgo(&s,true);}
                    {RoundRobin s(tq);runAlgo(&s,true);}
                    engine.getComparator().printComparison();
                }
                else if(ch==6){
                    if(lastAlgo.empty()){cout<<"Run an algorithm first.\n";continue;}
                    engine.saveToFile("simulation.txt",lastAlgo);
                }
                else if(ch==7){
                    if(engine.loadFromFile("simulation.txt")){
                        sv.print(pm.getAll(),engine.getTracker());
                    }
                }
                else if(ch==8){cout<<"Goodbye.\n";break;}
                else cout<<"Invalid choice.\n";
            }catch(exception&e){cout<<"Error: "<<e.what()<<"\n";}
        }while(1);
    }
};
