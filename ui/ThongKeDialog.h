#ifndef THONGKEDIALOG_H
#define THONGKEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QTableWidget>
#include <QTabWidget>
#include <QHeaderView>
#include <QDate>
#include "../core/QuanLyThueSan.h"
#include "../core/managers/ThanhToanManager.h"
#include "ChartWidgets.h"

class ThongKeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThongKeDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr);

private slots:
    void onThongKeClicked();
    void onKhoangThoiGianChanged(int index);

private:
    void setupUI();
    void capNhatThongKe(time_t tuNgay, time_t denNgay);
    
    QuanLyThueSan *quanLy;
    
    // Filter controls
    QComboBox *cboKhoangThoiGian;
    QDateEdit *dateFrom;
    QDateEdit *dateTo;
    QPushButton *btnThongKe;
    
    // Tổng quan labels
    QLabel *lblDoanhThuKhoang;
    QLabel *lblTongBooking;
    QLabel *lblDoanhThuTB;
    QLabel *lblTyLeHuy;
    QLabel *lblDoanhThuNgay;
    QLabel *lblDoanhThuThang;
    QLabel *lblDoanhThuQuy;
    QLabel *lblDoanhThuNam;
    
    // Detail tables
    QTableWidget *tableThongKeSan;
    QTableWidget *tableKhungGio;
    QTableWidget *tableTopKhach;
    
    // Chart widgets
    PieChartWidget *pieChartSan;
    BarChartWidget *barChartGio;
    BarChartWidget *barChartDichVu;
};

#endif // THONGKEDIALOG_H
