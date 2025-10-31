#ifndef PAYMENTDIALOG_H
#define PAYMENTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include "../core/QuanLyThueSan.h"

class PaymentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaymentDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr);

signals:
    void paymentCompleted(); // ✅ Signal khi thanh toán thành công

private slots:
    void onLichDatSelected();
    void onXacNhanThanhToan();
    void onInHoaDon();
    void onLuuPDF();

private:
    void setupUI();
    void loadLichDatChuaThanhToan();
    void generateInvoicePreview();
    QString generateInvoiceHTML();
    
    QuanLyThueSan *quanLy;
    
    // Left side: Danh sách lịch đặt chưa thanh toán
    QTableWidget *tableLichDat;
    
    // Right side: Hóa đơn preview
    QTextEdit *txtHoaDon;
    QLabel *lblTongTien;
    QLabel *lblGiamGia;
    QLabel *lblThanhToan;
    
    QPushButton *btnXacNhan;
    QPushButton *btnInHoaDon;
    QPushButton *btnLuuPDF;
    
    QString maLichDatDangChon;
};

#endif // PAYMENTDIALOG_H
