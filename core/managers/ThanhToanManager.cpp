#include "ThanhToanManager.h"
#include "LichDatManager.h"
#include "KhachHangManager.h"
#include "SanBongManager.h"
#include <map>
#include <algorithm>

// Constructor
ThanhToanManager::ThanhToanManager()
{
    // Không cần khởi tạo gì
}

// Thanh toán
bool ThanhToanManager::thanhToan(const string &maLichDat, double &tongTien, double &giamGia,
                                  LichDatManager *lichMgr, KhachHangManager *khMgr)
{
    if (lichMgr == nullptr || khMgr == nullptr)
        return false;
    
    LichDatSan *lich = lichMgr->timLichDat(maLichDat);
    if (lich == nullptr || lich->getTrangThaiTT() == "Đã Thanh Toán")
        return false;

    tongTien = lich->getTongTien();

    KhachHang *kh = khMgr->timKhachHang(lich->getMaKH());
    if (kh != nullptr)
    {
        double phanTramGiam = kh->tinhPhanTramGiam();
        giamGia = tongTien * phanTramGiam / 100.0;
        tongTien -= giamGia;
        kh->tichDiem(tongTien);
    }
    else
    {
        giamGia = 0.0;
    }

    lich->setTongTien(tongTien);
    lich->setTrangThaiTT("Đã Thanh Toán");
    lich->setThoiGianThanhToan(time(nullptr));

    return true;
}

double ThanhToanManager::tinhDoanhThuThang(int thang, int nam, LichDatManager *lichMgr)
{
    if (lichMgr == nullptr)
        return 0.0;
    
    double tong = 0.0;
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        if (lich.getTrangThaiTT() == "Đã Thanh Toán")
        {
            time_t thoiGian = lich.getThoiGianThanhToan();
            tm t = *localtime(&thoiGian);
            if (t.tm_mon + 1 == thang && t.tm_year + 1900 == nam)
            {
                tong += lich.getTongTien();
            }
        }
    }
    return tong;
}

double ThanhToanManager::tinhDoanhThuNgay(time_t ngay, LichDatManager *lichMgr)
{
    if (lichMgr == nullptr)
        return 0.0;
    
    double tong = 0.0;
    tm ngayTm = *localtime(&ngay);
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();

    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        if (lich.getTrangThaiTT() == "Đã Thanh Toán")
        {
            time_t thoiGian = lich.getThoiGianThanhToan();
            tm t = *localtime(&thoiGian);
            if (t.tm_mday == ngayTm.tm_mday &&
                t.tm_mon == ngayTm.tm_mon &&
                t.tm_year == ngayTm.tm_year)
            {
                tong += lich.getTongTien();
            }
        }
    }
    return tong;
}

double ThanhToanManager::tinhDoanhThuQuy(int quy, int nam, LichDatManager *lichMgr)
{
    if (lichMgr == nullptr || quy < 1 || quy > 4)
        return 0.0;
    
    double tong = 0.0;
    int thangBatDau = (quy - 1) * 3 + 1;
    int thangKetThuc = thangBatDau + 2;
    
    for (int thang = thangBatDau; thang <= thangKetThuc; thang++)
    {
        tong += tinhDoanhThuThang(thang, nam, lichMgr);
    }
    return tong;
}

double ThanhToanManager::tinhDoanhThuNam(int nam, LichDatManager *lichMgr)
{
    if (lichMgr == nullptr)
        return 0.0;
    
    double tong = 0.0;
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        if (lich.getTrangThaiTT() == "Đã Thanh Toán")
        {
            time_t thoiGian = lich.getThoiGianThanhToan();
            tm t = *localtime(&thoiGian);
            if (t.tm_year + 1900 == nam)
            {
                tong += lich.getTongTien();
            }
        }
    }
    return tong;
}

double ThanhToanManager::tinhDoanhThuKhoang(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr)
{
    if (lichMgr == nullptr)
        return 0.0;
    
    double tong = 0.0;
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        if (lich.getTrangThaiTT() == "Đã Thanh Toán")
        {
            time_t thoiGian = lich.getThoiGianThanhToan();
            if (thoiGian >= tuNgay && thoiGian <= denNgay)
            {
                tong += lich.getTongTien();
            }
        }
    }
    return tong;
}

// ========== THỐNG KÊ SÂN BÓNG ==========

MangDong<ThongKeSan> ThanhToanManager::thongKeTanSuatSan(time_t tuNgay, time_t denNgay,
                                                          LichDatManager *lichMgr, SanBongManager *sanMgr)
{
    MangDong<ThongKeSan> ketQua;
    if (lichMgr == nullptr || sanMgr == nullptr)
        return ketQua;
    
    std::map<std::string, std::pair<int, double>> thongKe;
    
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        time_t thoiGian = lich.getThoiGianBatDau();
        
        if (thoiGian >= tuNgay && thoiGian <= denNgay && 
            lich.getTrangThaiDat() == "Đã Đặt")
        {
            thongKe[lich.getMaSan()].first++;
            
            if (lich.getTrangThaiTT() == "Đã Thanh Toán")
                thongKe[lich.getMaSan()].second += lich.getTongTien();
        }
    }
    
    for (auto &item : thongKe)
    {
        ThongKeSan tk;
        tk.maSan = item.first;
        
        SanBong *san = sanMgr->timSanBong(item.first);
        tk.tenSan = san ? san->getTenSan() : "N/A";
        
        tk.soLanDat = item.second.first;
        tk.doanhThu = item.second.second;
        tk.tyLelapDay = tinhTyLelapDay(item.first, tuNgay, denNgay, lichMgr);
        
        ketQua.them(tk);
    }
    
    return ketQua;
}

double ThanhToanManager::tinhTyLelapDay(const std::string &maSan, time_t tuNgay, time_t denNgay, 
                                        LichDatManager *lichMgr)
{
    if (lichMgr == nullptr)
        return 0.0;
    
    const int GIO_MO_CUA = 6;
    const int GIO_DONG_CUA = 22;
    const int GIO_HOAT_DONG_MOI_NGAY = GIO_DONG_CUA - GIO_MO_CUA;
    
    double soNgay = difftime(denNgay, tuNgay) / (24.0 * 3600.0) + 1;
    double tongGioCoThe = soNgay * GIO_HOAT_DONG_MOI_NGAY;
    
    double tongGioDaDat = 0.0;
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        
        if (lich.getMaSan() == maSan && 
            lich.getTrangThaiDat() == "Đã Đặt" &&
            lich.getThoiGianBatDau() >= tuNgay && 
            lich.getThoiGianBatDau() <= denNgay)
        {
            double soGio = difftime(lich.getThoiGianKetThuc(), lich.getThoiGianBatDau()) / 3600.0;
            tongGioDaDat += soGio;
        }
    }
    
    return tongGioCoThe > 0 ? (tongGioDaDat / tongGioCoThe) * 100.0 : 0.0;
}

// ========== THỐNG KÊ KHUNG GIỜ ==========

MangDong<ThongKeKhungGio> ThanhToanManager::thongKeKhungGio(time_t tuNgay, time_t denNgay,
                                                             LichDatManager *lichMgr)
{
    MangDong<ThongKeKhungGio> ketQua;
    if (lichMgr == nullptr)
        return ketQua;
    
    std::map<int, std::pair<int, double>> thongKe;
    
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        
        if (lich.getThoiGianBatDau() >= tuNgay && 
            lich.getThoiGianBatDau() <= denNgay &&
            lich.getTrangThaiDat() == "Đã Đặt")
        {
            time_t batDau = lich.getThoiGianBatDau();
            tm t = *localtime(&batDau);
            int gio = t.tm_hour;
            
            thongKe[gio].first++;
            
            if (lich.getTrangThaiTT() == "Đã Thanh Toán")
                thongKe[gio].second += lich.getTongTien();
        }
    }
    
    for (auto &item : thongKe)
    {
        ThongKeKhungGio tk;
        tk.gio = item.first;
        tk.soLanDat = item.second.first;
        tk.doanhThu = item.second.second;
        ketQua.them(tk);
    }
    
    return ketQua;
}

// ========== THỐNG KÊ KHÁCH HÀNG ==========

// Hàm so sánh cho QuickSort
static bool soSanhThongKeKH(const ThongKeKhachHang &a, const ThongKeKhachHang &b)
{
    return a.tongChiTieu > b.tongChiTieu;
}

MangDong<ThongKeKhachHang> ThanhToanManager::topKhachHangVIP(int top, time_t tuNgay, time_t denNgay,
                                                              LichDatManager *lichMgr, KhachHangManager *khMgr)
{
    MangDong<ThongKeKhachHang> ketQua;
    if (lichMgr == nullptr || khMgr == nullptr)
        return ketQua;
    
    std::map<std::string, std::pair<int, double>> thongKe;
    
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        
        if (lich.getThoiGianBatDau() >= tuNgay && 
            lich.getThoiGianBatDau() <= denNgay &&
            lich.getTrangThaiTT() == "Đã Thanh Toán")
        {
            thongKe[lich.getMaKH()].first++;
            thongKe[lich.getMaKH()].second += lich.getTongTien();
        }
    }
    
    MangDong<ThongKeKhachHang> temp;
    for (auto &item : thongKe)
    {
        ThongKeKhachHang tk;
        tk.maKH = item.first;
        
        KhachHang *kh = khMgr->timKhachHang(item.first);
        tk.hoTen = kh ? kh->getHoTen() : "N/A";
        
        tk.soLanDat = item.second.first;
        tk.tongChiTieu = item.second.second;
        temp.them(tk);
    }
    
    // Sắp xếp theo tổng chi tiêu giảm dần
    quickSort(temp, soSanhThongKeKH);
    
    int soLuong = temp.getKichThuoc() < top ? temp.getKichThuoc() : top;
    for (int i = 0; i < soLuong; i++)
    {
        ketQua.them(temp[i]);
    }
    
    return ketQua;
}

// ========== THỐNG KÊ TỔNG QUAN ==========

int ThanhToanManager::demTongLichDat(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr)
{
    if (lichMgr == nullptr)
        return 0;
    
    int tong = 0;
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        if (lich.getThoiGianBatDau() >= tuNgay && lich.getThoiGianBatDau() <= denNgay)
        {
            tong++;
        }
    }
    return tong;
}

double ThanhToanManager::tinhTyLeHuyLich(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr)
{
    if (lichMgr == nullptr)
        return 0.0;
    
    int tongLich = 0;
    int lichHuy = 0;
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        if (lich.getThoiGianBatDau() >= tuNgay && lich.getThoiGianBatDau() <= denNgay)
        {
            tongLich++;
            if (lich.getTrangThaiDat() == "Đã Hủy")
                lichHuy++;
        }
    }
    
    return tongLich > 0 ? (lichHuy * 100.0 / tongLich) : 0.0;
}

double ThanhToanManager::tinhDoanhThuTrungBinh(time_t tuNgay, time_t denNgay, LichDatManager *lichMgr)
{
    if (lichMgr == nullptr)
        return 0.0;
    
    double tongDoanhThu = 0.0;
    int soBooking = 0;
    const MangDong<LichDatSan> &dsLich = lichMgr->getDsLichDatSan();
    
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLich[i];
        if (lich.getThoiGianBatDau() >= tuNgay && 
            lich.getThoiGianBatDau() <= denNgay &&
            lich.getTrangThaiTT() == "Đã Thanh Toán")
        {
            tongDoanhThu += lich.getTongTien();
            soBooking++;
        }
    }
    
    return soBooking > 0 ? tongDoanhThu / soBooking : 0.0;
}
