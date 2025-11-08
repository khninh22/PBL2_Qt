#ifndef DICHVUDIALOG_H
#define DICHVUDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include "../core/DichVu.h"
#include "../core/QuanLyThueSan.h"

class DichVuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DichVuDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr, DichVu *dv = nullptr);

    std::string getMaDV() const { return txtMaDV->text().toStdString(); }
    std::string getTenDV() const { return txtTenDV->text().toStdString(); }
    std::string getLoaiDV() const { return cboLoaiDV->currentText().toStdString(); }
    double getGiaDV() const { return spnGiaDV->value(); }

private:
    QString generateMaDV(); // ✅ Auto-generate mã dịch vụ
    
    QuanLyThueSan *quanLy;
    QLineEdit *txtMaDV;
    QLineEdit *txtTenDV;
    QComboBox *cboLoaiDV;
    QDoubleSpinBox *spnGiaDV;
    QLabel *lblSuggestion; // ✅ Gợi ý mã
};

#endif // DICHVUDIALOG_H
