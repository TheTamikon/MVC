#include "mymodel.h"
#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLinkedList>

MyModel::MyModel(QObject *parent): QAbstractTableModel (parent) {}
int MyModel::rowCount(const QModelIndex &index) const
{
    return _students.size();
}

int MyModel::columnCount(const QModelIndex &index) const
{
    return cCount;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    QLinkedList<Student>::const_iterator iterD = _students.begin();
    for (int i=0;i<index.row();i++) {
        iterD++;
    }
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole)
    {
        switch (index.column()) {
            case cID:
                return iterD->id;
            case cFIO:
                return iterD->fio;
        }
    }
    if (role == Qt::ToolTipRole){
        switch (index.column()){
        case cID:
            return "Student #" + QString::number(iterD->id);
        }
    }
    if (role == Qt::BackgroundRole){
        if(index.row() %2 == 0){
            return QColor("#6699ff");
        }
        else return QColor("#99ccff");
    }
    return QVariant();
}

void MyModel::addStudent(int id, const QString &fio)
{

   beginInsertRows(QModelIndex(),_students.size(),_students.size());
   Student s;
   s.id = id;
   s.fio = fio;
   _students.append(s);
   endInsertRows();
}

void MyModel::removeStudent(int row)
{
    beginRemoveRows(QModelIndex(),row,row);
    iter = _students.begin();
    for (int i=0;i<row;i++) {
        iter++;
    }
    _students.erase(iter);
    endRemoveRows();
}

void MyModel::updateStudent(int row, int id, const QString &fio)
{
    Student s;
    s.id = id;
    s.fio = fio;
    iter = _students.begin();
    for (int i=0;i<row;i++) {
        iter++;
    }
    *iter=s;
    emit dataChanged(index(row,cID),index(row,cFIO));
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch (section){
            case cID:
                return "Номер в списке";
            case cFIO:
                return "Фамилия и инициалы";
        }
    }
    return QAbstractTableModel::headerData(section,orientation,role);
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    iter = _students.begin();
    for (int i=0;i<index.row();i++) {
        iter++;
    }
    switch(index.column()){
    case cID:
       iter->id = value.toInt();
        break;
    case cFIO:
       iter->fio = value.toString();
        break;
    }
    emit dataChanged(index,index);
    return true;
}

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QString MyModel::serializeJson()
{
    QJsonDocument doc;
    QJsonObject root;
    root["group"] = "BSBO-01-19";
    QJsonArray jsonStudents;
    for (Student s: _students){
        QJsonObject jsonStudent;
        jsonStudent["id"] = s.id;
        jsonStudent["fio"] = s.fio;
        jsonStudents.append(jsonStudent);
    }
    root["students"]=jsonStudents;
    doc.setObject(root);
    return doc.toJson();
}

void MyModel::deserializeJson(const QString &json, QString *error)
{
    beginResetModel();
    _students.clear();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(),&jsonError);
    if(jsonError.error!=QJsonParseError::NoError){
        *error = jsonError.errorString();
        return;
    }
    QJsonObject root = doc.object();
    QJsonArray jsonStudents = root["students"].toArray();
    for(auto jStudent: jsonStudents)
    {
        Student s;
        s.id = jStudent.toObject()["id"].toInt();
        s.fio = jStudent.toObject()["fio"].toString();
        _students.append(s);
    }
    endResetModel();
}
