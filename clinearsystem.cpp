#include "clinearsystem.h"
#include <malloc.h>
#include <QMessageBox>
#include <stdio.h>

CLinearSystem::CLinearSystem(int n, int m)
{
    m_n = n;
    m_m = m;
    m_pMatrix = NULL;
    m_pMatrix = (double*)calloc(n*m,sizeof(double));
    if(m_pMatrix == NULL)
    {
        //QMessageBox("Fatal error","Could not allocate memory for m_pMatrix !!!");
        throw "Could not allocate memory for m_pMatrix";
    }

    printf("new system: %dx%d\n",n,m);
}

CLinearSystem::~CLinearSystem()
{
    delete [] m_pMatrix;
}

double CLinearSystem::Get(int n, int m) const
{
    CheckBounds(n,m);
    return m_pMatrix[m_m*n+m];
}

double CLinearSystem::Set(int n, int m, double value)
{
    CheckBounds(n,m);
    double previous = Get(n,m);
    m_pMatrix[m_m*n+m] = value;
    return previous;
}

void CLinearSystem::Set(const double *matrix)
{
    for(int n = 0; n < m_n; n++)
    {
        for(int m = 0; m < m_m; m++)
        {
            Set(n,m,matrix[n*m_m+m]);
        }
    }
}

inline void CLinearSystem::CheckBounds(int n, int m)const
{
    if(n < 0 || n > m_n-1 || m < 0 || m > m_m - 1)
        throw "Array out of bounds";
}

inline void CLinearSystem::CheckLine(int n)
{
    if(n < 0 || n > m_n - 1)
        throw "Line out of bounds";
}

void CLinearSystem::Swap(int n1, int n2)
{
    CheckLine(n1);
    CheckLine(n2);
    for(int m = 0; m < m_m; m++)
    {
        double l2 = Set(n2,m,Get(n1,m));
        Set(n1,m,l2);
    }
}

void CLinearSystem::MultiplyLine(int n, double val)
{
    CheckLine(n);
    for(int m = 0; m < m_m; m++)
    {
        Set(n,m,Get(n,m)*val);
    }
}

double CLinearSystem::NormalizeLine(int n)
{
    CheckLine(n);
    for(int m = 0; m < m_m;m++)
    {
        if(Get(n,m) != 0)
        {
            MultiplyLine(n,1/Get(n,m));
            return Get(n,m);
        }
    }
    return 0;
}

void CLinearSystem::Normalize()
{
    for(int n = 0; n < m_n; n++)
    {
        NormalizeLine(n);
    }
}

void CLinearSystem::Augment()
{
    int* pivot_pos = new int[m_n];
    for(int n = 0; n < m_n; n++)
    {
        for(int m = 0; m < m_m; m++)
        {
            if(Get(n,m) != 0)
            {
                pivot_pos[n]=m;
                break;
            }
        }
    }


    bool swapped = true;
    while(swapped)
    {
        swapped = false;
        for(int n = 0 ; n < m_n-1; n++)
        {
            if(pivot_pos[n] > pivot_pos[n+1])
            {
                Swap(n,n+1);
                swapped = true;

                //we have to swap pivot values too
                int prev = pivot_pos[n];
                pivot_pos[n] = pivot_pos[n+1];
                pivot_pos[n+1] = prev;
            }
        }
    }
    Normalize();
    delete [] pivot_pos;
}


void CLinearSystem::PrettyPrint() const
{
    for(int n = 0; n < m_n; n++)
    {
        for(int m = 0; m < m_m; m++)
        {
            printf("%1.2f ",Get(n,m));
        }
        printf("\n");
    }
    printf("\n");
    fflush(stdout);
}

int CLinearSystem::M() const
{
    return m_m;
}

int CLinearSystem::N() const
{
    return m_n;
}

int CLinearSystem::Reduce()
{
    int operations = 0;
    for(int n = 0; n < m_n; n++)
    {
        NormalizeLine(n);
        for(int m = 0; m < m_m-1; m++)
        {
            if(Get(n,m) == 1)
            {
                for(int n2 = 0; n2 < m_n; n2++)
                {
                    if(n == n2)
                        continue;
                    if(Get(n2,m) != 0)
                    {
                        SubstractLine(n,n2,Get(n2,m));
                    }
                }
            }
        }
    }
    return operations;
}

void CLinearSystem::SubstractLine(int what, int from, double times)
{
    CheckLine(what);
    CheckLine(from);

    for(int m = 0 ; m < m_m; m++)
    {
        Set(from,m,Get(from,m)-times*Get(what,m));
    }
}

double* CLinearSystem::Solve()
{
    printf("system to solve: \n");
    PrettyPrint();
    printf("==== solving ==== \n");
    Normalize();
    Augment();
    Reduce();
    PrettyPrint();
    double* result = new double[m_n];
    for(int n = 0; n < m_n; n++)
    {
        result[n] = Get(n,m_m-1);
    }
    return result;
}
