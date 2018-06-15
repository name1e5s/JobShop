/**
 * @file ganttchartprogress.cpp
 *
 * @author Name1e5s
 */
#include <ganttchartprogress.h>

#include <jobshop.h>
extern int best_makespan;

/**
 * @brief GanttChartProgress::run
 */
void GanttChartProgress::run() {
    double val = 0;
    while (val <= best_makespan) {
        msleep(1000);
        val += speed;
        emit updateline((int)val);
    }
}

/**
 * @brief GanttChartProgress::setSpeed
 * @param value
 */
void GanttChartProgress::setSpeed(double value)
{
    speed = value;
}
