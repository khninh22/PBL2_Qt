#ifndef DICHVUSELECTIONDIALOG_H
#define DICHVUSELECTIONDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include "../core/QuanLyThueSan.h"
#include "../core/ChiTietDichVu.h"

// Struct để lưu thông tin dịch vụ đã chọn
struct DichVuSelected
{
    std::string maDichVu;
    std::string tenDichVu;
    double donGia;
    int soLuong;
    double thanhTien; // = donGia * soLuong

    DichVuSelected(const std::string &maDV, const std::string &tenDV,
                   double gia, int sl)
        : maDichVu(maDV), tenDichVu(tenDV), donGia(gia), soLuong(sl)
    {
        thanhTien = donGia * soLuong;
    }
};

/**
 * @brief Dialog cho phép chọn dịch vụ khi đặt sân
 *
 * Flow:
 * 1. Hiển thị danh sách dịch vụ với checkbox + spinbox số lượng
 * 2. Tính tổng tiền tự động
 * 3. Có thể bỏ qua (không chọn dịch vụ)
 * 4. Trả về danh sách dịch vụ đã chọn
 */
class DichVuSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DichVuSelectionDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr);

    // Lấy danh sách dịch vụ đã chọn
    QList<DichVuSelected> getSelectedDichVu() const;

    // Tính tổng tiền dịch vụ
    double getTongTienDichVu() const;

private slots:
    void onTableItemChanged(int row, int column);
    void onSpinBoxValueChanged(int value);
    void onConfirm();
    void onSkip();

private:
    void setupUI();
    void loadDichVu();
    void updateTongTien();

    QuanLyThueSan *quanLy;

    // UI Components
    QTableWidget *tableDichVu;
    QLabel *lblTongTien;
    QPushButton *btnConfirm;
    QPushButton *btnSkip;

    // Data
    MangDong<DichVu> dsDichVu;
    QList<DichVuSelected> selectedDichVu;
    double tongTien;
};

#endif // DICHVUSELECTIONDIALOG_H
