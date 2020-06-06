#pragma once

#include <QWidget>
//#include <QUiLoader>
#include <String_calc.h>
#include "ui_calc_form.h"

#include <QDebug>
namespace Ui {
class Form;
}

class AE_CalcWidget : public QWidget
{
    Q_OBJECT

    public:
    AE_CalcWidget(QWidget *parent = 0);
    ~AE_CalcWidget();
private slots:
    void ins_sym();

    void on_pb_clear_clicked();
    void on_pb_rem_clicked();

    void on_pb_enter_clicked();

private:
     Ui::Form *ui;
};
