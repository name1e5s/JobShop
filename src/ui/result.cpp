/**
 * @file result.cpp
 *
 * Draw result dialog.
 *
 * @author Name1e5s
 */

#include <result.h>
#include "ui_result.h"
#include <jobshop.h>
#include <QTextStream>
#include <QApplication>
#include <cstdlib>
extern int best_makespan;

Result::Result(JobShop *instance,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Result),
    jssp(instance)
{
    ui->setupUi(this);
    m_chart = instance->generateGantt();
    m_scene = new QGraphicsScene(this);
    m_scene->addItem(m_chart);

    ui->resuleGanttChart->setScene(m_scene);
    ui->resuleGanttChart->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    connect(ui->fixButton,&QPushButton::toggled,this,&Result::on_fixButton_clicked);
}

Result::~Result()
{
    delete ui;
    delete m_chart;
}

struct pair
{
    int starttime;
    int endtime;
};
int cmp(const void *a, const void *b) {
    return ((pair *)a)->starttime - ((pair *)b)->starttime;
};
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
    qsort(pairs,job_size,sizeof(pair),cmp);
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

void Result::on_fixButton_clicked()
{
    int clock = 0;
    int machine = 0;
    int duration = 0;
    QString str = ui->fixCommandLineEdit->text();
    QTextStream stream(&str);
    stream >> clock >> machine >> duration;
    this->Fix(machine,clock,duration);
    m_scene->removeItem(m_chart);
    m_chart = jssp->generateGantt();
    m_scene->addItem(m_chart);
    ui->resuleGanttChart->update();
}
