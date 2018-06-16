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

#include <QObject>
#include <QString>
#include <QVector>

#include <ganttchartbase.h>

#include <bottle.h>

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
    job_t *getJob() {return privateJob;}
    int getJobSize() {return *privateJobSize;}
    int getMachineSize() {return *privateMachineSize;}
    void setJobSize(int i);
    void setMachineSize(int i);
    void getProb(QString);
    GanttChartBase* generateGantt();
    void runProb();
    QString colsoleOutput();
    void writeFile(QString file_name);
    QVector<Fixer*> fixer;
private:
    job_t *privateJob;
    int *privateJobSize;
    int *privateMachineSize;
};

#endif // JOBSHOP_H
