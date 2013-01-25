#include "ctoken.h"
#include "exception"

///TODO: add CToken garbage collector

CToken::CToken(double c, int p)
{
    m_c = c;
    m_p = p;
}

CToken* CToken::operator* (const CToken& t1)
{
    CToken* t3 = new CToken(1,1);
    t3->m_c = m_c*t1.m_c;
    t3->m_p = m_p + t1.m_p;
    return t3;
}

CToken* CToken::operator+ (const CToken& t1)
{
    CToken* t3 = new CToken(1,1);
    if(m_p != t1.m_p)
        throw 0;

    t3->m_c = m_c + t1.m_c;
    t3->m_p = m_p;
    return t3;
}

QString CToken::ToLatex()
{
    QString ret = "";
    if(m_c == 0)
        return ret;
    if(m_c != 1)
    {
        ret.append(QString("%1").arg(m_c));
    }
    if(m_c == 1 && m_p == 0)
    {
        ret.append('1');
    }
    if(m_p > 0)
    {
        ret.append("s");
    }
    if(m_p > 1)
    {
        ret.append(QString("^{%1}").arg(m_p));
    }
    return ret;
}
