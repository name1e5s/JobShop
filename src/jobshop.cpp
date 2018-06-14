/**
 * @file jobshop.cpp
 * Simple wrapper of previous version of JSSP solver.
 *
 * @author Name1e5s
 */

#include <jobshop.h>
#include <bottle.h>
#include <ganttchartbase.h>
#include <ganttchartmachine.h>
#include <ganttchartoperation.h>
#include <cstdlib>
#include <ctime>
#include <QDebug>
#include <QTextStream>
extern int best_makespan;

JobShop::JobShop() {
    m_job_size = &job_size;
    m_machine_size = &machine_size;
    *m_job_size = 1;
    *m_machine_size = 1;
    m_job = &job[0];
}

void JobShop::setJobSize(int i) {
    *m_job_size = i;
    qDebug() << *m_job_size;
}

void JobShop::setMachineSize(int i) {
    *m_machine_size = i;
}

void JobShop::getProb(QString str) {
    QTextStream stream(&str);
    for(int i = 0; i < job_size; i ++) {
        for(int j = 0; j < machine_size; j ++) {
            int machine;
            int process_time;
            stream >> machine >> process_time;
            job[i].process_time[machine] = process_time;
            job[i].order[j] = machine;
            job[i].step[machine] = j;
        }
    }
    qDebug() << "Done...";
}

void JobShop::runProb() {
    run_bottle_neck();
    qDebug() << machine_size << job_size;
    qDebug() << best_makespan;
}

GanttChartBase* JobShop::generateGantt() {
    GanttChartBase *m_chart = new GanttChartBase(best_makespan);

    QList<GanttChartMachine*> machines;
    GanttChartMachine* m;
    QList<QColor> Palette;
    double h=0;
    double golden_ratio = 0.618033988749895;
    for (int i=0; i< job_size; i++) {
        h = rand()*1.0/RAND_MAX;
        h += golden_ratio;
        h = fmod(h,1.0);
        Palette.append(QColor::fromHslF(h,0.75,0.6));
    }
    qDebug() << "Done...";
    for(int i=0; i<machine_size; i++)
    {
        m = new GanttChartMachine(QString("%1").arg(i), m_chart);
        machines.append(m);
        m->setPos(0, i * GanttChartBase::machineHeight);
        m->setMakespan(best_makespan);
    }
    qDebug() << "Done...";
    for(int i = 0; i < job_size; i ++) {
        for(int j = 0; j < machine_size; j ++) {
            int currmachine = job[i].order[j];
            GanttChartOperation *gop =new GanttChartOperation(QString::number(i),
                                      job[i].process_time[currmachine],Palette.at(i));
            gop->setParentItem(machines[currmachine]);
            QPointF pos = GanttChartBase::operationPosition(job[i].start[currmachine]);
            gop->setPos(pos + GanttChartBase::machineOffset());
        }
    }
    for(Fixer *f : fixer) {
        qDebug() << f->duration << " " << f->machine << f->starttime;
        GanttChartOperation *gop =new GanttChartOperation("Fix Me",
                                  f->duration,QColor("white"));
        gop->setParentItem(machines[f->machine]);
        QPointF pos = GanttChartBase::operationPosition(f->starttime);
        gop->setPos(pos + GanttChartBase::machineOffset());
    }
    return m_chart;
}
