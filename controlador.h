#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <QObject>

#include "obrero.h"

#define MATUTINO 1.66
#define VESPERTINO 1.80
#define NOCTURNO 2.07

#define HORA_EXTRA 50
#define IESS 9.5

class Controlador : public QObject
{
    Q_OBJECT
public:
    explicit Controlador(QObject *parent = nullptr);
    bool agregarObrero(QString nombre, int horas, TipoJornada jornada);
    bool calcularSalario();
    Obrero *obrero() const;
signals:

private:
    Obrero *m_obrero;;



};

#endif // CONTROLADOR_H
