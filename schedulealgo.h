#ifndef SCHEDULEALGO_H
#define SCHEDULEALGO_H


#include <QWidget>
#include <QVector>
#include <list>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "datastruct.h"

class MainWindow;
enum SCHE_STATE{SCHE_NEW_JOB, SCHE_ANOTHER_JOB, SCHE_RENEW_TIME,
                SCHE_NO_JOB, SCHE_COMPLETE_JOB, SCHE_RRTIME_ARRI,
                SCHE_PREE,
                SCHE_NULL};

/* Strategy design */
class ScheduleAlgorithm
{
protected:
    ScheduleAlgorithm();
public:
    virtual ~ScheduleAlgorithm();

    virtual void AddNewJob(const PCB &pcb);
    virtual void Exec();

    /*View get state from here*/
    SCHE_STATE GetState();

    void AddWidget(MainWindow *widget);
    void DeleteWidget(MainWindow *widget);
    void ClearJobs();
    void pop_wait_front()
    {
        m_PCBList_Wait.pop_front();
    }
    const std::list<PCB>* GetWaitList();
    const std::list<PCB>* GetRunList();
    const std::list<PCB>* GetFinishList();

protected:
    std::list<MainWindow *> m_WidgetList;
    std::list<PCB> m_PCBList_Wait;
    std::list<PCB> m_PCBList_Run;
    std::list<PCB> m_PCBList_Finish;
    SCHE_STATE m_State;
    int m_Timer;
};

class FCFS : public ScheduleAlgorithm
{

public:

protected:


};

class LEVEL : public ScheduleAlgorithm
{
public:
    virtual void AddNewJob(const PCB &pcb);
};

class RR : public ScheduleAlgorithm
{
public:
    RR(int rrtime):m_TimeCounter(0),m_RRTime(rrtime)
    {}
    virtual void Exec();

protected:
    int m_TimeCounter;
    int m_RRTime;
};

class SJF : public ScheduleAlgorithm
{
public:
    virtual void AddNewJob(const PCB &pcb);
    virtual void Exec();
};

class PreeSJF : public SJF
{
public:
    virtual void AddNewJob(const PCB &pcb);
    virtual void Exec();
};

class HRRN : public ScheduleAlgorithm
{
public:
    //virtual void AddNewJob(const PCB &pcb);
    virtual void Exec();
private:
    std::list<PCB>::iterator getNextJobIter();
};

#endif // SCHEDULEALGO_H
