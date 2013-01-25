#include "ctoken.h"

CToken::CToken(double c, int p)
{
    m_c = c;
    m_p = p;
}

CToken* CToken::operator* (CToken* t1)
{
    CToken* t3 = new CToken(1,1);
    t3->m_c = m_c*t1->m_c;
    t3->m_p = m_p + t1->m_p;
    return t3;
}
