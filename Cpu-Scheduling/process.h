#pragma once
#include<string>
#include<vector>
#include<cstdlib>
using namespace std;

class Process {
    int pid,at,bt,pr,ct,wt,tat,rem;
public:
    Process(int id,int a,int b,int p):pid(id),at(a),bt(b),pr(p),ct(0),wt(0),tat(0),rem(b){}
    ~Process(){}
    int getPid()const{return pid;}
    int getAt()const{return at;}
    int getBt()const{return bt;}
    int getPr()const{return pr;}
    int getCt()const{return ct;}
    int getWt()const{return wt;}
    int getTat()const{return tat;}
    int getRem()const{return rem;}
    void setCt(int v){ct=v;}
    void setWt(int v){wt=v;}
    void setTat(int v){tat=v;}
    void setRem(int v){rem=v;}
    void reset(){ct=0;wt=0;tat=0;rem=bt;}
};

class ProcessManager {
    vector<Process> procs;
public:
    ProcessManager(){}
    ~ProcessManager(){}
    void add(Process p){procs.push_back(p);}
    void clear(){procs.clear();}
    vector<Process>& getAll(){return procs;}
    int count()const{return procs.size();}
    void resetAll(){for(auto&p:procs)p.reset();}
};

class RandomProcessGenerator {
public:
    RandomProcessGenerator(){}
    ~RandomProcessGenerator(){}
    vector<Process> generate(int n){
        vector<Process> v;
        for(int i=0;i<n;i++){
            int at=i%4;
            int bt=(i*3)%7+1;
            int pr=(i%5)+1;
            v.push_back(Process(i+1,at,bt,pr));
        }
        return v;
    }
};
