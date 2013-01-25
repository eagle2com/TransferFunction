#include "functiontab.h"
#include "ui_functiontab.h"
#include "mainwindow.h"
#include <QDebug>

FunctionTab::FunctionTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FunctionTab)
{
    ui->setupUi(this);
    numerator_state = IDLE;
    denomintator_state = IDLE;
}

FunctionTab::~FunctionTab()
{
    delete ui;
}

void FunctionTab::on_pushButton_clicked()
{
    MainWindow::Debug("analyze pressed");
}

void FunctionTab::on_numerator_text_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        return;
    QChar last = arg1[arg1.size()-1];

    double constant = 1;

    switch(numerator_state)
    {
    case IDLE:
        if(last == "(") //we can only begin a polynom when idle, always with "("
        {
            numerator_state = POLY_START;
        }
        else
        {
            MainWindow::Debug(QString("%1 is invalid").arg(last));
            ui->numerator_text->backspace();
        }
        break;
    case POLY_START:
        if(QString("1234567890-").contains(last))
        {
            numerator_state = TOKEN_CONSTANT;
            s_constant.clear();
            s_constant.append(last);
            MainWindow::Debug(QString("starting s_constant with %1").arg(last));
        }
        else if(last == "s")
        {
            constant = 1;
            MainWindow::Debug(QString("constant skipped -> setting to 1"));
            numerator_state = TOKEN_POWER;
        }
        else
        {
            MainWindow::Debug(QString("%1 is invalid").arg(last));
            ui->numerator_text->backspace();
        }
        break;
    case TOKEN_CONSTANT:
        if(QString("1234567890").contains(last))
        {
            s_constant.append(last);
            MainWindow::Debug(QString("added %1 to s_constant").arg(last));
        }
        else if(last == "s")
        {

            constant = s_constant.toDouble();
            qDebug() << "s_constant: " << s_constant<<"   constant: "<<constant<<"\n";
            MainWindow::Debug(QString("finished constant -> set to %1").arg(constant));
            numerator_state = TOKEN_POWER;
        }
        else
        {
            MainWindow::Debug(QString("%1 is invalid").arg(last));
            ui->numerator_text->backspace();
        }
        break;
    default:
        MainWindow::Debug("unkown state!!!");
        break;
    }



}
