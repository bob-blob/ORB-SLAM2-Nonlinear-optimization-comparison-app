#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>
//#include "mainwindow.h"
#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <iostream>
#include <QTableWidget>
#include <vector>
#include <QCheckBox>
#include <QProcess>

using std::cout;
using std::endl;

namespace Ui {
class ResultsWidget;
}

class ResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget *parent = nullptr, QMainWindow* wd = nullptr);
    ~ResultsWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ResultsWidget *ui;
    QMainWindow* mw;
    QSqlDatabase db;
    QTableWidget* tableWidget;
    std::vector<QCheckBox*> boxes;
    std::vector<QString> resultIds;
};

#endif // RESULTSWIDGET_H
