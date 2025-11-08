#ifndef SANBOOKINGDIALOG_H
#define SANBOOKINGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include "../core/QuanLyThueSan.h"

// Widget đại diện cho một ô thời gian của sân
class TimeSlotWidget : public QWidget
{
    Q_OBJECT

public:
    TimeSlotWidget(const QString &maSan, int gioStart, double duration,
                   bool isBooked, QWidget *parent = nullptr);

    QString getMaSan() const { return maSan; }
    int getGioStart() const { return gioStart; }
    double getDuration() const { return duration; }
    bool isBooked() const { return booked; }

    void setBooked(bool book);
    void setSelected(bool selected);

signals:
    void clicked(TimeSlotWidget *widget);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString maSan;
    int gioStart;    // 6, 7, 8, ..., 21
    double duration; // 0.5 (30 phút), 1.0 (1 tiếng), 1.5, 2.0, ...
    bool booked;
    bool selected;
    bool hovered;
};

// Dialog chính cho booking sân
class SanBookingDialog : public QDialog
{
    Q_OBJECT

signals:
    void khachHangAdded(); // ✅ Signal để thông báo khách hàng mới được thêm
    void lichDatAdded();   // ✅ Signal để thông báo lịch đặt mới được thêm

public:
    explicit SanBookingDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr);

private slots:
    void onDateChanged();
    void onLoaiSanChanged();
    void onGioLocChanged(); // ✅ MỚI: Lọc theo giờ
    void onRefresh();
    void onTimeSlotClicked(TimeSlotWidget *widget);
    void onDatSan();
    void onClearSelection();

private:
    void setupUI();
    void loadSanBookingGrid();
    void updateBookingStatus();
    QString formatTime(int hour, int minute = 0) const;

    QuanLyThueSan *quanLy;

    // UI Components
    QDateEdit *dateEdit;
    QComboBox *cboLoaiSan;
    QComboBox *cboGioLoc;    // ✅ Combobox lọc theo giờ
    QComboBox *cboThoiLuong; // ✅ Combobox chọn thời lượng đặt sân
    // ✅ REMOVED: cboKhachHang - Sẽ chọn khi xác nhận đặt sân
    QTableWidget *bookingGrid;
    QPushButton *btnDatSan;
    QPushButton *btnClearSelection;
    QLabel *lblThongTin;
    QLabel *lblChiPhi;

    // Data
    QDate currentDate;
    int loaiSanFilter; // 0 = all, 5, 7, 11
    int gioLocBatDau;  // ✅ MỚI: Giờ bắt đầu lọc (0 = tất cả)
    int gioLocKetThuc; // ✅ MỚI: Giờ kết thúc lọc
    MangDong<SanBong> dsSanHienThi;

    // Selection state
    QList<TimeSlotWidget *> selectedSlots;

    // Constants
    static const int GIO_BAT_DAU = 6;   // 6:00
    static const int GIO_KET_THUC = 22; // 22:00
    static const int SO_KHUNG_GIO = 16; // 6-22 = 16 giờ
};

#endif // SANBOOKINGDIALOG_H
