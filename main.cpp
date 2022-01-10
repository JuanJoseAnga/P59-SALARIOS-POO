#include "salarios.h"
#include "obrero.h"
#include "controlador.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Salarios w;
    w.show();

    Obrero *obj = new Obrero();
    obj->agregarObrero("Juan",40,TipoJornada::Matutina);

    if(obj->calcularSalario)



    return a.exec();
}
