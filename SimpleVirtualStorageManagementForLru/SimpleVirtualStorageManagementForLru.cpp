//
//  main.cpp
//  SimpleVirtualStorageManagementForLru
//
//  Created by dream on 2019/11/17.
//  Copyright © 2019 JasonLiang. All rights reserved.
//


#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

class page{
public:
    int page_id;
    int block_id;
    int virtual_id;
    int flag;
    int count;
    page();
};

page::page(){
    page_id=-1;
    block_id=-1;
    virtual_id=-1;
    flag=0;
    count=0;
}

class page_table{
public:
    class page page[4];
    page_table();
    void putPage(class page obj);
    void popOnLru();
    void updateAddPage(int page_id);
    class page isExist(int page_id);
    bool isFull();
};
page_table::page_table(){
    
}
void page_table::putPage(class page obj){
    int i;
    for(i=0;i<4;i++) {
        if (page[i].page_id==-1) {
            break;
        }
    }
    page[i]=obj;
}
bool page_table::isFull(){
    for (int i=0; i<4; i++) {
        if (page[i].page_id==-1) {
            return false;
        }
    }
    return true;
}
void page_table::popOnLru(){
    int min=16;
    int record=0;
    class page obj;
    for (int i=0; i<4; i++) {
        if (page[i].block_id!=-1) {
            if (min>page[i].count) {
                record=i;
                min=page[i].count;
            }
        }
    }
    cout<<"按照Lru算法,page_id为"<<page[record].page_id<<"退出内存页面"<<endl;
    page[record]=obj;
}
page page_table::isExist(int page_id){
    for (int i=0; i<4; i++) {
        if (page[i].page_id==page_id) {
            return page[i];
        }
    }
    class page obj;
    return obj;
}
void page_table::updateAddPage(int page_id){
    int i;
    for (i=0; i<4; i++) {
        if (page[i].page_id==page_id) {
            page[i].count++;
            break;
        }
    }
    cout<<"页表访问项更新成功,更新页表访问项后数值:"<<page[i].count<<endl;
}

class vitual_table{
public:
    class page page[16];
    vitual_table();
    class page findPage(int page_id);
};
vitual_table::vitual_table(){
  
}
page vitual_table::findPage(int page_id){
    if (page[page_id%16].page_id!=-1) {
          cout<<"找到虚拟内存中的页，page_id:"<<page_id<<endl;
        return page[page_id%16];
    }else{
        class page obj;
          cout<<"没找到虚拟内存中的页，page_id:"<<endl;
        return obj;
    }
}
void ProducePage(vitual_table &vitual_table){
    for (int i=0; i<8; i++) {
        srand(i);
        int Rand_page_id=rand()%16;
        vitual_table.page[Rand_page_id].page_id=Rand_page_id;
        srand(i+10);
        int Rand_vitual_id;
        Rand_vitual_id=rand()%16;
        vitual_table.page[Rand_page_id].virtual_id=Rand_vitual_id;
        vitual_table.page[Rand_page_id].block_id=-1;
        vitual_table.page[Rand_page_id].flag=0;
    }
}

void RandProduceOrder(class vitual_table &table,int Order[16]){
    int count=0;
    int a[50];
    for(int i=0;i<50;i++){
        a[i]=rand()%16;
    }
    for (int i=0;i<50; i++) {
        if(table.page[a[i]].page_id!=-1){
            Order[count]=a[i];
            count++;
        }
        if(count>=14)
            break;
    }
}
void interrupt(vitual_table &v_table,page_table &p_table,int order){
    p_table.popOnLru();
    p_table.putPage(v_table.findPage(order));
}

void command(vitual_table &v_table,page_table &p_table,int Order[16]){
    for (int i=0; i<16; i++) {
        if(!p_table.isFull()){
            cout<<"内存页表未满，往页表添加页号为:"<<Order[i]<<endl;
            page obj=v_table.findPage(Order[i]);
            p_table.putPage(obj);
            cout<<"成功给页表添加页号:"<<obj.page_id<<endl;
        }else{
            cout<<"页表满了"<<endl;
            page obj=p_table.isExist(Order[i]);
            if(obj.block_id!=-1){
                cout<<"需要访问的页面在内存页表中，将更新页表访问项,原来的页表访问项数值为:"<<obj.count<<endl;
                p_table.updateAddPage(obj.page_id);
            }else{
                cout<<"访问页面不在内存页表中需要中断操作"<<endl;
                interrupt(v_table, p_table,Order[i]);
            }
        }
        cout<<"page_id:"<<Order[i]<<"访问完成"<<endl;
    }
}

int main(int argc, const char * argv[]) {
    int order[16];
    vitual_table v_table;
    page_table p_table;
    ProducePage(v_table);
    RandProduceOrder(v_table,order);
    cout<<"输出生成执行的页数序列:"<<endl;
    for (int i=0; i<16; i++) {
        cout<<order[i]<<" ";
    }
    cout<<endl;
    cout<<"输出虚拟储存器中的页表id:"<<endl;
    for (int i=0; i<16; i++) {
        cout<<v_table.page[i].page_id<<" ";
    }
    cout<<endl;
    command(v_table, p_table, order);
    return 0;
}
