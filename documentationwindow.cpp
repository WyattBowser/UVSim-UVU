#include "documentationwindow.h"
#include "ui_documentationwindow.h"
#include <QDebug>

documentationWindow::documentationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::documentationWindow)
{
    ui->setupUi(this);
}

documentationWindow::~documentationWindow()
{
    delete ui;
}
