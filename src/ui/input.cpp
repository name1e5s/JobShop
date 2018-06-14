/**
 * @file input.cpp
 * Draw input window.
 *
 * @author Name1e5s
 */

#include "ui_input.h"
#include <input.h>
#include <result.h>

#include <jobshop.h>

/**
 * @brief Set the input window.
 * @param parent
 */
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

/**
 * @brief Delete the window.
 */
Input::~Input()
{
    delete ui;
    delete jssp;
}

/**
 * @brief Action of start button.
 */
void Input::on_start_button_clicked()
{
    this->close();
    jssp->getProb(ui->plainTextEdit->toPlainText());
    jssp->runProb();
    Result *r = new Result(jssp,this);
    r->show();
    r->exec();
}
