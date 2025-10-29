#ifndef NHANVIENDIALOG_H
#define NHANVIENDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include "../core/NhanVien.h"

class NhanVienDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NhanVienDialog(QWidget *parent = nullptr, NhanVien *nv = nullptr);

    std::string getMaNV() const { return txtMaNV->text().toStdString(); }
    std::string getHoTen() const { return txtHoTen->text().toStdString(); }
    std::string getSdt() const { return txtSdt->text().toStdString(); }
    std::string getViTri() const { return cboViTri->currentText().toStdString(); }
    double getLuong() const { return spnLuong->value(); }
    std::string getTaiKhoan() const { return txtTaiKhoan->text().toStdString(); }
    std::string getMatKhau() const { return txtMatKhau->text().toStdString(); }

private:
    QLineEdit *txtMaNV;
    QLineEdit *txtHoTen;
    QLineEdit *txtSdt;
    QComboBox *cboViTri;
    QDoubleSpinBox *spnLuong;
    QLineEdit *txtTaiKhoan;
    QLineEdit *txtMatKhau;
};

#endif // NHANVIENDIALOG_H
