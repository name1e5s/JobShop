/**
 * @file jobshop.cpp
 * Simple wrapper of previous version of JSSP solver.
 *
 * @author Name1e5s
 */

#include <QTextStream>
#include <cstdlib>
#include <ctime>

#include <ganttchartbase.h>
#include <ganttchartmachine.h>
#include <ganttchartoperation.h>

#include <bottle.h>
#include <jobshop.h>
extern int best_makespan;

/**
 * @brief Get pointers of previous defined varibles.
 */
JobShop::JobShop() {
    privateJobSize = &job_size;
    privateMachineSize = &machine_size;
    *privateJobSize = 1;
    *privateMachineSize = 1;
    privateJob = &job[0];
}

/**
 * @brief Set job size using the given value
 * @param i Given value from the spin box
 */
void JobShop::setJobSize(int i) {
    *privateJobSize = i;
}

/**
 * @brief Set machine size using the given value
 * @param i Given value from the spin box
 */
void JobShop::setMachineSize(int i) {
    *privateMachineSize = i;
}

/**
 * @brief Read innstance file from the given str
 * @param str The given string grom the QPlainTextEdit
 */
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
}

/**
 * @brief Run the awesome soler algorithm
 */
void JobShop::runProb() {
    prestissimo();
}

/**
 * @brief Generate the gantt chart from our result.
 * @return The gantt chart.
 */
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
    for(int i=0; i<machine_size; i++)
    {
        m = new GanttChartMachine(QString("%1").arg(i), m_chart);
        machines.append(m);
        m->setPos(0, i * GanttChartBase::machineHeight);
        m->setMakespan(best_makespan);
    }
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
        GanttChartOperation *gop =new GanttChartOperation("Fix Me",
                                  f->duration,QColor("white"));
        gop->setParentItem(machines[f->machine]);
        QPointF pos = GanttChartBase::operationPosition(f->starttime);
        gop->setPos(pos + GanttChartBase::machineOffset());
    }
    return m_chart;
}
