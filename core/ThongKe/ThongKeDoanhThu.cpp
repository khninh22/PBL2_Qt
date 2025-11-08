#include "ThongKeDoanhThu.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Constructor
ThongKeDoanhThu::ThongKeDoanhThu(ThanhToanManager *ttMgr, LichDatManager *ldMgr,
                                 const NgayThang &tu, const NgayThang &den)
    : ThongKe(tu, den, "Thống Kê Doanh Thu"), thanhToanMgr(ttMgr), lichDatMgr(ldMgr),
      tongDoanhThu(0), doanhThuTrungBinh(0), soLuongGiaoDich(0)
{
}

// tinhToan - Calculate revenue statistics
void ThongKeDoanhThu::tinhToan()
{
    if (!thanhToanMgr || !lichDatMgr)
        return;
    
    time_t tuNgayT = tuNgay.toTimeT();
    time_t denNgayT = denNgay.toTimeT();
    denNgayT += 86400; // Add 1 day to include end date
    
    // Use ThanhToanManager methods
    tongDoanhThu = thanhToanMgr->tinhDoanhThuKhoang(tuNgayT, denNgayT, lichDatMgr);
    doanhThuTrungBinh = thanhToanMgr->tinhDoanhThuTrungBinh(tuNgayT, denNgayT, lichDatMgr);
    
    // Count transactions
    soLuongGiaoDich = 0;
    const MangDong<LichDatSan> &dsLich = lichDatMgr->getDsLichDatSan();
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        if (lich.getTrangThaiTT() == "Đã Thanh Toán")
        {
            time_t thoiGian = lich.getThoiGianBatDauTimeT();
            if (thoiGian >= tuNgayT && thoiGian < denNgayT)
            {
                soLuongGiaoDich++;
                
                // Group by day
                NgayThang ngay(thoiGian);
                std::string ngayStr = ngay.toString();
                doanhThuTheoNgay[ngayStr] += lich.getTongTien();
            }
        }
    }
}

// hienThi - Console output
void ThongKeDoanhThu::hienThi() const
{
    std::cout << "=== " << tieuDe << " ===" << std::endl;
    std::cout << "Từ ngày: " << tuNgay.toString() << " đến " << denNgay.toString() << std::endl;
    std::cout << "Tổng doanh thu: " << std::fixed << std::setprecision(0) << tongDoanhThu << " VNĐ" << std::endl;
    std::cout << "Doanh thu trung bình: " << doanhThuTrungBinh << " VNĐ/ngày" << std::endl;
    std::cout << "Số lượng giao dịch: " << soLuongGiaoDich << std::endl;
}

// toText - Export to string
std::string ThongKeDoanhThu::toText() const
{
    std::ostringstream oss;
    oss << tieuDe << "\n";
    oss << "Từ: " << tuNgay.toString() << " - Đến: " << denNgay.toString() << "\n";
    oss << "Tổng doanh thu: " << std::fixed << std::setprecision(0) << tongDoanhThu << " VNĐ\n";
    oss << "Trung bình: " << doanhThuTrungBinh << " VNĐ/ngày\n";
    oss << "Số giao dịch: " << soLuongGiaoDich << "\n";
    return oss.str();
}

// tinhDoanhThuThang
double ThongKeDoanhThu::tinhDoanhThuThang(int thang, int nam)
{
    if (!thanhToanMgr || !lichDatMgr)
        return 0.0;
    return thanhToanMgr->tinhDoanhThuThang(thang, nam, lichDatMgr);
}

// tinhDoanhThuQuy
double ThongKeDoanhThu::tinhDoanhThuQuy(int quy, int nam)
{
    if (!thanhToanMgr || !lichDatMgr)
        return 0.0;
    return thanhToanMgr->tinhDoanhThuQuy(quy, nam, lichDatMgr);
}

// tinhDoanhThuNam
double ThongKeDoanhThu::tinhDoanhThuNam(int nam)
{
    if (!thanhToanMgr || !lichDatMgr)
        return 0.0;
    return thanhToanMgr->tinhDoanhThuNam(nam, lichDatMgr);
}
