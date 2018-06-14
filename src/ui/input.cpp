/**
 * @file input.cpp
 * Draw input window.
 *
 * @author Name1e5s
 */

#include <input.h>
#include "ui_input.h"
#include <jobshop.h>
#include <result.h>
#include <QDebug>

Input::Input(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Input)
{
    ui->setupUi(this);
    jssp = new JobShop;
    connect(ui->job_spin_box,QOverload<int>::of(&QSpinBox::valueChanged),jssp,&JobShop::setJobSize);
    connect(ui->machine_spin_box,QOverload<int>::of(&QSpinBox::valueChanged),jssp,&JobShop::setMachineSize);
    connect(ui->start_button,&QPushButton::toggled,this,&Input::on_start_button_clicked);
}

Input::~Input()
{
    delete ui;
    delete jssp;
}

void Input::on_start_button_clicked()
{
    this->close();
    jssp->getProb(ui->plainTextEdit->toPlainText());
    jssp->runProb();
    Result *r = new Result(jssp,this);
    r->show();
    r->exec();
}
