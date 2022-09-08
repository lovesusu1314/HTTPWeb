#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<pthread.h>

#include <list>
#include <exception>
#include <cstdio>
#include "locker.h"



template<class T>
class threadpool
{
private:
    int m_pthread_number; //线程池的数量
    pthread_t* m_threads;  //线程池数组， 大小为m_pthread_number
    int m_max_requests; //请求队列中最多允许的，等待处理的请求数量
    std::list<T*> m_workqueue;  //请求队列
    locker m_queuelocker; //互斥锁
    sem m_queuestat; //信号量用来判断是否有多任务要处理
    bool m_stop;     //是否结束线程


    

public:
    threadpool(int thread_number = 8, int max_requests = 10000);
    ~threadpool();
    bool append(T* request);
    void run();

private:
    static void* worker(void * arg);

};


template<class T>
threadpool<T>::threadpool(int thread_number, int max_requests):
    m_pthread_number(thread_number),m_max_requests(max_requests),m_stop(NULL),m_threads(NULL)
{
    if((thread_number <= 0) || (max_requests <= 0)){
        throw std::exception();
    }

    m_threads = new pthread_t[m_pthread_number];
    if(!m_threads){
        throw std::exception();
    }

    //创建thread_number个线程 并将它们设置为线程脱离
    for(int i = 0; i < thread_number; i++){
        printf("create the %dth thread\n", i);
        

        //创建线程
        if(pthread_create(m_threads + i, NULL, worker, this) != 0){
            delete[] m_threads;
            throw std::exception();
        }

        //设置线程分离
        if(pthread_detach(m_threads[i]) != 0){
            delete[] m_threads;
            throw std::exception();
        }

    }

}


template<class T>
threadpool<T>::~threadpool()
{
    delete[] m_threads;
    m_stop = true;
     
}

template<class T>
bool threadpool<T>::append(T* request){
    m_queuelocker.lock();
    if(m_workqueue.size() > m_max_requests){
        m_queuelocker.unlock();
        return false;
    }

    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

template<class T>
void* threadpool<T>::worker(void* arg){
    threadpool* pool = (threadpool*) arg;
    pool->run();
    return pool;
    
}

template<class T>
void threadpool<T>::run(){
    while (!m_stop)
    {
        m_queuestat.wait();  //没有阻塞就是说明有数据
        m_queuelocker.lock();
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();   //取出之后就要删掉
        
        m_queuelocker.unlock();

        if(!request){
            continue;
        }
        request->process();


    }
    
}









#endif