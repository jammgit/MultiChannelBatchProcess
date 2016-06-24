#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QDate>
#include "memalgo.h"
#include "schedulealgo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //algorithm Exec call this function
    void UpdateProcessInfo();


private:
    void InitWidget();
    void InitConnect();
    void UpdateMemoryInfo();

private slots:
    void Exec();

    void on_BTN_START_clicked();

    void on_BTN_RESUME_clicked();

    void on_BTN_ADD_LINE_clicked();

    void renewTime();
//    void on_BTN_DEL_LINE_clicked();

private:
    Ui::MainWindow *ui;

    ScheduleAlgorithm *m_pJobScheAlgo;
    ScheduleAlgorithm *m_pProcScheAlgo;
    AllocAlgorithm *m_pAllocAlgo;

    QTimer *m_pTimer;
    QTimer *m_pRenewTime;
    int m_runTime;
    /*counter when start*/
    int m_counter;
    /*tape sum*/
    int m_tape[4];
    int m_tapes;
};

#endif // MAINWINDOW_H
