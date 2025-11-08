#include "ThongKeKhachHang.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Constructor
ThongKeKhachHang::ThongKeKhachHang(KhachHangManager *khMgr, LichDatManager *ldMgr,
                                   const NgayThang &tu, const NgayThang &den)
    : ThongKe(tu, den, "Thống Kê Khách Hàng"), khachHangMgr(khMgr), lichDatMgr(ldMgr),
      tongSoKhachHang(0), khachHangCoBooking(0)
{
}

// tinhToan - Calculate customer statistics
void ThongKeKhachHang::tinhToan()
{
    if (!khachHangMgr || !lichDatMgr)
        return;
    
    time_t tuNgayT = tuNgay.toTimeT();
    time_t denNgayT = denNgay.toTimeT();
    denNgayT += 86400; // Add 1 day
    
    soLanDatTheoKH.clear();
    topKhachHang.clear();
    
    // Count bookings per customer
    const MangDong<LichDatSan> &dsLich = lichDatMgr->getDsLichDatSan();
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        time_t thoiGian = lich.getThoiGianBatDauTimeT();
        
        if (thoiGian >= tuNgayT && thoiGian < denNgayT)
        {
            soLanDatTheoKH[lich.getMaKH()]++;
        }
    }
    
    // Build ThongTinKhachHang list
    for (const auto &pair : soLanDatTheoKH)
    {
        const std::string &maKH = pair.first;
        int soLan = pair.second;
        
        KhachHang *kh = khachHangMgr->timKhachHang(maKH);
        if (kh)
        {
            double tongTien = 0.0;
            for (int i = 0; i < dsLich.getKichThuoc(); i++)
            {
                const LichDatSan &lich = dsLich[i];
                if (lich.getMaKH() == maKH)
                {
                    time_t thoiGian = lich.getThoiGianBatDauTimeT();
                    if (thoiGian >= tuNgayT && thoiGian < denNgayT && 
                        lich.getTrangThaiTT() == "Đã Thanh Toán")
                    {
                        tongTien += lich.getTongTien();
                    }
                }
            }
            
            topKhachHang.push_back(ThongTinKhachHang(maKH, kh->getHoTen(), soLan, tongTien));
        }
    }
    
    // Sort by revenue using QuickSort
    if (!topKhachHang.empty())
    {
        // Simple bubble sort for now (or implement vector quicksort)
        for (size_t i = 0; i < topKhachHang.size(); i++)
        {
            for (size_t j = i + 1; j < topKhachHang.size(); j++)
            {
                if (topKhachHang[j] > topKhachHang[i])
                {
                    ThongTinKhachHang temp = topKhachHang[i];
                    topKhachHang[i] = topKhachHang[j];
                    topKhachHang[j] = temp;
                }
            }
        }
    }
    
    tongSoKhachHang = khachHangMgr->getDsKhachHang().getKichThuoc();
    khachHangCoBooking = soLanDatTheoKH.size();
}

// hienThi - Console output
void ThongKeKhachHang::hienThi() const
{
    std::cout << "=== " << tieuDe << " ===" << std::endl;
    std::cout << "Từ ngày: " << tuNgay.toString() << " đến " << denNgay.toString() << std::endl;
    std::cout << "Tổng số khách hàng: " << tongSoKhachHang << std::endl;
    std::cout << "Khách hàng có booking: " << khachHangCoBooking << std::endl;
    
    std::cout << "\nTop 10 Khách Hàng VIP:" << std::endl;
    std::cout << std::setw(15) << "Mã KH" << std::setw(25) << "Tên" 
              << std::setw(15) << "Số lần đặt" << std::setw(20) << "Tổng chi tiêu" << std::endl;
    std::cout << std::string(75, '-') << std::endl;
    
    for (size_t i = 0; i < topKhachHang.size() && i < 10; i++)
    {
        const auto &kh = topKhachHang[i];
        std::cout << std::setw(15) << kh.maKH << std::setw(25) << kh.tenKH
                  << std::setw(15) << kh.soLanDat 
                  << std::setw(20) << std::fixed << std::setprecision(0) << kh.tongChiTieu << std::endl;
    }
}

// toText - Export to string
std::string ThongKeKhachHang::toText() const
{
    std::ostringstream oss;
    oss << tieuDe << "\n";
    oss << "Từ: " << tuNgay.toString() << " - Đến: " << denNgay.toString() << "\n";
    oss << "Tổng khách hàng: " << tongSoKhachHang << "\n";
    oss << "Có booking: " << khachHangCoBooking << "\n\n";
    oss << "TOP KHÁCH HÀNG VIP:\n";
    
    for (size_t i = 0; i < topKhachHang.size() && i < 10; i++)
    {
        const auto &kh = topKhachHang[i];
        oss << (i + 1) << ". " << kh.tenKH << " (" << kh.maKH << "): "
            << kh.soLanDat << " lần, " << kh.tongChiTieu << " VNĐ\n";
    }
    
    return oss.str();
}

// getTopKhachHang
std::vector<ThongTinKhachHang> ThongKeKhachHang::getTopKhachHang(int limit)
{
    std::vector<ThongTinKhachHang> result;
    for (size_t i = 0; i < topKhachHang.size() && i < (size_t)limit; i++)
    {
        result.push_back(topKhachHang[i]);
    }
    return result;
}

// topKhachHangVIP - Wrapper for compatibility
std::vector<ThongTinKhachHang> ThongKeKhachHang::topKhachHangVIP(int limit)
{
    tinhToan(); // Ensure fresh calculation
    return getTopKhachHang(limit);
}

// demSoLanDatCuaKhachHang
int ThongKeKhachHang::demSoLanDatCuaKhachHang(const std::string &maKH)
{
    auto it = soLanDatTheoKH.find(maKH);
    if (it != soLanDatTheoKH.end())
        return it->second;
    return 0;
}
