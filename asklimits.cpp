#include "asklimits.h"
#include "ui_asklimits.h"

AskLimits::AskLimits(QWidget *parent,ArrayRange* I,ArrayRange* J) : QDialog(parent), ui(new Ui::AskLimits){
    ui->setupUi(this);
    this->setWindowTitle("Limites de la Matriz Irregular");

    this->II = I;
    this->JJ = J;
}

AskLimits::~AskLimits()
{

    delete ui;
}

void AskLimits::on_buttonBox_accepted()
{
    II->Low = ui->spinBox_I_Desde->value();
    II->Sup = ui->spinBox_I_Hasta->value();

    JJ->Low = ui->spinBox_J_Desde->value();
    JJ->Sup = ui->spinBox_J_Hasta->value();
}
