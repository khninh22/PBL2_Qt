#ifndef LICHDATMANAGER_H
#define LICHDATMANAGER_H

#include "../LichDatSan.h"
#include "../SanBong.h"
#include "../KhachHang.h"
#include "../MangDong.h"
#include "../HashTable.h"
#include <string>
#include <ctime>
using namespace std;

// Forward declaration
class SanBongManager;
class KhachHangManager;

// ============================
//  LICH DAT MANAGER
//  Quản lý toàn bộ nghiệp vụ liên quan đến lịch đặt sân
// ============================

class LichDatManager
{
private:
    MangDong<LichDatSan> dsLichDatSan;
    HashTable<LichDatSan> indexLichDatSan;

    // Cấu hình giờ
    static const int GIO_MO_CUA = 6;
    static const int GIO_DONG_CUA = 22;

    // Tính tiền theo khung giờ
    double donGiaBanNgay(int loaiSan);
    double donGiaBanToi(int loaiSan);
    double tinhTienTheoKhoang(int loaiSan, time_t batDau, time_t ketThuc);

public:
    // Constructor
    LichDatManager();

    // Getter cho danh sách
    const MangDong<LichDatSan> &getDsLichDatSan() const { return dsLichDatSan; }
    MangDong<LichDatSan> &getDsLichDatSanRef() { return dsLichDatSan; }

    // Tìm kiếm - O(1) với Hash Table
    LichDatSan *timLichDat(const string &maLichDat);

    // Kiểm tra trùng lịch
    bool kiemTraTrungLich(const string &maSan, time_t batDau, time_t ketThuc);

    // Đặt sân (cần access đến SanBongManager và KhachHangManager)
    bool datSan(const string &maKH, const string &maSan, time_t batDau, time_t ketThuc,
                string &maLichMoi, SanBongManager *sanMgr, KhachHangManager *khMgr);

    // Hủy lịch
    bool huyLichDat(const string &maLichDat);

    // Tính tiền đặt sân
    double tinhTienDatSan(int loaiSan, time_t batDau, time_t ketThuc);

    // Tạo mã lịch mới
    string taoMaLichMoi();

    // Sắp xếp
    void sapXepTheoThoiGian(bool tangDan = true);

    // Index Management
    void rebuildIndex();

    // File I/O
    void luuFile(const string &filename);
    void docFile(const string &filename);
};

#endif // LICHDATMANAGER_H
