#ifndef FUNCTIONTAB_H
#define FUNCTIONTAB_H

#include <QWidget>
#include <vector>
#include "cpoly.h"

enum STATE{IDLE,POLY_START,POLY_END, TOKEN_CONSTANT, TOKEN_S, TOKEN_POWER};

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

private:
    Ui::FunctionTab *ui;
    int numerator_state;
    int denominator_state;

    std::vector<CPoly*> m_numerator;
    std::vector<CPoly*> m_denominator;

     QString s_constant;
     QString p_constant;

     CToken* current_token;
     CPoly* current_poly;

     bool token_had_point;

     void NumeratorInvalid(QChar last);
     void DenominatorInvalid(QChar last);
     void FinishedNumPoly();
     void FinishedDenPoly();

};

#endif // FUNCTIONTAB_H
