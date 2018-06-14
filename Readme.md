## Yet Another JSSP solver - GUI(QT) Version

This project is a final project of "Course Exercise of Introduction to Computation" in BUPT.

During the whole time of this project, our excellent teachers changed the requirment of this project again and again, which gave us a REAL experience. The experience of develop a application under a fucking mess management.

### Introduction

Job shop scheduling problem(**JSSP**) is an optimization problem in computer science and operations research in which jobs are assigned to resources at particular times. The basic version is as follows: We are given n jobs J1, J2, ..., Jn of varying processing times, which need to be scheduled on m machines with varying processing power, while trying to minimize the makespan. The makespan is the total length of the schedule (that is, when all the jobs have finished processing).

In this JSSP solver, we use a method called "shifting bottleneck procedure(**SB**)" to find a not-so-bad solution. The method is first described by J.Adams et al in "[The Shifting Bottleneck Procedure for Job Shop Scheduling]( http://www.jstor.org/stable/2632051)". In our implementation, we apply the backtracing technique on the first version of SB to get a better solution. Which is proved to be useful in practice.

### Usage

It is very easy, just double click the program and input your instance. Then all you need to do is have a drink and get the solution.Our instance file have as almost same format as those in orlib. For example:
```
 0 29 1 78 2  9 3 36 4 49 5 11 6 62 7 56 8 44 9 21
 0 43 2 90 4 75 9 11 3 69 1 28 6 46 5 46 7 72 8 30
 1 91 0 85 3 39 2 74 8 90 5 10 7 12 6 89 9 45 4 33
 1 81 2 95 0 71 4 99 6  9 8 52 7 85 3 98 9 22 5 43
 2 14 0  6 1 22 5 61 3 26 4 69 8 21 7 49 9 72 6 53
 2 84 1  2 5 52 3 95 8 48 9 72 0 47 6 65 4  6 7 25
 1 46 0 37 3 61 2 13 6 32 5 21 9 32 8 89 7 30 4 55
 2 31 0 86 1 46 5 74 4 32 6 88 8 19 9 48 7 36 3 79
 0 76 1 69 3 76 5 51 2 85 9 11 6 40 7 89 4 26 8 74
 1 85 0 13 2 61 6  7 8 64 9 76 5 47 3 52 4 90 7 45
 ```

NOTE: In our instance file, we don't have comment in the head of the instance. Which is part of our fucking requirement.

### Compilation

The compilation is extremely easy:

```bash
$ mkdir build
$ cd build
$ qmake -r ../src
$ make # For Visual Studio, use nmake instead.
```

### About License

This project is distributed with WTFPL. But you still can't steal any piece of code in this project
as your homework.