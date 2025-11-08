#ifndef SANBONGDIALOG_H
#define SANBONGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include "../core/SanBong.h"
#include "../core/QuanLyThueSan.h"

class SanBongDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SanBongDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr, SanBong *san = nullptr);

    std::string getMaSan() const { return txtMaSan->text().toStdString(); }
    std::string getTenSan() const { return txtTenSan->text().toStdString(); }
    int getLoaiSan() const { return cboLoaiSan->currentText().toInt(); }
    double getGiaThue() const { return spnGiaThue->value(); }

private:
    QString generateMaSan(); // ✅ Auto-generate mã sân
    
    QuanLyThueSan *quanLy;
    QLineEdit *txtMaSan;
    QLineEdit *txtTenSan;
    QComboBox *cboLoaiSan;
    QDoubleSpinBox *spnGiaThue;
    QLabel *lblSuggestion; // ✅ Gợi ý mã
};

#endif // SANBONGDIALOG_H
