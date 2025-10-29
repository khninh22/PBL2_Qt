#ifndef DICHVUDIALOG_H
#define DICHVUDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include "../core/DichVu.h"

class DichVuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DichVuDialog(QWidget *parent = nullptr, DichVu *dv = nullptr);

    std::string getMaDV() const { return txtMaDV->text().toStdString(); }
    std::string getTenDV() const { return txtTenDV->text().toStdString(); }
    std::string getLoaiDV() const { return cboLoaiDV->currentText().toStdString(); }
    double getGiaDV() const { return spnGiaDV->value(); }

private:
    QLineEdit *txtMaDV;
    QLineEdit *txtTenDV;
    QComboBox *cboLoaiDV;
    QDoubleSpinBox *spnGiaDV;
};

#endif // DICHVUDIALOG_H
