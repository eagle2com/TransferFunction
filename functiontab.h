#ifndef FUNCTIONTAB_H
#define FUNCTIONTAB_H

#include <QWidget>
#include <vector>
#include "cpoly.h"
#include "clinearsystem.h"
#include <vector>

enum STATE{IDLE,POLY,TOKEN_INT,TOKEN_DEC,VAR,EXP};
enum TRANSITION{DIGIT,SIGN,POINT,OPEN_POLY,CLOSE_POLY,VAR_S,UNUSED};
#define ERROR -1

const int state_machine[7][6] =
{/*              IDLE   POLY        TOKEN_INT   TOKEN_DEC   VAR     EXP*/
/*DIGIT*/       {ERROR, TOKEN_INT,  TOKEN_INT,  TOKEN_DEC,  EXP,    EXP},
/*SIGN*/        {ERROR, TOKEN_INT,  POLY,       POLY,       POLY,   POLY},
/*POINT*/       {ERROR, TOKEN_DEC,  TOKEN_DEC,  ERROR,      ERROR,  ERROR},
/*OPEN_POLY*/   {POLY,  ERROR,      ERROR,      ERROR,      ERROR,  ERROR},
/*CLOSE_POLY*/  {ERROR, IDLE,       IDLE,       IDLE,       IDLE,   IDLE},
/*VAR_S*/       {ERROR, VAR,        VAR,        VAR,        ERROR,  ERROR},
/*UNUSED*/      {ERROR, ERROR,      ERROR,     ERROR,       ERROR,  ERROR},
};



namespace Ui {
class FunctionTab;
}

class FunctionTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit FunctionTab(QWidget *parent = 0);
    ~FunctionTab();
    
private slots:
    void on_pushButton_clicked();

    void on_numerator_text_textChanged(const QString &arg1);

    void on_reset_button_clicked();

    void on_denominator_text_textChanged(const QString &arg1);

    void on_numerator_text_returnPressed();

    void on_denominator_text_returnPressed();

private:
    Ui::FunctionTab *ui;
    int numerator_state;
    int denominator_state;

    std::vector<CPoly*> m_numerator;
    std::vector<CPoly*> m_denominator;

     QString cst_string;
     QString pwr_string;

     CToken* current_token;
     CPoly* current_poly;

     void NumeratorInvalid(QChar last);
     void DenominatorInvalid(QChar last);

     bool AnalyzeNumChar(QChar c);     //returns true if valid (not ERROR)
     bool AnalyzeDenChar(QChar c);

     int TransitionNum(int t);
     int TransitionDen(int t);
     int ClassifyChar(QChar c);

     QString StateName(int s);

     int sign;

     void PrettyPrintInput();
     void PrettyPrintOutput();

     CLinearSystem* linear_system;

     CPoly* m_expanded_numerator;
     std::vector<CPoly*> m_poly_factors;
     std::vector<CPoly*> m_poly_denom;
};

#endif // FUNCTIONTAB_H
