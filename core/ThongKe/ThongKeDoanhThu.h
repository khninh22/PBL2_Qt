#ifndef THONGKEDOANHTHU_H
#define THONGKEDOANHTHU_H

#include "ThongKe.h"
#include "../managers/ThanhToanManager.h"
#include "../managers/LichDatManager.h"
#include <map>

/**
 * @brief Thống kê doanh thu theo khoảng thời gian
 * @details PRACTICAL APPLICATION:
 *          - Revenue analysis by day/month/quarter/year
 *          - Growth rate calculation
 *          - Average revenue calculation
 */
class ThongKeDoanhThu : public ThongKe
{
private:
    ThanhToanManager *thanhToanMgr;
    LichDatManager *lichDatMgr;
    
    // Results
    double tongDoanhThu;
    double doanhThuTrungBinh;
    std::map<std::string, double> doanhThuTheoNgay; // "DD/MM/YYYY" -> revenue
    int soLuongGiaoDich;

public:
    // Constructor
    ThongKeDoanhThu(ThanhToanManager *ttMgr, LichDatManager *ldMgr,
                    const NgayThang &tu, const NgayThang &den);
    
    // Getters
    double getTongDoanhThu() const { return tongDoanhThu; }
    double getDoanhThuTrungBinh() const { return doanhThuTrungBinh; }
    int getSoLuongGiaoDich() const { return soLuongGiaoDich; }
    std::map<std::string, double> getDoanhThuTheoNgay() const { return doanhThuTheoNgay; }
    
    // Override virtual methods
    virtual void tinhToan() override;
    virtual void hienThi() const override;
    virtual std::string toText() const override;
    
    // Specific methods
    double tinhDoanhThuThang(int thang, int nam);
    double tinhDoanhThuQuy(int quy, int nam);
    double tinhDoanhThuNam(int nam);
};

#endif // THONGKEDOANHTHU_H
