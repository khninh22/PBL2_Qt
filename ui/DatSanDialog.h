#ifndef DATSANDIALOG_H
#define DATSANDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDateTimeEdit>
#include "../core/QuanLyThueSan.h"

class DatSanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatSanDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr);

private slots:
    void onTimSanTrong();
    void onDatSan();

private:
    QuanLyThueSan *quanLy;
    QComboBox *cboKhachHang;
    QComboBox *cboLoaiSan;
    QComboBox *cboSanTrong;
    QDateTimeEdit *dteBatDau;
    QDateTimeEdit *dteKetThuc;
};

#endif // DATSANDIALOG_H
