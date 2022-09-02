#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mymodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_load_action_JSON_triggered();

    void on_save_action_JSON_triggered();

    void on_addButton_clicked();

    void on_changeButton_clicked();

    void on_removeButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    MyModel *model;
};

#endif // MAINWINDOW_H
