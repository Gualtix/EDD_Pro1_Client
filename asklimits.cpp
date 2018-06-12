#include "asklimits.h"
#include "ui_asklimits.h"

AskLimits::AskLimits(QWidget *parent,MainWindow* frmApp,QString FlName) : QDialog(parent), ui(new Ui::AskLimits){
    ui->setupUi(this);
    this->setWindowTitle("Limites de la Matriz Irregular");

    this->II = new ArrayRange();
    this->JJ = new ArrayRange();

    this->App = frmApp;
    this->Name = FlName;
}

AskLimits::~AskLimits()
{

    delete ui;
}

void AskLimits::on_buttonBox_accepted()
{
    int Pb;

    int I_Desde = ui->spinBox_I_Desde->value();
    int I_Hasta = ui->spinBox_I_Hasta->value();

    int J_Desde = ui->spinBox_J_Desde->value();
    int J_Hasta = ui->spinBox_J_Hasta->value();

    if(I_Desde > I_Hasta){
        Pb = I_Hasta;
        I_Hasta = I_Desde;
        I_Desde = Pb;
    }

    if(I_Desde > I_Hasta){
        Pb = J_Hasta;
        J_Hasta = J_Desde;
        J_Desde = Pb;
    }

    //II->Low = ui->spinBox_I_Desde->value();
    //II->Sup = ui->spinBox_I_Hasta->value();

    //JJ->Low = ui->spinBox_J_Desde->value();
    //JJ->Sup = ui->spinBox_J_Hasta->value();

    II->Low = I_Desde;
    II->Sup = I_Hasta;

    JJ->Low = J_Desde;
    JJ->Sup = J_Hasta;

    II->SetSize();
    JJ->SetSize();

    this->App->Load_Canvas(II,JJ,true);

}
