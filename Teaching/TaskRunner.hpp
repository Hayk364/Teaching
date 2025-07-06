//
//  TaskRunner.hpp
//  Teaching
//
//  Created by АА on 05.06.25.
//

#ifndef TaskRunner_hpp
#define TaskRunner_hpp

#include <stdio.h>
//#include <thread>
//#include <iostream>
//#include <vector>
//#include <map>
//#include <cstdint>
//
//class ITask{
//public:
//    virtual void execute() = 0;
//    virtual ~ITask() = default;
//};
//
//class SumTask : public ITask{
//private:
//    int a,b;
//public:
//    SumTask(int x,int y) : a(x), b(y){}
//    void execute() override {}
//};
//
//class PrintTask: public ITask{
//private:
//    std::string msg;
//public:
//    PrintTask(std::string m) : msg(std::move(m)) {} 
//    void execute() override {}
//};
//
//namespace my_system {
//    class TaskRunner{
//    private:
//        std::vector<std::function<void()>> tasks;
//    public:
//        template <typename T,
//        typename = std::enable_if_t<std::is_base_of<ITask, T>::value>>
//        void addTask(T* task){}
//        
//        void run(){}
//    };
//};
//
//inline void run(my_system::TaskRunner& runner){}
//
//

#endif /* TaskRunner_hpp */
