/**
    CS-2450 Milestone 2: Main driver for UVSim.
    @file main.cpp
    @author IED (Scrum Team 10)
    @version 1.1 3/21/21
*/
#include "mainwindow.h"

#include <QApplication>

#include "UVsim.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <QDebug>

using namespace std;

int main(int argc, char* argv[]) {

    QApplication application(argc, argv);
    MainWindow window(&application);

    window.show();

    return application.exec();
}
