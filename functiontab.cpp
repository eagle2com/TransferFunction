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
    m_expanded_numerator = NULL;
    linear_system = NULL;
}

FunctionTab::~FunctionTab()
{
    delete ui;
}

void FunctionTab::on_pushButton_clicked()
{
    PrettyPrintInput();
    m_poly_factors.clear();
    m_poly_denom.clear();


    //========= expanding numerator ==============
    printf("Expanding...\n");
    if(m_expanded_numerator != NULL)
        delete m_expanded_numerator;

    m_expanded_numerator = new CPoly();
    m_expanded_numerator->Add(new CToken(1,0));
    for(int i = 0; i < m_numerator.size(); i++)
    {
        //MainWindow::Debug(QString("%1 x %2").arg(m_expanded_numerator->ToLatex()).arg(m_numerator[i]->ToLatex()));
        m_expanded_numerator = (*m_expanded_numerator)*(*m_numerator[i]);
    }
    double* constants = m_expanded_numerator->GetConstants();
    MainWindow::Debug(QString("expanded numerator: %1").arg(m_expanded_numerator->ToLatex()));

    //============= computing polynom factors ====================
    printf("Computing factors...\n");
    int size_m = 0;
    for(int i = 0; i < m_denominator.size();i++)
    {
        CPoly* poly_factor = new CPoly();
        poly_factor->Add(new CToken(1,0));
        for(int n = 0; n < m_denominator.size(); n++)
        {
            if(i == n) continue;

            poly_factor = (*poly_factor)*(*m_denominator[n]);
        }
        size_m += m_denominator[i]->Order();
        m_poly_factors.push_back(poly_factor);
        m_poly_denom.push_back(m_denominator[i]);

        MainWindow::Debug(QString("new poly factor: %1").arg(poly_factor->ToLatex()));
    }
    if(linear_system != NULL)
        delete linear_system;
    printf("expanded numerator: %s\n",m_expanded_numerator->ToLatex().toStdString().c_str());
    linear_system = new CLinearSystem(size_m,size_m+1);

    //set the B part of Ax = B
    for(int n = 0; n < linear_system->N(); n++)
    {
        linear_system->Set(n,linear_system->M()-1,constants[n]);
    }

    //set the A,B,C,D constants
    int constant_m = 0;
    //loop through every poly factor
    for(int i = 0; i < m_poly_factors.size();i++)
    {
        double* factor_constants = m_poly_factors[i]->GetConstants();
        printf("constants: \n");
        for(int ii = 0; ii < m_poly_factors[i]->Order()+1;ii++)
        {
            printf("%1.1f ",factor_constants[ii]);
        }
        printf("\n");
        //loop through every constant multiplier of the poly factor
        for(int k = 0; k < m_poly_denom[i]->Order();k++)
        {
             //loop through every power of the poly factor
            for(int j=0; j < m_poly_factors[i]->Order()+1;j++)
            {
                int power = j+k;
                double cst = factor_constants[j];
                printf("setting %1.2f at %d:%d\n",cst,power,constant_m);
                linear_system->Set(power,constant_m,cst);

            }
            constant_m++;
        }
        delete [] factor_constants;
    }
    linear_system->Solve();

    delete [] constants;
}

void FunctionTab::PrettyPrintInput()
{
    QString num_latex = "";
    QString den_latex = "";

    for(int i = 0; i < m_numerator.size(); i++)
    {
        if(!m_numerator[i]->isEmpty())
            num_latex.append(m_numerator[i]->ToLatex());
    }
    for(int i = 0; i < m_denominator.size(); i++)
    {
        if(!m_denominator[i]->isEmpty())
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

void FunctionTab::PrettyPrintOutput()
{
    QString num_latex = "";
    QString den_latex = "";

    for(int i = 0; i < m_numerator.size(); i++)
    {
        if(!m_numerator[i]->isEmpty())
            num_latex.append(m_numerator[i]->ToLatex());
    }
    for(int i = 0; i < m_denominator.size(); i++)
    {
        if(!m_denominator[i]->isEmpty())
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

void FunctionTab::on_denominator_text_textChanged(const QString &arg1)
{
    sign = 1;
    MainWindow::ClearDebug();
    m_denominator.clear();
    denominator_state = IDLE;
    for(int i = 0; i < arg1.size(); i++)
    {
        if(!AnalyzeDenChar(arg1[i]))
            break;
    }
}

bool FunctionTab::AnalyzeDenChar(QChar c)
{
    int type = ClassifyChar(c);
    //MainWindow::Debug(QString("type: ").arg())

    if(type == ERROR)
    {
        DenominatorInvalid(c);
        return false;
    }

    int transition = TransitionDen(type);
    MainWindow::Debug(QString("%1 -> %2").arg(StateName(denominator_state),StateName(transition)));
    if(transition == ERROR)
    {
        DenominatorInvalid(c);
        return false;
    }


    switch(denominator_state)
    {
    case IDLE:
        switch(transition)
        {
        case POLY:
            current_poly = new CPoly();
            sign = 1;
            break;
        default:
            DenominatorInvalid(c);
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
            m_denominator.push_back(current_poly);
            break;
        case VAR:
            current_token = new CToken;
            current_token->m_c = 1;
            current_token->SetSign(sign);
            break;
        default:
            DenominatorInvalid(c);
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
            m_denominator.push_back(current_poly);
            break;
        case VAR:
            current_token->m_c = cst_string.toDouble();
            if(cst_string[0]== '-' || cst_string[0]=='+')
                current_token->m_c = 1;
            break;
        default:
            DenominatorInvalid(c);
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
            m_denominator.push_back(current_poly);
            break;
        case VAR:
            current_token->m_c = cst_string.toDouble();
            break;
        default:
            DenominatorInvalid(c);
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
            m_denominator.push_back(current_poly);
            break;

        default:
            DenominatorInvalid(c);
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
            m_denominator.push_back(current_poly);
            break;
        default:
            DenominatorInvalid(c);
            return false;
            break;
        }
        break;

    default:
        MainWindow::Debug("Unknow state");
        break;
    }

    denominator_state = transition;

   // MainWindow::Debug("OK");
    return true;
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
            current_token->SetSign(sign);
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
            if(cst_string[0]== '-' || cst_string[0]=='+')
                current_token->m_c = 1;

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

int FunctionTab::TransitionDen(int t)
{
    return state_machine[t][denominator_state];
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

void FunctionTab::on_denominator_text_returnPressed()
{
    on_pushButton_clicked();
}
