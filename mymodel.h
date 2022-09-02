#ifndef MYMODEL_H
#define MYMODEL_H
#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QLinkedList>

struct Student{
    int id;
    QString fio;
};

class MyModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Colums{
        cID,
        cFIO,
        cCount
    };
    MyModel(QObject * parent = nullptr);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    void addStudent(int id, const QString &fio);
    void removeStudent(int row);
    void updateStudent(int row, int id, const QString &fio);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QString serializeJson();
    void deserializeJson(const QString &json, QString *error);
private:
    QLinkedList<Student> _students;
    QLinkedList<Student>::iterator iter;
};

#endif // MYMODEL_H
