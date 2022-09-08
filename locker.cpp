#include "locker.h"


locker::locker()
{
    if(pthread_mutex_init(&m_mutex,NULL) != 0){
        throw std::exception();
    }
}

locker::~locker()
{
    pthread_mutex_destroy(&m_mutex);
}

bool locker::lock(){
    return pthread_mutex_lock(&m_mutex) == 0;
}

bool locker::unlock(){
    return pthread_mutex_unlock(&m_mutex) == 0;
}

pthread_mutex_t* locker::get(){
    return &m_mutex;
}



cond::cond(/* args */)
{
    //创建
    if(pthread_cond_init(&m_cond,NULL) != 0){
        throw std::exception();
    }
}

cond::~cond()
{
    //销毁
    pthread_cond_destroy(&m_cond);
}

bool cond::wait(pthread_mutex_t* mutex){
    return pthread_cond_wait(&m_cond, mutex) == 0;  //等待，调用了该函数，线程会阻塞。
}

bool cond::timewait(pthread_mutex_t* mutex, struct timespec t){
    //等待多长时间，调用了这个函数，线程会阻塞，直到指定的时间结束。
    return pthread_cond_timedwait(&m_cond, mutex, &t) == 0;
}

bool cond::signal(pthread_mutex_t* mutex){
    //唤醒一个或者多个等待的线程
    return pthread_cond_signal(&m_cond) == 0;

}

bool cond::broadcast(){
    //唤醒所有的等待的线程
    return pthread_cond_broadcast(&m_cond) == 0;
}


sem::sem()
{
    if(sem_init(&m_sem,0,0) != 0){
        throw std::exception();
    }
}

sem::sem(int num)
{
    if(sem_init(&m_sem,0,num) != 0){
        throw std::exception();
    }
}

sem::~sem()
{
    sem_destroy(&m_sem);
}

bool sem::wait(){
    return sem_wait(&m_sem) == 0;
}

bool sem::post(){
    return sem_post(&m_sem) == 0;
}