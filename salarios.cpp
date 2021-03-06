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
    m_controlador->m_totalIESS=0;
    m_controlador->m_totalNeto=0;
    m_controlador->m_totalbruto=0;
}


void Salarios::on_cmdCalcular_clicked()
{
    calcular();
    ui->outSalarioBruto->setText(QString::number(m_controlador->m_totalbruto, 'f',2));
    ui->outIESS->setText(QString::number(m_controlador->m_totalIESS));
    ui->outTotal->setText(QString::number(m_controlador->m_totalNeto, 'f',2));
}

void Salarios::limpiar()
{
    ui->inNombre->setText("");
    ui->inHoras->setValue(0);
    ui->inMatutino->setChecked(true);
    ui->inNombre->setFocus();
    ui->outIESS->setText("0.00");
    ui->outSalarioBruto->setText("0.00");
    ui->outTotal->setText("0.00");



}

void Salarios::guardar()
{
    // Abrir cuadro de diálogo para seleccionar ubicación y nombre del archivo.
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar archivo",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de salarios (*.slr)");

    // Crear un objeto QFile
    QFile archivo(nombreArchivo);
    // Abrirlo para escritura
    if(archivo.open(QFile::WriteOnly | QFile::Truncate)){
        // Crear un 'stream' de texto
        QTextStream salida(&archivo);
        // Enviar los datos del resultado a la salida
        salida << ui->outResultado->toPlainText();
        salida.operator<<("----------------------");
        salida.operator<<("\nTOTAL:\n");
        salida.operator<<("Salario Bruto: "+QString::number(m_controlador->m_totalbruto)+"\n");
        salida.operator<<("Descuento TOTAL: "+QString::number(m_controlador->m_totalIESS)+"\n");
        salida.operator<<("Salario Neto: "+QString::number(m_controlador->m_totalNeto)+"\n");
        salida.operator<<("----------------------");
        // Mostrar 5 segundo que todo fue bien
        ui->statusbar->showMessage("Datos almacenados en " + nombreArchivo, 5000);
    }else {
        // Mensaje de error si no se puede abrir el archivo
        QMessageBox::warning(this,
                             "Guardar datos",
                             "No se pudo guardar el archivo");
    }
    // Cerrar el archivo
    archivo.close();

}

void Salarios::abrir()
{
    // Abrir cuadro de diálogo para seleccionar ubicación y nombre del archivo.
    QString nombreArchivo = QFileDialog::getOpenFileName(this,
                                                         "Abrir archivo",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de salarios (*.slr)");

    // Crear un objeto QFile
    QFile archivo(nombreArchivo);
    // Abrirlo para lectura
    if(archivo.open(QFile::ReadOnly)){
        // Crear un 'stream' de texto
        QTextStream entrada(&archivo);
        // Cargar el contenido al área de texto

        QString datos="",linea="";

        while(entrada.atEnd()==false && linea!="----------------------"){
            linea=entrada.readLine();
            datos+=linea + "\n";

        }
        entrada.readLine();

        linea=entrada.readLine();
        linea.remove(0,15);
        ui->outSalarioBruto->setText(linea);
        m_controlador->m_totalbruto = ui->outSalarioBruto->text().toDouble();

        linea=entrada.readLine();
        linea.remove(0,17);
        ui->outIESS->setText(linea);
        m_controlador->m_totalIESS = ui->outIESS->text().toDouble();

        linea=entrada.readLine();
        linea.remove(0,14);
        ui->outTotal->setText(linea);
        m_controlador->m_totalNeto = ui->outTotal->text().toDouble();




        //Cargar el contenido al area de texto
        ui->outResultado->clear();
        ui->outResultado->setPlainText(datos);
        // Mostrar 5 segundo que todo fue bien
        ui->statusbar->showMessage("Datos leidos desde " + nombreArchivo, 5000);

    }else {
        // Mensaje de error si no se puede abrir el archivo
        QMessageBox::warning(this,
                             "Abrir datos",
                             "No se pudo abrir el archivo");
    }
    // Cerrar el archivo
    archivo.close();
}


void Salarios::on_actionCalcular_triggered()
{
    calcular();
}

void Salarios::calcular()
{
    // Obteber datos de la GUI
    QString nombre = ui->inNombre->text();
    int horas = ui->inHoras->value();
    TipoJornada jornada;
    if (ui->inMatutino->isChecked()){
        jornada = TipoJornada::Matutina;
    } else if (ui->inVespertina->isChecked()){
        jornada = TipoJornada::Vespertina;
    } else {
        jornada = TipoJornada::Nocturna;
    }

    // Validar datos correctos
    if (nombre == "" || horas == 0){
        /*
        QMessageBox msgBox;
        msgBox.setText("El nombre o el número de horas está vacío");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        */
        QMessageBox::warning(this,"Advertencia","El nombre o el número de horas está vacío");
        return;
    }

    // Agregar obrero al controlador
    m_controlador->agregarObrero(nombre, horas, jornada);
    // Calcular
    if (m_controlador->calcularSalario()){
        // muestra los resultados de los calculos del obrero
        ui->outResultado->appendPlainText(m_controlador->obrero()->toString());
        // limpiar la interfaz
        limpiar();
        // Mostrar mensaje por 5 segundos en la barra de estado
        ui->statusbar->showMessage("Calculos procesados para " + nombre, 5000);
    }else {
        QMessageBox::critical(
                    this,
                    "Error",
                    "Error al calcular el salario.");
    }
}


void Salarios::on_actionGuardar_triggered()
{
    guardar();
}


void Salarios::on_actionNuevo_triggered()
{
    limpiar();
    ui->outResultado->clear();

}


void Salarios::on_actionAbrir_triggered()
{
    abrir();
}


void Salarios::on_actionAcerca_de_Salarios_triggered()
{



}

void Salarios::on_actioAcerca_de_triggered()
{
    // Crear un objeto de la ventana que queremos invocar
    Acerca *dialogo = new Acerca(this);
    // Enviar parámetro a la ventana
    dialogo->setVersion(VERSION);
    // Mostrar la ventana (diálogo) MODAL
    dialogo->exec();
    // Obtener datos de la ventana
    qDebug() << dialogo->valor();
}

