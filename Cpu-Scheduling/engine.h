#pragma once
#include"scheduler.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<cstdio>

class CPUTracker {
    int totalTime,idleTime;
public:
    CPUTracker():totalTime(0),idleTime(0){}
    void compute(vector<GE>&g){
        idleTime=0;
        if(g.empty()){totalTime=0;return;}
        totalTime=g.back().e-g.front().s;
        for(auto&x:g) if(x.pid==-1) idleTime+=x.e-x.s;
    }
    int getIdle()const{return idleTime;}
    float getUtil()const{return totalTime==0?0:100.0*(totalTime-idleTime)/totalTime;}
};

class AlgorithmComparator {
public:
    struct Entry{string name;float awt,atat,util;int idle,sw;};
    vector<Entry> results;

    void add(string nm,vector<Process>&p,CPUTracker&ct,vector<GE>&g){
        float tw=0,tt=0;
        for(auto&x:p){tw+=x.getWt();tt+=x.getTat();}
        int n=p.size();
        int sw=0;
        for(int i=1;i<(int)g.size();i++)
            if(g[i].pid!=-1&&g[i-1].pid!=-1&&g[i].pid!=g[i-1].pid) sw++;
        results.push_back({nm,tw/n,tt/n,ct.getUtil(),ct.getIdle(),sw});
    }

    void printComparison(){
        int nr=results.size();
        if(nr==0){cout<<"No results to compare.\n";return;}
        cout<<"\n========== Algorithm Comparison ==========\n";
        cout<<"Algorithm                 AvgWT  AvgTAT  Idle  Switches  Util%\n";
        for(auto&r:results)
            printf("%-25s %.2f   %.2f    %d     %d         %.1f\n",
                r.name.c_str(),r.awt,r.atat,r.idle,r.sw,r.util);

        auto rank=[&](string label,auto key,bool asc){
            vector<int> idx;for(int i=0;i<nr;i++) idx.push_back(i);
            for(int i=0;i<nr;i++)
                for(int j=i+1;j<nr;j++){
                    float a=key(results[idx[i]]),b=key(results[idx[j]]);
                    if((asc&&a>b)||(!asc&&a<b)) swap(idx[i],idx[j]);
                }
            cout<<label<<"\n";
            for(int i=0;i<nr;i++) cout<<"  "<<i+1<<". "<<results[idx[i]].name<<"\n";
        };

        cout<<"\n--- Rankings ---\n";
        rank("Average Waiting Time (lower=better):",[](Entry&e){return e.awt;},true);
        rank("Average Turnaround Time (lower=better):",[](Entry&e){return e.atat;},true);
        rank("CPU Utilization (higher=better):",[](Entry&e){return e.util;},false);
        rank("Context Switches (lower=better):",[](Entry&e){return (float)e.sw;},true);

        vector<int> score(nr,0);
        auto addScore=[&](auto key,bool asc){
            vector<int> idx;for(int i=0;i<nr;i++) idx.push_back(i);
            for(int i=0;i<nr;i++)
                for(int j=i+1;j<nr;j++){
                    float a=key(results[idx[i]]),b=key(results[idx[j]]);
                    if((asc&&a>b)||(!asc&&a<b)) swap(idx[i],idx[j]);
                }
            for(int i=0;i<nr;i++) score[idx[i]]+=i;
        };
        addScore([](Entry&e){return e.awt;},true);
        addScore([](Entry&e){return e.atat;},true);
        addScore([](Entry&e){return e.util;},false);
        addScore([](Entry&e){return (float)e.sw;},true);

        vector<int> oi;for(int i=0;i<nr;i++) oi.push_back(i);
        for(int i=0;i<nr;i++)
            for(int j=i+1;j<nr;j++)
                if(score[oi[i]]>score[oi[j]]) swap(oi[i],oi[j]);
        cout<<"\n--- Overall Ranking ---\n";
        for(int i=0;i<nr;i++)
            cout<<i+1<<". "<<results[oi[i]].name<<" (score: "<<score[oi[i]]<<")\n";
    }
};

class SimulationEngine {
    ProcessManager& pm;
    CPUTracker tracker;
    AlgorithmComparator cmp;
public:
    SimulationEngine(ProcessManager&m):pm(m){}

    void run(Scheduler*s,bool addToCmp=false){
        pm.resetAll();
        auto procs=pm.getAll();
        try{
            s->execute(procs);
        }catch(exception&ex){
            cout<<"Error: "<<ex.what()<<"\n";
            return;
        }
        pm.getAll()=procs;
        tracker.compute(s->getGantt());
        if(addToCmp) cmp.add(s->name(),procs,tracker,s->getGantt());
    }

    CPUTracker& getTracker(){return tracker;}
    AlgorithmComparator& getComparator(){return cmp;}

    void saveToFile(string fname,string algoName){
        ofstream f(fname);
        if(!f){cout<<"Cannot open file.\n";return;}
        f<<algoName<<"\n";
        f<<pm.count()<<"\n";
        for(auto&p:pm.getAll())
            f<<p.getPid()<<" "<<p.getAt()<<" "<<p.getBt()<<" "<<p.getPr()
             <<" "<<p.getCt()<<" "<<p.getWt()<<" "<<p.getTat()<<"\n";
        cout<<"Saved to "<<fname<<"\n";
    }

    bool loadFromFile(string fname){
        ifstream f(fname);
        if(!f){cout<<"File not found.\n";return false;}
        string algo; int n;
        getline(f,algo); f>>n;
        pm.clear();
        for(int i=0;i<n;i++){
            int pid,at,bt,pr,ct,wt,tat;
            f>>pid>>at>>bt>>pr>>ct>>wt>>tat;
            Process p(pid,at,bt,pr);
            p.setCt(ct);p.setWt(wt);p.setTat(tat);
            pm.add(p);
        }
        cout<<"Loaded simulation: "<<algo<<"\n";
        return true;
    }
};
