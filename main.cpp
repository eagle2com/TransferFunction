#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QProcess>

#include "cpoly.h"


int main(int argc, char *argv[])
{
    /*CPoly poly,poly1,poly2;

    poly1.Add(new CToken(1,1));
    poly1.Add(new CToken(1,0));

    poly2.Add(new CToken(1,1));
    poly2.Add(new CToken(1,0));

    poly = *(poly1*poly2);
    poly = *(poly*poly);

    QString latex = poly.ToLatex();
    QString command = "tex2png -c \" $  "+latex+" $\" -T -o equ.png";


    QProcess process;//, QStringList() << docPath
    process.start(command);
    process.waitForFinished();

    process.start("gpicview equ.png");
    process.waitForFinished();



    return 0;*/
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
