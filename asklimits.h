#ifndef ASKLIMITS_H
#define ASKLIMITS_H

#include <QDialog>
#include "Canvas/wbutton.h"
#include "mainwindow.h"

namespace Ui {
class AskLimits;
}

class AskLimits : public QDialog
{
    Q_OBJECT

public:
    //explicit AskLimits(QWidget *parent = 0);
    explicit AskLimits(QWidget *parent,MainWindow* frmApp,QString FlName);
    ~AskLimits();

private slots:
    void on_buttonBox_accepted();

public:
    MainWindow* App;
    QString Name;

private:
    Ui::AskLimits *ui;
    ArrayRange* II;
    ArrayRange* JJ;
};

#endif // ASKLIMITS_H
