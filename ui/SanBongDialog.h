#ifndef SANBONGDIALOG_H
#define SANBONGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include "../core/SanBong.h"

class SanBongDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SanBongDialog(QWidget *parent = nullptr, SanBong *san = nullptr);

    std::string getMaSan() const { return txtMaSan->text().toStdString(); }
    std::string getTenSan() const { return txtTenSan->text().toStdString(); }
    int getLoaiSan() const { return cboLoaiSan->currentText().toInt(); }
    double getGiaThue() const { return spnGiaThue->value(); }

private:
    QLineEdit *txtMaSan;
    QLineEdit *txtTenSan;
    QComboBox *cboLoaiSan;
    QDoubleSpinBox *spnGiaThue;
};

#endif // SANBONGDIALOG_H
