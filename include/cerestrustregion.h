#ifndef CERESTRUSTREGION_H
#define CERESTRUSTREGION_H

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>

class CeresTrustRegion : public QWidget
{

Q_OBJECT
public:
    CeresTrustRegion(QWidget *parent = nullptr);

    QComboBox* mTrustRegionStrategyType; // LEVENBERG_MARQUARDT; DOGLEG
    QComboBox* mDoglegType; // TRADITIONAL_DOGLEG; SUBSPACE_DOGLEG
    QComboBox* mInitialTrustRegionRadius; // 1e4
    QVBoxLayout* layout;
    QHBoxLayout* element_dogleg;
    QLabel* doglegType;

private slots:
    void on_mTrustRegionStrategyType_currentTextChanged(const QString& text);
};

#endif // CERESTRUSTREGION_H
