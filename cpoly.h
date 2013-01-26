#ifndef CPOLY_H
#define CPOLY_H

#include "ctoken.h"
#include <map>

typedef std::map<int,CToken*> TokenMap;
typedef TokenMap::iterator TokenMapIt;

//polynom containing tokens
class CPoly
{
public:
    CPoly();
    void Add(CToken* t1);
    CPoly* operator*(CPoly& p1);
    QString ToLatex();
    bool isEmpty();

protected:
    TokenMap m_tokens;
    int m_p; //global power of the polynom
};

#endif // CPOLY_H
