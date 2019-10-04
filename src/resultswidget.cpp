#include "resultswidget.h"
#include "ui_resultswidget.h"
#include <QDebug>
#include <stdio.h>
#include <QHeaderView>
#include <QColor>
#include <string>

ResultsWidget::ResultsWidget(QWidget *parent, QMainWindow *wd) :
    QWidget(parent),
    ui(new Ui::ResultsWidget)
{
    mw = wd;
    ui->setupUi(this);

    // Connect to the database
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("thesis");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setPassword("1234");
    bool ok = db.open();
    cout << "DB connection status: " << ok << endl;

    // Set table
    tableWidget = new QTableWidget();
    QHeaderView* header = tableWidget->verticalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setDefaultSectionSize(74);

    QSqlQuery expQuery;
    expQuery.exec("select * from experiments;");

    tableWidget->setRowCount(expQuery.size()+1);
    tableWidget->setColumnCount(7);

    int row = 1;
    //std::vector<QCheckBox*> boxes;
    QTableWidgetItem* protoItemHeader = new QTableWidgetItem();
    protoItemHeader->setTextAlignment(Qt::AlignCenter);
    protoItemHeader->setBackgroundColor(QColor(240, 248, 255));

    QTableWidgetItem* protoItemCell = new QTableWidgetItem();
    protoItemCell->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem* checkExp(protoItemHeader->clone());
    checkExp->setText("Check experiment");
    tableWidget->setItem(0, 0, checkExp);
    QTableWidgetItem* expID(protoItemHeader->clone());
    expID->setText("Exp_ID");
    tableWidget->setItem(0, 1, expID);
    QTableWidgetItem* dataset(protoItemHeader->clone());
    dataset->setText("Dataset");
    tableWidget->setItem(0, 2, dataset);
    QTableWidgetItem* libraries(protoItemHeader->clone());
    libraries->setText("Libraries");
    tableWidget->setItem(0, 3, libraries);
    QTableWidgetItem* methods(protoItemHeader->clone());
    methods->setText("Methods");
    tableWidget->setItem(0, 4, methods);
    QTableWidgetItem* advanced(protoItemHeader->clone());
    advanced->setText("Advanced");
    tableWidget->setItem(0, 5, advanced);
    QTableWidgetItem* results(protoItemHeader->clone());
    results->setText("Results");
    tableWidget->setItem(0, 6, results);

    while(expQuery.next()) {
        QCheckBox* box = new QCheckBox();
        box->setStyleSheet("margin: auto;");
        boxes.push_back(box);
        tableWidget->setCellWidget(row, 0, box);

        QTableWidgetItem* dataset(protoItemCell->clone());
        dataset->setText(expQuery.value(0).toString());
        tableWidget->setItem(row, 1, dataset);
        QTableWidgetItem* newItem(protoItemCell->clone());
        newItem->setText(expQuery.value(1).toString());
        tableWidget->setItem(row, 2, newItem);

        // get Libraries
        QSqlQuery getLibs;
        getLibs.exec("select * from libraries where library_id ="+expQuery.value(2).toString()+";");
        QString libraries;
        while (getLibs.next()) {
            libraries = "Pose: " + getLibs.value(1).toString() + "\n" + "Lba: " + getLibs.value(2).toString() + "\nSim3: " + getLibs.value(3).toString() + "\nGraph: " + getLibs.value(4).toString();
        }
        QTableWidgetItem* newItem1(protoItemCell->clone());
        newItem1->setText(libraries);
        tableWidget->setItem(row, 3, newItem1);

        // get Methods
        QSqlQuery getMethods;
        getMethods.exec("select * from methods where method_id ="+expQuery.value(3).toString()+";");
        QString methods;
        while (getMethods.next()) {
            methods = getMethods.value(1).toString() + "\n" + getMethods.value(2).toString() + "\n" + getMethods.value(3).toString() + "\n" + getMethods.value(4).toString();
        }

        QTableWidgetItem* newItem2(protoItemCell->clone());
        newItem2->setText(methods);
        tableWidget->setItem(row, 4, newItem2);

        QTableWidgetItem* newItem3(protoItemCell->clone());
        newItem3->setText(expQuery.value(4).toString());
        tableWidget->setItem(row, 5, newItem3);

        // get Results

        resultIds.push_back(expQuery.value(5).toString());
        QSqlQuery getResults;
        getResults.exec("select * from results where result_id ="+expQuery.value(5).toString()+";");
        QString results;
        while (getResults.next()) {
            results = "Mean Track: " + getResults.value(2).toString() + "\nMedian Track: " + getResults.value(3).toString();
        }
        QTableWidgetItem* newItem4(protoItemCell->clone());
        newItem4->setText(results);
        tableWidget->setItem(row, 6, newItem4);
        row++;
    }
    ui->horizontalLayout->insertWidget(0, tableWidget);
}

ResultsWidget::~ResultsWidget()
{
    delete ui;
    db.close();
    QSqlDatabase::removeDatabase("connection");
}

void ResultsWidget::on_pushButton_clicked()
{
    this->hide();
    mw->show();
}

void ResultsWidget::on_pushButton_2_clicked()
{

    int picked=0;
    QStringList trajectoryFiles;
    cout << "1) HERE" << endl;
    for(unsigned long i = 0; i < boxes.size() ; i++) {
        cout << "2) HERE" << boxes[i]->isChecked() << endl;
        if (boxes[i]->isChecked()) {
            picked = i+1;
            trajectoryFiles << "experiments/KeyFrameTrajectory"+resultIds[i]+".txt";
            qDebug() << "experiments/KeyFrameTrajectory"+resultIds[i]+".txt";
        }
    }

    cout << "3) HERE" << endl;
    QStringList arguments;
    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    cout << "3) HERE" << endl;
    if (!tableWidget->item(picked, 2)->text().toStdString().compare("mono_tum")) {
        arguments << "tum";
        for ( const auto& i : trajectoryFiles  )
        {
            arguments << i;
        }
        arguments << "--ref"
                  << "/home/borisqa/Documents/orb-compiled/rgbd_dataset_freiburg1_desk/groundtruth.txt"
                  << "--align_origin"
                  << "--plot";

        for ( const auto& i : arguments  )
        {
            qDebug() << i;
        }
        process->start("evo_traj", arguments);
    } else if (!tableWidget->item(picked, 2)->text().toStdString().compare("mono_euroc")) {
        arguments << "euroc";
        for ( const auto& i : trajectoryFiles  )
        {
            arguments << i;
        }
        arguments << "--ref"
                  << "/home/borisqa/Documents/datasets/mav0/state_groundtruth_estimate0/data.csv"
                  << "--align_origin"
                  << "--plot";

        for ( const auto& i : arguments  )
        {
            qDebug() << i;
        }
        process->start("evo_traj", arguments);
    } else if (!tableWidget->item(picked, 2)->text().toStdString().compare("mono_kitti")) {
        arguments << "kitti";
        for ( const auto& i : trajectoryFiles  )
        {
            arguments << i;
        }
        arguments << "--ref"
                  << "/home/borisqa/Documents/datasets/dataset/poses/01.txt"
                  << "--align_origin"
                  << "--plot";

        for ( const auto& i : arguments  )
        {
            qDebug() << i;
        }
        process->start("evo_traj", arguments);
    }

    cout << "4) HERE" << endl;

    //process->startDetached(fullPath);
    cout << "Process PID: " << process->pid() << endl;
    if (!process->waitForFinished(180000))
        qDebug() << "Orb-Slam Failed: " << process->errorString();
    else {
        qDebug() << "Orb-slam output: " << process->readAll();
    }
}

void ResultsWidget::on_pushButton_3_clicked()
{
    QStringList trajectoryFiles;
    QString experimentID=0;
    int picked = 0;
    for(unsigned long i = 0; i < boxes.size() ; i++) {
        if (boxes[i]->isChecked()) {
            picked = i+1;
            trajectoryFiles << "experiments/KeyFrameTrajectory"+resultIds[i]+".txt";
            qDebug() << "RES NUMBER" << std::stoi(resultIds[i].toUtf8().constData());
            experimentID = tableWidget->item(std::stoi(resultIds[i].toUtf8().constData()), 1)->text();
            qDebug() << experimentID;
        }
    }

    QStringList arguments;
    arguments << "tum" << "/home/borisqa/Documents/orb-compiled/rgbd_dataset_freiburg1_desk/groundtruth.txt";
    for ( const auto& i : trajectoryFiles  )
    {
        arguments << i;
    }
    arguments << "-v"
              << "--align_origin"
              << "--plot"
              << "--save_results"
              << "results/ape_for_"+experimentID+".zip";

    if (remove("results/ape_for_"+experimentID.toUtf8()+".zip") == 0) {
        cout << "deleted ape file";
    }

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start("evo_ape", arguments);
    //process->startDetached(fullPath);
    cout << "Process PID: " << process->pid() << endl;
    if (!process->waitForFinished(180000))
        qDebug() << "Orb-Slam Failed: " << process->errorString();
    else {
        qDebug() << "Orb-slam output: " << process->readAll();
    }
}

void ResultsWidget::on_pushButton_4_clicked()
{
    QStringList trajectoryFiles;
    for(unsigned long i = 0; i < boxes.size() ; i++) {
        if (boxes[i]->isChecked()) {
            trajectoryFiles << "results/ape_for_"+resultIds[i]+".zip";
        }
    }

    QStringList arguments;
    for ( const auto& i : trajectoryFiles  )
    {
        arguments << i;
    }

    arguments << "-p"
              << "--save_table"
              << "results/table.csv";
    if (remove("results/table.csv") == 0) {
        cout << "deleted file" << endl;
    }

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start("evo_res", arguments);
    //process->startDetached(fullPath);
    cout << "Process PID: " << process->pid() << endl;
    if (!process->waitForFinished(180000))
        qDebug() << "Orb-Slam Failed: " << process->errorString();
    else {
        qDebug() << "Orb-slam output: " << process->readAll();
    }
}
