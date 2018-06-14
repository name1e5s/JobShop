/**
 * @file result.h
 * This file defines a simple dialog to show our Gantt chart and it
 * allows user to fix a machins at a given time. The format of the fix
 * command is:
 * $[time] [machine to be fixed] [duration]
 *
 * @author Name1e5s
 */

#ifndef RESULT_H
#define RESULT_H

#include <QDialog>
#include <ganttchartbase.h>
#include <ganttchartmachine.h>
#include <ganttchartoperation.h>
#include <jobshop.h>

namespace Ui {
class Result;
}

class Result : public QDialog
{
    Q_OBJECT

public:
    Result(JobShop *instance,QWidget *parent = 0);
    void Fix(int machine, int clock ,int duration);
    ~Result();

private slots:
    void on_fixButton_clicked();

private:
    Ui::Result *ui;
    JobShop *jssp;
    GanttChartBase *private_chart;
    QGraphicsScene *private_scene;
};

#endif // RESULT_H
