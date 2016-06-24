#ifndef MEMALGO_H
#define MEMALGO_H


#include <stdlib.h>
#include "datastruct.h"

class AllocAlgorithm
{
protected:
    AllocAlgorithm() = default;
    void MergeNearPartition();
public:
    virtual ~AllocAlgorithm(){}
    void Clear()
    {
        m_MemoryList.clear();
    }
    virtual const Job& AddNewJob(Job& job) = 0;
    virtual void ReleaseJob(const Job& job);
    virtual void RandomCreateMemoryList(int size);
    const std::list<MemoryItem> &GetMemoryList()
    {
        return m_MemoryList;
    }
protected:
    std::list<MemoryItem> m_MemoryList;
};

class FF : public AllocAlgorithm
{
public:
    FF(int size = 500)
    {
        AllocAlgorithm::RandomCreateMemoryList(size);
    }
    virtual const Job& AddNewJob(Job& job);
};

class BF : public AllocAlgorithm
{
public:
    BF(int size = 500)
    {
        this->RandomCreateMemoryList(size);
    }

    virtual const Job& AddNewJob(Job& job);
    virtual void ReleaseJob(const Job& job);
protected:
    virtual void RandomCreateMemoryList(int size);
};

class NF : public AllocAlgorithm
{
public:
    NF(int size = 500):m_counter(0)
    {
        AllocAlgorithm::RandomCreateMemoryList(size);
    }
    virtual const Job& AddNewJob(Job& job);
protected:
    size_t m_counter;
};

class WF : public AllocAlgorithm
{
public:
    WF(int size = 500)
    {
        this->RandomCreateMemoryList(size);
    }
    virtual const Job& AddNewJob(Job& job);
    virtual void ReleaseJob(const Job& job);
protected:
    virtual void RandomCreateMemoryList(int size);
};

#endif // MEMALGO_H
