#ifndef BOOKINGCONFIRMDIALOG_H
#define BOOKINGCONFIRMDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QSpinBox>
#include "../core/QuanLyThueSan.h"
#include "../core/ChiTietDichVu.h"

// Struct để lưu thông tin dịch vụ đã chọn
struct DichVuInfo
{
    std::string maDichVu;
    std::string tenDichVu;
    double donGia;
    int soLuong;
    double thanhTien;

    DichVuInfo(const std::string &maDV, const std::string &tenDV, 
               double gia, int sl)
        : maDichVu(maDV), tenDichVu(tenDV), donGia(gia), soLuong(sl)
    {
        thanhTien = donGia * soLuong;
    }
};

/**
 * @brief Dialog thống nhất để chọn khách hàng VÀ dịch vụ trong cùng 1 màn hình
 * 
 * Layout:
 * ┌─────────────────────────────────────────────┐
 * │  📋 THÔNG TIN ĐẶT SÂN                      │
 * ├─────────────────────────────────────────────┤
 * │  [Tab 1: Khách hàng] [Tab 2: Dịch vụ]     │
 * │  ┌───────────────────────────────────────┐ │
 * │  │ Content của tab hiện tại              │ │
 * │  └───────────────────────────────────────┘ │
 * │                                             │
 * │  [Hủy]                          [Xác nhận] │
 * └─────────────────────────────────────────────┘
 */
class BookingConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookingConfirmDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr);

    // Lấy thông tin đã chọn
    QString getSelectedMaKH() const { return selectedMaKH; }
    QList<DichVuInfo> getSelectedDichVu() const { return selectedDichVu; }
    double getTongTienDichVu() const { return tongTienDichVu; }

private slots:
    // Tab Khách hàng
    void onModeChanged();
    void onSearchKhachHang(const QString &text);
    void onTableKhachHangClicked(int row, int column);
    
    // Tab Dịch vụ
    void onDichVuCheckChanged(Qt::CheckState state);
    void onDichVuSpinChanged(int value);
    
    // Buttons
    void onConfirm();
    void onCancel();

private:
    void setupUI();
    void setupTabKhachHang();
    void setupTabDichVu();
    
    void loadKhachHang();
    void loadDichVu();
    void updateDichVuTongTien();
    
    bool validateKhachHang();
    bool validateAndCreateKhachHang();

    QuanLyThueSan *quanLy;

    // UI Components
    QTabWidget *tabWidget;
    QPushButton *btnConfirm;
    QPushButton *btnCancel;

    // ========== TAB 1: KHÁCH HÀNG ==========
    QWidget *tabKhachHang;
    QRadioButton *rbChonKhachHang;
    QRadioButton *rbThemMoi;
    QButtonGroup *modeGroup;
    
    // Chọn khách hàng có sẵn
    QWidget *widgetChonKhachHang;
    QLineEdit *txtSearchKH;
    QTableWidget *tableKhachHang;
    
    // Thêm khách hàng mới
    QWidget *widgetThemMoi;
    QLineEdit *txtHoTen;
    QLineEdit *txtSdt;
    QLabel *lblMaKHMoi;

    // ========== TAB 2: DỊCH VỤ ==========
    QWidget *tabDichVu;
    QTableWidget *tableDichVu;
    QLabel *lblTongTienDichVu;

    // Data
    MangDong<KhachHang> dsKhachHang;
    MangDong<DichVu> dsDichVu;
    
    QString selectedMaKH;
    QList<DichVuInfo> selectedDichVu;
    double tongTienDichVu;
};

#endif // BOOKINGCONFIRMDIALOG_H
