/**
 * @file main.cpp
 * Enterpoint of the program.
 *
 * @author Name1e5s
 */
#include <QApplication>
#include <QDebug>
#include <input.h>

int main(int ac,char *av[]) {
    QApplication a(ac,av);
    Input w;
    w.show();
    return a.exec();
}
