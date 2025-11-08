#ifndef THONGKEKHACHHANG_H
#define THONGKEKHACHHANG_H

#include "ThongKe.h"
#include "../managers/KhachHangManager.h"
#include "../managers/LichDatManager.h"
#include <vector>
#include <map>

/**
 * @brief Customer analytics and VIP ranking
 * @details PRACTICAL APPLICATION:
 *          - Top customer identification
 *          - Customer booking frequency analysis
 *          - VIP customer rewards program
 */

// ✅ CLASS cho thông tin khách hàng (OOP thay vì struct)
class ThongTinKhachHang
{
public:
    std::string maKH;
    std::string tenKH;
    int soLanDat;
    double tongChiTieu;
    
    ThongTinKhachHang() : soLanDat(0), tongChiTieu(0.0) {}
    ThongTinKhachHang(const std::string &ma, const std::string &ten, int solan, double tien)
        : maKH(ma), tenKH(ten), soLanDat(solan), tongChiTieu(tien) {}
    
    // For sorting by revenue
    bool operator>(const ThongTinKhachHang &other) const
    {
        return tongChiTieu > other.tongChiTieu;
    }
};

class ThongKeKhachHang : public ThongKe
{
private:
    KhachHangManager *khachHangMgr;
    LichDatManager *lichDatMgr;
    
    // Results
    std::vector<ThongTinKhachHang> topKhachHang;
    std::map<std::string, int> soLanDatTheoKH; // maKH -> count
    int tongSoKhachHang;
    int khachHangCoBooking;

public:
    // Constructor
    ThongKeKhachHang(KhachHangManager *khMgr, LichDatManager *ldMgr,
                     const NgayThang &tu, const NgayThang &den);
    
    // Getters
    std::vector<ThongTinKhachHang> getTopKhachHang(int limit = 10);
    int getTongSoKhachHang() const { return tongSoKhachHang; }
    int getKhachHangCoBooking() const { return khachHangCoBooking; }
    
    // Override virtual methods
    virtual void tinhToan() override;
    virtual void hienThi() const override;
    virtual std::string toText() const override;
    
    // Specific methods
    std::vector<ThongTinKhachHang> topKhachHangVIP(int limit = 10);
    int demSoLanDatCuaKhachHang(const std::string &maKH);
};

#endif // THONGKEKHACHHANG_H
