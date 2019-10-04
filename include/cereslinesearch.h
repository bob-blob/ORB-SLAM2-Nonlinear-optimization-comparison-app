#ifndef CERESLINESEARCH_H
#define CERESLINESEARCH_H

#include <QWidget>
#include <QComboBox>

class CeresLineSearch : public QWidget
{
public:
    explicit CeresLineSearch(QWidget *parent = nullptr);
    // Line search settings
    QComboBox* mLineSearchDirectionType; // LBFGS; STEEPEST_DESCENT, NONLINEAR_CONJUGATE_GRADIENT, BFGS, LBFGS
    QComboBox* mLineSearchType; // WOLFE (should be for BFGS and LBFGS always!); ARMIJO
    QComboBox* mNonlinearConjugateGradientType; // FLETHCER_REEVES; POLAK_RIBIERE, HESTENES_STIEFEL
    // A lot of settings could be added here
};

#endif // CERESLINESEARCH_H
