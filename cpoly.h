#ifndef CPOLY_H
#define CPOLY_H

#include "ctoken.h"
#include <map>



//polynom containing tokens
class CPoly
{
public:
    CPoly();

private:
    std::map<int,CToken*> m_tokens;
    int m_p; //global power of the polynom
};

#endif // CPOLY_H
