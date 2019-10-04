#include <QLabel>
#include <QStringList>

#include "cerestrustregion.h"

CeresTrustRegion::CeresTrustRegion(QWidget* parent) : QWidget (parent)
{
    layout = new QVBoxLayout(this);

    // 1) Trust region strategy type
    QHBoxLayout* element1 = new QHBoxLayout;
    QLabel* strategy = new QLabel("Trust region strategy type:");
    mTrustRegionStrategyType = new QComboBox;
    QStringList stratTypes = {"LEVENBERG_MARQUARDT", "DOGLEG"};
    mTrustRegionStrategyType->addItems(stratTypes);
    element1->addWidget(strategy);
    element1->addWidget(mTrustRegionStrategyType);
    layout->addLayout(element1);

    // 2) Nonlinear Conjugate Gradient
    QHBoxLayout* element3 = new QHBoxLayout;
    QLabel* radius = new QLabel("Initial trust region radius:");
    mInitialTrustRegionRadius = new QComboBox;
    QStringList radiuses = {"1e4", "1e5"};
    mInitialTrustRegionRadius->addItems(radiuses);
    element3->addWidget(radius);
    element3->addWidget(mInitialTrustRegionRadius);
    layout->addLayout(element3);

    connect(mTrustRegionStrategyType, SIGNAL(currentTextChanged(QString)), this, SLOT(on_mTrustRegionStrategyType_currentTextChanged(QString)));

    this->setLayout(layout);
}

void CeresTrustRegion::on_mTrustRegionStrategyType_currentTextChanged(const QString& text)
{
    std::string trustRegion("LEVENBERG_MARQUARDT");

    if (!trustRegion.compare(text.toUtf8().constData())) {
        //delete element_dogleg;
        delete mDoglegType;
        delete doglegType;
        layout->removeItem(element_dogleg);

    } else {
        element_dogleg = new QHBoxLayout;
        doglegType = new QLabel("Dogleg type:");
        mDoglegType = new QComboBox;
        QStringList doglegTypes = {"TRADITIONAL_DOGLEG", "SUBSPACE_DOGLEG"};
        mDoglegType->addItems(doglegTypes);
        element_dogleg->addWidget(doglegType);
        element_dogleg->addWidget(mDoglegType);
        layout->addLayout(element_dogleg);
        //this->setLayout(layout);
    }
}
