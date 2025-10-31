#ifndef QUANLYTHUESAN_H
#define QUANLYTHUESAN_H

// ✅ FACADE PATTERN: Include các Manager thay vì data structures
#include "managers/SanBongManager.h"
#include "managers/KhachHangManager.h"
#include "managers/LichDatManager.h"
#include "managers/DichVuManager.h"
#include "managers/NhanVienManager.h"
#include "managers/ThanhToanManager.h"

// Include các class cần thiết
#include "SanBong.h"
#include "KhachHang.h"
#include "LichDatSan.h"
#include "DichVu.h"
#include "ChiTietDichVu.h"
#include "NhanVien.h"
#include "MangDong.h"
#include <string>
using namespace std;

// ============================
//  QUAN LY THUE SAN - FACADE PATTERN
//  Class trung tâm delegate calls tới các managers
//  Giữ nguyên interface cũ để tương thích với UI
// ============================

class QuanLyThueSan
{
private:
    // ✅ MANAGERS - Single Responsibility Principle
    SanBongManager sanBongMgr;
    KhachHangManager khachHangMgr;
    LichDatManager lichDatMgr;
    DichVuManager dichVuMgr;
    NhanVienManager nhanVienMgr;
    ThanhToanManager thanhToanMgr;

public:
    // Constructor
    QuanLyThueSan();

    // ============================
    //  GETTERS - Delegate to Managers
    // ============================
    const MangDong<SanBong> &getDsSanBong() const { return sanBongMgr.getDsSanBong(); }
    const MangDong<KhachHang> &getDsKhachHang() const { return khachHangMgr.getDsKhachHang(); }
    const MangDong<LichDatSan> &getDsLichDatSan() const { return lichDatMgr.getDsLichDatSan(); }
    const MangDong<DichVu> &getDsDichVu() const { return dichVuMgr.getDsDichVu(); }
    const MangDong<ChiTietDichVu> &getDsChiTietDichVu() const { return dichVuMgr.getDsChiTietDichVu(); }
    const MangDong<NhanVien> &getDsNhanVien() const { return nhanVienMgr.getDsNhanVien(); }

    // ============================
    //  TÌM KIẾM - Delegate to Managers (O(1) với Hash Table)
    // ============================
    SanBong *timSanBong(const string &maSan) { return sanBongMgr.timSanBong(maSan); }
    KhachHang *timKhachHang(const string &maKH) { return khachHangMgr.timKhachHang(maKH); }
    LichDatSan *timLichDat(const string &maLichDat) { return lichDatMgr.timLichDat(maLichDat); }
    DichVu *timDichVu(const string &maDV) { return dichVuMgr.timDichVu(maDV); }
    NhanVien *timNhanVien(const string &maNV) { return nhanVienMgr.timNhanVien(maNV); }

    // ============================
    //  QUẢN LÝ SÂN BÓNG
    // ============================
    bool themSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue);
    bool suaSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue);
    bool xoaSanBong(const string &maSan);
    bool batBaoTriSan(const string &maSan);
    bool tatBaoTriSan(const string &maSan);
    MangDong<SanBong> timSanTrong(int loaiSan, time_t batDau, time_t ketThuc);
    void sapXepSanBongTheoGia(bool tangDan = true);

    // ============================
    //  QUẢN LÝ KHÁCH HÀNG
    // ============================
    bool themKhachHang(const string &maKH, const string &hoTen, const string &sdt);
    bool suaKhachHang(const string &maKH, const string &hoTen, const string &sdt);
    bool xoaKhachHang(const string &maKH);
    void sapXepKhachHangTheoDiem(bool tangDan = false);

    // ============================
    //  QUẢN LÝ LỊCH ĐẶT SÂN
    // ============================
    bool datSan(const string &maKH, const string &maSan, time_t batDau, time_t ketThuc, string &maLichMoi);
    bool huyLichDat(const string &maLichDat);
    bool kiemTraTrungLich(const string &maSan, time_t batDau, time_t ketThuc);
    double tinhTienDatSan(int loaiSan, time_t batDau, time_t ketThuc);
    string taoMaLichMoi();
    void sapXepLichDatTheoThoiGian(bool tangDan = true);

    // ============================
    //  QUẢN LÝ DỊCH VỤ
    // ============================
    bool themDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV);
    bool suaDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV);
    bool xoaDichVu(const string &maDV);
    bool themDichVuVaoLich(const string &maLichDat, const string &maDV, int soLuong);

    // ============================
    //  QUẢN LÝ NHÂN VIÊN
    // ============================
    bool themNhanVien(const string &maNV, const string &hoTen, const string &sdt,
                      const string &viTri, double luong,
                      const string &taiKhoan = "", const string &matKhau = "");
    bool suaNhanVien(const string &maNV, const string &hoTen, const string &sdt,
                     const string &viTri, double luong,
                     const string &taiKhoan = "", const string &matKhau = "");
    bool xoaNhanVien(const string &maNV);

    // ============================
    //  THANH TOÁN & THỐNG KÊ
    // ============================
    bool thanhToan(const string &maLichDat, double &tongTien, double &giamGia);
    
    // Thống kê doanh thu cơ bản
    double tinhDoanhThuThang(int thang, int nam);
    double tinhDoanhThuNgay(time_t ngay);
    
    // ✅ MỚI: Thống kê doanh thu nâng cao
    double tinhDoanhThuQuy(int quy, int nam);
    double tinhDoanhThuNam(int nam);
    double tinhDoanhThuKhoang(time_t tuNgay, time_t denNgay);
    
    // ✅ MỚI: Thống kê sân bóng
    MangDong<ThongKeSan> thongKeTanSuatSan(time_t tuNgay, time_t denNgay);
    double tinhTyLelapDay(const string &maSan, time_t tuNgay, time_t denNgay);
    
    // ✅ MỚI: Thống kê khung giờ
    MangDong<ThongKeKhungGio> thongKeKhungGio(time_t tuNgay, time_t denNgay);
    
    // ✅ MỚI: Thống kê khách hàng
    MangDong<ThongKeKhachHang> topKhachHangVIP(int top, time_t tuNgay, time_t denNgay);
    
    // ✅ MỚI: Thống kê tổng quan
    int demTongLichDat(time_t tuNgay, time_t denNgay);
    double tinhTyLeHuyLich(time_t tuNgay, time_t denNgay);
    double tinhDoanhThuTrungBinh(time_t tuNgay, time_t denNgay);

    // ============================
    //  FILE I/O - Lưu/Tải dữ liệu
    // ============================
    void taiDuLieu();
    void luuDuLieu();
    void khoiTaoDuLieuMau();

    // ============================
    //  REBUILD INDEX (gọi cho tất cả managers)
    // ============================
    void rebuildIndex();
};

#endif // QUANLYTHUESAN_H
