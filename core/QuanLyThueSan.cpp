#include "QuanLyThueSan.h"
#include <ctime>
#include <cmath>

// Constructor
QuanLyThueSan::QuanLyThueSan()
{
    taiDuLieu();
    // ✅ MỚI: Rebuild index sau khi load dữ liệu
    rebuildIndex();

    // Nếu chưa có dữ liệu, khởi tạo dữ liệu mẫu
    if (dsSanBong.getKichThuoc() == 0)
    {
        khoiTaoDuLieuMau();
        rebuildIndex(); // Rebuild lại sau khi khởi tạo
    }
}

// ============================
//  ✅ MỚI: REBUILD INDEX
//  Xây dựng lại Hash Table Index
// ============================
void QuanLyThueSan::rebuildIndex()
{
    // Clear old index
    indexSanBong.clear();
    indexKhachHang.clear();
    indexLichDatSan.clear();
    indexDichVu.clear();
    indexNhanVien.clear();

    // Rebuild index từ mảng
    for (int i = 0; i < dsSanBong.getKichThuoc(); i++)
        indexSanBong.insert(dsSanBong[i].getMaSan(), &dsSanBong[i]);

    for (int i = 0; i < dsKhachHang.getKichThuoc(); i++)
        indexKhachHang.insert(dsKhachHang[i].getMaKH(), &dsKhachHang[i]);

    for (int i = 0; i < dsLichDatSan.getKichThuoc(); i++)
        indexLichDatSan.insert(dsLichDatSan[i].getMaLichDat(), &dsLichDatSan[i]);

    for (int i = 0; i < dsDichVu.getKichThuoc(); i++)
        indexDichVu.insert(dsDichVu[i].getMaDV(), &dsDichVu[i]);

    for (int i = 0; i < dsNhanVien.getKichThuoc(); i++)
        indexNhanVien.insert(dsNhanVien[i].getMaNV(), &dsNhanVien[i]);
}

// ============================
//  TÌM KIẾM - ĐÃ CẢI TIẾN
//  Cũ: Linear Search O(n)
//  Mới: Hash Table O(1) ✅
// ============================
SanBong *QuanLyThueSan::timSanBong(const string &maSan)
{
    return indexSanBong.search(maSan); // ✅ O(1) thay vì O(n)
}

KhachHang *QuanLyThueSan::timKhachHang(const string &maKH)
{
    return indexKhachHang.search(maKH); // ✅ O(1)
}

LichDatSan *QuanLyThueSan::timLichDat(const string &maLichDat)
{
    return indexLichDatSan.search(maLichDat); // ✅ O(1)
}

DichVu *QuanLyThueSan::timDichVu(const string &maDV)
{
    return indexDichVu.search(maDV); // ✅ O(1)
}

NhanVien *QuanLyThueSan::timNhanVien(const string &maNV)
{
    return indexNhanVien.search(maNV); // ✅ O(1)
}

// Quản lý sân bóng
bool QuanLyThueSan::themSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue)
{
    if (timSanBong(maSan) != nullptr)
        return false;

    SanBong san(maSan, tenSan, loaiSan, giaThue, false);
    dsSanBong.them(san);
    // ✅ MỚI: Cập nhật index ngay sau khi thêm
    indexSanBong.insert(dsSanBong[dsSanBong.getKichThuoc() - 1].getMaSan(),
                        &dsSanBong[dsSanBong.getKichThuoc() - 1]);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::suaSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue)
{
    SanBong *san = timSanBong(maSan);
    if (san == nullptr)
        return false;

    san->setTenSan(tenSan);
    san->setLoaiSan(loaiSan);
    san->setGiaThue(giaThue);
    // Index tự động cập nhật vì trỏ tới cùng object
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::xoaSanBong(const string &maSan)
{
    for (int i = 0; i < dsSanBong.getKichThuoc(); i++)
    {
        if (dsSanBong[i].getMaSan() == maSan)
        {
            // ✅ MỚI: Xóa khỏi index trước
            indexSanBong.remove(maSan);
            dsSanBong.xoaTheoViTri(i);
            // ✅ MỚI: Rebuild index vì địa chỉ thay đổi sau khi xóa
            rebuildIndex();
            luuDuLieu();
            return true;
        }
    }
    return false;
}

bool QuanLyThueSan::batBaoTriSan(const string &maSan)
{
    SanBong *san = timSanBong(maSan);
    if (san == nullptr)
        return false;

    san->setBaoTri(true);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::tatBaoTriSan(const string &maSan)
{
    SanBong *san = timSanBong(maSan);
    if (san == nullptr)
        return false;

    san->setBaoTri(false);
    luuDuLieu();
    return true;
}

// Quản lý khách hàng
bool QuanLyThueSan::themKhachHang(const string &maKH, const string &hoTen, const string &sdt)
{
    if (timKhachHang(maKH) != nullptr)
        return false;

    KhachHang kh(maKH, hoTen, sdt, 0, 1);
    dsKhachHang.them(kh);
    // ✅ MỚI: Cập nhật index
    indexKhachHang.insert(dsKhachHang[dsKhachHang.getKichThuoc() - 1].getMaKH(),
                          &dsKhachHang[dsKhachHang.getKichThuoc() - 1]);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::suaKhachHang(const string &maKH, const string &hoTen, const string &sdt)
{
    KhachHang *kh = timKhachHang(maKH);
    if (kh == nullptr)
        return false;

    kh->setHoTen(hoTen);
    kh->setSdt(sdt);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::xoaKhachHang(const string &maKH)
{
    for (int i = 0; i < dsKhachHang.getKichThuoc(); i++)
    {
        if (dsKhachHang[i].getMaKH() == maKH)
        {
            // ✅ MỚI: Xóa khỏi index và rebuild
            indexKhachHang.remove(maKH);
            dsKhachHang.xoaTheoViTri(i);
            rebuildIndex();
            luuDuLieu();
            return true;
        }
    }
    return false;
}

// Kiểm tra trùng lịch
bool QuanLyThueSan::kiemTraTrungLich(const string &maSan, time_t batDau, time_t ketThuc)
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
bool QuanLyThueSan::datSan(const string &maKH, const string &maSan, time_t batDau, time_t ketThuc, string &maLichMoi)
{
    // ✅ THÊM: Kiểm tra thời gian hợp lệ
    time_t now = time(nullptr);

    // Không cho đặt sân trong quá khứ (trừ buffer 5 phút để tránh lỗi đồng bộ thời gian)
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

    // Kiểm tra sân tồn tại
    SanBong *san = timSanBong(maSan);
    if (san == nullptr || san->getBaoTri())
        return false;

    // Kiểm tra khách hàng
    if (timKhachHang(maKH) == nullptr)
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
    // ✅ MỚI: Cập nhật index
    indexLichDatSan.insert(dsLichDatSan[dsLichDatSan.getKichThuoc() - 1].getMaLichDat(),
                           &dsLichDatSan[dsLichDatSan.getKichThuoc() - 1]);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::huyLichDat(const string &maLichDat)
{
    LichDatSan *lich = timLichDat(maLichDat);
    if (lich == nullptr)
        return false;

    lich->setTrangThaiDat("Đã Hủy");
    luuDuLieu();
    return true;
}

// Quản lý dịch vụ
bool QuanLyThueSan::themDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV)
{
    if (timDichVu(maDV) != nullptr)
        return false;

    DichVu dv(maDV, tenDV, loaiDV, giaDV, true);
    dsDichVu.them(dv);
    // ✅ MỚI: Cập nhật index
    indexDichVu.insert(dsDichVu[dsDichVu.getKichThuoc() - 1].getMaDV(),
                       &dsDichVu[dsDichVu.getKichThuoc() - 1]);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::suaDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV)
{
    DichVu *dv = timDichVu(maDV);
    if (dv == nullptr)
        return false;

    dv->setTenDV(tenDV);
    dv->setLoaiDV(loaiDV);
    dv->setGiaDV(giaDV);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::xoaDichVu(const string &maDV)
{
    for (int i = 0; i < dsDichVu.getKichThuoc(); i++)
    {
        if (dsDichVu[i].getMaDV() == maDV)
        {
            // ✅ MỚI: Xóa khỏi index
            indexDichVu.remove(maDV);
            dsDichVu.xoaTheoViTri(i);
            rebuildIndex();
            luuDuLieu();
            return true;
        }
    }
    return false;
}

bool QuanLyThueSan::themDichVuVaoLich(const string &maLichDat, const string &maDV, int soLuong)
{
    // Kiểm tra lịch đặt
    if (timLichDat(maLichDat) == nullptr)
        return false;

    // Kiểm tra dịch vụ
    DichVu *dv = timDichVu(maDV);
    if (dv == nullptr || !dv->getConHang())
        return false;

    // Thêm chi tiết dịch vụ
    ChiTietDichVu ct(maLichDat, maDV, soLuong, dv->getGiaDV());
    dsChiTietDichVu.them(ct);

    // Cập nhật tổng tiền lịch đặt
    LichDatSan *lich = timLichDat(maLichDat);
    double tongMoi = lich->getTongTien() + ct.getThanhTien();
    lich->setTongTien(tongMoi);

    luuDuLieu();
    return true;
}

// Quản lý nhân viên
bool QuanLyThueSan::themNhanVien(const string &maNV, const string &hoTen, const string &sdt, const string &viTri, double luong, const string &taiKhoan, const string &matKhau)
{
    if (timNhanVien(maNV) != nullptr)
        return false;

    NhanVien nv(maNV, hoTen, sdt, viTri, luong, true, taiKhoan, matKhau);
    dsNhanVien.them(nv);
    // ✅ MỚI: Cập nhật index
    indexNhanVien.insert(dsNhanVien[dsNhanVien.getKichThuoc() - 1].getMaNV(),
                         &dsNhanVien[dsNhanVien.getKichThuoc() - 1]);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::suaNhanVien(const string &maNV, const string &hoTen, const string &sdt, const string &viTri, double luong, const string &taiKhoan, const string &matKhau)
{
    NhanVien *nv = timNhanVien(maNV);
    if (nv == nullptr)
        return false;

    nv->setHoTen(hoTen);
    nv->setSdt(sdt);
    nv->setViTri(viTri);
    nv->setLuongCoBan(luong);
    if (!taiKhoan.empty())
        nv->setTaiKhoan(taiKhoan);
    if (!matKhau.empty())
        nv->setMatKhau(matKhau);
    luuDuLieu();
    return true;
}

bool QuanLyThueSan::xoaNhanVien(const string &maNV)
{
    for (int i = 0; i < dsNhanVien.getKichThuoc(); i++)
    {
        if (dsNhanVien[i].getMaNV() == maNV)
        {
            // ✅ MỚI: Xóa khỏi index
            indexNhanVien.remove(maNV);
            dsNhanVien.xoaTheoViTri(i);
            rebuildIndex();
            luuDuLieu();
            return true;
        }
    }
    return false;
}

// Thanh toán
bool QuanLyThueSan::thanhToan(const string &maLichDat, double &tongTien, double &giamGia)
{
    LichDatSan *lich = timLichDat(maLichDat);
    if (lich == nullptr || lich->getTrangThaiTT() == "Đã Thanh Toán")
        return false;

    tongTien = lich->getTongTien();

    // Tính giảm giá theo cấp độ khách hàng
    KhachHang *kh = timKhachHang(lich->getMaKH());
    if (kh != nullptr)
    {
        double phanTramGiam = kh->tinhPhanTramGiam();
        giamGia = tongTien * phanTramGiam / 100.0;
        tongTien -= giamGia;

        // Tích điểm
        kh->tichDiem(tongTien);
    }

    // Cập nhật trạng thái thanh toán
    lich->setTrangThaiTT("Đã Thanh Toán");
    lich->setThoiGianThanhToan(time(nullptr));

    luuDuLieu();
    return true;
}

// Giá theo khung giờ
double QuanLyThueSan::donGiaBanNgay(int loaiSan)
{
    if (loaiSan == 5)
        return 200000.0;
    if (loaiSan == 7)
        return 400000.0;
    if (loaiSan == 11)
        return 800000.0;
    return 0.0;
}

double QuanLyThueSan::donGiaBanToi(int loaiSan)
{
    if (loaiSan == 5)
        return 300000.0;
    if (loaiSan == 7)
        return 500000.0;
    if (loaiSan == 11)
        return 1000000.0;
    return 0.0;
}

double QuanLyThueSan::tinhTienTheoKhoang(int loaiSan, time_t batDau, time_t ketThuc)
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

double QuanLyThueSan::tinhTienDatSan(int loaiSan, time_t batDau, time_t ketThuc)
{
    return tinhTienTheoKhoang(loaiSan, batDau, ketThuc);
}

// Tìm sân trống
MangDong<SanBong> QuanLyThueSan::timSanTrong(int loaiSan, time_t batDau, time_t ketThuc)
{
    MangDong<SanBong> kq;

    for (int i = 0; i < dsSanBong.getKichThuoc(); i++)
    {
        const SanBong &san = dsSanBong[i];
        if (san.getLoaiSan() != loaiSan || san.getBaoTri())
            continue;

        bool trung = false;
        for (int j = 0; j < dsLichDatSan.getKichThuoc(); j++)
        {
            const LichDatSan &lich = dsLichDatSan[j];
            if (lich.getMaSan() == san.getMaSan() && lich.getTrangThaiDat() == "Đã Đặt")
            {
                if (batDau < lich.getThoiGianKetThuc() && ketThuc > lich.getThoiGianBatDau())
                {
                    trung = true;
                    break;
                }
            }
        }

        if (!trung)
            kq.them(san);
    }

    return kq;
}

// Tạo mã lịch mới
string QuanLyThueSan::taoMaLichMoi()
{
    return "LD" + to_string(dsLichDatSan.getKichThuoc() + 1);
}

// Thống kê
double QuanLyThueSan::tinhDoanhThuThang(int thang, int nam)
{
    double tong = 0.0;
    for (int i = 0; i < dsLichDatSan.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLichDatSan[i];
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

double QuanLyThueSan::tinhDoanhThuNgay(time_t ngay)
{
    double tong = 0.0;
    tm ngayTm = *localtime(&ngay);

    for (int i = 0; i < dsLichDatSan.getKichThuoc(); i++)
    {
        const LichDatSan &lich = dsLichDatSan[i];
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

// File I/O
void QuanLyThueSan::taiDuLieu()
{
    // Đọc dữ liệu sân bóng
    ifstream fileSan("data/sanbong.dat", ios::binary);
    if (fileSan.is_open())
    {
        while (fileSan.peek() != EOF)
        {
            SanBong san;
            san.deserialize(fileSan);
            dsSanBong.them(san);
        }
        fileSan.close();
    }

    // Đọc dữ liệu khách hàng
    ifstream fileKH("data/khachhang.dat", ios::binary);
    if (fileKH.is_open())
    {
        while (fileKH.peek() != EOF)
        {
            KhachHang kh;
            kh.deserialize(fileKH);
            dsKhachHang.them(kh);
        }
        fileKH.close();
    }

    // Đọc dữ liệu lịch đặt
    ifstream fileLich("data/lichdatsan.dat", ios::binary);
    if (fileLich.is_open())
    {
        while (fileLich.peek() != EOF)
        {
            LichDatSan lich;
            lich.deserialize(fileLich);
            dsLichDatSan.them(lich);
        }
        fileLich.close();
    }

    // Đọc dữ liệu dịch vụ
    ifstream fileDV("data/dichvu.dat", ios::binary);
    if (fileDV.is_open())
    {
        while (fileDV.peek() != EOF)
        {
            DichVu dv;
            dv.deserialize(fileDV);
            dsDichVu.them(dv);
        }
        fileDV.close();
    }

    // Đọc chi tiết dịch vụ
    ifstream fileCTDV("data/chitietdichvu.dat", ios::binary);
    if (fileCTDV.is_open())
    {
        while (fileCTDV.peek() != EOF)
        {
            ChiTietDichVu ct;
            ct.deserialize(fileCTDV);
            dsChiTietDichVu.them(ct);
        }
        fileCTDV.close();
    }

    // Đọc nhân viên
    ifstream fileNV("data/nhanvien.dat", ios::binary);
    if (fileNV.is_open())
    {
        while (fileNV.peek() != EOF)
        {
            NhanVien nv;
            nv.deserialize(fileNV);
            dsNhanVien.them(nv);
        }
        fileNV.close();
    }

    // Rebuild index sau khi load dữ liệu
    rebuildIndex();
}

void QuanLyThueSan::luuDuLieu()
{
    // Lưu sân bóng
    ofstream fileSan("data/sanbong.dat", ios::binary);
    for (int i = 0; i < dsSanBong.getKichThuoc(); i++)
    {
        dsSanBong[i].serialize(fileSan);
    }
    fileSan.close();

    // Lưu khách hàng
    ofstream fileKH("data/khachhang.dat", ios::binary);
    for (int i = 0; i < dsKhachHang.getKichThuoc(); i++)
    {
        dsKhachHang[i].serialize(fileKH);
    }
    fileKH.close();

    // Lưu lịch đặt
    ofstream fileLich("data/lichdatsan.dat", ios::binary);
    for (int i = 0; i < dsLichDatSan.getKichThuoc(); i++)
    {
        dsLichDatSan[i].serialize(fileLich);
    }
    fileLich.close();

    // Lưu dịch vụ
    ofstream fileDV("data/dichvu.dat", ios::binary);
    for (int i = 0; i < dsDichVu.getKichThuoc(); i++)
    {
        dsDichVu[i].serialize(fileDV);
    }
    fileDV.close();

    // Lưu chi tiết dịch vụ
    ofstream fileCTDV("data/chitietdichvu.dat", ios::binary);
    for (int i = 0; i < dsChiTietDichVu.getKichThuoc(); i++)
    {
        dsChiTietDichVu[i].serialize(fileCTDV);
    }
    fileCTDV.close();

    // Lưu nhân viên
    ofstream fileNV("data/nhanvien.dat", ios::binary);
    for (int i = 0; i < dsNhanVien.getKichThuoc(); i++)
    {
        dsNhanVien[i].serialize(fileNV);
    }
    fileNV.close();
}

void QuanLyThueSan::khoiTaoDuLieuMau()
{
    // Khởi tạo 5 sân bóng
    SanBong san1("SB001", "Sân A - Loại 5", 5, 200000.0, false);
    SanBong san2("SB002", "Sân B - Loại 5", 5, 200000.0, false);
    SanBong san3("SB003", "Sân C - Loại 7", 7, 400000.0, false);
    SanBong san4("SB004", "Sân D - Loại 7", 7, 400000.0, false);
    SanBong san5("SB005", "Sân E - Loại 11", 11, 800000.0, false);
    dsSanBong.them(san1);
    dsSanBong.them(san2);
    dsSanBong.them(san3);
    dsSanBong.them(san4);
    dsSanBong.them(san5);

    // Khởi tạo 3 khách hàng
    KhachHang kh1("KH001", "Nguyễn Văn A", "0901234567", 0, 1);
    KhachHang kh2("KH002", "Trần Thị B", "0912345678", 150, 2);
    KhachHang kh3("KH003", "Lê Văn C", "0923456789", 350, 3);
    dsKhachHang.them(kh1);
    dsKhachHang.them(kh2);
    dsKhachHang.them(kh3);

    // Khởi tạo 5 dịch vụ
    DichVu dv1("DV001", "Nước Suối", "Đồ Uống", 10000.0, true);
    DichVu dv2("DV002", "Nước Tăng Lực", "Đồ Uống", 15000.0, true);
    DichVu dv3("DV003", "Bánh Mì", "Đồ Ăn", 20000.0, true);
    DichVu dv4("DV004", "Cơm Hộp", "Đồ Ăn", 35000.0, true);
    DichVu dv5("DV005", "Cho Thuê Áo", "Khác", 50000.0, true);
    dsDichVu.them(dv1);
    dsDichVu.them(dv2);
    dsDichVu.them(dv3);
    dsDichVu.them(dv4);
    dsDichVu.them(dv5);

    // Khởi tạo 3 nhân viên
    NhanVien nv1("NV001", "Phạm Văn D", "0934567890", "Quản Lý", 10000000.0, true, "admin", "admin123");
    NhanVien nv2("NV002", "Hoàng Thị E", "0945678901", "Thu Ngân", 6000000.0, true, "thungan1", "thungan123");
    NhanVien nv3("NV003", "Võ Văn F", "0956789012", "Bảo Trì", 5500000.0, true, "baotri1", "baotri123");
    dsNhanVien.them(nv1);
    dsNhanVien.them(nv2);
    dsNhanVien.them(nv3);

    // Khởi tạo 3 lịch đặt sân
    // Lịch 1: Ngày mai 8:00-10:00
    time_t now = time(nullptr);
    tm *ngayMai = localtime(&now);
    ngayMai->tm_mday += 1;
    ngayMai->tm_hour = 8;
    ngayMai->tm_min = 0;
    ngayMai->tm_sec = 0;
    time_t batDau1 = mktime(ngayMai);
    ngayMai->tm_hour = 10;
    time_t ketThuc1 = mktime(ngayMai);
    double tien1 = tinhTienDatSan(5, batDau1, ketThuc1);
    LichDatSan lich1("LD001", "KH001", "SB001", batDau1, ketThuc1, tien1, "Đã Đặt");
    dsLichDatSan.them(lich1);

    // Lịch 2: Ngày mai 14:00-16:00
    ngayMai->tm_hour = 14;
    time_t batDau2 = mktime(ngayMai);
    ngayMai->tm_hour = 16;
    time_t ketThuc2 = mktime(ngayMai);
    double tien2 = tinhTienDatSan(7, batDau2, ketThuc2);
    LichDatSan lich2("LD002", "KH002", "SB003", batDau2, ketThuc2, tien2, "Đã Đặt");
    dsLichDatSan.them(lich2);

    // Lịch 3: Ngày mai 18:00-20:00 (giờ tối)
    ngayMai->tm_hour = 18;
    time_t batDau3 = mktime(ngayMai);
    ngayMai->tm_hour = 20;
    time_t ketThuc3 = mktime(ngayMai);
    double tien3 = tinhTienDatSan(11, batDau3, ketThuc3);
    LichDatSan lich3("LD003", "KH003", "SB005", batDau3, ketThuc3, tien3, "Đã Đặt");
    dsLichDatSan.them(lich3);

    // Thêm dịch vụ vào một vài lịch đặt
    ChiTietDichVu ct1("LD001", "DV001", 2, 10000.0); // 2 chai nước suối
    ChiTietDichVu ct2("LD002", "DV003", 3, 20000.0); // 3 bánh mì
    ChiTietDichVu ct3("LD002", "DV002", 2, 15000.0); // 2 nước tăng lực
    ChiTietDichVu ct4("LD003", "DV004", 2, 35000.0); // 2 cơm hộp
    dsChiTietDichVu.them(ct1);
    dsChiTietDichVu.them(ct2);
    dsChiTietDichVu.them(ct3);
    dsChiTietDichVu.them(ct4);

    // Cập nhật tổng tiền cho các lịch có dịch vụ
    lich1.setTongTien(tien1 + ct1.getThanhTien());
    lich2.setTongTien(tien2 + ct2.getThanhTien() + ct3.getThanhTien());
    lich3.setTongTien(tien3 + ct4.getThanhTien());

    // Lưu dữ liệu
    luuDuLieu();
}

// ============================
//  ✅ MỚI: SẮP XẾP VỚI QUICK SORT
// ============================

// Hàm so sánh cho sân bóng theo giá
bool soSanhSanBongTheoGia(const SanBong &a, const SanBong &b)
{
    return a.getGiaThue() < b.getGiaThue();
}

bool soSanhSanBongTheoGiaGiam(const SanBong &a, const SanBong &b)
{
    return a.getGiaThue() > b.getGiaThue();
}

void QuanLyThueSan::sapXepSanBongTheoGia(bool tangDan)
{
    if (tangDan)
        quickSort(dsSanBong, soSanhSanBongTheoGia);
    else
        quickSort(dsSanBong, soSanhSanBongTheoGiaGiam);

    // ✅ Rebuild index sau khi sắp xếp (địa chỉ thay đổi)
    rebuildIndex();
}

// Hàm so sánh cho khách hàng theo điểm
bool soSanhKhachHangTheoDiem(const KhachHang &a, const KhachHang &b)
{
    return a.getDiemTichLuy() < b.getDiemTichLuy();
}

bool soSanhKhachHangTheoDiemGiam(const KhachHang &a, const KhachHang &b)
{
    return a.getDiemTichLuy() > b.getDiemTichLuy();
}

void QuanLyThueSan::sapXepKhachHangTheoDiem(bool tangDan)
{
    if (tangDan)
        quickSort(dsKhachHang, soSanhKhachHangTheoDiem);
    else
        quickSort(dsKhachHang, soSanhKhachHangTheoDiemGiam);

    rebuildIndex();
}

// Hàm so sánh cho lịch đặt theo thời gian
bool soSanhLichDatTheoThoiGian(const LichDatSan &a, const LichDatSan &b)
{
    return a.getThoiGianBatDau() < b.getThoiGianBatDau();
}

bool soSanhLichDatTheoThoiGianGiam(const LichDatSan &a, const LichDatSan &b)
{
    return a.getThoiGianBatDau() > b.getThoiGianBatDau();
}

void QuanLyThueSan::sapXepLichDatTheoThoiGian(bool tangDan)
{
    if (tangDan)
        quickSort(dsLichDatSan, soSanhLichDatTheoThoiGian);
    else
        quickSort(dsLichDatSan, soSanhLichDatTheoThoiGianGiam);

    rebuildIndex();
}
