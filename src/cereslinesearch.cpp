#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QStringList>

#include "cereslinesearch.h"

CeresLineSearch::CeresLineSearch(QWidget* parent) : QWidget (parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 1) Line Search Direction Type
    QHBoxLayout* element1 = new QHBoxLayout;
    QLabel* searchDirection = new QLabel("Line search direction type:");
    mLineSearchDirectionType = new QComboBox;
    QStringList dirTypes = {"LBFGS", "STEEPEST_DESCENT", "NONLINEAR_CONJUGATE_GRADIENT", "BFGS"};
    mLineSearchDirectionType->addItems(dirTypes);
    element1->addWidget(searchDirection);
    element1->addWidget(mLineSearchDirectionType);
    layout->addLayout(element1);

    // 2) Line Search Type
    QHBoxLayout* element2 = new QHBoxLayout;
    QLabel* searchType = new QLabel("Line search type:");
    mLineSearchType = new QComboBox;
    QStringList searchTypes = {"WOLFE", "ARMIJO"};
    mLineSearchType->addItems(searchTypes);
    element2->addWidget(searchType);
    element2->addWidget(mLineSearchType);
    layout->addLayout(element2);

    // 3) Nonlinear Conjugate Gradient
    QHBoxLayout* element3 = new QHBoxLayout;
    QLabel* nonlin = new QLabel("Nonlinear Conjugate Gradient type:");
    mNonlinearConjugateGradientType = new QComboBox;
    QStringList nonlinTypes = {"FLETCHER_REEVES", "POLAK_RIBIERE", "HESTENES_STIEFEL"};
    mNonlinearConjugateGradientType->addItems(nonlinTypes);
    element3->addWidget(nonlin);
    element3->addWidget(mNonlinearConjugateGradientType);
    layout->addLayout(element3);

    this->setLayout(layout);
}
