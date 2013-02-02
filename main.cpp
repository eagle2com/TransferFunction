#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>

#include "cpoly.h"

#include "clinearsystem.h"


int main(int argc, char *argv[])
{

    CLinearSystem sys1(2,3);
    double matrix[] = {2,1,3,
                       1,1,1};
    sys1.Set(matrix);
    double* result = sys1.Solve();

    printf("result: \n");
    for(int i = 0 ; i < sys1.N(); i++)
    {
        printf("%1.2f ",result[i]);
    }
    printf("\n");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
