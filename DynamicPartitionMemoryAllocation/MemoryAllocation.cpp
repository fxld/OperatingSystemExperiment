//
//  main.cpp
//  memory_allocation_bf
//
//  Created by dream on 2019/11/12.
//  Copyright © 2019 JasonLiang. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

class element_memory{
public:
    int id_partition;
    element_memory *next;
    element_memory(){
        id_partition=-1;
        next=NULL;
    }
};

class free_memory_table_element{
public:
    int id_partition;
    int count_now;
    int count_sum;
    element_memory *next;
    free_memory_table_element(){
        id_partition=-1;
        count_now=0;
        count_sum=0;
        next=NULL;
    }
    void build_list();
};

void free_memory_table_element::build_list(){
    element_memory *p=NULL;
    int i=1;
    p=new element_memory();
    next=p;
    while(i<count_sum){
        p=p->next;
        p=new element_memory();
        i++;
    }
}

class free_memory_table{
public:
    int sum_free_memory;
    free_memory_table_element free_table[8];
    free_memory_table(){
        sum_free_memory=0;
        for(int i=0;i<8;i++){
            free_table[i].id_partition=i;
            free_table[i].count_sum=(int)pow(2,i);
            free_table[i].count_now=free_table[i].count_sum;
            free_table[i].build_list();
            sum_free_memory=sum_free_memory+free_table[i].count_sum;
        }
    }
    int setSumFreeMemory();
    bool popFromFreeMomoryTableByBf(int n,element_memory *&p);
    bool popFromFreeMemoryTable(int n,element_memory *&p);
    void putToFreeMomoryTable(element_memory *&p);
    void show();
};

int free_memory_table::setSumFreeMemory(){
    int sum=0;
    for (int i=0;i<8; i++) {
        sum=sum+free_table[i].count_now;
    }
    return sum;
}

bool free_memory_table::popFromFreeMomoryTableByBf(int n,element_memory *&p){
    int max=65;
    int record=9;
    int count=1;
    for(int i=0;i<8;i++){
        if(free_table[i].count_sum<n){
            if(max>free_table[i].count_now){
                record=i;
                max=free_table[i].count_now;
            }
        }
    }
    if(max==65||record==9){
        cout<<"无足够空间给进程分配内存"<<endl;
        return false;
    }
    free_table[record].count_now=free_table[record].count_now-n;
    p=free_table[record].next;
    element_memory *p2=p;
    while(count<n){
        p2=p2->next;
        count++;
    }
    free_table[record].next=p2->next;
    p2->next=NULL;
    return true;
}

bool free_memory_table::popFromFreeMemoryTable(int n, element_memory *&p){
    if(n>setSumFreeMemory()){
        cout<<"没有足够多的内存空间了请释放"<<endl;
        return false;
    }
    int sum=0;
    for (int i=0; i<8; i++) {
        if(free_table[i].count_now>0){
            if (p==NULL) {
                p=free_table[i].next;
                free_table[i].next=NULL;
                sum=sum+free_table[i].count_now;
                free_table[i].count_now=0;
            }else{
                element_memory *p3=p;
                while (p3->next!=NULL) {
                    p3=p3->next;
                }
                if(sum+free_table[i].count_now>n){
                    int i=0;
                    element_memory *p2=free_table[i].next;
                    while(sum+i<n){
                        p3->next=p2;
                        p3=p3->next;
                        p2=p2->next;
                    }
                    free_table[i].next=p2;
                    free_table[i].count_now=free_table[i].count_now-i;
                }else{
                    p3->next=free_table[i].next;
                    free_table[i].count_now=0;
                }
            }
        }
    }
    return true;
}

void free_memory_table::putToFreeMomoryTable(element_memory *&p){
    if(p==NULL){
        cout<<"错误，添加内存段指针为空"<<endl;
    }
    while(p!=NULL){
        element_memory *p2=free_table[p->id_partition].next;
        free_table[p->id_partition].count_now++;
        element_memory *p3=p->next;
        free_table[p->id_partition].next=p;
        free_table[p->id_partition].next->next=p2;
        p=p3;
    }
}

void free_memory_table::show(){
    for (int i=0; i<8; i++) {
        cout<<i<<"剩余空间:"<<free_table[i].count_now<<endl;
    }
}

class work_memory_table_element{
public:
    int id_work;
    int record_partition[8];
    element_memory *next;
    work_memory_table_element(){
        id_work=-1;
        for(int i=0; i<8; i++){
            record_partition[i]=0;
        }
        next=NULL;
    }
};

class work_memory_table{
public:
    work_memory_table_element busy_table[8];
    void putToBusyMemoryTable(int id_work,element_memory *&p);
    bool popFromBusyMemoryTable(int id_work,element_memory *&p);
};

void work_memory_table::putToBusyMemoryTable(int id_work, element_memory *&p){
    int i=0;
    for(;i<8;i++){
        if(busy_table[i].id_work==-1){
            busy_table[i].id_work=id_work;
            busy_table[i].next=p;
        }
    }
    if(i>=8){
        cout<<"太多进程了，没有多余的工作内存挂载空间了"<<endl;
    }
}

bool work_memory_table::popFromBusyMemoryTable(int id_work, element_memory *&p){
    int i=0;
    for (; i<8; i++) {
        if(busy_table[i].id_work==id_work){
            p=busy_table[i].next;
            busy_table[i].id_work=-1;
            busy_table[i].next=NULL;
            return true;
        }
    }
    cout<<"没有找到对应的工作号空间"<<endl;
    return false;
}

void allocation(int id_work,int n,free_memory_table &free_memory_table,work_memory_table &work_memory_table){
    element_memory *p;
    if(free_memory_table.popFromFreeMomoryTableByBf(n, p)){
        work_memory_table.putToBusyMemoryTable(id_work, p);
    }else{
        cout<<"popFreeError"<<endl;
    }
}

void release(int id_work,free_memory_table &free_memory_table,work_memory_table &work_memory_table){
    element_memory *p;
    if(work_memory_table.popFromBusyMemoryTable(id_work, p)){
        free_memory_table.putToFreeMomoryTable(p);
    }else{
        cout<<"popBusyError"<<endl;
    }
}

void show(free_memory_table &free_memory_table){
    free_memory_table.show();
}

int main(){
    int id_work,n,choice;
    free_memory_table free_memory_table;
    work_memory_table work_memory_table;
    while(1){
        cin>>id_work>>n>>choice;
        if(choice==0){
            exit(1);
        }else if(choice==1){
            allocation(id_work, n, free_memory_table, work_memory_table);
        }else if(choice==2){
            release(id_work,free_memory_table,work_memory_table);
        }else if(choice==3){
            show(free_memory_table);
        }
    }
}
