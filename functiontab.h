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

private:
    Ui::FunctionTab *ui;
    int numerator_state;
    int denomintator_state;

    std::vector<CPoly*> m_numerator;
    std::vector<CPoly*> m_denominator;



};

#endif // FUNCTIONTAB_H
