#include "functiontab.h"
#include "ui_functiontab.h"
#include "mainwindow.h"

FunctionTab::FunctionTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionTab)
{
    ui->setupUi(this);
}

FunctionTab::~FunctionTab()
{
    delete ui;
}

void FunctionTab::on_pushButton_clicked()
{
    MainWindow::Debug("analyze pressed");
}
