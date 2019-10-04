#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QCheckBox>
#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include "advancedconfigceres.h"
#include "advancedconfigg2o.h"
#include <nlohmann/json.hpp>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "resultswidget.h"

using json = nlohmann::json;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QHBoxLayout* l1;
    //json gatherDataToJSON(std::vector<std::string> methods);
    //void checkAdvancedProcessor(int arg, std::string method);

private slots:
    void on_pushButton_clicked();

    void on_checkPoseAdvanced_stateChanged(int arg1);

    void on_checkLbaAdvanced_stateChanged(int arg1);

    void on_checkGraphAdvanced_stateChanged(int arg1);

    void on_checkSim3Advanced_stateChanged(int arg1);

    void on_pushButton_2_clicked();

private:
    QSqlDatabase db;
    Ui::MainWindow *ui;
    QComboBox *ch1;
    QCheckBox *cb1;
    AdvancedConfigG2O* poseConfG2O;
    AdvancedConfigG2O* lbaConfG2O;
    AdvancedConfigG2O* graphConfG2O;
    AdvancedConfigG2O* sim3ConfG2O;
    AdvancedConfigCeres* poseConfCeres;
    AdvancedConfigCeres* lbaConfCeres;
    AdvancedConfigCeres* graphConfCeres;
    AdvancedConfigCeres* sim3ConfCeres;

    ResultsWidget *resultWindow;
};

#endif // MAINWINDOW_H
