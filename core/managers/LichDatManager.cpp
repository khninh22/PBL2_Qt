#include "LichDatManager.h"
#include "SanBongManager.h"
#include "KhachHangManager.h"
#include <fstream>

// Constructor
LichDatManager::LichDatManager()
{
    // Index sẽ được rebuild khi load data
}

// Tìm kiếm - O(1) với Hash Table
LichDatSan *LichDatManager::timLichDat(const string &maLichDat)
{
    return indexLichDatSan.search(maLichDat);
}

// Kiểm tra trùng lịch
bool LichDatManager::kiemTraTrungLich(const string &maSan, time_t batDau, time_t ketThuc)
{
    for (int i = 0; i < dsLichDatSan.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLichDatSan[i];
        if (lich.getMaSan() == maSan && lich.getTrangThaiDat() == "Đã Đặt")
        {
            if (batDau < lich.getThoiGianKetThuc() && ketThuc > lich.getThoiGianBatDau())
                return true;
        }
    }
    return false;
}

// Đặt sân
bool LichDatManager::datSan(const string &maKH, const string &maSan, time_t batDau, time_t ketThuc,
                             string &maLichMoi, SanBongManager *sanMgr, KhachHangManager *khMgr)
{
    // Kiểm tra thời gian hợp lệ
    time_t now = time(nullptr);

    // Không cho đặt sân trong quá khứ (buffer 5 phút)
    if (batDau < (now - 300))
        return false;

    // Kiểm tra giờ hoạt động (6:00 - 22:00)
    tm *start_tm = localtime(&batDau);
    tm *end_tm = localtime(&ketThuc);

    if (start_tm->tm_hour < GIO_MO_CUA ||
        end_tm->tm_hour > GIO_DONG_CUA ||
        (end_tm->tm_hour == GIO_DONG_CUA && end_tm->tm_min > 0))
        return false;

    // Kiểm tra duration hợp lý (tối thiểu 30 phút, tối đa 12 giờ)
    double hours = difftime(ketThuc, batDau) / 3600.0;
    if (hours < 0.5 || hours > 12)
        return false;

    // Kiểm tra sân tồn tại (cần SanBongManager)
    if (sanMgr == nullptr)
        return false;
    
    SanBong *san = sanMgr->timSanBong(maSan);
    if (san == nullptr || san->getBaoTri())
        return false;

    // Kiểm tra khách hàng (cần KhachHangManager)
    if (khMgr == nullptr)
        return false;
    
    if (khMgr->timKhachHang(maKH) == nullptr)
        return false;

    // Kiểm tra trùng lịch
    if (kiemTraTrungLich(maSan, batDau, ketThuc))
        return false;

    // Tính tiền
    double tongTien = tinhTienDatSan(san->getLoaiSan(), batDau, ketThuc);

    // Tạo lịch đặt mới
    maLichMoi = taoMaLichMoi();
    LichDatSan lich(maLichMoi, maKH, maSan, batDau, ketThuc, tongTien, "Đã Đặt");
    dsLichDatSan.them(lich);
    
    // Cập nhật index
    indexLichDatSan.insert(dsLichDatSan[dsLichDatSan.getKichThuoc() - 1].getMaLichDat(),
                          &dsLichDatSan[dsLichDatSan.getKichThuoc() - 1]);
    return true;
}

// Hủy lịch đặt
bool LichDatManager::huyLichDat(const string &maLichDat)
{
    LichDatSan *lich = timLichDat(maLichDat);
    if (lich == nullptr)
        return false;

    lich->setTrangThaiDat("Đã Hủy");
    return true;
}

// Giá theo khung giờ
double LichDatManager::donGiaBanNgay(int loaiSan)
{
    if (loaiSan == 5)
        return 200000.0;
    if (loaiSan == 7)
        return 400000.0;
    if (loaiSan == 11)
        return 800000.0;
    return 0.0;
}

double LichDatManager::donGiaBanToi(int loaiSan)
{
    if (loaiSan == 5)
        return 300000.0;
    if (loaiSan == 7)
        return 500000.0;
    if (loaiSan == 11)
        return 1000000.0;
    return 0.0;
}

// Tính tiền theo khoảng thời gian
double LichDatManager::tinhTienTheoKhoang(int loaiSan, time_t batDau, time_t ketThuc)
{
    if (ketThuc <= batDau)
        return 0.0;

    tm d = *localtime(&batDau);
    tm moc = d;
    moc.tm_hour = 17;
    moc.tm_min = 0;
    moc.tm_sec = 0;
    time_t tMoc = mktime(&moc);

    double tong = 0.0;

    // Tính giờ ban ngày (trước 17:00)
    if (batDau < tMoc)
    {
        time_t endDay = (ketThuc < tMoc) ? ketThuc : tMoc;
        double gio = difftime(endDay, batDau) / 3600.0;
        tong += gio * donGiaBanNgay(loaiSan);
    }

    // Tính giờ ban tối (sau 17:00)
    if (ketThuc > tMoc)
    {
        time_t startNight = (batDau > tMoc) ? batDau : tMoc;
        double gio = difftime(ketThuc, startNight) / 3600.0;
        tong += gio * donGiaBanToi(loaiSan);
    }

    return tong;
}

// Tính tiền đặt sân
double LichDatManager::tinhTienDatSan(int loaiSan, time_t batDau, time_t ketThuc)
{
    return tinhTienTheoKhoang(loaiSan, batDau, ketThuc);
}

// Tạo mã lịch mới
string LichDatManager::taoMaLichMoi()
{
    return "LD" + to_string(dsLichDatSan.getKichThuoc() + 1);
}

// Hàm so sánh cho Quick Sort
static bool soSanhThoiGianTang(const LichDatSan &a, const LichDatSan &b)
{
    return a.getThoiGianBatDau() < b.getThoiGianBatDau();
}

static bool soSanhThoiGianGiam(const LichDatSan &a, const LichDatSan &b)
{
    return a.getThoiGianBatDau() > b.getThoiGianBatDau();
}

// Sắp xếp theo thời gian
void LichDatManager::sapXepTheoThoiGian(bool tangDan)
{
    if (tangDan)
        quickSort(dsLichDatSan, soSanhThoiGianTang);
    else
        quickSort(dsLichDatSan, soSanhThoiGianGiam);
    rebuildIndex();
}

// Rebuild index
void LichDatManager::rebuildIndex()
{
    indexLichDatSan.clear();
    for (int i = 0; i < dsLichDatSan.getKichThuoc(); i++)
        indexLichDatSan.insert(dsLichDatSan[i].getMaLichDat(), &dsLichDatSan[i]);
}

// Lưu file
void LichDatManager::luuFile(const string &filename)
{
    ofstream file(filename, ios::binary);
    if (file.is_open())
    {
        for (int i = 0; i < dsLichDatSan.getKichThuoc(); i++)
            dsLichDatSan[i].serialize(file);
        file.close();
    }
}

// Đọc file
void LichDatManager::docFile(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (file.is_open())
    {
        while (file.peek() != EOF)
        {
            LichDatSan lich;
            lich.deserialize(file);
            dsLichDatSan.them(lich);
        }
        file.close();
        rebuildIndex();
    }
}
