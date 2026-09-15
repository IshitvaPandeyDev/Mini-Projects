#pragma once
#include"process.h"
#include<climits>
#include<stdexcept>

struct GE{int pid,s,e;};

class Scheduler {
protected:
    vector<GE> gantt;
public:
    virtual ~Scheduler(){}
    virtual void execute(vector<Process>&procs)=0;
    virtual string name()const=0;
    vector<GE>& getGantt(){return gantt;}
};

class FCFS:public Scheduler {
public:
    void execute(vector<Process>&p)override{
        gantt.clear();
        int n=p.size(),t=0;
        if(n==0)throw runtime_error("No processes to schedule.");
        vector<bool>vis(n,false);
        for(int i=0;i<n;i++){
            int idx=-1;
            for(int j=0;j<n;j++)
                if(!vis[j]&&(idx==-1||p[j].getAt()<p[idx].getAt())) idx=j;
            vis[idx]=true;
            if(t<p[idx].getAt()){gantt.push_back({-1,t,p[idx].getAt()});t=p[idx].getAt();}
            gantt.push_back({p[idx].getPid(),t,t+p[idx].getBt()});
            t+=p[idx].getBt();
            p[idx].setCt(t);
            p[idx].setTat(t-p[idx].getAt());
            p[idx].setWt(p[idx].getTat()-p[idx].getBt());
        }
    }
    string name()const override{return "FCFS";}
};

class SJF:public Scheduler {
public:
    void execute(vector<Process>&p)override{
        gantt.clear();
        int n=p.size(),done=0,t=0;
        if(n==0)throw runtime_error("No processes to schedule.");
        vector<bool>vis(n,false);
        while(done<n){
            int idx=-1;
            for(int j=0;j<n;j++)
                if(!vis[j]&&p[j].getAt()<=t&&(idx==-1||p[j].getBt()<p[idx].getBt())) idx=j;
            if(idx==-1){
                int nx=INT_MAX;
                for(int j=0;j<n;j++) if(!vis[j]) nx=min(nx,p[j].getAt());
                gantt.push_back({-1,t,nx});t=nx;continue;
            }
            vis[idx]=true;done++;
            gantt.push_back({p[idx].getPid(),t,t+p[idx].getBt()});
            t+=p[idx].getBt();
            p[idx].setCt(t);
            p[idx].setTat(t-p[idx].getAt());
            p[idx].setWt(p[idx].getTat()-p[idx].getBt());
        }
    }
    string name()const override{return "SJF (Non-Preemptive)";}
};

class PriorityScheduler:public Scheduler {
public:
    void execute(vector<Process>&p)override{
        gantt.clear();
        int n=p.size(),done=0,t=0;
        if(n==0)throw runtime_error("No processes to schedule.");
        vector<bool>vis(n,false);
        while(done<n){
            int idx=-1;
            for(int j=0;j<n;j++)
                if(!vis[j]&&p[j].getAt()<=t&&(idx==-1||p[j].getPr()<p[idx].getPr())) idx=j;
            if(idx==-1){
                int nx=INT_MAX;
                for(int j=0;j<n;j++) if(!vis[j]) nx=min(nx,p[j].getAt());
                gantt.push_back({-1,t,nx});t=nx;continue;
            }
            vis[idx]=true;done++;
            gantt.push_back({p[idx].getPid(),t,t+p[idx].getBt()});
            t+=p[idx].getBt();
            p[idx].setCt(t);
            p[idx].setTat(t-p[idx].getAt());
            p[idx].setWt(p[idx].getTat()-p[idx].getBt());
        }
    }
    string name()const override{return "Priority (Non-Preemptive)";}
};

class RoundRobin:public Scheduler {
    int tq;
public:
    RoundRobin(int q):tq(q){}
    void execute(vector<Process>&p)override{
        gantt.clear();
        int n=p.size(),done=0,t=0;
        if(n==0)throw runtime_error("No processes to schedule.");
        if(tq<=0)throw invalid_argument("Time quantum must be positive.");
        for(auto&x:p) x.setRem(x.getBt());
        vector<int>que;
        vector<bool>inq(n,false);
        for(int j=0;j<n;j++) if(p[j].getAt()==0){que.push_back(j);inq[j]=true;}
        int qi=0;
        while(done<n){
            if(qi>=(int)que.size()){
                int nx=INT_MAX;
                for(int j=0;j<n;j++) if(p[j].getRem()>0) nx=min(nx,p[j].getAt());
                if(nx==INT_MAX)break;
                gantt.push_back({-1,t,nx});t=nx;
                for(int j=0;j<n;j++) if(!inq[j]&&p[j].getAt()<=t){que.push_back(j);inq[j]=true;}
                continue;
            }
            int i=que[qi++];
            if(p[i].getRem()==0) continue;
            int run=min(tq,p[i].getRem());
            gantt.push_back({p[i].getPid(),t,t+run});
            t+=run;p[i].setRem(p[i].getRem()-run);
            for(int j=0;j<n;j++) if(!inq[j]&&p[j].getAt()<=t){que.push_back(j);inq[j]=true;}
            if(p[i].getRem()==0){
                done++;
                p[i].setCt(t);
                p[i].setTat(t-p[i].getAt());
                p[i].setWt(p[i].getTat()-p[i].getBt());
            } else que.push_back(i);
        }
    }
    string name()const override{return "Round Robin";}
};
