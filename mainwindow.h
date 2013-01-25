#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static void Debug(QString msg)
    {
        instance->dbg(msg);
    }

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void dbg(QString msg);

private slots:
    void on_action_new_Transfer_Function_triggered();

private:
    Ui::MainWindow *ui;
    static MainWindow* instance;
};



#endif // MAINWINDOW_H
