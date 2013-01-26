#include "cpoly.h"
#include "ctoken.h"
#include "mainwindow.h"

CPoly::CPoly()
{
    m_tokens.clear();
    m_p = 1;            //by default, the power of the polynom is 1
}

void CPoly::Add(CToken* t1)
{
    t1->CheckSign();
    MainWindow::Debug(QString("[CPoly] add token: %1").arg(t1->ToLatex()));
    TokenMapIt it = m_tokens.find(t1->m_p);
    if(it != m_tokens.end())        //does the specific power already exist in the polynom?
    {
        CToken* new_token = *(it->second)+*t1;
        delete it->second;
        m_tokens.erase(it);
        if(new_token->m_c != 0)
            m_tokens[new_token->m_p] = new_token;
    }
    else
    {
        m_tokens[t1->m_p] = t1;

    }
}

QString CPoly::ToLatex()
{
    QString ret = "(";
    int size = m_tokens.size();
    int count = 0;
    for(TokenMapIt it = m_tokens.begin();it != m_tokens.end();++it)
    {
        count++;
        ret.append(it->second->ToLatex());
        if(count < size)
        {
            ret.append(" + ");
        }
    }
    ret.append(")");
    return ret;
}


CPoly* CPoly::operator *(CPoly& p1)
{
    CPoly* ret = new CPoly();
    for(TokenMapIt it1 = m_tokens.begin(); it1 != m_tokens.end(); it1++)
    {
       for(TokenMapIt it2 = p1.m_tokens.begin();it2 != p1.m_tokens.end() ; it2++)
        {
            ret->Add(*(it1->second)*(*it2->second));
        }
    }
    return ret;
}
