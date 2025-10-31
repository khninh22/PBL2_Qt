#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QFrame>
#include <QPropertyAnimation>
#include <QComboBox>
#include <QDateEdit>
#include "../core/QuanLyThueSan.h"
#include "../core/AuthManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void logoutRequested();

public:
    MainWindow(AuthManager *auth, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Sidebar navigation
    void toggleSidebar();
    void navigateToPage(int index);
    // Sân bóng
    void onThemSanBong();
    void onSuaSanBong();
    void onXoaSanBong();
    void onBaoTriSan();
    void onRefreshSanBong();

    // Khách hàng
    void onThemKhachHang();
    void onSuaKhachHang();
    void onXoaKhachHang();
    void onRefreshKhachHang();

    // Lịch đặt sân
    void onDatSanTrucQuan(); // Giao diện đặt sân trực quan
    void onHuyLich();
    void onThanhToan();
    void onRefreshLichDat();

    // Dịch vụ
    void onThemDichVu();
    void onSuaDichVu();
    void onXoaDichVu();
    void onRefreshDichVu();

    // Nhân viên
    void onThemNhanVien();
    void onSuaNhanVien();
    void onXoaNhanVien();
    void onRefreshNhanVien();

    // Thống kê
    void onXemThongKe();

    // Authentication
    void onLogout();
    void onChangePassword();

private:
    void setupUI();
    void setupSidebar();
    void setupContent();
    void applyAccessControl(); // Áp dụng kiểm soát quyền truy cập

    // Menu items
    QPushButton *createMenuButton(const QString &icon, const QString &text, int pageIndex);

    // Pages
    QWidget *createSanBongPage();
    QWidget *createKhachHangPage();
    QWidget *createLichDatPage();
    QWidget *createDichVuPage();
    QWidget *createNhanVienPage();
    QWidget *createThongKePage();

    // Update tables
    void updateSanBongTable();
    void updateKhachHangTable();
    void updateLichDatTable();
    void updateDichVuTable();
    void updateNhanVienTable();
    void updateThongKeDisplay();

    // Core
    QuanLyThueSan *quanLy;
    AuthManager *authManager;

    // UI Components - New Layout
    QFrame *sidebar;
    QStackedWidget *stackedWidget;
    QPushButton *btnToggleSidebar;
    QLabel *lblLogo;
    bool sidebarVisible;

    // Menu buttons
    QList<QPushButton *> menuButtons;
    QPushButton *btnMenuSanBong;
    QPushButton *btnMenuKhachHang;
    QPushButton *btnMenuLichDat;
    QPushButton *btnMenuDichVu;
    QPushButton *btnMenuNhanVien;
    QPushButton *btnMenuThongKe;
    QPushButton *btnMenuLogout;

    // Sân bóng
    QTableWidget *tableSanBong;
    QPushButton *btnThemSan;
    QPushButton *btnSuaSan;
    QPushButton *btnXoaSan;
    QPushButton *btnBaoTri;

    // Khách hàng
    QTableWidget *tableKhachHang;
    QPushButton *btnThemKH;
    QPushButton *btnSuaKH;
    QPushButton *btnXoaKH;

    // Lịch đặt
    QTableWidget *tableLichDat;
    QPushButton *btnDatSan;
    QPushButton *btnHuyLich;
    QPushButton *btnThanhToan;

    // Dịch vụ
    QTableWidget *tableDichVu;
    QPushButton *btnThemDV;
    QPushButton *btnSuaDV;
    QPushButton *btnXoaDV;

    // Nhân viên
    QTableWidget *tableNhanVien;
    QPushButton *btnThemNV;
    QPushButton *btnSuaNV;
    QPushButton *btnXoaNV;

    // Thống kê cơ bản (tab cũ)
    QLabel *lblDoanhThuNgay;
    QLabel *lblDoanhThuThang;
    QLabel *lblTongSan;
    QLabel *lblTongKhach;
    
    // ✅ MỚI: Thống kê nâng cao
    QComboBox *cboKhoangThoiGian;  // Chọn: Ngày/Tuần/Tháng/Quý/Năm/Tùy chỉnh
    QDateEdit *dateFrom;
    QDateEdit *dateTo;
    QPushButton *btnThongKe;
    
    // Hiển thị doanh thu theo khoảng
    QLabel *lblDoanhThuKhoang;
    QLabel *lblTongBooking;
    QLabel *lblDoanhThuTB;
    QLabel *lblTyLeHuy;
    
    // Bảng thống kê sân
    QTableWidget *tableThongKeSan;
    
    // Bảng khung giờ hot
    QTableWidget *tableKhungGio;
    
    // Bảng Top khách hàng VIP
    QTableWidget *tableTopKhach;
};

#endif // MAINWINDOW_H
