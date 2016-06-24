#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QDateTime>

typedef struct
{
    int number;
    int first_addr;
    int size;
}MemoryItem;

typedef struct
{
    int number;
    int first_addr;
    int size;
    int mem_number;
}Job,Job_Mem_Info;

typedef struct
{
    QString job_name;

    int need_time;
    int exec_time;
    int wait_time;
    QDateTime arrive_time;
    QDateTime finish_time;
    Job_Mem_Info mem_info;
    QString tape_str;
    int tape_sum;

    int level;

    double arround_time;
    double weight_arround_time;
}PCB,JCB;



#endif // DATASTRUCT_H

