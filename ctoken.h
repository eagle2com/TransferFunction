#ifndef CTOKEN_H
#define CTOKEN_H

class CToken
{
public:
    CToken(double c, int p);
    CToken* operator* (CToken* t1);

protected:
    double m_c; //multiplier constant
    int m_p; //power constant
};

#endif // CTOKEN_H
