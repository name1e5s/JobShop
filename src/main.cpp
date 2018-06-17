/**
 * @file main.cpp
 * Enterpoint of the program.
 *
 * @author Name1e5s
 */
#include <QApplication>
#include <QTextCodec>

#include <input.h>

int main(int ac,char *av[]) {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(ac,av);
    Input w;
    w.show();
    return a.exec();
}
