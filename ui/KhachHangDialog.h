#ifndef KHACHHANGDIALOG_H
#define KHACHHANGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "../core/KhachHang.h"

class KhachHangDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KhachHangDialog(QWidget *parent = nullptr, KhachHang *kh = nullptr);

    std::string getMaKH() const { return txtMaKH->text().toStdString(); }
    std::string getHoTen() const { return txtHoTen->text().toStdString(); }
    std::string getSdt() const { return txtSdt->text().toStdString(); }

private:
    QLineEdit *txtMaKH;
    QLineEdit *txtHoTen;
    QLineEdit *txtSdt;
};

#endif // KHACHHANGDIALOG_H
