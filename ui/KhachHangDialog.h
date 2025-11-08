#ifndef KHACHHANGDIALOG_H
#define KHACHHANGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include "../core/KhachHang.h"
#include "../core/QuanLyThueSan.h"

class KhachHangDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KhachHangDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr, KhachHang *kh = nullptr);

    std::string getMaKH() const { return txtMaKH->text().toStdString(); }
    std::string getHoTen() const { return txtHoTen->text().toStdString(); }
    std::string getSdt() const { return txtSdt->text().toStdString(); }

private:
    QString generateMaKH(); // ✅ Auto-generate mã khách hàng
    
    QuanLyThueSan *quanLy;
    QLineEdit *txtMaKH;
    QLineEdit *txtHoTen;
    QLineEdit *txtSdt;
    QLabel *lblSuggestion; // ✅ Gợi ý mã
};

#endif // KHACHHANGDIALOG_H
