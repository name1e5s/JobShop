/**
 * @file input.h
 * This file defines the input window.
 *
 * @author Name1e5s
 */

#ifndef INPUT_H
#define INPUT_H

#include <QMainWindow>

#include <jobshop.h>

namespace Ui {
class Input;
}

class Input : public QMainWindow
{
    Q_OBJECT

public:
    explicit Input(QWidget *parent = 0);
    ~Input();

private slots:
    void on_start_button_clicked();

private:
    Ui::Input *ui;
    JobShop *jssp;
};

#endif // INPUT_H

