#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>
#include <exception>
#include <semaphore.h>

//线程同步机制封装类

//互斥锁类
class locker
{
private:
    pthread_mutex_t m_mutex;
public:
    locker();
    ~locker();
    bool lock();  //上锁
    bool unlock();  //解锁
    pthread_mutex_t* get();  //获取互斥量


};


//条件变量类
//条件变量是要配合互斥锁去使用的
//条件变量不是锁，就是满足某个条件阻塞线程，或者满足某个调节解除线程。
class cond
{
private:
    pthread_cond_t m_cond;
public:
    cond();
    ~cond();
    bool wait(pthread_mutex_t* mutex);
    bool timewait(pthread_mutex_t* mutex, struct timespec t);
    bool signal(pthread_mutex_t* mutex);
    bool broadcast();   //所有的线程都唤醒
};





//信号量类
class sem
{
private:
    sem_t m_sem;
public:
    sem();
    sem(int num);
    ~sem();
    bool wait();  //等待信号量
    bool post(); // 增加信号量
};









#endif