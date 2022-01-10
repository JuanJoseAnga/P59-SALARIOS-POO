#include "salarios.h"
#include "ui_salarios.h"

Salarios::Salarios(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Salarios)
{
    ui->setupUi(this);
    m_controlador = new Controlador();
}

Salarios::~Salarios()
{
    delete ui;
}

void Salarios::on_cmdcalcular_clicked()
{
    QString nombre = ui->inNombre->text();
    int horas = ui->inHoras->value();
    TipoJornada jornada;
    if(ui->inmatutina->isChecked()){
        jornada = TipoJornada::Matutina;
    }else if(ui->inVespertina->isChecked()){
        jornada = TipoJornada::Vespertina;
    }else {
        jornada = TipoJornada::Nocturna;
    )
    //agregar obrero al controlador
            m_controlador->agregarObrero(nombre,horas,jornada);
    //Calcular
    if (m_controlador->calcularSalario()){
        ui->outResultado->appendPlainText((m_controlador->obrero()->toString()));
    }
}

