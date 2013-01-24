#include "functiontab.h"
#include "ui_functiontab.h"

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
