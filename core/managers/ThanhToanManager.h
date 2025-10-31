#ifndef THANHTOANMANAGER_H
#define THANHTOANMANAGER_H

#include "../LichDatSan.h"
#include "../KhachHang.h"
#include "../SanBong.h"
#include "../MangDong.h"
#include <string>
#include <ctime>
#include <map>
using namespace std;

// Forward declarations
class LichDatManager;
class KhachHangManager;
class SanBongManager;

// Struct cho thống kê sân
struct ThongKeSan {
    string maSan;
    string tenSan;
    int soLanDat;
    double doanhThu;
    double tyLelapDay; // % thời gian được sử dụng
};

// Struct cho thống kê khách hàng
struct ThongKeKhachHang {
    string maKH;
    string hoTen;
    int soLanDat;
    double tongChiTieu;
};

// Struct cho thống kê khung giờ
struct ThongKeKhungGio {
    int gio; // 6-22
    int soLanDat;
    double doanhThu;
};

// ============================
//  THANH TOAN MANAGER
//  Quản lý thanh toán và thống kê doanh thu nâng cao
// ============================

class ThanhToanManager
{
public:
    // Constructor
    ThanhToanManager();

    // Thanh toán (cần access LichDatManager và KhachHangManager)
    bool thanhToan(const string &maLichDat, double &tongTien, double &giamGia,
                   LichDatManager *lichMgr, KhachHangManager *khMgr);

    // ========== THỐNG KÊ DOANH THU ==========
    double tinhDoanhThuNgay(time_t ngay, LichDatManager *lichMgr);
    double tinhDoanhThuThang(int thang, int nam, LichDatManager *lichMgr);
    double tinhDoanhThuQuy(int quy, int nam, LichDatManager *lichMgr); // Quý 1-4
    double tinhDoanhThuNam(int nam, LichDatManager *lichMgr);
    double tinhDoanhThuKhoang(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr);
    
    // ========== THỐNG KÊ SÂN BÓNG ==========
    // Tần suất sử dụng từng sân (Top sân hot)
    MangDong<ThongKeSan> thongKeTanSuatSan(time_t tuNgay, time_t denNgay, 
                                           LichDatManager *lichMgr, SanBongManager *sanMgr);
    
    // Tỷ lệ lấp đầy sân (% thời gian sân được sử dụng)
    double tinhTyLelapDay(const string &maSan, time_t tuNgay, time_t denNgay, LichDatManager *lichMgr);
    
    // ========== THỐNG KÊ KHUNG GIỜ ==========
    // Phân tích khung giờ nào đông khách nhất
    MangDong<ThongKeKhungGio> thongKeKhungGio(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr);
    
    // ========== THỐNG KÊ KHÁCH HÀNG ==========
    // Top khách hàng VIP (chi tiêu nhiều nhất)
    MangDong<ThongKeKhachHang> topKhachHangVIP(int top, time_t tuNgay, time_t denNgay,
                                               LichDatManager *lichMgr, KhachHangManager *khMgr);
    
    // ========== THỐNG KÊ TỔNG QUAN ==========
    // Tổng số lịch đặt
    int demTongLichDat(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr);
    
    // Tỷ lệ hủy lịch (%)
    double tinhTyLeHuyLich(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr);
    
    // Doanh thu trung bình mỗi booking
    double tinhDoanhThuTrungBinh(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr);
    
    // Tổng số khách hàng mới trong khoảng thời gian (cần field ngày tạo trong KhachHang - optional)
    // int demKhachHangMoi(time_t tuNgay, time_t denNgay, KhachHangManager *khMgr);
};

#endif // THANHTOANMANAGER_H
