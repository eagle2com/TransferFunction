#ifndef CTOKEN_H
#define CTOKEN_H

#include <QString>
#include "mainwindow.h"

class CPoly;
class FunctionTab;

class CToken
{
    friend class CPoly;
    friend class FunctionTab;
public:
    CToken(double c=0, int p=0);
    CToken* operator* (const CToken& t1);
    CToken* operator+ (const CToken& t1);

    QString ToLatex(bool first);
    void SetSign(int s)
    {
        sign = s;

        MainWindow::Debug(QString("set sign: %1").arg(s));
    }

    void CheckSign()
    {
        if(m_c*sign < 0)
        {
            m_c *= sign;
        }
    }

protected:
    int sign;
    double m_c; //multiplier constant
    int m_p; //power constant
};

#endif // CTOKEN_H
