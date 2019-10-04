#include "advancedconfigceres.h"
#include <QLabel>
#include <QStringList>

AdvancedConfigCeres::AdvancedConfigCeres(QWidget *parent) : QWidget(parent)
{

    layout = new QVBoxLayout(this);

    // 1) Minimizer Type
    QLabel* minType = new QLabel("Minimizer type:");
    QStringList types = {"TRUST_REGION", "LINE_SEARCH"};
    mMinimizerType = new QComboBox;
    mMinimizerType->addItems(types);
    QHBoxLayout* element1 = new QHBoxLayout;
    element1->addWidget(minType);
    element1->addWidget(mMinimizerType);
    layout->addLayout(element1);

    // 2) Gradient Tolerance
    QLabel* grad = new QLabel("Gradient tolerance:");
    QStringList grads = {"1e-16", "1e-15", "1e-14", "1e-13", "1e-12", "1e-11",
                         "1e-10", "1e-9", "1e-8", "1e-7", "1e-6", "1e-5",
                         "1e-4", "1e-3", "1e-2"};
    mGradientTolerance = new QComboBox;
    mGradientTolerance->addItems(grads);
    QHBoxLayout* element2 = new QHBoxLayout;
    element2->addWidget(grad);
    element2->addWidget(mGradientTolerance);
    layout->addLayout(element2);

    // 3) Function Tolerance
    QLabel* func = new QLabel("Function tolerance:");
    mFunctionTolerance = new QComboBox;
    mFunctionTolerance->addItems(grads);
    QHBoxLayout* element3 = new QHBoxLayout;
    element3->addWidget(func);
    element3->addWidget(mFunctionTolerance);
    layout->addLayout(element3);

    // 4) Linear solver type
    QLabel* linearSolver = new QLabel("Linear solver type:");
    QStringList linSolvers = {"SPARSE_SCHUR", "FIX#1", "FIX#2", "FIX#3"};
    mLinearSolverType = new QComboBox;
    mLinearSolverType->addItems(linSolvers);
    QHBoxLayout* element4 = new QHBoxLayout;
    element4->addWidget(linearSolver);
    element4->addWidget(mLinearSolverType);
    layout->addLayout(element4);

    // 5) Maximum number of iterations
    QLabel* maxIter = new QLabel("Maximum Iterations:");
    mMaxNumIterations = new QSpinBox;
    mMaxNumIterations->setRange(1, 300);
    QHBoxLayout* element5 = new QHBoxLayout;
    element5->addWidget(maxIter);
    element5->addWidget(mMaxNumIterations);
    layout->addLayout(element5);

    // 6) Minimizer progress to stdout
    QLabel* progressOut = new QLabel("Minimizer progress to stdout:");
    mMinimizerProgressToStdout = new QCheckBox;
    QHBoxLayout* element6 = new QHBoxLayout;
    element6->addWidget(progressOut);
    element6->addWidget(mMinimizerProgressToStdout);
    layout->addLayout(element6);

    // 7) Number of threads
    QLabel* numThreads = new QLabel("Number of threads:");
    mNumthreads = new QSpinBox;
    mNumthreads->setRange(1, 4);
    QHBoxLayout* element7 = new QHBoxLayout;
    element7->addWidget(numThreads);
    element7->addWidget(mNumthreads);
    layout->addLayout(element7);

    // 8) Set eta parameter
    QLabel* eta = new QLabel("Eta parameter:");
    mEta = new QComboBox;
    QStringList etas = {"1e-2", "1e-3", "1e-4", "1e-5", "1e-6"};
    mEta->addItems(etas);
    QHBoxLayout* element8 = new QHBoxLayout;
    element8->addWidget(eta);
    element8->addWidget(mEta);
    layout->addLayout(element8);

    // 9) Max solver time in seconds
    QLabel* solverTime = new QLabel("Solver time in sec's:");
    mMaxSolverTimeInSeconds = new QComboBox;
    mMaxSolverTimeInSeconds->addItem("1e32");
    QHBoxLayout* element9 = new QHBoxLayout;
    element9->addWidget(solverTime);
    element9->addWidget(mMaxSolverTimeInSeconds);
    layout->addLayout(element9);

    // 10) Use Nonmonotonic steps
    QLabel* nonmonotonic = new QLabel("Use nonmonotonic steps:");
    mUseMonotonicSteps = new QCheckBox;
    QHBoxLayout* element10 = new QHBoxLayout;
    element10->addWidget(nonmonotonic);
    element10->addWidget(mUseMonotonicSteps);
    layout->addLayout(element10);

    // 11) Additional settings
    mTrustRegionWidget = new CeresTrustRegion(this);
    layout->addWidget(mTrustRegionWidget);

    connect(mMinimizerType, SIGNAL(currentTextChanged(QString)), this, SLOT(on_mMinimizerType_currentTextChanged(QString)));
    this->setLayout(layout);
}

void AdvancedConfigCeres::on_mMinimizerType_currentTextChanged(const QString& text)
{
    std::string trustRegion("TRUST_REGION");

    if (!trustRegion.compare(text.toUtf8().constData())) {
        delete mLineSearchWidget;
        mTrustRegionWidget = new CeresTrustRegion(this);
        layout->addWidget(mTrustRegionWidget);
    } else {
        delete mTrustRegionWidget;
        mLineSearchWidget = new CeresLineSearch(this);
        layout->addWidget(mLineSearchWidget);
    }
}
