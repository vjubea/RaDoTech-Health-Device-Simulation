#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new Model();
}

MainWindow::~MainWindow()
{
    delete ui;
}

