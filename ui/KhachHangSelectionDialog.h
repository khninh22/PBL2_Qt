#ifndef KHACHHANGSELECTIONDIALOG_H
#define KHACHHANGSELECTIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include "../core/QuanLyThueSan.h"
#include "../core/DichVu.h"

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
 * Dialog thống nhất cho phép:
 * TAB 1 - KHÁCH HÀNG:
 *   1. Chọn khách hàng có sẵn từ danh sách
 *   2. Thêm khách hàng mới với đầy đủ thông tin
 * TAB 2 - DỊCH VỤ:
 *   3. Chọn dịch vụ cần sử dụng (không bắt buộc)
 *
 * Flow: Đặt sân -> Dialog này (KH + DV) -> Xác nhận
 */
class KhachHangSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KhachHangSelectionDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr);

    /**
     * Trả về mã khách hàng đã chọn hoặc mới tạo
     * Trả về empty string nếu user hủy
     */
    QString getSelectedMaKH() const { return selectedMaKH; }

    /**
     * Trả về danh sách dịch vụ đã chọn
     */
    QList<DichVuInfo> getSelectedDichVu() const { return selectedDichVu; }
    double getTongTienDichVu() const { return tongTienDichVu; }

private slots:
    // Tab Khách hàng
    void onModeChanged();
    void onConfirm();
    void onSearchKhachHang();
    
    // Tab Dịch vụ
    void onDichVuCheckChanged(Qt::CheckState state);
    void onDichVuSpinChanged(int value);

private:
    void setupUI();
    void setupTabKhachHang();
    void setupTabDichVu();
    
    bool validateNewCustomer();
    QString generateMaKH(); // Tự động tạo mã KH mới
    
    void loadDichVu();
    void updateDichVuTongTien();

    QuanLyThueSan *quanLy;
    QString selectedMaKH;

    // Main layout
    QTabWidget *tabWidget;

    // ========== TAB 1: KHÁCH HÀNG ==========
    QWidget *tabKhachHang;
    
    // Mode selection
    QRadioButton *rbChonCoSan;
    QRadioButton *rbThemMoi;
    QStackedWidget *stackedWidget;

    // Page 1: Chọn khách hàng có sẵn
    QWidget *pageChonKH;
    QLineEdit *txtTimKiem;
    QComboBox *cboKhachHang;
    QLabel *lblThongTinKH;

    // Page 2: Thêm khách hàng mới
    QWidget *pageThemMoi;
    QLineEdit *txtHoTen;
    QLineEdit *txtSDT;

    // ========== TAB 2: DỊCH VỤ ==========
    QWidget *tabDichVu;
    QTableWidget *tableDichVu;
    QLabel *lblTongTienDichVu;
    
    // Data
    MangDong<DichVu> dsDichVu;
    QList<DichVuInfo> selectedDichVu;
    double tongTienDichVu;

    // Buttons
    QPushButton *btnConfirm;
    QPushButton *btnCancel;
};

#endif // KHACHHANGSELECTIONDIALOG_H
