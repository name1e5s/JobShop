/**
 * @file result.cpp
 *
 * Draw result dialog.
 *
 * @author Name1e5s
 */

#include <QTextStream>
#include <QApplication>
#include <cstdlib>

#include "ui_result.h"
#include <result.h>

#include <jobshop.h>

extern int best_makespan;

/**
 * @brief The pair struct for sorting machines.
 */
struct pair
{
    int starttime;
    int endtime;
};

/**
 * @brief Function to compare starttime of two pairs for qsort.
 * @param a The first pair.
 * @param b The second pair.
 * @return If start time of a is lesser than b, then return a positive
 * value, else return a non-positive value.
 */
int starttime_cmp(const void *a, const void *b) {
    return ((pair *)a)->starttime - ((pair *)b)->starttime;
}

/**
 * @brief Construct the result window.
 * @param instance The instance class.
 * @param parent ...
 */
Result::Result(JobShop *instance,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Result),
    jssp(instance)
{
    ui->setupUi(this);
    private_chart = instance->generateGantt();
    private_scene = new QGraphicsScene(this);
    private_scene->addItem(private_chart);

    ui->resuleGanttChart->setScene(private_scene);
    ui->resuleGanttChart->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    connect(ui->fixButton,&QPushButton::toggled,this,&Result::on_fixButton_clicked);
}

/**
 * @brief Delete the window.
 */
Result::~Result()
{
    delete ui;
    delete private_chart;
}

/**
 * @brief Handle the fix command
 * @param machine
 * @param clock
 * @param duration
 */
void Result::Fix(int machine, int clock ,int duration) {
    pair pairs[30];
    int index = 0;
    int real_duration;
    int real_start;
    int next_starttime;
    for(int i = 0; i < job_size; i ++) {
                pairs[i].starttime = job[i].start[machine];
                pairs[i].endtime = job[i].start[machine] + job[i].process_time[machine];
    }
    qsort(pairs,job_size,sizeof(pair),starttime_cmp);
    index = 0;
    int i;
    for(i = 0; i < job_size; i++) {
        if(pairs[i].starttime >= clock) {
            index = 1;
            break;
        }
        if(pairs[i].endtime >= clock) {
            if(i != 9) {
                index = 2;
                break;
            } else {
                index = 3;
                break;
            }
        }
    }
    if(index == 1) {
        real_start = clock;
        real_duration = MAX(0,duration + clock - pairs[i].starttime);
        next_starttime = pairs[i].starttime;
    } else if(index == 2) {
        real_start = pairs[i].endtime;
        real_duration = MAX(0,duration + real_start - pairs[i + 1].starttime);
        next_starttime = pairs[i + 1].starttime;
    } else if(index == 3) {
        real_start = pairs[i].endtime;
        real_duration = duration;
        next_starttime = 0x7fffffff;
    } else {
        real_start = clock;
        real_duration = duration;
        next_starttime = 0x7fffffff;
    }

    for(int i = 0; i < job_size; i ++) {
        for(int j = 0; j < machine_size; j ++) {
            if(job[i].start[j] >= next_starttime)
                job[i].start[j] += real_duration;
        }
    }

    Fixer *f = new Fixer;
    f->starttime = real_start;
    f->duration = duration;
    f->machine = machine;
    this->jssp->fixer.append(f);
    best_makespan += real_duration;
}

/**
 * @brief The action of fixButton.
 */
void Result::on_fixButton_clicked()
{
    int clock = 0;
    int machine = 0;
    int duration = 0;
    QString str = ui->fixCommandLineEdit->text();
    QTextStream stream(&str);
    stream >> clock >> machine >> duration;
    this->Fix(machine,clock,duration);
    private_scene->removeItem(private_chart);
    private_chart = jssp->generateGantt();
    private_scene->addItem(private_chart);
    ui->resuleGanttChart->update();
}
