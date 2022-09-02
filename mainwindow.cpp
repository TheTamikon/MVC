#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mymodel.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QIntValidator(ui->lineEdit));
    model = new MyModel(this);
    model->addStudent(1, "Чехов А.П.");
    model->addStudent(2, "Пушкин А.С.");
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_load_action_JSON_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(this,"select file");
    if(fileName.isEmpty())
    {
        QMessageBox::critical(this,"error","File is empty");
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this,"error","File not open");
        return;
    }
    QByteArray data = file.readAll();
    QString error;
    model->deserializeJson(data, &error);
    if(!error.isEmpty())
            QMessageBox::critical(this,"error",error);
    file.close();
}

void MainWindow::on_save_action_JSON_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,"select file");
    if(fileName.isEmpty())
    {
        QMessageBox::critical(this,"error","File is empty");
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this,"error","File not open");
        return;
    }
    QString json = model->serializeJson();
    file.write(json.toUtf8());
    file.close();
}

void MainWindow::on_addButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty())
    {
        QString id = ui->lineEdit->text();
        QString fio = ui->lineEdit_2->text();
        model->addStudent(id.toInt(), fio);
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
}

void MainWindow::on_changeButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty())
    {
        QString id = ui->lineEdit->text();
        QString fio = ui->lineEdit_2->text();
        model->updateStudent(ui->tableView->currentIndex().row(),id.toInt(), fio);
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
}

void MainWindow::on_removeButton_clicked()
{
    model->removeStudent(ui->tableView->currentIndex().row());
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    qDebug()<<"s";
}
