###############################################################
#       _  _____ _____ _____     _____       _
#      | |/ ____/ ____|  __ \   / ____|     | |
#      | | (___| (___ | |__) | | (___   ___ | |_   _____ _ __
#  _   | |\___ \\___ \|  ___/   \___ \ / _ \| \ \ / / _ \ '__|
# | |__| |____) |___) | |       ____) | (_) | |\ V /  __/ |
#  \____/|_____/_____/|_|      |_____/ \___/|_| \_/ \___|_|
#
#      Yet Another JSSP Solver by Name1e5s -- QT Ver.
###############################################################

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JSSP-QT
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Add our include path into this project
INCLUDEPATH += $$PWD/include

SOURCES += \
        main.cpp                            \
        jobshop.cpp                         \
        algorithm/bottle.cpp                \
        algorithm/common_definition.cpp     \
        algorithm/eval.cpp                  \
        algorithm/onemachine.cpp            \
        gantt/arrow.cpp                     \
        gantt/ganttchartbase.cpp            \
        gantt/ganttchartmachine.cpp         \
        gantt/ganttchartoperation.cpp       \
        ui/input.cpp                        \
	ui/result.cpp						

HEADERS += \
        include/bottle.h                    \
        include/jobshop.h                   \
        include/arrow.h                     \
        include/ganttchartbase.h            \
        include/ganttchartmachine.h         \
        include/ganttchartoperation.h       \
        include/input.h                     \
        include/result.h

FORMS += \
        forms/input.ui                      \
        forms/result.ui

# For windows, we can add an icon to make the program
# better.
win32:RC_ICONS += \
        icon/JobShop-QT.ico