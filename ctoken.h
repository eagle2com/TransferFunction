#ifndef CTOKEN_H
#define CTOKEN_H

#include <QString>

class CPoly;
class FunctionTab;

class CToken
{
    friend class CPoly;
    friend class FunctionTab;
public:
    CToken(double c, int p);
    CToken* operator* (const CToken& t1);
    CToken* operator+ (const CToken& t1);

    QString ToLatex();

protected:

    double m_c; //multiplier constant
    int m_p; //power constant
};

#endif // CTOKEN_H
