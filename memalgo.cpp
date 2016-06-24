#include "memalgo.h"

void AllocAlgorithm::RandomCreateMemoryList(int size)
{
//    int idx = 1;
//    while(size > 0)
//    {
//        int s = 20 + random()%80;
//        MemoryItem mitem;
//        if (m_MemoryList.empty())
//            mitem.first_addr = 0;
//        else
//            mitem.first_addr = m_MemoryList.back().first_addr + m_MemoryList.back().size;
//        mitem.size = s;
//        mitem.number = idx++;
//        m_MemoryList.push_back(mitem);
//        size -= s;
//    }
    MemoryItem item;
    item.first_addr = 0;
    item.number = 1;
    item.size = size;
    m_MemoryList.push_back(item);
}

void AllocAlgorithm::ReleaseJob(const Job& job)
{
    auto iter = m_MemoryList.begin();
    auto eiter = m_MemoryList.end();
    for (;iter != eiter; ++iter)
    {
        if (iter->first_addr + iter->size == job.first_addr)
        {
            iter->size += job.size;
            break;
        }
        else if (job.first_addr + job.size == iter->first_addr)
        {
            iter->first_addr = job.first_addr;
            iter->size += job.size;
            break;
        }
    }
    if (iter == eiter)
    {
        MemoryItem tmp;
        tmp.first_addr = job.first_addr;
        tmp.size = job.size;
        if (!m_MemoryList.empty())
            tmp.number = m_MemoryList.back().number+1;
        else
            tmp.number = m_MemoryList.size() + 1;
        m_MemoryList.push_back(tmp);
    }
    /* merge */
    else
        this->MergeNearPartition();
}

void AllocAlgorithm::MergeNearPartition()
{
    auto iter = m_MemoryList.begin();
    auto eiter = m_MemoryList.end();
    auto tmp = iter;
    for (;iter != eiter; ++iter)
    {
        tmp = iter;
        ++tmp;
        auto tmp_2 = iter;
        for (;tmp != eiter; ++tmp)
        {

            if (iter->first_addr + iter->size == tmp->first_addr)
            {
                iter->size += tmp->size;
                m_MemoryList.erase(tmp);
                tmp = tmp_2;

            }
            else if (tmp->first_addr + tmp->size == iter->first_addr)
            {
                iter->first_addr = tmp->first_addr;
                iter->size += tmp->size;
                m_MemoryList.erase(tmp);
                tmp = tmp_2;
            }
            else
                tmp_2++;
        }

    }

}


const Job& FF::AddNewJob(Job& job)
{
    std::list<MemoryItem>::iterator iter = m_MemoryList.begin();
    std::list<MemoryItem>::iterator eiter = m_MemoryList.end();
    for (;iter != eiter; ++iter)
    {
        if(iter->size >= job.size)
        {
            job.mem_number = iter->number;
            job.first_addr = iter->first_addr;
            iter->first_addr += job.size;
            iter->size -= job.size;
            return job;
        }
    }
    job.mem_number = -1;
    job.first_addr = -1;
    return job;
}

void BF::ReleaseJob(const Job& job)
{
    auto iter = m_MemoryList.begin();
    auto eiter = m_MemoryList.end();

    for (;iter != eiter; ++iter)
    {
        if (iter->first_addr + iter->size == job.first_addr)
        {
            iter->size += job.size;
            break;
        }
        else if (job.first_addr + job.size == iter->first_addr)
        {
            iter->first_addr = job.first_addr;
            iter->size += job.size;
            break;
        }
    }
    if (iter == eiter)
    {
        MemoryItem tmp;
        tmp.first_addr = job.first_addr;
        tmp.size = job.size;
        tmp.number = m_MemoryList.size() + 1;
        m_MemoryList.push_back(tmp);

        iter = m_MemoryList.end();
        --iter;
    }
    else/* merge near partition if the release's job merge to the partition */
        this->MergeNearPartition();

    auto idxiter = m_MemoryList.begin();
    eiter = m_MemoryList.end();
    while (iter->size > idxiter->size || idxiter == iter)
    {
        idxiter++;
        if (idxiter == eiter)
            break;
    }

    if (eiter == idxiter)
    {
        m_MemoryList.push_back(*iter);
        m_MemoryList.erase(iter);
    }
    else
    {
        m_MemoryList.insert(idxiter, *iter);
        m_MemoryList.erase(iter);
    }

}

void BF::RandomCreateMemoryList(int size)
{
    AllocAlgorithm::RandomCreateMemoryList(size);
    auto biter = m_MemoryList.begin();
    auto eiter = m_MemoryList.end();
    auto dump_eiter = eiter;
    --dump_eiter;
    decltype(biter) iter;
    for (;biter != dump_eiter; ++biter)
    {
        for (iter = biter, ++iter; iter != eiter; ++iter)
        {
            if(biter->size > iter->size)
            {
                auto tmp = *biter;
                *biter = *iter;
                *iter = tmp;
            }
        }
    }

}
const Job& BF::AddNewJob(Job& job)
{
    auto iter = m_MemoryList.begin();
    auto eiter = m_MemoryList.end();
    for (;iter != eiter; ++iter)
        if (iter->size >= job.size)
        {
            job.mem_number = iter->number;
            job.first_addr = iter->first_addr;
            iter->first_addr += job.size;
            iter->size -= job.size;
            break;
        }

    if (iter == eiter)
    {
        job.first_addr = -1;
        job.mem_number = -1;
    }
    else
    {
        auto idxiter = m_MemoryList.begin();

        eiter = m_MemoryList.end();

        while (iter->size > idxiter->size || idxiter == iter)
        {
            idxiter++;
            if (idxiter == eiter)
                break;
        }

        if (eiter == idxiter)
        {
            m_MemoryList.push_back(*iter);
            m_MemoryList.erase(iter);
        }
        else
        {
            m_MemoryList.insert(idxiter, *iter);
            m_MemoryList.erase(iter);
        }
    }
    return job;
}

const Job& NF::AddNewJob(Job& job)
{
    auto iter = m_MemoryList.begin();
    auto eiter = m_MemoryList.end();
    size_t idx;
    for (idx = 0; idx < m_counter; ++idx)
    {
        iter++;
    }
    auto size = m_MemoryList.size();

    for (idx = 0; idx < size; ++idx)
    {
        m_counter = (m_counter + 1)%size;

        if(iter->size >= job.size)
        {
            job.mem_number = iter->number;
            job.first_addr = iter->first_addr;
            iter->first_addr += job.size;
            iter->size -= job.size;
            return job;
        }
        ++iter;
        if (iter == eiter)
            iter = m_MemoryList.begin();
    }

    job.first_addr = -1;
    job.mem_number = -1;
    return job;
}


void WF::ReleaseJob(const Job& job)
{
    auto iter = m_MemoryList.begin();
    auto eiter = m_MemoryList.end();
    for (;iter != eiter; ++iter)
    {
        if (iter->first_addr + iter->size == job.first_addr)
        {
            iter->size += job.size;
            break;
        }
        else if (job.first_addr + job.size == iter->first_addr)
        {
            iter->first_addr = job.first_addr;
            iter->size += job.size;
            break;
        }
    }
    if (iter == eiter)
    {
        MemoryItem tmp;
        tmp.first_addr = job.first_addr;
        tmp.size = job.size;
        tmp.number = m_MemoryList.size() + 1;
        m_MemoryList.push_back(tmp);

        iter = m_MemoryList.end();
        --iter;
    }
    else/* merge near partition if the release's job merge to the partition */
        this->MergeNearPartition();

    auto idxiter = m_MemoryList.begin();
    eiter = m_MemoryList.end();
    while (iter->size < idxiter->size || idxiter == iter)
    {
        idxiter++;
        if (idxiter == eiter)
            break;
    }

    if (eiter == idxiter)
    {
        m_MemoryList.push_back(*iter);
        m_MemoryList.erase(iter);
    }
    else
    {
        m_MemoryList.insert(idxiter, *iter);
        m_MemoryList.erase(iter);
    }

}

void WF::RandomCreateMemoryList(int size)
{
    AllocAlgorithm::RandomCreateMemoryList(size);
    auto biter = m_MemoryList.begin();
    auto eiter = m_MemoryList.end();
    auto dump_eiter = eiter;
    --dump_eiter;
    decltype(biter) iter;
    for (;biter != dump_eiter; ++biter)
    {
        for (iter = biter,++iter; iter != eiter; ++iter)
        {
            if(biter->size < iter->size)
            {
                auto tmp = *biter;
                *biter = *iter;
                *iter = tmp;
            }
        }
    }
}

const Job& WF::AddNewJob(Job& job)
{
    auto iter = m_MemoryList.begin();
    if (iter->size >= job.size)
    {
        job.mem_number = iter->number;
        job.first_addr = iter->first_addr;
        iter->first_addr += job.size;
        iter->size -= job.size;

        auto idxiter = iter;
        idxiter++;
        auto eiter = m_MemoryList.end();

        while (iter->size < idxiter->size)
        {
            idxiter++;
            if (idxiter == eiter)
                break;
        }

        if (eiter == idxiter)
        {
            m_MemoryList.push_back( m_MemoryList.front());
            m_MemoryList.pop_front();
        }
        else
        {
            m_MemoryList.insert(idxiter, *iter);
            m_MemoryList.erase(iter);
        }
    }
    else
    {
        job.first_addr = -1;
        job.mem_number = -1;
    }

    return job;
}

