#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>
#include <iomanip>
#include <map>
#include <iostream>
#include <QProcess>
#include <QDebug>


using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("thesis");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setPassword("1234");
    bool ok = db.open();
    cout << "DB connection status: " << ok << endl;

   // QSqlDatabase test1 = QSqlDatabase::database("thesis");
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
    QSqlDatabase::removeDatabase("connection");
}

bool copyFile(const char *SRC, const char* DEST)
{
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

std::string getText(QComboBox* box)
{
    return box->currentText().toUtf8().constData();
}

int getAdvIndex(json config) {

    // check if this config is already in database
    QString minimizer_type = QString::fromStdString(config["minimizer_type"].get<std::string>());
    QString trust_region_strategy_type("is NULL");
    QString dogleg_type("is NULL");
    QString initial_trust_region_radius("is NULL");
    QString line_search_direction_type("is NULL");
    QString line_search_type("is NULL");
    QString nonlinear_conjugate_gradient_type("is NULL");
    QString sNULL = "NULL";
    bool trust = false;
    bool dogleg = false;
    bool line = false;
    cout << "start config parsing! " << endl;
    if (!config["minimizer_type"].get<std::string>().compare("TRUST_REGION")) {
        trust = true;
        trust_region_strategy_type = QString::fromStdString("= '"+config["trust_region_strategy_type"].get<std::string>()+"'");
        if (!config["trust_region_strategy_type"].get<std::string>().compare("DOGLEG")) {
            dogleg = true;
            dogleg_type = QString::fromStdString("= '"+config["dogleg_type"].get<std::string>()+"'");
        }
        initial_trust_region_radius = QString::fromStdString("= "+config["initial_trust_region_radius"].get<std::string>());
    } else {
        line = true;
        line_search_direction_type = QString::fromStdString("= '"+config["line_search_direction_type"].get<std::string>()+"'");
        line_search_type = QString::fromStdString("= '"+config["line_search_type"].get<std::string>()+"'");
        nonlinear_conjugate_gradient_type = QString::fromStdString("= '"+config["nonlinear_conjugate_gradient_type"].get<std::string>()+"'");
    }
    QString gradient_tolerance = QString::fromStdString(config["gradient_tolerance"].get<std::string>());
    QString function_tolerance = QString::fromStdString(config["function_tolerance"].get<std::string>());
    QString linear_solver_type = QString::fromStdString(config["linear_solver_type"].get<std::string>());
    QString max_num_iterations = QString::number(config["max_num_iterations"].get<int>());
    QString minimizer_progress_to_stdout = config["minimizer_progress_to_stdout"].get<bool>() ? "TRUE" : "FALSE";
    QString num_threads = QString::number(config["num_threads"].get<int>());
    QString eta = QString::fromStdString(config["eta"].get<std::string>());
    QString max_solver_time_in_seconds = QString::fromStdString(config["max_solver_time_in_seconds"].get<std::string>());
    QString use_nonmonotonic_steps = config["use_nonmonotonic_steps"].get<bool>() ? "TRUE" : "FALSE";

    cout << config["use_nonmonotonic_steps"].get<bool>() << " " << use_nonmonotonic_steps.toUtf8().constData() << endl;

    QSqlQuery query;
    query.exec("SELECT ceres_adv_id FROM advanced_ceres WHERE minimizer_type = '"+ minimizer_type +"' and "
               "trust_region_type "+trust_region_strategy_type+" and "
               "dogleg_type "+dogleg_type+" and "
               "initial_trust_region_radius "+initial_trust_region_radius+" and "
               "line_search_direction_type "+line_search_direction_type+" and "
               "line_search_type "+line_search_type+" and "
               "nonlinear_conjugate_gradient_type "+nonlinear_conjugate_gradient_type+" and "
               "gradient_tolerance ="+gradient_tolerance+" and "
               "function_tolerance ="+function_tolerance+" and "
               "linear_solver_type ='"+linear_solver_type+"' and "
               "max_num_iterations ="+max_num_iterations+" and "
               "minimizer_progress_to_stdout ="+minimizer_progress_to_stdout+" and "
               "num_threads ="+num_threads+" and "
               "eta ="+eta+" and "
               "max_solver_time_in_seconds ="+max_solver_time_in_seconds+" and "
               "use_nonmonotonic_steps ="+use_nonmonotonic_steps+";");

    // If query returned already exists
    if (query.size()) {
        cout << "QUERY EXISTS - RETURNING THE OLD INDEX ))))" << endl;
        while (query.next()) {
            return query.value(0).toInt();
        }
    } else {
        // Else we need to insert the new config and return index
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO advanced_ceres (minimizer_type, trust_region_type, dogleg_type, initial_trust_region_radius, line_search_direction_type, line_search_type, nonlinear_conjugate_gradient_type,"
                            "gradient_tolerance, function_tolerance, linear_solver_type, max_num_iterations, minimizer_progress_to_stdout, num_threads, eta, max_solver_time_in_seconds, use_nonmonotonic_steps) VALUES "
                            "(:minimizer_type, :trust_region_type, :dogleg_type, :initial_trust_region_radius, :line_search_direction_type,"
                            " :line_search_type, :nonlinear_conjugate_gradient_type, :gradient_tolerance, :function_tolerance, :linear_solver_type, :max_num_iterations, :minimizer_progress_to_stdout, :num_threads, :eta, :max_solver_time_in_seconds, :use_nonmonotonic_steps) returning ceres_adv_id;");
        cout << "A"<< endl;

        insertQuery.bindValue(":minimizer_type", minimizer_type);
        insertQuery.bindValue(":trust_region_type", trust ? QString::fromStdString(config["trust_region_strategy_type"].get<std::string>()) : nullptr);
        insertQuery.bindValue(":dogleg_type", dogleg ? QString::fromStdString(config["dogleg_type"].get<std::string>()) : nullptr);
        insertQuery.bindValue(":initial_trust_region_radius", trust ? QString::fromStdString(config["initial_trust_region_radius"].get<std::string>()) : nullptr);
        insertQuery.bindValue(":line_search_direction_type", line ? QString::fromStdString(config["line_search_direction_type"].get<std::string>()) : nullptr);
        insertQuery.bindValue(":line_search_type", line ? QString::fromStdString(config["line_search_type"].get<std::string>()) : nullptr);
        insertQuery.bindValue(":nonlinear_conjugate_gradient_type", line ? QString::fromStdString(config["nonlinear_conjugate_gradient_type"].get<std::string>()) : nullptr);
        insertQuery.bindValue(":gradient_tolerance", gradient_tolerance);
        insertQuery.bindValue(":function_tolerance", function_tolerance);
        insertQuery.bindValue(":linear_solver_type", linear_solver_type);
        insertQuery.bindValue(":max_num_iterations", max_num_iterations);
        insertQuery.bindValue(":minimizer_progress_to_stdout", minimizer_progress_to_stdout);
        insertQuery.bindValue(":num_threads", num_threads);
        insertQuery.bindValue(":eta", eta);
        insertQuery.bindValue(":max_solver_time_in_seconds", max_solver_time_in_seconds);
        insertQuery.bindValue(":use_nonmonotonic_steps", use_nonmonotonic_steps);
        cout << "B"<< endl;

        insertQuery.exec();
        while(insertQuery.next()) {
            cout << "SUCCESS! New index is: " << insertQuery.value(0).toInt() << endl;
            return insertQuery.value(0).toInt();
        }
    }

    return 1;
}

void MainWindow::on_pushButton_clicked()
{
    /* Submit task.
       1. Parse inputs to JSON
       2. Launch QProcess with example.
    */
    json j;

    // Get Library and method data
    std::string poseLib, poseMethod, graphLib, graphMethod, sim3Lib, sim3Method, lbaLib, lbaMethod;
    poseLib = getText(ui->poseLib);
    poseMethod = getText(ui->poseMethod);
    graphLib = getText(ui->graphLib);
    graphMethod = getText(ui->graphMethod);
    sim3Lib = getText(ui->sim3Lib);
    sim3Method = getText(ui->sim3Method);
    lbaLib = getText(ui->lbaLib);
    lbaMethod = getText(ui->lbaMethod);

    std::vector<std::string> pose {poseLib, poseMethod},
                             lba {lbaLib, lbaMethod},
                             graph {graphLib, graphMethod},
                             sim3 {sim3Lib, sim3Method};

    j["PoseOptimization"]["main"] = pose;
    j["LocalBundleAdjustment"]["main"] = lba;
    j["Sim3Optimization"]["main"] = sim3;
    j["EssentialGraphOptimization"]["main"] = graph;

    std::vector<std::string> methods{"PoseOptimization", "LocalBundleAdjustment",
                                     "Sim3Optimization", "EssentialGraphOptimization"};


    // first insert advanced settings
    int advPoseOptim=1;
    int advLbaOptim=1;
    int advSim3Optim=1;
    int advGraphOptim=1;

    cout << "check 1" << endl;
    // Function that gathers settings for each method
    //gatherDataToJSON(methods);
    // Gather Advanced settings data for Ceres or from g2o or from nothing
    if (ui->checkPoseAdvanced->isChecked() && !poseLib.compare("ceres")) {

        j["PoseOptimization"]["advanced"]["minimizer_type"] = getText(poseConfCeres->mMinimizerType);
        if (!getText(poseConfCeres->mMinimizerType).compare("TRUST_REGION")) {
            cout << "check 1.1.1" << endl;
            j["PoseOptimization"]["advanced"]["trust_region_strategy_type"] = getText(poseConfCeres->mTrustRegionWidget->mTrustRegionStrategyType);
            if (!j["PoseOptimization"]["advanced"]["trust_region_strategy_type"].get<std::string>().compare("DOGLEG"))
                j["PoseOptimization"]["advanced"]["dogleg_type"] = getText(poseConfCeres->mTrustRegionWidget->mDoglegType);
            j["PoseOptimization"]["advanced"]["initial_trust_region_radius"] = getText(poseConfCeres->mTrustRegionWidget->mInitialTrustRegionRadius);
        } else {
            cout << "check 1.1.2" << endl;
            j["PoseOptimization"]["advanced"]["line_search_direction_type"] = getText(poseConfCeres->mLineSearchWidget->mLineSearchDirectionType);
            j["PoseOptimization"]["advanced"]["line_search_type"] = getText(poseConfCeres->mLineSearchWidget->mLineSearchType);
            j["PoseOptimization"]["advanced"]["nonlinear_conjugate_gradient_type"] = getText(poseConfCeres->mLineSearchWidget->mNonlinearConjugateGradientType);
        }
        cout << "check BOB" << endl;
        j["PoseOptimization"]["advanced"]["gradient_tolerance"] = getText(poseConfCeres->mGradientTolerance);
        j["PoseOptimization"]["advanced"]["function_tolerance"] = getText(poseConfCeres->mFunctionTolerance);
        j["PoseOptimization"]["advanced"]["linear_solver_type"] = getText(poseConfCeres->mLinearSolverType);
        j["PoseOptimization"]["advanced"]["max_num_iterations"] = poseConfCeres->mMaxNumIterations->value();
        j["PoseOptimization"]["advanced"]["minimizer_progress_to_stdout"] = poseConfCeres->mMinimizerProgressToStdout->isChecked();
        j["PoseOptimization"]["advanced"]["num_threads"] = poseConfCeres->mNumthreads->value();
        j["PoseOptimization"]["advanced"]["eta"] = getText(poseConfCeres->mEta);
        j["PoseOptimization"]["advanced"]["max_solver_time_in_seconds"] = getText(poseConfCeres->mMaxSolverTimeInSeconds);
        j["PoseOptimization"]["advanced"]["use_nonmonotonic_steps"] = poseConfCeres->mUseMonotonicSteps->isChecked();

        advPoseOptim = getAdvIndex(j["PoseOptimization"]["advanced"]);
        cout << "Success!" << endl;
    } else if (ui->checkPoseAdvanced->isChecked() && !poseLib.compare("g2o")) {



        advPoseOptim = 1;
    }
    cout << "check 2" << endl;
    if (ui->checkLbaAdvanced->isChecked() && !lbaLib.compare("ceres")) {
        j["LocalBundleAdjustment"]["advanced"]["minimizer_type"] = getText(lbaConfCeres->mMinimizerType);
        if (!getText(lbaConfCeres->mMinimizerType).compare("TRUST_REGION")) {
            j["LocalBundleAdjustment"]["advanced"]["trust_region_strategy_type"] = getText(lbaConfCeres->mTrustRegionWidget->mTrustRegionStrategyType);
            if (!j["LocalBundleAdjustment"]["advanced"]["trust_region_strategy_type"].get<std::string>().compare("DOGLEG"))
                j["LocalBundleAdjustment"]["advanced"]["dogleg_type"] = getText(lbaConfCeres->mTrustRegionWidget->mDoglegType);
            j["LocalBundleAdjustment"]["advanced"]["initial_trust_region_radius"] = getText(lbaConfCeres->mTrustRegionWidget->mInitialTrustRegionRadius);
        } else {
            j["LocalBundleAdjustment"]["advanced"]["line_search_direction_type"] = getText(lbaConfCeres->mLineSearchWidget->mLineSearchDirectionType);
            j["LocalBundleAdjustment"]["advanced"]["line_search_type"] = getText(lbaConfCeres->mLineSearchWidget->mLineSearchType);
            j["LocalBundleAdjustment"]["advanced"]["nonlinear_conjugate_gradient_type"] = getText(lbaConfCeres->mLineSearchWidget->mNonlinearConjugateGradientType);
        }
        j["LocalBundleAdjustment"]["advanced"]["gradient_tolerance"] = getText(lbaConfCeres->mGradientTolerance);
        j["LocalBundleAdjustment"]["advanced"]["function_tolerance"] = getText(lbaConfCeres->mFunctionTolerance);
        j["LocalBundleAdjustment"]["advanced"]["linear_solver_type"] = getText(lbaConfCeres->mLinearSolverType);
        j["LocalBundleAdjustment"]["advanced"]["max_num_iterations"] = lbaConfCeres->mMaxNumIterations->value();
        j["LocalBundleAdjustment"]["advanced"]["minimizer_progress_to_stdout"] = lbaConfCeres->mMinimizerProgressToStdout->isChecked();
        j["LocalBundleAdjustment"]["advanced"]["num_threads"] = lbaConfCeres->mNumthreads->value();
        j["LocalBundleAdjustment"]["advanced"]["eta"] = getText(lbaConfCeres->mEta);
        j["LocalBundleAdjustment"]["advanced"]["max_solver_time_in_seconds"] =getText(lbaConfCeres->mMaxSolverTimeInSeconds);
        j["LocalBundleAdjustment"]["advanced"]["use_nonmonotonic_steps"] = lbaConfCeres->mUseMonotonicSteps->isChecked();

        advLbaOptim = getAdvIndex(j["LocalBundleAdjustment"]["advanced"]);
    } else if (ui->checkLbaAdvanced->isChecked() && !lbaLib.compare("g2o")) {
        // TO DO
        advLbaOptim = 1;
    }

    if (ui->checkSim3Advanced->isChecked() && !sim3Lib.compare("ceres")) {
        j["Sim3Optimization"]["advanced"]["minimizer_type"] = getText(sim3ConfCeres->mMinimizerType);
        if (!getText(sim3ConfCeres->mMinimizerType).compare("TRUST_REGION")) {
            j["Sim3Optimization"]["advanced"]["trust_region_strategy_type"] = getText(sim3ConfCeres->mTrustRegionWidget->mTrustRegionStrategyType);
            if (!j["Sim3Optimization"]["advanced"]["trust_region_strategy_type"].get<std::string>().compare("DOGLEG"))
                j["Sim3Optimization"]["advanced"]["dogleg_type"] = getText(sim3ConfCeres->mTrustRegionWidget->mDoglegType);
            j["Sim3Optimization"]["advanced"]["initial_trust_region_radius"] = getText(sim3ConfCeres->mTrustRegionWidget->mInitialTrustRegionRadius);
        } else {
            j["Sim3Optimization"]["advanced"]["line_search_direction_type"] = getText(sim3ConfCeres->mLineSearchWidget->mLineSearchDirectionType);
            j["Sim3Optimization"]["advanced"]["line_search_type"] = getText(sim3ConfCeres->mLineSearchWidget->mLineSearchType);
            j["Sim3Optimization"]["advanced"]["nonlinear_conjugate_gradient_type"] = getText(sim3ConfCeres->mLineSearchWidget->mNonlinearConjugateGradientType);
        }
        j["Sim3Optimization"]["advanced"]["gradient_tolerance"] = getText(sim3ConfCeres->mGradientTolerance);
        j["Sim3Optimization"]["advanced"]["function_tolerance"] = getText(sim3ConfCeres->mFunctionTolerance);
        j["Sim3Optimization"]["advanced"]["linear_solver_type"] = getText(sim3ConfCeres->mLinearSolverType);
        j["Sim3Optimization"]["advanced"]["max_num_iterations"] = sim3ConfCeres->mMaxNumIterations->value();
        j["Sim3Optimization"]["advanced"]["minimizer_progress_to_stdout"] = sim3ConfCeres->mMinimizerProgressToStdout->isChecked();
        j["Sim3Optimization"]["advanced"]["num_threads"] = sim3ConfCeres->mNumthreads->value();
        j["Sim3Optimization"]["advanced"]["eta"] = getText(sim3ConfCeres->mEta);
        j["Sim3Optimization"]["advanced"]["max_solver_time_in_seconds"] = getText(sim3ConfCeres->mMaxSolverTimeInSeconds);
        j["Sim3Optimization"]["advanced"]["use_nonmonotonic_steps"] = sim3ConfCeres->mUseMonotonicSteps->isChecked();

        advSim3Optim = getAdvIndex(j["Sim3Optimization"]["advanced"]);
    } else if (ui->checkSim3Advanced->isChecked() && !sim3Lib.compare("g2o")) {
        // TO DO
        advSim3Optim = 1;
    }

    if (ui->checkGraphAdvanced->isChecked() && !graphLib.compare("ceres")) {
        j["EssentialGraphOptimization"]["advanced"]["minimizer_type"] = getText(graphConfCeres->mMinimizerType);
        if (!getText(graphConfCeres->mMinimizerType).compare("TRUST_REGION")) {
            j["EssentialGraphOptimization"]["advanced"]["trust_region_strategy_type"] = getText(graphConfCeres->mTrustRegionWidget->mTrustRegionStrategyType);
            if (!j["EssentialGraphOptimization"]["advanced"]["trust_region_strategy_type"].get<std::string>().compare("DOGLEG"))
                j["EssentialGraphOptimization"]["advanced"]["dogleg_type"] = getText(graphConfCeres->mTrustRegionWidget->mDoglegType);
            j["EssentialGraphOptimization"]["advanced"]["initial_trust_region_radius"] = getText(graphConfCeres->mTrustRegionWidget->mInitialTrustRegionRadius);
        } else {
            j["EssentialGraphOptimization"]["advanced"]["line_search_direction_type"] = getText(graphConfCeres->mLineSearchWidget->mLineSearchDirectionType);
            j["EssentialGraphOptimization"]["advanced"]["line_search_type"] = getText(graphConfCeres->mLineSearchWidget->mLineSearchType);
            j["EssentialGraphOptimization"]["advanced"]["nonlinear_conjugate_gradient_type"] = getText(graphConfCeres->mLineSearchWidget->mNonlinearConjugateGradientType);
        }
        j["EssentialGraphOptimization"]["advanced"]["gradient_tolerance"] = getText(graphConfCeres->mGradientTolerance);
        j["EssentialGraphOptimization"]["advanced"]["function_tolerance"] = getText(graphConfCeres->mFunctionTolerance);
        j["EssentialGraphOptimization"]["advanced"]["linear_solver_type"] = getText(graphConfCeres->mLinearSolverType);
        j["EssentialGraphOptimization"]["advanced"]["max_num_iterations"] = graphConfCeres->mMaxNumIterations->value();
        j["EssentialGraphOptimization"]["advanced"]["minimizer_progress_to_stdout"] = graphConfCeres->mMinimizerProgressToStdout->isChecked();
        j["EssentialGraphOptimization"]["advanced"]["num_threads"] = graphConfCeres->mNumthreads->value();
        j["EssentialGraphOptimization"]["advanced"]["eta"] = getText(graphConfCeres->mEta);
        j["EssentialGraphOptimization"]["advanced"]["max_solver_time_in_seconds"] =  getText(graphConfCeres->mMaxSolverTimeInSeconds);
        j["EssentialGraphOptimization"]["advanced"]["use_nonmonotonic_steps"] = graphConfCeres->mUseMonotonicSteps->isChecked();

        advGraphOptim = getAdvIndex(j["EssentialGraphOptimization"]["advanced"]);
    } else if (ui->checkGraphAdvanced->isChecked() && !graphLib.compare("g2o")) {
        // TO DO
        advGraphOptim = 1;
    }

    // write prettified JSON to another file
    std::ofstream o("data.json");
    o << std::setw(4) << j << endl;

    // Copy file to destination folder.
    bool status = copyFile("/home/borisqa/qt/build-comp-frame-Desktop_Qt_5_12_3_GCC_64bit-Debug/data.json",
                           "/home/borisqa/Documents/orb-compiled/data.json");
    cout << "Status of copy: " << status << endl;

    QString mono_tum = "/home/borisqa/Documents/orb-compiled/Examples/Monocular/mono_tum";
    QString mono_kitti = "/home/borisqa/Documents/orb-compiled/Examples/Monocular/mono_kitti";
    QString mono_euroc = "/home/borisqa/Documents/orb-compiled/Examples/Monocular/mono_euroc";
    QString rgbd_tum = "/home/borisqa/Documents/orb-compiled/Examples/RGB-D/rgbd_tum";

    QString dataset = ui->comboBox->currentText();
    int exp = 1;
    QSqlQuery argQuery;
    argQuery.exec("select max(result_id) from results;");
    while(argQuery.next()) {
        exp = argQuery.value(0).toInt() + 1;
    }
    QString experimentID = QString::number(exp);
    QStringList arguments;

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);

    if (!dataset.toStdString().compare("mono_tum")) {
        cout << "ASD" << endl;
        arguments << "/home/borisqa/Documents/orb-compiled/Vocabulary/ORBvoc.txt"
                  << "/home/borisqa/Documents/orb-compiled/Examples/Monocular/TUM1.yaml"
                  << "/home/borisqa/Documents/orb-compiled/rgbd_dataset_freiburg1_desk/"
                  << "/home/borisqa/Documents/orb-compiled/data.json"
                  << experimentID;
        process->start(mono_tum, arguments);
    } else if (!dataset.toStdString().compare("mono_kitti")) {
        cout << "ASD2" << endl;
        arguments << "/home/borisqa/Documents/orb-compiled/Vocabulary/ORBvoc.txt"
                  << "/home/borisqa/Documents/orb-compiled/Examples/Monocular/KITTI00-02.yaml"
                  << "/home/borisqa/Documents/datasets/dataset/sequences/01"
                  << "/home/borisqa/Documents/orb-compiled/data.json"
                  << experimentID;
        process->start(mono_kitti, arguments);
    } else if (!dataset.toStdString().compare("mono_euroc")) {
        cout << "ASD3" << endl;
        arguments << "/home/borisqa/Documents/orb-compiled/Vocabulary/ORBvoc.txt"
                  << "/home/borisqa/Documents/orb-compiled/Examples/Monocular/EuRoC.yaml"
                  << "/home/borisqa/Documents/datasets/mav0/cam0/data"
                  << "/home/borisqa/Documents/orb-compiled/Examples/Monocular/EuRoC_TimeStamps/V101.txt"
                  << "/home/borisqa/Documents/orb-compiled/data.json"
                  << experimentID;
        process->start(mono_euroc, arguments);
    }
    cout << "ASD4" << endl;
    //process->startDetached(fullPath);
    cout << "Process PID: " << process->pid() << endl;
    if (!process->waitForFinished(380000))
        qDebug() << "Orb-Slam Failed: " << process->errorString();
    else {
        qDebug() << "Orb-slam output: " << process->readAll();

        // If the program execution successful, then insert results to table;
        json res;
        std::ifstream res_file("results.json");
        res_file >> res;

        //QSqlQuery query;
        //query.prepare("INSERT INTO test1 (value) "
        //              "VALUES (:value)");
        //cout << res["mean_tracking_time"].get<double>() << endl;
        //query.bindValue(":value", res["mean_tracking_time"].get<double>());
        //query.exec();
        //cout << res; //This will print the entire json object.

        cout << "-----------------------------" << endl << "START Inserting stuff!!!" << endl << "1) Insert adv. query" << endl;
        // Now the results of the experiment will be saved in database
        // Insert advanced
        QSqlQuery advSettingsQuery;
        advSettingsQuery.exec("select * from advanced where pose_ceres="+QString::number(advPoseOptim)+" and pose_g2o=1 and lba_ceres="+QString::number(advLbaOptim)+" and lba_g2o=1 and "
                                                           "sim3_ceres="+QString::number(advSim3Optim)+" and sim3_g2o=1 and graph_ceres="+QString::number(advGraphOptim)+" and graph_g2o=1;");
        int advSettingsID=0;
        if (advSettingsQuery.size()) {
            cout << "QUERY EXISTS - RETURNING THE OLD INDEX ))))" << endl;
            while (advSettingsQuery.next()) {
                advSettingsID = advSettingsQuery.value(0).toInt();
            }
        } else {
            // Else we need to insert the new config and return index
            QSqlQuery insertQuery;
            insertQuery.prepare("insert into advanced (pose_ceres, pose_g2o, lba_ceres, lba_g2o, sim3_ceres, sim3_g2o, graph_ceres, graph_g2o) values \
                                     (:pose_ceres, :pose_g2o, :lba_ceres, :lba_g2o, :sim3_ceres, :sim3_g2o, :graph_ceres, :graph_g2o) returning advanced_id;");
            insertQuery.bindValue(":pose_ceres", advPoseOptim);
            insertQuery.bindValue(":pose_g2o", 1);
            insertQuery.bindValue(":lba_ceres", advLbaOptim);
            insertQuery.bindValue(":lba_g2o", 1);
            insertQuery.bindValue(":sim3_ceres", advSim3Optim);
            insertQuery.bindValue(":sim3_g2o", 1);
            insertQuery.bindValue(":graph_ceres", advGraphOptim);
            insertQuery.bindValue(":graph_g2o", 1);
            insertQuery.exec();
            cout << "HEY" << insertQuery.size() << endl;

            while (insertQuery.next()) {
                advSettingsID = insertQuery.value(0).toInt();
            }

        }
        cout << "Got advSettingsID = " << advSettingsID << endl;


        cout << "2) Insert results query" << endl;
        // Insert results
        QSqlQuery resultsQuery;
        resultsQuery.prepare("insert into results (trajectory_path, tracking_time_average, tracking_time_median) values \
                             (:trajectory_path, :tracking_time_average, :tracking_time_median) returning result_id;");
        resultsQuery.bindValue(":trajectory_path", res["experiment_trajectory"].get<std::string>().c_str());
        resultsQuery.bindValue(":tracking_time_average", res["mean_tracking_time"].get<double>());
        resultsQuery.bindValue(":tracking_time_median", res["medium_tracking_time"].get<double>());
        resultsQuery.exec();

        int resultsID=0;
        while (resultsQuery.next()) {
            resultsID = resultsQuery.value(0).toInt();
        }
        cout << "Got resultsID = " << resultsID << endl;


        // Get the index of libraries
        cout << "3) Get libraries" << endl;
        QSqlQuery libQuery;
        libQuery.exec("select * from libraries where pose_optim_lib='"+QString::fromStdString(poseLib)+"' and lba_optim_lib='"+QString::fromStdString(lbaLib)+"' and sim3_optim_lib='"+QString::fromStdString(sim3Lib)+"' and graph_optim_lib='"+QString::fromStdString(graphLib)+"';");
        int librariesID=0;
        while (libQuery.next()) {
            librariesID = libQuery.value(0).toInt();
        }
        cout << "Got librariesID = " << librariesID << endl;

        // Get the index of methods
        cout << "4) Get methods" << endl;
        QSqlQuery methodQuery;
        methodQuery.exec("select * from methods where pose_optim_method='"+QString::fromStdString(poseMethod)+"' and lba_optim_method='"+QString::fromStdString(lbaMethod)+"' and sim3_optim_method='"+QString::fromStdString(sim3Method)+"' and graph_optim_method='"+QString::fromStdString(graphMethod)+"';");
        int methodsID=0;
        while (methodQuery.next()) {
            methodsID = methodQuery.value(0).toInt();
        }
        cout << "Got methodsID = " << methodsID << endl;

        // Insert Experiment
        cout << "5) Insert experiment !!!" << endl;

        QSqlQuery expQuery;
        expQuery.prepare("insert into experiments (dataset_name, libraries, methods, advanced, results) values (:dataset_name, :libraries, :methods, :advanced, :results);");
        expQuery.bindValue(":dataset_name", dataset);
        expQuery.bindValue(":libraries", librariesID);
        expQuery.bindValue(":methods", methodsID);
        expQuery.bindValue(":advanced", advSettingsID);
        expQuery.bindValue(":results", resultsID);
        expQuery.exec();

        cout << "The end!" << endl;
        cout << "-------------------------------------" << endl << endl;
    }

    cout << "Blocked or not" << endl;

}



void MainWindow::on_checkPoseAdvanced_stateChanged(int arg1)
{
    std::string g2o("g2o");
    if (arg1 == 2) {
        if (!g2o.compare(ui->poseLib->currentText().toUtf8().constData())) {
            cb1 = new QCheckBox("Name", this);
            ui->vlPose->addWidget(cb1);
        } else {
            poseConfCeres = new AdvancedConfigCeres(this);
            ui->vlPose->addWidget(poseConfCeres);
        }
    } else if (arg1 == 0) {
        if (!g2o.compare(ui->poseLib->currentText().toUtf8().constData())) {
            delete cb1;
        } else {
            delete poseConfCeres;
        }

    }
}

void MainWindow::on_checkLbaAdvanced_stateChanged(int arg1)
{
    std::string g2o("g2o");
    if (arg1 == 2) {
        if (!g2o.compare(ui->lbaLib->currentText().toUtf8().constData())) {
            cb1 = new QCheckBox("Name", this);
            ui->vlLba->addWidget(cb1);
        } else {
            lbaConfCeres = new AdvancedConfigCeres(this);
            ui->vlLba->addWidget(lbaConfCeres);
        }
    } else if (arg1 == 0) {
        if (!g2o.compare(ui->lbaLib->currentText().toUtf8().constData())) {
            delete cb1;
        } else {
            delete lbaConfCeres;
        }

    }
}

void MainWindow::on_checkGraphAdvanced_stateChanged(int arg1)
{
    std::string g2o("g2o");
    if (arg1 == 2) {
        if (!g2o.compare(ui->graphLib->currentText().toUtf8().constData())) {
            cb1 = new QCheckBox("Name", this);
            ui->vlGraph->addWidget(cb1);
        } else {
            graphConfCeres = new AdvancedConfigCeres(this);
            ui->vlGraph->addWidget(graphConfCeres);
        }
    } else if (arg1 == 0) {
        if (!g2o.compare(ui->graphLib->currentText().toUtf8().constData())) {
            delete cb1;
        } else {
            delete graphConfCeres;
        }

    }
}

void MainWindow::on_checkSim3Advanced_stateChanged(int arg1)
{
    std::string g2o("g2o");
    if (arg1 == 2) {
        if (!g2o.compare(ui->sim3Lib->currentText().toUtf8().constData())) {
            cb1 = new QCheckBox("Name", this);
            ui->vlSim3->addWidget(cb1);
        } else {
            sim3ConfCeres = new AdvancedConfigCeres(this);
            ui->vlSim3->addWidget(sim3ConfCeres);
        }
    } else if (arg1 == 0) {
        if (!g2o.compare(ui->sim3Lib->currentText().toUtf8().constData())) {
            delete cb1;
        } else {
            delete sim3ConfCeres;
        }

    }
}

void MainWindow::on_pushButton_2_clicked()
{
    resultWindow = new ResultsWidget(nullptr, this);
    resultWindow->show();
    hide();

}
