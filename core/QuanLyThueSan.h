#ifndef QUANLYTHUESAN_H
#define QUANLYTHUESAN_H

#include "KhachHang.h"
#include "SanBong.h"
#include "LichDatSan.h"
#include "DichVu.h"
#include "ChiTietDichVu.h"
#include "NhanVien.h"
#include "MangDong.h"
#include "HashTable.h"  // ✅ Hash Table cho tìm kiếm O(1)
#include "Validator.h"  // ✅ MỚI: Validation
#include "Exceptions.h" // ✅ MỚI: Exception handling
#include <string>
using namespace std;

class QuanLyThueSan
{
private:
    // Danh sách chính (MangDong)
    MangDong<SanBong> dsSanBong;
    MangDong<KhachHang> dsKhachHang;
    MangDong<LichDatSan> dsLichDatSan;
    MangDong<DichVu> dsDichVu;
    MangDong<ChiTietDichVu> dsChiTietDichVu;
    MangDong<NhanVien> dsNhanVien;

    // ✅ THÊM MỚI: Hash Table Index cho tìm kiếm nhanh O(1)
    HashTable<SanBong> indexSanBong;
    HashTable<KhachHang> indexKhachHang;
    HashTable<LichDatSan> indexLichDatSan;
    HashTable<DichVu> indexDichVu;
    HashTable<NhanVien> indexNhanVien;

    // Cấu hình giờ
    static const int GIO_MO_CUA = 6;
    static const int GIO_DONG_CUA = 22;

public:
    // Constructor
    QuanLyThueSan();

    // Getters cho các danh sách
    const MangDong<SanBong> &getDsSanBong() const { return dsSanBong; }
    const MangDong<KhachHang> &getDsKhachHang() const { return dsKhachHang; }
    const MangDong<LichDatSan> &getDsLichDatSan() const { return dsLichDatSan; }
    const MangDong<DichVu> &getDsDichVu() const { return dsDichVu; }
    const MangDong<ChiTietDichVu> &getDsChiTietDichVu() const { return dsChiTietDichVu; }
    const MangDong<NhanVien> &getDsNhanVien() const { return dsNhanVien; }

    // Tìm kiếm
    SanBong *timSanBong(const string &maSan);
    KhachHang *timKhachHang(const string &maKH);
    LichDatSan *timLichDat(const string &maLichDat);
    DichVu *timDichVu(const string &maDV);
    NhanVien *timNhanVien(const string &maNV);

    // Quản lý sân bóng
    bool themSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue);
    bool suaSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue);
    bool xoaSanBong(const string &maSan);
    bool batBaoTriSan(const string &maSan);
    bool tatBaoTriSan(const string &maSan);

    // Quản lý khách hàng
    bool themKhachHang(const string &maKH, const string &hoTen, const string &sdt);
    bool suaKhachHang(const string &maKH, const string &hoTen, const string &sdt);
    bool xoaKhachHang(const string &maKH);

    // Quản lý lịch đặt sân
    bool datSan(const string &maKH, const string &maSan, time_t batDau, time_t ketThuc, string &maLichMoi);
    bool huyLichDat(const string &maLichDat);
    bool kiemTraTrungLich(const string &maSan, time_t batDau, time_t ketThuc);

    // Quản lý dịch vụ
    bool themDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV);
    bool suaDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV);
    bool xoaDichVu(const string &maDV);
    bool themDichVuVaoLich(const string &maLichDat, const string &maDV, int soLuong);

    // Quản lý nhân viên
    bool themNhanVien(const string &maNV, const string &hoTen, const string &sdt, const string &viTri, double luong, const string &taiKhoan = "", const string &matKhau = "");
    bool suaNhanVien(const string &maNV, const string &hoTen, const string &sdt, const string &viTri, double luong, const string &taiKhoan = "", const string &matKhau = "");
    bool xoaNhanVien(const string &maNV);

    // Thanh toán
    bool thanhToan(const string &maLichDat, double &tongTien, double &giamGia);

    // Thống kê
    double tinhDoanhThuThang(int thang, int nam);
    double tinhDoanhThuNgay(time_t ngay);

    // File I/O
    void taiDuLieu();
    void luuDuLieu();
    void khoiTaoDuLieuMau(); // Khởi tạo dữ liệu mẫu

    // Tiện ích
    double tinhTienDatSan(int loaiSan, time_t batDau, time_t ketThuc);
    MangDong<SanBong> timSanTrong(int loaiSan, time_t batDau, time_t ketThuc);
    string taoMaLichMoi();

    // ✅ THÊM MỚI: Sắp xếp nhanh với Quick Sort
    void sapXepSanBongTheoGia(bool tangDan = true);
    void sapXepKhachHangTheoDiem(bool tangDan = false);
    void sapXepLichDatTheoThoiGian(bool tangDan = true);

    // ✅ THÊM MỚI: Rebuild index sau khi load dữ liệu
    void rebuildIndex();

private:
    // Giá theo khung giờ
    double donGiaBanNgay(int loaiSan);
    double donGiaBanToi(int loaiSan);
    double tinhTienTheoKhoang(int loaiSan, time_t batDau, time_t ketThuc);
};

#endif // QUANLYTHUESAN_H
