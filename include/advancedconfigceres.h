#ifndef ADVANCEDCONFIGCERES_H
#define ADVANCEDCONFIGCERES_H

#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>

#include "cereslinesearch.h"
#include "cerestrustregion.h"

class AdvancedConfigCeres : public QWidget
{

Q_OBJECT

public:
    explicit AdvancedConfigCeres(QWidget *parent = nullptr);
    QComboBox* mMinimizerType;  // TRUST_REGION; LINE_SEARCH
    QComboBox* mGradientTolerance;
    QComboBox* mFunctionTolerance;
    QComboBox* mLinearSolverType;
    QSpinBox* mMaxNumIterations;
    QCheckBox* mMinimizerProgressToStdout;
    QSpinBox* mNumthreads;
    QComboBox* mEta;
    QComboBox* mMaxSolverTimeInSeconds;
    QCheckBox* mUseMonotonicSteps;

    CeresLineSearch* mLineSearchWidget;
    CeresTrustRegion* mTrustRegionWidget;

    QVBoxLayout* layout;

private slots:
    void on_mMinimizerType_currentTextChanged(const QString& text);
};

#endif // ADVANCEDCONFIGCERES_H
