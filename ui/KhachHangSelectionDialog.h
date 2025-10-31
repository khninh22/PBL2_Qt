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
#include <QDate>
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

// ✅ Struct để lưu thông tin sân đã chọn
struct BookingSlot
{
    QString maSan;
    QString tenSan;
    QString thoiGian; // VD: "06:00 - 07:30"
    double soGio;
    double giaTien;
};

/**
 * Dialog HOÀN CHỈNH cho quy trình đặt sân:
 * TAB 1 - KHÁCH HÀNG: Chọn KH có sẵn hoặc thêm mới
 * TAB 2 - DỊCH VỤ: Chọn dịch vụ (tùy chọn)
 * TAB 3 - TÓM TẮT: Xem thông tin đầy đủ và xác nhận
 *
 * Flow: Đặt sân -> Dialog này (KH + DV + Tóm tắt) -> XÁC NHẬN LƯU LUÔN
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

    /**
     * ✅ Truyền thông tin đặt sân từ SanBookingDialog
     */
    void setBookingInfo(const QDate &ngayDat, const QList<BookingSlot> &slotList);
    
    /**
     * ✅ Kiểm tra xem đã xác nhận đặt sân hay chưa
     */
    bool isBookingConfirmed() const { return bookingConfirmed; }

private slots:
    // Tab Khách hàng
    void onModeChanged();
    void onSearchKhachHang();
    
    // Tab Dịch vụ
    void onDichVuCheckChanged(Qt::CheckState state);
    void onDichVuSpinChanged(int value);
    
    // ✅ Tab Tóm tắt & Xác nhận
    void onNextToSummary(); // Chuyển sang tab tóm tắt
    void onConfirmBooking(); // Xác nhận đặt sân (lưu vào DB)

private:
    void setupUI();
    void setupTabKhachHang();
    void setupTabDichVu();
    void setupTabTomTat(); // ✅ Tab 3: Tóm tắt
    
    bool validateNewCustomer();
    bool validateCustomerSelection(); // ✅ Validate trước khi chuyển tab
    QString generateMaKH(); // Tự động tạo mã KH mới
    
    void loadDichVu();
    void updateDichVuTongTien();
    void updateSummary(); // ✅ Cập nhật nội dung tab tóm tắt

    QuanLyThueSan *quanLy;
    QString selectedMaKH;
    bool bookingConfirmed; // ✅ Đánh dấu đã xác nhận đặt sân

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
    
    // ========== TAB 3: TÓM TẮT ==========
    QWidget *tabTomTat;
    QLabel *lblTomTatNoiDung; // Hiển thị HTML tóm tắt đầy đủ
    QLabel *lblTongThanhToan;  // Tổng tiền lớn, nổi bật
    
    // ✅ Thông tin đặt sân được truyền từ SanBookingDialog
    QDate ngayDat;
    QList<BookingSlot> bookingSlots;
    double tongTienSan; // Tổng tiền sân (trước giảm giá)

    // Buttons
    QPushButton *btnNext;      // "Tiếp tục →" (hiện ở tab 1 và 2)
    QPushButton *btnConfirm;   // "✅ Xác nhận đặt sân" (hiện ở tab 3)
    QPushButton *btnCancel;
};

#endif // KHACHHANGSELECTIONDIALOG_H
