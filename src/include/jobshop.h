/**
 * @file jobshop.h
 * This file is a simple wrapper for the previous CLI version
 * of this project to make it compatible with QT.
 *
 * !!!Note: This implementation is not a good pratice.
 *
 * @author Name1e5s
 */

#ifndef JOBSHOP_H
#define JOBSHOP_H
#include <bottle.h>
#include <QString>
#include <QObject>
#include <QVector>
#include <ganttchartbase.h>

struct Fixer {
    int machine;
    int starttime;
    int duration;
};

// Wrapper for previous job shop scheduler.
class JobShop:public QObject
{
    Q_OBJECT
public:
    JobShop();
    job_t *getJob() {return m_job;}
    int getJobSize() {return *m_job_size;}
    int getMachineSize() {return *m_machine_size;}
    void setJobSize(int i);
    void setMachineSize(int i);
    void getProb(QString);
    GanttChartBase* generateGantt();
    void runProb();
    QVector<Fixer*> fixer;
private:
    job_t *m_job;
    int *m_job_size;
    int *m_machine_size;
};

#endif // JOBSHOP_H
