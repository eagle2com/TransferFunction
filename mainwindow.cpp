#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functiontab.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    ui->tabWidget->addTab(new FunctionTab(ui->tabWidget),QString("har"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_new_Transfer_Function_triggered()
{
    ui->tabWidget->addTab(new FunctionTab(ui->tabWidget),QString("har"));
}
