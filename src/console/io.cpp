﻿/**
 * @file io.cpp
 * Functions to handle input and output.
 *
 * @brief IO
 * @author TJenica
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bottle.h>
#include <QVector>
#include <algorithm>
#include <jobshop.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>

/**
  A temporary struct for converting the internal representation
  of the solution to the format required by those soreheads.
 */
typedef struct PAIR_ASSISTANT_TYPE {
	int start_time;		/**< Start time of this node. */
	int job_num;		/**< Serial number of the job of this node. */
	int mach_num;		/**< Serial number of the machine of this node. */
	int proc_time;		/**< Process time (a.k.a duration) of this node.*/
	int step;			/**< Serial number of the order of this node in the job. */
} pair_ass_t;

float used_time;
extern int best_makespan;

/**
  Convert internal solution representation structure to
  the format required by those nitpickers and print it.

  @param filename Instance file path
 */
void prestissimo(void) {
	clock_t t = clock();
	run_bottle_neck();
    used_time = (clock() - t) * 1.0f / CLOCKS_PER_SEC;
	return;
}

QString JobShop::colsoleOutput() {
    QString ans = "";
    QVector<pair_ass_t> pairs;
    for (int i = 0; i < job_size; i++) {
        for (int j = 0; j < machine_size; j++) {
            pair_ass_t ass;
            ass.start_time = job[i].start[j];
            ass.job_num = i;
            ass.mach_num = j;
            ass.proc_time = job[i].process_time[j];
            ass.step = job[i].step[j];
            pairs.append(ass);
        }
    }

    for(auto t = fixer.begin(); t != fixer.end(); t++) {
        pair_ass_t ass;
        Fixer *f = *t;
        ass.start_time = f->starttime;
        ass.proc_time = f->duration;
        ass.mach_num = f->machine;
        ass.step = -1;
        ass.job_num = -1;
        pairs.append(ass);
    }

    std::sort(pairs.begin(),pairs.end(),
              [](const pair_ass_t& a,const pair_ass_t& b) -> bool {
        return a.mach_num < b.mach_num;
    });
    auto it = pairs.begin();
    for (int j = 0; j < machine_size; j++) {
        auto end = it;
        for(; end != pairs.end() && end->mach_num == j;end++);
        std::sort(it, end,
                  [](const pair_ass_t& a,const pair_ass_t& b) -> bool {
            return a.start_time < b.start_time;
        });
        ans += QString::asprintf("M%d",j);
        qDebug() << end - it;
        for(auto temp = it; temp != end; temp++) {
            if(temp->job_num == -1) {
                ans += QString::asprintf(" (%d,检修,%d)", temp->start_time, temp->start_time + temp->proc_time);
            } else
            ans += QString::asprintf(" (%d,%d-%d,%d)", temp->start_time, temp->job_num, temp->step, temp->start_time + temp->proc_time);
        }
        ans += "\n";
        it = end;
    }
    return ans;
}
/**
  Print result to file...

  @param file_name Instance file path
  @param pairs Pair to be printed...
 */
void JobShop::writeFile(QString file_name) {
    QFile output(file_name);
    output.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream s(&output);
    s << this->colsoleOutput();
    s << QString::asprintf("Time Used: %.3fs\nEnd Time: %d\n",used_time, best_makespan);
}
