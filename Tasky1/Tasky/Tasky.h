#ifndef TASKY_H
#define TASKY_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <tarea.h>
#include <QMessageBox>


QT_BEGIN_NAMESPACE //macros, es un codigo de c++, va a ser reemplazado por sentencias
namespace Ui { class Tasky; }//
QT_END_NAMESPACE

class Tasky : public QMainWindow
{
    Q_OBJECT //dentro del framework, debe tener algunas directrices.

public:
    Tasky(QWidget *parent = nullptr); //modificador de accedo, constructor parametrizado, podria tener una ventana padre
    ~Tasky(); // destructor

private slots:
    void on_pushAgregar_clicked();

    void on_tblTareas_cellDoubleClicked(int row, int column);

private:
    Ui::Tasky *ui; //puntero, donde se guardan todos los controles de la interfaz grafica

    void agregarTarea(Tarea *t);
    void limpiar();
    void guardar();
    void cargar();
    void eliminar(int fila);
    bool colorTarea(const Tarea *t);
    const QString ARCHIVO = "tarea.csv";
    QList<Tarea*> m_tareas;
    enum Columna
    {
        TAREA, ASIGNATURA, FECHA, HORA
    };

};
#endif // TASKY_H
