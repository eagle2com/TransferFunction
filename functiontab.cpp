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
    sign = 1;
    MainWindow::ClearDebug();
    m_numerator.clear();
    numerator_state = IDLE;
    for(int i = 0; i < arg1.size(); i++)
    {
        if(!AnalyzeNumChar(arg1[i]))
            break;
    }
}

bool FunctionTab::AnalyzeNumChar(QChar c)
{
    int type = ClassifyChar(c);
    //MainWindow::Debug(QString("type: ").arg())

    if(type == ERROR)
    {
        NumeratorInvalid(c);
        return false;
    }

    int transition = TransitionNum(type);
    MainWindow::Debug(QString("%1 -> %2").arg(StateName(numerator_state),StateName(transition)));
    if(transition == ERROR)
    {
        NumeratorInvalid(c);
        return false;
    }


    switch(numerator_state)
    {
    case IDLE:
        switch(transition)
        {
        case POLY:
            current_poly = new CPoly();
            sign = 1;
            break;
        default:
            NumeratorInvalid(c);
            return false;
            break;
        }
        break;

    case POLY:
        switch(transition)
        {
        case TOKEN_INT:
            current_token = new CToken;
            cst_string = c;
            current_token->SetSign(sign);
            break;
        case TOKEN_DEC:
            current_token = new CToken;
            cst_string = c;
            break;
        case IDLE:
            m_numerator.push_back(current_poly);
            break;
        case VAR:
            current_token = new CToken;
            current_token->m_c = 1;
            break;
        default:
            NumeratorInvalid(c);
            return false;
            break;
        }
        break;

    case TOKEN_INT:
        switch(transition)
        {
        case TOKEN_INT:
            cst_string.append(c);
            break;
        case POLY:
            current_token->m_c = cst_string.toDouble();
            current_poly->Add(current_token);
            break;
        case TOKEN_DEC:
            cst_string.append(c);
            break;
        case IDLE:
            current_token->m_c = cst_string.toDouble();
            current_poly->Add(current_token);
            m_numerator.push_back(current_poly);
            break;
        case VAR:
            current_token->m_c = cst_string.toDouble();
            break;
        default:
            NumeratorInvalid(c);
            return false;
            break;
        }
        break;

    case TOKEN_DEC:
        switch(transition)
        {
        case TOKEN_DEC:
            cst_string.append(c);
            break;
        case POLY:
            current_token->m_c = cst_string.toDouble();
            current_poly->Add(current_token);
            break;
        case IDLE:
            current_token->m_c = cst_string.toDouble();
            current_poly->Add(current_token);
            m_numerator.push_back(current_poly);
            break;
        case VAR:
            current_token->m_c = cst_string.toDouble();
            break;
        default:
            NumeratorInvalid(c);
            return false;
            break;
        }
        break;

    case VAR:
        switch(transition)
        {
        case POLY:
            current_token->m_p = 1;
            current_poly->Add(current_token);
            break;
        case EXP:
            pwr_string = c;
            break;
        case IDLE:
            current_token->m_p = 1;
            current_poly->Add(current_token);
            m_numerator.push_back(current_poly);
            break;

        default:
            NumeratorInvalid(c);
            return false;
            break;
        }
        break;

    case EXP:
        switch(transition)
        {
        case EXP:
            pwr_string.append(c);
            break;
        case POLY:
            current_token->m_p = pwr_string.toInt();
            current_poly->Add(current_token);
            break;
        case IDLE:
            current_token->m_p = pwr_string.toInt();
            current_poly->Add(current_token);
            m_numerator.push_back(current_poly);
            break;
        default:
            NumeratorInvalid(c);
            return false;
            break;
        }
        break;

    default:
        MainWindow::Debug("Unknow state");
        break;
    }

    numerator_state = transition;

   // MainWindow::Debug("OK");
    return true;
}

int FunctionTab::TransitionNum(int t)
{
    return state_machine[t][numerator_state];
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


/*
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
}*/

/*
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
}*/
void FunctionTab::on_denominator_text_textChanged(const QString &arg1)
{

}

int FunctionTab::ClassifyChar(QChar c)
{
    if(c == '+')
        sign = 1;
    else if(c=='-')
        sign = -1;

    if(c.isDigit())
        return DIGIT;
    else if(c == '+' || c == '-')
        return SIGN;
    else if(c == '.')
        return POINT;
    else if(c == '(')
        return OPEN_POLY;
    else if(c == ')')
        return CLOSE_POLY;
    else if(c == 's')
        return VAR_S;
    else
        return ERROR;
}

QString FunctionTab::StateName(int s)
{
    switch(s)
    {
    case IDLE:
        return "IDLE";
    case POLY:
        return "POLY";
    case TOKEN_INT:
        return "TOKEN_INT";
    case TOKEN_DEC:
        return "TOKEN_DEC";
    case VAR:
        return "VAR";
    case EXP:
        return "EXP";
    default:
        return QString("<undefined(%1)>").arg(s);
    }
}

void FunctionTab::on_numerator_text_returnPressed()
{
    on_pushButton_clicked();
}
