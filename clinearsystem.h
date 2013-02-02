#ifndef CLINEARSYSTEM_H
#define CLINEARSYSTEM_H

class CLinearSystem
{
public:
    CLinearSystem(int n, int m);    //number of lines / columns
    ~CLinearSystem();

    double Get(int n, int m) const;
    double Set(int n, int m, double value);
    void Set(const double *matrix);
    void Swap(int n1, int n2);
    void Augment();
    double NormalizeLine(int n);   //normalizes the pivot value of the line, returns multiplier
    void Normalize();
    void PrettyPrint() const;
    void MultiplyLine(int n,double val);
    int Reduce(); //returns the number of operations needed
    void SubstractLine(int what, int from, double times);

    int M() const;
    int N() const;

    double* Solve();

private:
    double *m_pMatrix;
    int m_n, m_m;
    inline void CheckBounds(int n, int m) const;
    inline void CheckLine(int n);
};

#endif // CLINEARSYSTEM_H
