#ifndef RESULTS_H
#define RESULTS_H

#include <QWidget>

namespace Ui {
class Results;
}

class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = nullptr);
    ~Results();

private:
    Ui::Results *ui;
};

#endif // RESULTS_H
