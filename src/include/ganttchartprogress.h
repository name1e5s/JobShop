/**
 * @file ganttchartprogress.h
 * .
 *
 * @author Name1e5s
 */
#ifndef GANTTCHARTPROGRESS_H
#define GANTTCHARTPROGRESS_H
#include <QThread>

#include <bottle.h>
extern int best_makespan;

class GanttChartProgress : public QThread {
    Q_OBJECT
public:
    explicit GanttChartProgress(double in_speed = 1.0,QObject *parent = 0) : speed(in_speed),QThread(parent){}
    void setSpeed(double value);

protected:
    virtual void run() Q_DECL_OVERRIDE;
signals:
    void updateline(int n);
private:
    double speed;
};
#endif
