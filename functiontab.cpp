#include "functiontab.h"
#include "ui_functiontab.h"
#include "mainwindow.h"
#include <QDebug>
#include <QProcess>

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
    QString num_latex = "";
    QString den_latex = "";

    for(int i = 0; i < m_numerator.size(); i++)
    {
        num_latex.append(m_numerator[i]->ToLatex());
    }
    for(int i = 0; i < m_denominator.size(); i++)
    {
        den_latex.append(m_denominator[i]->ToLatex());
    }
    MainWindow::Debug(QString("num: %1").arg(num_latex));
    MainWindow::Debug(QString("den: %1").arg(den_latex));

    QString total_latex = "\\frac{"+num_latex+"}{"+den_latex+"}";
    QString command = "tex2png -c \" $  "+total_latex+" $\" -T -s 2000 -o input.png";
     MainWindow::Debug(QString("exec: %1").arg(command));


    QProcess process;//, QStringList() << docPath
    process.start(command);
    process.waitForFinished();

   // process.start("gpicview input.png");
    //process.waitForFinished();
    ui->input_label->setPixmap(QPixmap("input.png"));
}

void FunctionTab::on_numerator_text_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        return;
    QChar last = arg1[arg1.size()-1];




}

void FunctionTab::on_reset_button_clicked()
{
    numerator_state = IDLE;
    denominator_state = IDLE;
    ui->numerator_text->clear();
    ui->denominator_text->clear();
    m_numerator.clear();
    m_denominator.clear();
}


void FunctionTab::NumeratorInvalid(QChar last)
{
    //MainWindow::Debug(QString("%1 is invalid").arg(last));
    ui->numerator_text->blockSignals(true);
    ui->numerator_text->backspace();
    ui->numerator_text->blockSignals(false);
}

void FunctionTab::DenominatorInvalid(QChar last)
{
    //MainWindow::Debug(QString("%1 is invalid").arg(last));
    ui->denominator_text->blockSignals(true);
    ui->denominator_text->backspace();
    ui->denominator_text->blockSignals(false);
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

void FunctionTab::FinishedDenPoly()
{
    current_poly->Add(current_token);
    m_denominator.push_back(current_poly);
    MainWindow::Debug(QString("finished poly: %1").arg(current_poly->ToLatex()));
    s_constant = "";
    p_constant = "",
    denominator_state = IDLE;
    current_poly = NULL;
    current_token = NULL;
}

void FunctionTab::on_denominator_text_textChanged(const QString &arg1)
{

}
