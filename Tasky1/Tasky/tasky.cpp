#include "Tasky.h"
#include "ui_Tasky.h"



Tasky::Tasky(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Tasky)
{
    ui->setupUi(this);
    limpiar();


    //CONFIGURAR LA TABLA
    ui->tblTareas->setColumnCount(4);
    QStringList cabecera;
    cabecera<<"Tarea"<<"Asignatura"<<"Fecha"<<"Hora";
    ui->tblTareas->setHorizontalHeaderLabels(cabecera);
    cargar();

}

Tasky::~Tasky() //destructor por defecto
{
    delete ui;
}

void Tasky::on_pushAgregar_clicked()
{

    QString nombre = ui->txtTarea->text(); //todo esta en mi objeto ui y despues llamamos al cuadro de texto
    //Validar datos de la tarea
    if (nombre.length()==0){
        QMessageBox::warning(this, "Agregar tarea", "El espacio de tarea no puede estar vacio");
        ui->txtTarea->setFocus();
        return;
    }


    QString asignatura = ui->cmbAsignatura->currentText();
    QDate fecha = ui->dateFecha->date();
    QTime hora = ui->timeHora->time();

    //m_tareas.append(new Tarea(nombre, asignatura, fecha, hora));
    Tarea *t=  new Tarea(nombre, asignatura, fecha, hora);
    //agregar a la tarea
    agregarTarea(t);
    limpiar();
    guardar();
}
void Tasky::agregarTarea(Tarea *t)
{
    //agregar a la lista
    m_tareas.append(t);
    //Agregar a la tabla
    int fila =ui->tblTareas->rowCount();

    // Va insertando una nueva fila
        ui->tblTareas->insertRow(fila);

    // guarda los datos en las celdas de cada columna creada (fila)

    ui->tblTareas->setItem(fila, TAREA, new QTableWidgetItem(t->nombre()));
    ui->tblTareas->setItem(fila, ASIGNATURA, new QTableWidgetItem(t->asignatura()));
    ui->tblTareas->setItem(fila, FECHA, new QTableWidgetItem(t->fecha().toString("dd/MM/yyyy")));
    ui->tblTareas->setItem(fila, HORA, new QTableWidgetItem(t->hora().toString("hh:mm")));

    // Verificador para cambiar el color
    if(colorTarea(t))
    {
        QColor colorFondo(255, 153, 153);
        for (int i = 0; i < ui->tblTareas->columnCount(); ++i)
        {
            QTableWidgetItem *item = ui->tblTareas->item(fila, i);
            if (item)
            {
                item->setBackground(colorFondo);
            }
        }
    }
}



void Tasky::limpiar()
{
    QDate hoy=QDate::currentDate();
    QTime ahora=QTime::currentTime();
    ui->dateFecha->setMinimumDate(hoy);
    ui->dateFecha->setDate(hoy);
    ui->timeHora->setTime(ahora);
    //limpiar campos
    ui->txtTarea->clear();
    ui->cmbAsignatura->setCurrentIndex(0);
    //setear el foco al campo de tarea
    ui->txtTarea->setFocus();

}

void Tasky::guardar()
{

    // Abrir el archivo y guardar
    QFile archivo(ARCHIVO);
    if (archivo.open(QFile::WriteOnly | QFile::Truncate)) { //truncar, quiere decir que no se puede abrir el archivo desde otro terminal
        QTextStream salida(&archivo);

        Tarea *t;
        foreach(t, m_tareas){
            salida<<t->nombre()<<";"<<t->asignatura()<<";";
            salida<<t->fecha().toString("dd/MM/yyyy")<<";";
            salida<<t->hora().toString("hh:mm")<<"\n";

        }

        archivo.close();
        //QMessageBox::information(this,"Guardar contactos","Contactos guardados con éxito");
    }else{
        QMessageBox::critical(this,"Guardar contactos", "No se puede escribir sobre " + ARCHIVO);
    }
};
void Tasky::eliminar(int fila)
{
    if(fila >= 0 && fila < ui->tblTareas->rowCount())
    {
        // Elimina la tarea de la lista
        Tarea *t = m_tareas.takeAt(fila);
        delete t;

        // Elimina la tarea de la tabla
        ui->tblTareas->removeRow(fila);
        guardar();
    }
}
void Tasky::on_tblTareas_cellDoubleClicked(int row, int column)
{
    if(column == TAREA)
    {
        QMessageBox::StandardButton respuesta = QMessageBox::question(this, "Eliminar tarea", "Desea eliminar esta tarea?", QMessageBox::Yes | QMessageBox::No);
        if (respuesta == QMessageBox::Yes)
        {
            eliminar(row);
        }
    }
}
bool Tasky::colorTarea(const Tarea *t)
{
    // Para la fecha actual
    QDate fechaActual = QDate::currentDate();
    // Para la fecha limite
    QDate fechaLimite = t->fecha();
    return (fechaLimite >= fechaActual && fechaLimite <= fechaActual.addDays(1));
}
void Tasky::cargar()
{


    QFile archivo(ARCHIVO);
        if (!archivo.exists())
            return;

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream entrada(&archivo);

        while (!entrada.atEnd())
        {
            //Leer una linea del archivo
            QString linea = entrada.readLine();
            //Seprar los campor por ";"
            QStringList datos = linea.split(";");
            qDebug()<<datos;
            //Definir el nombre y la asignatura
            QString nombre=datos[0];
            QString asignatura=datos[1];
            //Obtener la fecha
            QStringList fecha=datos[2].split("/");
            QDate f(fecha[2].toInt(), fecha[1].toInt(), fecha[0].toInt());
            //Obtener la hora
            QStringList hora=datos[3].split(":");
            QTime h(hora[0].toInt(), hora[1].toInt());
            //Crear tarea
            Tarea *t=new Tarea(nombre, asignatura, f, h);
            agregarTarea(t);



        }

        archivo.close();
    }
    else
    {
        QMessageBox::critical(this, "Cargar tareas", "No se puede leer desde " + ARCHIVO);
    }
}
