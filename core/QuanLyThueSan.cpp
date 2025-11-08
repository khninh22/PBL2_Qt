#include "QuanLyThueSan.h"
#include <ctime>

QuanLyThueSan::QuanLyThueSan() : backupMgr(&fileMgr)
{
    taiDuLieu();
    rebuildIndex();
    if (sanBongMgr.getDsSanBong().getKichThuoc() == 0)
    {
        khoiTaoDuLieuMau();
        rebuildIndex();
    }
}

void QuanLyThueSan::rebuildIndex()
{
    sanBongMgr.rebuildIndex();
    khachHangMgr.rebuildIndex();
    lichDatMgr.rebuildIndex();
    dichVuMgr.rebuildIndex();
    nhanVienMgr.rebuildIndex();
}

bool QuanLyThueSan::themSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue)
{
    bool result = sanBongMgr.themSanBong(maSan, tenSan, loaiSan, giaThue);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::suaSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue)
{
    bool result = sanBongMgr.suaSanBong(maSan, tenSan, loaiSan, giaThue);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::xoaSanBong(const string &maSan)
{
    bool result = sanBongMgr.xoaSanBong(maSan);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::batBaoTriSan(const string &maSan)
{
    bool result = sanBongMgr.batBaoTriSan(maSan);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::tatBaoTriSan(const string &maSan)
{
    bool result = sanBongMgr.tatBaoTriSan(maSan);
    if (result) luuDuLieu();
    return result;
}

MangDong<SanBong> QuanLyThueSan::timSanTrong(int loaiSan, time_t batDau, time_t ketThuc)
{
    return sanBongMgr.timSanTrong(loaiSan, batDau, ketThuc, lichDatMgr.getDsLichDatSan());
}

void QuanLyThueSan::sapXepSanBongTheoGia(bool tangDan)
{
    sanBongMgr.sapXepTheoGia(tangDan);
}

bool QuanLyThueSan::themKhachHang(const string &maKH, const string &hoTen, const string &sdt)
{
    bool result = khachHangMgr.themKhachHang(maKH, hoTen, sdt);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::suaKhachHang(const string &maKH, const string &hoTen, const string &sdt)
{
    bool result = khachHangMgr.suaKhachHang(maKH, hoTen, sdt);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::xoaKhachHang(const string &maKH)
{
    bool result = khachHangMgr.xoaKhachHang(maKH);
    if (result) luuDuLieu();
    return result;
}

void QuanLyThueSan::sapXepKhachHangTheoDiem(bool tangDan)
{
    khachHangMgr.sapXepTheoDiem(tangDan);
}

bool QuanLyThueSan::datSan(const string &maKH, const string &maSan, time_t batDau, time_t ketThuc, string &maLichMoi)
{
    bool result = lichDatMgr.datSan(maKH, maSan, batDau, ketThuc, maLichMoi, &sanBongMgr, &khachHangMgr);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::huyLichDat(const string &maLichDat)
{
    bool result = lichDatMgr.huyLichDat(maLichDat);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::kiemTraTrungLich(const string &maSan, time_t batDau, time_t ketThuc)
{
    return lichDatMgr.kiemTraTrungLich(maSan, batDau, ketThuc);
}

double QuanLyThueSan::tinhTienDatSan(int loaiSan, time_t batDau, time_t ketThuc)
{
    return lichDatMgr.tinhTienDatSan(loaiSan, batDau, ketThuc);
}

string QuanLyThueSan::taoMaLichMoi()
{
    return lichDatMgr.taoMaLichMoi();
}

void QuanLyThueSan::sapXepLichDatTheoThoiGian(bool tangDan)
{
    lichDatMgr.sapXepTheoThoiGian(tangDan);
}

bool QuanLyThueSan::themDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV)
{
    bool result = dichVuMgr.themDichVu(maDV, tenDV, loaiDV, giaDV);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::suaDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV)
{
    bool result = dichVuMgr.suaDichVu(maDV, tenDV, loaiDV, giaDV);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::xoaDichVu(const string &maDV)
{
    bool result = dichVuMgr.xoaDichVu(maDV);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::themDichVuVaoLich(const string &maLichDat, const string &maDV, int soLuong)
{
    bool result = dichVuMgr.themDichVuVaoLich(maLichDat, maDV, soLuong, &lichDatMgr);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::themNhanVien(const string &maNV, const string &hoTen, const string &sdt,
                                  const string &viTri, double luong,
                                  const string &taiKhoan, const string &matKhau)
{
    bool result = nhanVienMgr.themNhanVien(maNV, hoTen, sdt, viTri, luong, taiKhoan, matKhau);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::suaNhanVien(const string &maNV, const string &hoTen, const string &sdt,
                                 const string &viTri, double luong,
                                 const string &taiKhoan, const string &matKhau)
{
    bool result = nhanVienMgr.suaNhanVien(maNV, hoTen, sdt, viTri, luong, taiKhoan, matKhau);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::xoaNhanVien(const string &maNV)
{
    bool result = nhanVienMgr.xoaNhanVien(maNV);
    if (result) luuDuLieu();
    return result;
}

bool QuanLyThueSan::thanhToan(const string &maLichDat, double &tongTien, double &giamGia)
{
    bool result = thanhToanMgr.thanhToan(maLichDat, tongTien, giamGia, &lichDatMgr, &khachHangMgr);
    if (result) luuDuLieu();
    return result;
}

double QuanLyThueSan::tinhDoanhThuThang(int thang, int nam)
{
    return thanhToanMgr.tinhDoanhThuThang(thang, nam, &lichDatMgr);
}

double QuanLyThueSan::tinhDoanhThuNgay(time_t ngay)
{
    return thanhToanMgr.tinhDoanhThuNgay(ngay, &lichDatMgr);
}

// ✅ MỚI: Thống kê doanh thu nâng cao
double QuanLyThueSan::tinhDoanhThuQuy(int quy, int nam)
{
    return thanhToanMgr.tinhDoanhThuQuy(quy, nam, &lichDatMgr);
}

double QuanLyThueSan::tinhDoanhThuNam(int nam)
{
    return thanhToanMgr.tinhDoanhThuNam(nam, &lichDatMgr);
}

double QuanLyThueSan::tinhDoanhThuKhoang(time_t tuNgay, time_t denNgay)
{
    return thanhToanMgr.tinhDoanhThuKhoang(tuNgay, denNgay, &lichDatMgr);
}

// ✅ MỚI: Thống kê sân bóng
MangDong<ThongKeSan> QuanLyThueSan::thongKeTanSuatSan(time_t tuNgay, time_t denNgay)
{
    return thanhToanMgr.thongKeTanSuatSan(tuNgay, denNgay, &lichDatMgr, &sanBongMgr);
}

double QuanLyThueSan::tinhTyLelapDay(const string &maSan, time_t tuNgay, time_t denNgay)
{
    return thanhToanMgr.tinhTyLelapDay(maSan, tuNgay, denNgay, &lichDatMgr);
}

// ✅ MỚI: Thống kê khung giờ
MangDong<ThongKeKhungGio> QuanLyThueSan::thongKeKhungGio(time_t tuNgay, time_t denNgay)
{
    return thanhToanMgr.thongKeKhungGio(tuNgay, denNgay, &lichDatMgr);
}

// ✅ MỚI: Thống kê khách hàng
MangDong<ThongKeKhachHang> QuanLyThueSan::topKhachHangVIP(int top, time_t tuNgay, time_t denNgay)
{
    return thanhToanMgr.topKhachHangVIP(top, tuNgay, denNgay, &lichDatMgr, &khachHangMgr);
}

// ✅ MỚI: Thống kê tổng quan
int QuanLyThueSan::demTongLichDat(time_t tuNgay, time_t denNgay)
{
    return thanhToanMgr.demTongLichDat(tuNgay, denNgay, &lichDatMgr);
}

double QuanLyThueSan::tinhTyLeHuyLich(time_t tuNgay, time_t denNgay)
{
    return thanhToanMgr.tinhTyLeHuyLich(tuNgay, denNgay, &lichDatMgr);
}

double QuanLyThueSan::tinhDoanhThuTrungBinh(time_t tuNgay, time_t denNgay)
{
    return thanhToanMgr.tinhDoanhThuTrungBinh(tuNgay, denNgay, &lichDatMgr);
}

void QuanLyThueSan::taiDuLieu()
{
    // Load all data from files (string IDs only)
    sanBongMgr.docFile("data/sanbong.dat");
    khachHangMgr.docFile("data/khachhang.dat");
    lichDatMgr.docFile("data/lichdatsan.dat");
    dichVuMgr.docFileDichVu("data/dichvu.dat");
    dichVuMgr.docFileChiTiet("data/chitietdichvu.dat");
    nhanVienMgr.docFile("data/nhanvien.dat");
    
    // ✅ NEW: Resolve object pointers after loading
    resolveObjectPointers();
}

void QuanLyThueSan::resolveObjectPointers()
{
    // Resolve LichDatSan pointers (KhachHang*, SanBong*)
    MangDong<LichDatSan> &dsLich = lichDatMgr.getDsLichDatSanRef();
    for (int i = 0; i < dsLich.getKichThuoc(); i++)
    {
        LichDatSan &lich = dsLich[i];
        
        // Resolve KhachHang pointer
        string maKH = lich.getTempMaKH();
        if (!maKH.empty())
        {
            KhachHang *kh = khachHangMgr.timKhachHang(maKH);
            lich.setKhachHang(kh);
        }
        
        // Resolve SanBong pointer
        string maSan = lich.getTempMaSan();
        if (!maSan.empty())
        {
            SanBong *san = sanBongMgr.timSanBong(maSan);
            lich.setSanBong(san);
        }
    }
    
    // Resolve ChiTietDichVu pointers (DichVu*)
    MangDong<ChiTietDichVu> &dsChiTiet = dichVuMgr.getDsChiTietDichVuRef();
    for (int i = 0; i < dsChiTiet.getKichThuoc(); i++)
    {
        ChiTietDichVu &ct = dsChiTiet[i];
        
        // Resolve DichVu pointer
        string maDV = ct.getTempMaDV();
        if (!maDV.empty())
        {
            DichVu *dv = dichVuMgr.timDichVu(maDV);
            ct.setDichVu(dv);
        }
    }
}

void QuanLyThueSan::luuDuLieu()
{
    // ✅ AUTO-BACKUP: Backup before saving
    taoBackupToanBo();
    
    // Save all data
    sanBongMgr.luuFile("data/sanbong.dat");
    khachHangMgr.luuFile("data/khachhang.dat");
    lichDatMgr.luuFile("data/lichdatsan.dat");
    dichVuMgr.luuFileDichVu("data/dichvu.dat");
    dichVuMgr.luuFileChiTiet("data/chitietdichvu.dat");
    nhanVienMgr.luuFile("data/nhanvien.dat");
}

void QuanLyThueSan::khoiTaoDuLieuMau()
{
    sanBongMgr.themSanBong("SB001", "San A - Loai 5", 5, 200000.0);
    sanBongMgr.themSanBong("SB002", "San B - Loai 5", 5, 200000.0);
    sanBongMgr.themSanBong("SB003", "San C - Loai 7", 7, 400000.0);
    sanBongMgr.themSanBong("SB004", "San D - Loai 7", 7, 400000.0);
    sanBongMgr.themSanBong("SB005", "San E - Loai 11", 11, 800000.0);

    khachHangMgr.themKhachHang("KH001", "Nguyen Van A", "0901234567");
    khachHangMgr.themKhachHang("KH002", "Tran Thi B", "0912345678");
    khachHangMgr.themKhachHang("KH003", "Le Van C", "0923456789");
    
    KhachHang *kh2 = khachHangMgr.timKhachHang("KH002");
    if (kh2) {
        kh2->setDiemTichLuy(150);
        kh2->setCapDoTV(2);
    }
    KhachHang *kh3 = khachHangMgr.timKhachHang("KH003");
    if (kh3) {
        kh3->setDiemTichLuy(350);
        kh3->setCapDoTV(3);
    }

    dichVuMgr.themDichVu("DV001", "Nuoc Suoi", "Do Uong", 10000.0);
    dichVuMgr.themDichVu("DV002", "Nuoc Tang Luc", "Do Uong", 15000.0);
    dichVuMgr.themDichVu("DV003", "Banh Mi", "Do An", 20000.0);
    dichVuMgr.themDichVu("DV004", "Com Hop", "Do An", 35000.0);
    dichVuMgr.themDichVu("DV005", "Cho Thue Ao", "Khac", 50000.0);

    nhanVienMgr.themNhanVien("NV001", "Pham Van D", "0934567890", "Quan Ly", 10000000.0, "admin", "admin123");
    nhanVienMgr.themNhanVien("NV002", "Hoang Thi E", "0945678901", "Thu Ngan", 6000000.0, "thungan1", "thungan123");
    nhanVienMgr.themNhanVien("NV003", "Vo Van F", "0956789012", "Bao Tri", 5500000.0, "baotri1", "baotri123");

    // ===== TẠO LỊCH ĐẶT MẪU CHO THÁNG HIỆN TẠI =====
    time_t now = time(nullptr);
    tm *currentTime = localtime(&now);
    
    // Lịch 1: Hôm nay 8h-10h (Đã thanh toán)
    currentTime->tm_hour = 8;
    currentTime->tm_min = 0;
    currentTime->tm_sec = 0;
    time_t batDau1 = mktime(currentTime);
    currentTime->tm_hour = 10;
    time_t ketThuc1 = mktime(currentTime);
    string maLich1;
    if (lichDatMgr.datSan("KH001", "SB001", batDau1, ketThuc1, maLich1, &sanBongMgr, &khachHangMgr))
    {
        dichVuMgr.themDichVuVaoLich(maLich1, "DV001", 2, &lichDatMgr);
        double tongTien, giamGia;
        thanhToanMgr.thanhToan(maLich1, tongTien, giamGia, &lichDatMgr, &khachHangMgr);
    }
    
    // Lịch 2: Hôm nay 14h-16h (Đã thanh toán)
    currentTime = localtime(&now);
    currentTime->tm_hour = 14;
    currentTime->tm_min = 0;
    time_t batDau2 = mktime(currentTime);
    currentTime->tm_hour = 16;
    time_t ketThuc2 = mktime(currentTime);
    string maLich2;
    if (lichDatMgr.datSan("KH002", "SB003", batDau2, ketThuc2, maLich2, &sanBongMgr, &khachHangMgr))
    {
        dichVuMgr.themDichVuVaoLich(maLich2, "DV002", 3, &lichDatMgr);
        dichVuMgr.themDichVuVaoLich(maLich2, "DV003", 2, &lichDatMgr);
        double tongTien, giamGia;
        thanhToanMgr.thanhToan(maLich2, tongTien, giamGia, &lichDatMgr, &khachHangMgr);
    }
    
    // Lịch 3: Hôm nay 18h-20h (Đã thanh toán)
    currentTime = localtime(&now);
    currentTime->tm_hour = 18;
    time_t batDau3 = mktime(currentTime);
    currentTime->tm_hour = 20;
    time_t ketThuc3 = mktime(currentTime);
    string maLich3;
    if (lichDatMgr.datSan("KH003", "SB005", batDau3, ketThuc3, maLich3, &sanBongMgr, &khachHangMgr))
    {
        dichVuMgr.themDichVuVaoLich(maLich3, "DV004", 1, &lichDatMgr);
        double tongTien, giamGia;
        thanhToanMgr.thanhToan(maLich3, tongTien, giamGia, &lichDatMgr, &khachHangMgr);
    }
    
    // Lịch 4: Ngày mai 8h-10h (Chưa thanh toán)
    currentTime = localtime(&now);
    currentTime->tm_mday += 1;
    currentTime->tm_hour = 8;
    time_t batDau4 = mktime(currentTime);
    currentTime->tm_hour = 10;
    time_t ketThuc4 = mktime(currentTime);
    string maLich4;
    lichDatMgr.datSan("KH001", "SB002", batDau4, ketThuc4, maLich4, &sanBongMgr, &khachHangMgr);
    
    // Lịch 5: 3 ngày trước 9h-11h (Đã thanh toán)
    currentTime = localtime(&now);
    currentTime->tm_mday -= 3;
    currentTime->tm_hour = 9;
    time_t batDau5 = mktime(currentTime);
    currentTime->tm_hour = 11;
    time_t ketThuc5 = mktime(currentTime);
    string maLich5;
    if (lichDatMgr.datSan("KH002", "SB001", batDau5, ketThuc5, maLich5, &sanBongMgr, &khachHangMgr))
    {
        double tongTien, giamGia;
        thanhToanMgr.thanhToan(maLich5, tongTien, giamGia, &lichDatMgr, &khachHangMgr);
    }
    
    // Lịch 6: 5 ngày trước 15h-17h (Đã thanh toán)
    currentTime = localtime(&now);
    currentTime->tm_mday -= 5;
    currentTime->tm_hour = 15;
    time_t batDau6 = mktime(currentTime);
    currentTime->tm_hour = 17;
    time_t ketThuc6 = mktime(currentTime);
    string maLich6;
    if (lichDatMgr.datSan("KH003", "SB004", batDau6, ketThuc6, maLich6, &sanBongMgr, &khachHangMgr))
    {
        dichVuMgr.themDichVuVaoLich(maLich6, "DV001", 4, &lichDatMgr);
        dichVuMgr.themDichVuVaoLich(maLich6, "DV005", 2, &lichDatMgr);
        double tongTien, giamGia;
        thanhToanMgr.thanhToan(maLich6, tongTien, giamGia, &lichDatMgr, &khachHangMgr);
    }
    
    // Lịch 7: 7 ngày trước 19h-21h (Đã hủy - để test tỷ lệ hủy)
    currentTime = localtime(&now);
    currentTime->tm_mday -= 7;
    currentTime->tm_hour = 19;
    time_t batDau7 = mktime(currentTime);
    currentTime->tm_hour = 21;
    time_t ketThuc7 = mktime(currentTime);
    string maLich7;
    if (lichDatMgr.datSan("KH001", "SB003", batDau7, ketThuc7, maLich7, &sanBongMgr, &khachHangMgr))
    {
        lichDatMgr.huyLichDat(maLich7);
    }
    
    // Lịch 8: 10 ngày trước 10h-12h (Đã thanh toán)
    currentTime = localtime(&now);
    currentTime->tm_mday -= 10;
    currentTime->tm_hour = 10;
    time_t batDau8 = mktime(currentTime);
    currentTime->tm_hour = 12;
    time_t ketThuc8 = mktime(currentTime);
    string maLich8;
    if (lichDatMgr.datSan("KH002", "SB005", batDau8, ketThuc8, maLich8, &sanBongMgr, &khachHangMgr))
    {
        double tongTien, giamGia;
        thanhToanMgr.thanhToan(maLich8, tongTien, giamGia, &lichDatMgr, &khachHangMgr);
    }

    luuDuLieu();
}

// ====================================================
// ✅ BACKUP & RESTORE METHODS
// ====================================================

bool QuanLyThueSan::taoBackupToanBo()
{
    return backupMgr.taoBackupToanBo();
}

bool QuanLyThueSan::khoiPhucDuLieu(const string &tenBackup)
{
    // Restore all data files from backup
    vector<string> dsFile = {
        "sanbong.dat",
        "khachhang.dat",
        "lichdat.dat",
        "dichvu.dat",
        "nhanvien.dat",
        "thanhtoan.dat"
    };
    
    bool success = true;
    for (const auto &tenFile : dsFile)
    {
        // Extract base filename from backup (backup_YYYYMMDD_HHMMSS_filename.dat)
        // Find the backup file that matches
        string backupFile = tenBackup;
        if (backupFile.find(tenFile) != string::npos)
        {
            if (!backupMgr.khoiPhuc(backupFile, tenFile))
                success = false;
        }
        else
        {
            // Build full backup filename
            string fullBackupName = tenBackup;
            // If tenBackup is just timestamp, append filename
            // This needs better logic - for now try direct restore
            if (!backupMgr.khoiPhuc(tenBackup + "_" + tenFile, tenFile))
            {
                // Try with just the timestamp prefix
                auto backupList = backupMgr.layDanhSachBackup();
                for (const auto &bf : backupList)
                {
                    if (bf.find(tenBackup) == 0 && bf.find(tenFile) != string::npos)
                    {
                        backupMgr.khoiPhuc(bf, tenFile);
                        break;
                    }
                }
            }
        }
    }
    
    if (success)
    {
        taiDuLieu();  // Reload all data
        rebuildIndex(); // Rebuild all hash tables
    }
    return success;
}

MangDong<string> QuanLyThueSan::layDanhSachBackup()
{
    // Return unique backup timestamps (extract from filenames)
    auto allBackups = backupMgr.layDanhSachBackup();
    MangDong<string> uniqueBackups;
    
    for (const auto &backup : allBackups)
    {
        // Extract timestamp: backup_YYYYMMDD_HHMMSS_filename.dat
        size_t pos1 = backup.find("_");
        if (pos1 != string::npos)
        {
            size_t pos2 = backup.find("_", pos1 + 1);
            if (pos2 != string::npos)
            {
                size_t pos3 = backup.find("_", pos2 + 1);
                if (pos3 != string::npos)
                {
                    string timestamp = backup.substr(0, pos3); // backup_YYYYMMDD_HHMMSS
                    
                    // Check if already added
                    bool found = false;
                    for (int i = 0; i < uniqueBackups.getKichThuoc(); i++)
                    {
                        if (uniqueBackups[i] == timestamp)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        uniqueBackups.them(timestamp);
                }
            }
        }
    }
    
    return uniqueBackups;
}


