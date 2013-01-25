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
    denominator_state = IDLE;

}

FunctionTab::~FunctionTab()
{
    delete ui;
}

void FunctionTab::on_pushButton_clicked()
{

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
        if(last == '(') //we can only begin a polynom when idle, always with "("
        {
            numerator_state = POLY_START;
            current_poly = new CPoly();
        }
        else
        {
            NumeratorInvalid(last);
        }
        break;
    case POLY_START:
        if(QString("1234567890-").contains(last))
        {
            current_token = new CToken(1,0);
            token_had_point = false;
            numerator_state = TOKEN_CONSTANT;
            s_constant = "";
            s_constant.append(last);
            MainWindow::Debug(QString("starting s_constant with %1").arg(last));
        }
        else if(last == 's')
        {
            current_token = new CToken(1,0);
            constant = 1;
            MainWindow::Debug(QString("constant skipped -> setting to 1"));
            numerator_state = TOKEN_S;
        }
        else
        {
            NumeratorInvalid(last);
        }
        break;
    case TOKEN_CONSTANT:
        if(current_token == NULL)
            current_token = new CToken(1,0);
        if(QString("1234567890.").contains(last) && !token_had_point)
        {
            if(last == '.')
                token_had_point = true;
            s_constant.append(last);
            MainWindow::Debug(QString("added %1 to s_constant").arg(last));
        }
        else if(last == 's')
        {
            constant = s_constant.toDouble();
            current_token->m_c = constant;
            MainWindow::Debug(QString("finished constant -> set to %1").arg(constant));
            numerator_state = TOKEN_S;
        }
        else if(last == ')')
        {
            constant = s_constant.toDouble();
            current_token->m_c = constant;
            FinishedNumPoly();
        }
        else
        {
            NumeratorInvalid(last);
        }
        break;
    case TOKEN_S:
        if(last == '^')
        {
            p_constant = "";
            numerator_state = TOKEN_POWER;
        }
        else if(last==')')
        {
            current_token->m_p = 1;
            FinishedNumPoly();
        }
        else if(last == '-' || last == '+')
        {
            current_token->m_p = 1;
            current_poly->Add(current_token);
            MainWindow::Debug(QString("added new token: %1").arg(current_token->ToLatex()));
            current_token = NULL;
            numerator_state = TOKEN_CONSTANT;
        }
        else
        {
            NumeratorInvalid(last);
        }
        break;
    default:
        MainWindow::Debug("unkown state!!!");
        break;
    }



}

void FunctionTab::on_reset_button_clicked()
{
    numerator_state = IDLE;
    denominator_state = IDLE;
    ui->numerator_text->clear();
    ui->denominator_text->clear();
}


void FunctionTab::NumeratorInvalid(QChar last)
{
    //MainWindow::Debug(QString("%1 is invalid").arg(last));
    ui->numerator_text->blockSignals(true);
    ui->numerator_text->backspace();
    ui->numerator_text->blockSignals(false);
}


void FunctionTab::FinishedNumPoly()
{
    current_poly->Add(current_token);
    m_numerator.push_back(current_poly);
    MainWindow::Debug(QString("finished poly: %1").arg(current_poly->ToLatex()));
    s_constant = "";
    p_constant = "",
    numerator_state = IDLE;
    current_poly = NULL;
    current_token = NULL;
}
