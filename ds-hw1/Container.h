#pragma once
class Container
{
public:
    Container() {}
    virtual void Insert(int key, int value) = 0;
    virtual void Display() = 0;
    virtual int getASL()=0;//平均搜索长度
    virtual int getInfo(){return 0;}
};
