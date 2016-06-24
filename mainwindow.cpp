#include "mainwindow.h"
#include "ui_mainwindow.h"

QString tape_str[4] ={"1","2","3","4"};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_pTimer = new QTimer();
    m_pRenewTime = new QTimer();
    m_runTime = 0;

    ui->setupUi(this);
    this->InitWidget();
    this->InitConnect();
    m_pRenewTime->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pRenewTime;
    delete m_pTimer;
}

void MainWindow::InitWidget()
{
    ui->LABLE_RUN_TIME->setText(QString::number(m_runTime));

    ui->TABLE_INPUT_JOB->verticalHeader()->setHidden(true);
    ui->TABLE_FINISH->verticalHeader()->setHidden(true);
    ui->TABLE_RUN->verticalHeader()->setHidden(true);
    ui->TABLE_READY->verticalHeader()->setHidden(true);
    ui->TABLE_WAIT->verticalHeader()->setHidden(true);
    ui->TABLE_FREE_LIST->verticalHeader()->setHidden(true);
    ui->TABLE_USED_LIST->verticalHeader()->setHidden(true);

    ui->BTN_TAPE_FOUR->setStyleSheet("background-color:green");
    ui->BTN_TAPE_THREE->setStyleSheet("background-color:green");
    ui->BTN_TAPE_TWO->setStyleSheet("background-color:green");
    ui->BTN_TAPE_ONE->setStyleSheet("background-color:green");


    int row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB1"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("0"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("25"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("15"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("2"));

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB2"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("20"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("30"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("60"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("1"));

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB3"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("30"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("10"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("50"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("3"));

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB4"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("35"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("20"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("10"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("2"));

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB5"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("40"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("15"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("30"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("2"));

    m_pAllocAlgo = new FF(100);
    std::list<MemoryItem> l = m_pAllocAlgo->GetMemoryList();
    size_t size = l.size();
    auto iter = l.begin();
    for (size_t i = 0; i < size; ++i)
    {
        int row = ui->TABLE_FREE_LIST->rowCount();
        ui->TABLE_FREE_LIST->setRowCount(row+1);
        ui->TABLE_FREE_LIST->setItem(row,0, new QTableWidgetItem(QString::number(iter->number)));
        ui->TABLE_FREE_LIST->setItem(row,1, new QTableWidgetItem(QString::number(iter->first_addr)));
        ui->TABLE_FREE_LIST->setItem(row,2, new QTableWidgetItem(QString::number(iter->size)));
    }
}
void MainWindow::InitConnect()
{
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Exec()));
    connect(m_pRenewTime, SIGNAL(timeout()), this, SLOT(renewTime()));
}

void MainWindow::renewTime()
{
    ui->LABLE_TIME->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));

}

void MainWindow::Exec()
{
    /* renew run time*/
    m_runTime++;
    ui->LABLE_RUN_TIME->setText(QString::number(m_runTime));

    /* add job to backup queue */
    while (ui->TABLE_INPUT_JOB->rowCount() != 0)
    {
        if (ui->TABLE_INPUT_JOB->item(0,0)
                && ui->TABLE_INPUT_JOB->item(0,1)
                && ui->TABLE_INPUT_JOB->item(0,2)
                && ui->TABLE_INPUT_JOB->item(0,3)
                && ui->TABLE_INPUT_JOB->item(0,4))
        {
            if (ui->TABLE_INPUT_JOB->item(0,1)->text().toInt() == m_counter)
            {/* job arrive */
                JCB job;
                /* In C++, use c library for class/struct is reasonless after define*/
                //memset(&job, 0, sizeof(job));
                job.arrive_time = QDateTime::currentDateTime();
                job.exec_time = 0;
                job.wait_time = 0;
                job.level = 0;
                job.weight_arround_time = 0;
                job.arround_time = 0;
                job.mem_info.number = m_counter+1;
                job.mem_info.size = ui->TABLE_INPUT_JOB->item(0,3)->text().toInt();
                job.mem_info.first_addr = -1;
                job.mem_info.mem_number = -1;
                job.job_name = ui->TABLE_INPUT_JOB->item(0,0)->text();
                job.need_time = ui->TABLE_INPUT_JOB->item(0,2)->text().toInt();
                job.tape_sum = ui->TABLE_INPUT_JOB->item(0,4)->text().toInt();
                m_pJobScheAlgo->AddNewJob(job);

                ui->TABLE_INPUT_JOB->removeRow(0);
            }
            else
                break;
        }
        else
        {
            ui->TABLE_INPUT_JOB->removeRow(0);
            //QMessageBox::information(nullptr, "ERROR", "There have invalid row.(Have ignore it)");
        }
    }
    /* renew m_counter */
    m_counter++;

    /* run the process,it will call UpdateProcessINfo() */
    m_pProcScheAlgo->Exec();

    /* add job to process from backup queue */
    auto pJobWait = m_pJobScheAlgo->GetWaitList();
    while (!pJobWait->empty())
    {
        JCB job = pJobWait->front();
        /* only when the memory and tape is enough , the
        *   system distribute resource for job.
        */
        if (job.tape_sum > m_tapes)
        {/* the negative express that tape is not enough */
            break;
        }
        else
        {
            m_pAllocAlgo->AddNewJob(job.mem_info);
            if (job.mem_info.first_addr != -1
                    && job.mem_info.mem_number != -1)
            {/* Have enough resource for new job */

                /* renew the sum of tape */
                m_tapes -= job.tape_sum;
                /* change color for tapes */
                int count = 0;
                for (int i = 0; i < 4; ++i)
                {
                    if (m_tape[i] == 1)
                    {

                        job.tape_str += (tape_str[i] + ',');
                        m_tape[i] = 0;
                        switch(i)
                        {
                        case 0:
                            ui->BTN_TAPE_ONE->setStyleSheet("background-color:red");
                            break;
                        case 1:
                            ui->BTN_TAPE_TWO->setStyleSheet("background-color:red");
                            break;
                        case 2:
                            ui->BTN_TAPE_THREE->setStyleSheet("background-color:red");
                            break;
                        case 3:
                            ui->BTN_TAPE_FOUR->setStyleSheet("background-color:red");
                            break;
                        default:
                            break;
                        }
                        count++;
                        if (count == job.tape_sum)
                            break;
                    }
                }
                m_pProcScheAlgo->AddNewJob(job);

                /* renew backup queue*/
                m_pJobScheAlgo->pop_wait_front();
                pJobWait = m_pJobScheAlgo->GetWaitList();
                /* renew ui(TABLE_USED_LIST)*/
                int row = ui->TABLE_USED_LIST->rowCount();
                ui->TABLE_USED_LIST->setRowCount(row+1);
                ui->TABLE_USED_LIST->setItem(row,0, new QTableWidgetItem(job.job_name));
                ui->TABLE_USED_LIST->setItem(row,1, new QTableWidgetItem(QString::number(job.mem_info.first_addr)));
                ui->TABLE_USED_LIST->setItem(row,2, new QTableWidgetItem(QString::number(job.mem_info.size)));
                /*renew the free list view*/
                this->UpdateMemoryInfo();
            }
            else
                break;
        }
    }
}

void MainWindow::on_BTN_START_clicked()
{
    ui->BTN_RESUME->setEnabled(true);
    ui->BTN_START->setEnabled(false);
    /* Start exec function:Exec(), per second*/


    m_pJobScheAlgo = new SJF();
    m_pProcScheAlgo = new PreeSJF();
    /*add widget for algorithm to notice update itself */
    m_pProcScheAlgo->AddWidget(this);


    m_counter = 0;
    m_tapes = 4;
    /* 1 is usable */
    for (int i = 0; i < 4; ++i)
        m_tape[i] = 1;

    m_pTimer->start(1000);

}



void MainWindow::on_BTN_RESUME_clicked()
{
    ui->BTN_RESUME->setEnabled(false);
    ui->BTN_START->setEnabled(true);

    m_pTimer->stop();
    m_runTime = 0;
    ui->LABLE_RUN_TIME->setText(QString::number(m_runTime));

    ui->BTN_TAPE_FOUR->setStyleSheet("background-color:green");
    ui->BTN_TAPE_THREE->setStyleSheet("background-color:green");
    ui->BTN_TAPE_TWO->setStyleSheet("background-color:green");
    ui->BTN_TAPE_ONE->setStyleSheet("background-color:green");

    int row = ui->TABLE_INPUT_JOB->rowCount();
    for (int i =0; i < row; ++i)
        ui->TABLE_INPUT_JOB->removeRow(0);

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB1"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("0"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("25"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("15"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("2"));

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB2"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("20"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("30"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("60"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("1"));

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB3"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("30"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("10"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("50"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("3"));

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB4"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("35"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("20"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("10"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("2"));

    row = ui->TABLE_INPUT_JOB->rowCount();
    ui->TABLE_INPUT_JOB->setRowCount(row+1);
    ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem("JOB5"));
    ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem("40"));
    ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem("15"));
    ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem("30"));
    ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem("2"));

    row = ui->TABLE_FREE_LIST->rowCount();
    for (int i=0; i < row;++i)
        ui->TABLE_FREE_LIST->removeRow(0);


    row = ui->TABLE_USED_LIST->rowCount();
    for (int i=0; i < row;++i)
        ui->TABLE_USED_LIST->removeRow(0);

    row = ui->TABLE_RUN->rowCount();
    for (int i=0; i < row;++i)
        ui->TABLE_RUN->removeRow(0);

    row = ui->TABLE_WAIT->rowCount();
    for (int i=0; i < row;++i)
        ui->TABLE_WAIT->removeRow(0);

    row = ui->TABLE_FINISH->rowCount();
    for (int i=0; i < row;++i)
        ui->TABLE_FINISH->removeRow(0);

    row = ui->TABLE_READY->rowCount();
    for (int i=0; i < row;++i)
        ui->TABLE_READY->removeRow(0);

    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");

    delete m_pAllocAlgo;
    delete m_pJobScheAlgo;
    delete m_pProcScheAlgo;

    m_pAllocAlgo = new FF(100);
    std::list<MemoryItem> l = m_pAllocAlgo->GetMemoryList();
    size_t size = l.size();
    auto iter = l.begin();
    for (size_t i = 0; i < size; ++i)
    {
        int row = ui->TABLE_FREE_LIST->rowCount();
        ui->TABLE_FREE_LIST->setRowCount(row+1);
        ui->TABLE_FREE_LIST->setItem(row,0, new QTableWidgetItem(QString::number(iter->number)));
        ui->TABLE_FREE_LIST->setItem(row,1, new QTableWidgetItem(QString::number(iter->first_addr)));
        ui->TABLE_FREE_LIST->setItem(row,2, new QTableWidgetItem(QString::number(iter->size)));
    }

}

void MainWindow::on_BTN_ADD_LINE_clicked()
{
    int row = ui->TABLE_INPUT_JOB->rowCount();

    int arrive = m_counter + ui->lineEdit_11->text().toInt();
    int i;
    for (i = 0; i < row; ++i)
    {
        if (arrive < ui->TABLE_INPUT_JOB->item(i, 1)->text().toInt())
        {/* move row(i) to row(last), the row(i) distribute to new job*/
            break;
        }
    }

    if (i == row)
    {
        ui->TABLE_INPUT_JOB->setRowCount(row + 1);
        ui->TABLE_INPUT_JOB->setItem(row,0, new QTableWidgetItem(
                                         ui->lineEdit_10->text()));
        ui->TABLE_INPUT_JOB->setItem(row,1, new QTableWidgetItem(
                                         QString::number(arrive)));
        ui->TABLE_INPUT_JOB->setItem(row,2, new QTableWidgetItem(
                                         ui->lineEdit_12->text()));
        ui->TABLE_INPUT_JOB->setItem(row,3, new QTableWidgetItem(
                                         ui->lineEdit_13->text()));
        ui->TABLE_INPUT_JOB->setItem(row,4, new QTableWidgetItem(
                                         ui->lineEdit_14->text()));
    }
    else
    {
        ui->TABLE_INPUT_JOB->insertRow(i);
        ui->TABLE_INPUT_JOB->setItem(i,0, new QTableWidgetItem(
                                         ui->lineEdit_10->text()));
        ui->TABLE_INPUT_JOB->setItem(i,1, new QTableWidgetItem(
                                         QString::number(arrive)));
        ui->TABLE_INPUT_JOB->setItem(i,2, new QTableWidgetItem(
                                         ui->lineEdit_12->text()));
        ui->TABLE_INPUT_JOB->setItem(i,3, new QTableWidgetItem(
                                         ui->lineEdit_13->text()));
        ui->TABLE_INPUT_JOB->setItem(i,4, new QTableWidgetItem(
                                         ui->lineEdit_14->text()));
    }


}

//void MainWindow::on_BTN_DEL_LINE_clicked()
//{
//    int row = ui->TABLE_INPUT_JOB->rowCount();
//    ui->TABLE_INPUT_JOB->setRowCount(row - 1);
//}

void MainWindow::UpdateProcessInfo()
{
    const std::list<PCB> *pwait_proc = m_pProcScheAlgo->GetWaitList();
    const std::list<PCB> *prun = m_pProcScheAlgo->GetRunList();
    const std::list<PCB> *pfinish = m_pProcScheAlgo->GetFinishList();
    const std::list<PCB> *pwait_job = m_pJobScheAlgo->GetWaitList();
    int row;

    /* there have new work had being done ,then we should update memory view*/
    if (pfinish->size() != (size_t)ui->TABLE_FINISH->rowCount())
    {
        JCB job = pfinish->front();
        /*return memory to system*/
        m_pAllocAlgo->ReleaseJob(job.mem_info);
        /*return tape to system*/
        m_tapes += job.tape_sum;
        for (int j = 0; j < 4; ++j)
        {
            /* the job used the tape */
            if (job.tape_str.indexOf(tape_str[j])>=0)
            {
                m_tape[j] = 1;
                switch(j)
                {
                case 0:
                    ui->BTN_TAPE_ONE->setStyleSheet("background-color:green");
                    break;
                case 1:
                    ui->BTN_TAPE_TWO->setStyleSheet("background-color:green");
                    break;
                case 2:
                    ui->BTN_TAPE_THREE->setStyleSheet("background-color:green");
                    break;
                case 3:
                    ui->BTN_TAPE_FOUR->setStyleSheet("background-color:green");
                    break;
                default:
                    break;
                }
            }
        }

        row = ui->TABLE_USED_LIST->rowCount();
        for (int i = 0; i < row; ++i)
        {
            if (ui->TABLE_USED_LIST->item(i,0)->text() == job.job_name)
            {/* find the work in used list ,then delete it in view */
                ui->TABLE_USED_LIST->removeRow(i);
                break;
            }
        }

        /* renew finish table*/
        row = ui->TABLE_FINISH->rowCount();
        ui->TABLE_FINISH->setRowCount(row+1);
        ui->TABLE_FINISH->setItem(row,0, new QTableWidgetItem(job.job_name));
        ui->TABLE_FINISH->setItem(row,1, new QTableWidgetItem(job.arrive_time.toString("hh:mm:ss")));
        ui->TABLE_FINISH->setItem(row,2, new QTableWidgetItem(QString::number(job.need_time)));
        ui->TABLE_FINISH->setItem(row,3, new QTableWidgetItem(QString::number(job.mem_info.size)));
        ui->TABLE_FINISH->setItem(row,4, new QTableWidgetItem(QString::number(job.tape_sum)));
        ui->TABLE_FINISH->setItem(row,5, new QTableWidgetItem(job.finish_time.toString("hh:mm:ss")));

        /* if work is finished , then the resource return to system.
         * we should distribute it to new work */
        auto job_list = m_pJobScheAlgo->GetWaitList();
        while (!job_list->empty())
        {
            JCB new_job = job_list->front();
            if (m_tapes >= new_job.tape_sum)
            {
                m_pAllocAlgo->AddNewJob(new_job.mem_info);
                if (new_job.mem_info.first_addr != -1
                        && new_job.mem_info.mem_number != -1)
                {/*resource  is enough*/

                    m_tapes -= new_job.tape_sum;
                    int count = 0;
                    for (int i = 0; i < 4; ++i)
                    {
                        if (m_tape[i] == 1)
                        {

                            new_job.tape_str += (tape_str[i] + ',');
                            m_tape[i] = 0;
                            switch(i)
                            {
                            case 0:
                                ui->BTN_TAPE_ONE->setStyleSheet("background-color:red");
                                break;
                            case 1:
                                ui->BTN_TAPE_TWO->setStyleSheet("background-color:red");
                                break;
                            case 2:
                                ui->BTN_TAPE_THREE->setStyleSheet("background-color:red");
                                break;
                            case 3:
                                ui->BTN_TAPE_FOUR->setStyleSheet("background-color:red");
                                break;
                            default:
                                break;
                            }
                            count++;
                            if (count == new_job.tape_sum)
                                break;
                        }
                    }

                    m_pJobScheAlgo->pop_wait_front();
                    m_pProcScheAlgo->AddNewJob(new_job);
                    job_list = m_pJobScheAlgo->GetWaitList();

                    /*renew memory info*/
                    int row = ui->TABLE_USED_LIST->rowCount();
                    ui->TABLE_USED_LIST->setRowCount(row+1);
                    ui->TABLE_USED_LIST->setItem(row,0, new QTableWidgetItem(new_job.job_name));
                    ui->TABLE_USED_LIST->setItem(row,1, new QTableWidgetItem(QString::number(new_job.mem_info.first_addr)));
                    ui->TABLE_USED_LIST->setItem(row,2, new QTableWidgetItem(QString::number(new_job.mem_info.size)));

                }
                else
                    break;
            }
            else
                break;
        }
        this->UpdateMemoryInfo();
    }

    /*renew wait table*/
    int wait_row = ui->TABLE_WAIT->rowCount();
    int wait_job_size =pwait_job->size();
    for (int i = 0; i < wait_row; ++i)
    {
        ui->TABLE_WAIT->removeRow(0);
    }
    auto wait_iter = pwait_job->begin();
    for (int i = 0; i < wait_job_size; ++i)
    {
         wait_row = ui->TABLE_WAIT->rowCount();
         ui->TABLE_WAIT->setRowCount(wait_row+1);
         ui->TABLE_WAIT->setItem(wait_row,0, new QTableWidgetItem(wait_iter->job_name));
         ui->TABLE_WAIT->setItem(wait_row,1, new QTableWidgetItem(wait_iter->arrive_time.toString("hh:mm:ss")));
         ui->TABLE_WAIT->setItem(wait_row,2, new QTableWidgetItem(QString::number(wait_iter->need_time)));
         ui->TABLE_WAIT->setItem(wait_row,3, new QTableWidgetItem(QString::number(wait_iter->mem_info.size)));
         ui->TABLE_WAIT->setItem(wait_row,4, new QTableWidgetItem(QString::number(wait_iter->tape_sum)));
         ui->TABLE_WAIT->setItem(wait_row,5, new QTableWidgetItem(QString::number(wait_iter->exec_time)));
         ++wait_iter;
    }


    /* renew ready table*/
    int proc_row = ui->TABLE_READY->rowCount();
    int proc_job_size =pwait_proc->size();
    for (int i = 0; i < proc_row; ++i)
    {
        ui->TABLE_READY->removeRow(0);
    }
    auto ready_iter = pwait_proc->begin();
    for (int i = 0; i < proc_job_size; ++i)
    {
         proc_row = ui->TABLE_READY->rowCount();
         ui->TABLE_READY->setRowCount(proc_row+1);
         ui->TABLE_READY->setItem(proc_row,0, new QTableWidgetItem(ready_iter->job_name));
         ui->TABLE_READY->setItem(proc_row,1, new QTableWidgetItem(ready_iter->arrive_time.toString("hh:mm:ss")));
         ui->TABLE_READY->setItem(proc_row,2, new QTableWidgetItem(QString::number(ready_iter->need_time)));
         ui->TABLE_READY->setItem(proc_row,3, new QTableWidgetItem(QString::number(ready_iter->mem_info.size)));
         ui->TABLE_READY->setItem(proc_row,4, new QTableWidgetItem(ready_iter->tape_str));
         ui->TABLE_READY->setItem(proc_row,5, new QTableWidgetItem(QString::number(ready_iter->exec_time)));
         ++ready_iter;
    }
    /* renew run table*/
    int prun_row = ui->TABLE_RUN->rowCount();
    int prun_job_size =prun->size();
    for (int i = 0; i < prun_row; ++i)
    {
        ui->TABLE_RUN->removeRow(0);
    }
    auto run_iter = prun->begin();
    for (int i = 0; i < prun_job_size; ++i)
    {
         prun_row = ui->TABLE_RUN->rowCount();
         ui->TABLE_RUN->setRowCount(prun_row+1);
         ui->TABLE_RUN->setItem(prun_row,0, new QTableWidgetItem(run_iter->job_name));
         ui->TABLE_RUN->setItem(prun_row,1, new QTableWidgetItem(run_iter->arrive_time.toString("hh:mm:ss")));
         ui->TABLE_RUN->setItem(prun_row,2, new QTableWidgetItem(QString::number(run_iter->need_time)));
         ui->TABLE_RUN->setItem(prun_row,3, new QTableWidgetItem(QString::number(run_iter->mem_info.size)));
         ui->TABLE_RUN->setItem(prun_row,4, new QTableWidgetItem(run_iter->tape_str));
         ui->TABLE_RUN->setItem(prun_row,5, new QTableWidgetItem(QString::number(run_iter->exec_time)));
         ++run_iter;
    }

    /*calculate arround time and weight arround time*/
    if (m_pJobScheAlgo->GetWaitList()->empty()
            && m_pProcScheAlgo->GetWaitList()->empty()
            && m_pProcScheAlgo->GetRunList()->empty()
            && ui->TABLE_INPUT_JOB->rowCount() == 0)
    {
        double arround = 0;
        double weight_arround = 0;
        int count = 0;
        pfinish = m_pProcScheAlgo->GetFinishList();
        std::list<PCB>::const_iterator iter = pfinish->begin();
        auto eiter = pfinish->end();
        for (; iter != eiter; ++iter)
        {
            arround += iter->arround_time;
            weight_arround += iter->weight_arround_time;
            ++count;
        }
        arround /= count;
        weight_arround /= count;
        ui->lineEdit->setText(QString::number(arround));
        ui->lineEdit_2->setText(QString::number(weight_arround));
    }
}

void MainWindow::UpdateMemoryInfo()
{
    int row = ui->TABLE_FREE_LIST->rowCount();
    for (int i = 0; i < row; ++i)
    {
        ui->TABLE_FREE_LIST->removeRow(0);
    }

    std::list<MemoryItem> l = m_pAllocAlgo->GetMemoryList();
    size_t size = l.size();
    auto iter = l.begin();
    for (size_t i = 0; i < size; ++i)
    {
        row = ui->TABLE_FREE_LIST->rowCount();
        ui->TABLE_FREE_LIST->setRowCount(row+1);

        ui->TABLE_FREE_LIST->setItem(row,0, new QTableWidgetItem(QString::number(iter->number)));
        ui->TABLE_FREE_LIST->setItem(row,1, new QTableWidgetItem(QString::number(iter->first_addr)));
        ui->TABLE_FREE_LIST->setItem(row,2, new QTableWidgetItem(QString::number(iter->size)));
        ++iter;
    }
}

