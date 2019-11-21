//
//  main.cpp
//  process_schedule
//
//  Created by Jason Liang on 2019/10/31.
//  Copyright © 2019 dream. All rights reserved.
//

#include<string>
#include<iostream>
#include<algorithm>

using namespace std;

//ifndef PROCESS_H
//define PROCESS_H

class pcb;
class pcb_deqeue;
class pcb_pool;
class block_pcb_deqeue;

#define time_slice 2;
#define null NULL;

class pcb{
public:
    int id;
    int level;
    int work_time;
    int wait_time;
    int dependency_record;
    int dependency_process_name;
    string status;
    pcb *next;
    pcb(){
        id=-1;
        level=0;
        work_time=0;
        wait_time=0;
        dependency_record=0;
        dependency_process_name=-1;
        status="suspend";
        next=null;
    }
    
    void setPcb(){
        id=-1;
        level=0;
        work_time=0;
        wait_time=0;
        dependency_record=0;
        dependency_process_name=-1;
        status="suspend";
        next=null;
    }
    
    int setLevel(){
        int record=(work_time+wait_time+level)/work_time;
        level=record;
        return record;
    }
};

class pcb_deqeue{
public:
    int pcb_deqeue_front;
    int pcb_deqeue_rear;
    int pcb_deqeue_count;
    pcb pcb_deqeue_array[5];
    
    pcb_deqeue(){
        pcb_deqeue_front=0;
        pcb_deqeue_rear=0;
        pcb_deqeue_count=0;
        for(int i=0;i<5;i++){
            pcb_deqeue_array[i].setPcb();
        }
    }
    
    int returnDeqeueCount(){
        pcb_deqeue_count=pcb_deqeue_rear;
        return pcb_deqeue_count;
    }
    
    bool pcb_deqeue_full(){
        if((pcb_deqeue_rear+1)%5==pcb_deqeue_front)
            return true;
        else
            return false;
    }
    
    bool pcb_deqeue_empty(){
        if(pcb_deqeue_rear==pcb_deqeue_front)
            return true;
        else
            return false;
    }
    
    bool putPcb_Deqeue(pcb obj){
        if(!pcb_deqeue_full()){
            pcb_deqeue_array[pcb_deqeue_rear]=obj;
            pcb_deqeue_rear=(pcb_deqeue_rear+1)%5;
            return true;
        }else{
            return false;
        }
    }
    
    pcb popPcb_Deqeue(){
         pcb obj;
        if(!pcb_deqeue_empty()){
            obj=pcb_deqeue_array[pcb_deqeue_front];
            pcb_deqeue_array[pcb_deqeue_front].setPcb();
            pcb_deqeue_front=(pcb_deqeue_front+1)%5;
        }
        return obj;
    }
};

class block_pcb_deqeue
{
public:
    int block_table[5];
    pcb pcb[5];
    int block_pcb_deqeue_record;
    
    block_pcb_deqeue(){
        block_pcb_deqeue_record=0;
        for(int i=0;i<5;i++){
            block_table[i]=-1;
            pcb[i].setPcb();
        }
    }
    
    int *getBlock_table(){
        return block_table;
    }
    
    bool putBlock_table(class pcb obj){
        if(block_pcb_deqeue_record<5){
            block_table[block_pcb_deqeue_record]=obj.dependency_process_name;
            pcb[block_pcb_deqeue_record]=obj;
            block_pcb_deqeue_record++;
            return true;
        }else{
            return false;
        }
    }
    
    bool popBlock_table(class pcb &obj){
        if(block_pcb_deqeue_record>=0){
            obj=pcb[block_pcb_deqeue_record];
            pcb[block_pcb_deqeue_record].setPcb();
            block_pcb_deqeue_record--;
            return true;
        }else{
            return false;
        }
    }
    
};

bool cmp(pcb a,pcb b){
    if(a.level>=b.level)
        return     true;
    else
        return false;
}

class pcb_pool{
public:
    int pcb_use_count;
    int pcb_count;
    pcb pcb[11];
    pcb_pool(){
        pcb_use_count=0;
        pcb_count=10;
        for(int i=0;i<=10;i++)
            pcb[i].setPcb();
    }
    
    void buildPool(pcb_pool &p,int n){
        int i=1,j;
        p.pcb_use_count=n;
        int parent,children;
        class pcb tmp;
        for(j=i/2;j>=1;j--){
            tmp=p.pcb[j];
            for(parent=j;parent<=p.pcb_use_count;parent=children){
                children=parent*2;
                if(children!=p.pcb_use_count&&p.pcb[children].level<p.pcb[children+1].level){
                    children++;
                }
                if(tmp.level>p.pcb[children+1].level){
                    break;
                }else{
                    p.pcb[parent]=p.pcb[children];
                }
            }
            p.pcb[parent]=tmp;
        }
    }
    
    bool pcbPool_empty(){
        if(pcb_use_count==0){
            cout<<"pcbPool empty"<<endl;
            return true;
        }else
            return false;
    }
    
    bool pcbPool_full(){
        if(pcb_use_count>=pcb_count){
             cout<<"pcbPool full"<<endl;
            return true;
        }else
            return false;
    }
    
    bool findPcbIsInPool(int id){
        for(int i=0;i<=10;i++){
            if(pcb[i].id==id)
                return true;
        }
        return false;
    }
    
    bool putPcbPool(pcb_pool &p,class pcb obj){
        int i;
        if(pcbPool_full())
            return false;
        i=p.pcb_use_count+1;
        p.pcb_use_count=p.pcb_use_count+1;
        for(;p.pcb[i/2].level<obj.level;i=i/2){
            p.pcb[i]=p.pcb[i/2];
        }
        p.pcb[i]=obj;
        return true;
    }
    
    void sortPcbPool(){
        sort(pcb,pcb+10,cmp);
    }
    
    bool popPcbPool(pcb_pool &p,class pcb &obj){
        if(pcbPool_empty())
            return false;
        obj=p.pcb[1];
        class pcb temp=p.pcb[pcb_use_count];
        p.pcb_use_count--;
        int parent=1,children;
        for( ;parent*2<p.pcb_use_count;parent=children){
            children=parent*2;
            if(children!=p.pcb_use_count&&p.pcb[children].level<p.pcb[children+1].level)
                children++;
            if(temp.level>p.pcb[children].level)
                break;
            else{
                p.pcb[parent]=p.pcb[children];
            }
        }
        p.pcb[parent]=temp;
        return true;
    }
    
    bool popByPcbIdFromPcbPool(pcb_pool &p,int id,class pcb &obj){
        for(int i=1;i<=p.pcb_use_count;i++){
            if(p.pcb[i].id==id){
                obj=pcb[i];
                pcb[i].setPcb();
                break;
            }
        }
        p.pcb_use_count--;
        p.sortPcbPool();
        p.buildPool(p, p.pcb_use_count);
        return true;
    }
    
    void showThePool(pcb_pool *p){
        cout<<p->pcb_use_count<<endl;
        for (int i=1; i<=p->pcb_use_count; i++) {
            cout<<p->pcb[i].id<<" ";
        }
    }
};

bool createProcess(int n,pcb_pool &pool){
    if(n<0){
        cout<<"n<0,error"<<endl;
        return false;
    }else if(n>=10){
        cout<<"n>=10,beyond"<<endl;
        return false;
    }else{
        int i;
        for(i=0;i<n;i++){
            pool.pcb[i].id=i+1;
            pool.pcb[i].status="suspending";
            pool.pcb[i].wait_time=0;
            cin>>pool.pcb[i].level>>pool.pcb[i].work_time>>pool.pcb[i].dependency_record>>pool.pcb[i].dependency_process_name;
        }
        pool.buildPool(pool,n);
        if(i<n){
            cout<<"create process error"<<endl;
            return false;
        }else{
            return true;
        }
    }
}

bool dispatcherForProcess(pcb_deqeue &deqeue,pcb_pool &pool,block_pcb_deqeue &block_deqeue){
    int *block_record=block_deqeue.getBlock_table();
    pool.sortPcbPool();
    int i=0;
    int count=0;
    pcb obj;
    for(i=0;i<5;i++){
        if(block_record[i]!=-1){
            count++;
            pool.popByPcbIdFromPcbPool(pool,block_record[i],obj);
            obj.status="runing";
            deqeue.putPcb_Deqeue(obj);
        }
    }
    if(count<5){
        for(int i=0;i<count;i++){
            if(pool.popPcbPool(pool,obj)){
                obj.status="runing";
                deqeue.putPcb_Deqeue(obj);
            }
        }
    }
    return true;
}

bool cpuWorkUpdateForProcess(pcb_deqeue &deqeue,pcb_pool &pool,block_pcb_deqeue &block_deqeue){
    int deqeue_count=deqeue.returnDeqeueCount();
    for(int i=0;i<10;i++){
        if(pool.pcb[i].id!=-1){
            pool.pcb[i].wait_time=pool.pcb[i].wait_time+2*deqeue_count;
            pool.pcb[i].setLevel();
        }
        if(pool.pcb[i].next!=NULL){
            pcb *p;
            p=pool.pcb[i].next;
            while(p!=NULL)
            {
                p->wait_time=p->wait_time+2*deqeue_count;
                p->setLevel();
                p=p->next;
            }
        }
    }
    for (int i=0; i<5; i++) {
        if(block_deqeue.pcb[i].id!=-1){
            block_deqeue.pcb[i].wait_time=block_deqeue.pcb[i].wait_time+2*deqeue_count;
            block_deqeue.pcb[i].setLevel();
        }
    }
    for (int i=0; i<5; i++) {
        pcb obj=deqeue.popPcb_Deqeue();
        if((obj.work_time-2)>obj.dependency_record||obj.dependency_record==0){
            obj.work_time=obj.work_time-time_slice;
            obj.setLevel();
            obj.status="suspending";
            pool.putPcbPool(pool,obj);
        }else if((obj.work_time-2)==obj.dependency_record||(obj.work_time-2)==(obj.dependency_record-1)){
            if(pool.findPcbIsInPool(obj.dependency_process_name)){
                obj.status="blocking";
                block_deqeue.putBlock_table(obj);
            }
        }else if((obj.work_time-2)<=0){
            cout<<"is finish for process"<<obj.id<<endl;
        }
    }
    return true;
}

bool inspectBlockDeqeue(pcb_deqeue &deqeue,pcb_pool &pool,block_pcb_deqeue &block_deqeue){
    for(int i=0;i<5;i++){
        if(block_deqeue.pcb[i].id!=-1){
            if(!pool.findPcbIsInPool(block_deqeue.pcb[i].dependency_process_name)){
                pcb obj;
                if(block_deqeue.popBlock_table(obj)){
                    obj.status="suspending";
                    pool.putPcbPool(pool,obj);
                }
            }
        }
    }
    return true;
}

int main(){
    pcb_pool pool;
    pcb_deqeue deqeue;
    block_pcb_deqeue block_deqeue;
    int n;
    cin>>n;
    int i=0;
    createProcess(n,pool);
    while(!pool.pcbPool_empty()){
        if (!dispatcherForProcess(deqeue, pool, block_deqeue)) {
            cout<<i<<"分发器错误"<<endl;
        }
        if(!cpuWorkUpdateForProcess(deqeue, pool, block_deqeue)){
            cout<<i<<"cpu运转错误"<<endl;
        }
        if(!inspectBlockDeqeue(deqeue, pool, block_deqeue)){
            cout<<i<<"检查阻塞表错误"<<endl;
        }
        i++;
    }
}
