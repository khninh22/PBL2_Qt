#ifndef QUANLYTHUESAN_VALIDATED_H
#define QUANLYTHUESAN_VALIDATED_H

#include "QuanLyThueSan.h"
#include "Validator.h"
#include "Exceptions.h"

// ============================
//  WRAPPER CLASS VỚI VALIDATION
//  Thêm validation vào mọi thao tác
// ============================

class QuanLyThueSanValidated : public QuanLyThueSan
{
public:
    // Override themSanBong với validation
    bool themSanBongValidated(const string &maSan, const string &tenSan,
                              int loaiSan, double giaThue)
    {
        // ✅ Validate input
        if (!Validator::kiemTraMa(maSan))
            throw DuLieuKhongHopLeException("Ma san", Validator::thongBaoLoiMa());

        if (!Validator::kiemTraTen(tenSan))
            throw DuLieuKhongHopLeException("Ten san", Validator::thongBaoLoiTen());

        if (!Validator::kiemTraLoaiSan(loaiSan))
            throw DuLieuKhongHopLeException("Loai san", Validator::thongBaoLoiLoaiSan());

        if (!Validator::kiemTraGia(giaThue))
            throw DuLieuKhongHopLeException("Gia thue", Validator::thongBaoLoiGia());

        // ✅ Kiểm tra trùng
        if (timSanBong(maSan) != nullptr)
            throw TrungLapException("San bong", maSan);

        // ✅ Gọi hàm gốc
        return themSanBong(maSan, tenSan, loaiSan, giaThue);
    }

    // Override themKhachHang với validation
    bool themKhachHangValidated(const string &maKH, const string &hoTen, const string &sdt)
    {
        if (!Validator::kiemTraMa(maKH))
            throw DuLieuKhongHopLeException("Ma KH", Validator::thongBaoLoiMa());

        if (!Validator::kiemTraTen(hoTen))
            throw DuLieuKhongHopLeException("Ho ten", Validator::thongBaoLoiTen());

        if (!Validator::kiemTraSDT(sdt))
            throw DuLieuKhongHopLeException("So dien thoai", Validator::thongBaoLoiSDT());

        if (timKhachHang(maKH) != nullptr)
            throw TrungLapException("Khach hang", maKH);

        return themKhachHang(maKH, hoTen, sdt);
    }

    // Override datSan với validation
    bool datSanValidated(const string &maKH, const string &maSan,
                         time_t batDau, time_t ketThuc, string &maLichMoi)
    {
        // Validate mã
        if (!Validator::kiemTraMa(maKH))
            throw DuLieuKhongHopLeException("Ma KH", Validator::thongBaoLoiMa());

        if (!Validator::kiemTraMa(maSan))
            throw DuLieuKhongHopLeException("Ma san", Validator::thongBaoLoiMa());

        // Validate thời gian
        if (!Validator::kiemTraThoiGian(batDau, ketThuc))
            throw DuLieuKhongHopLeException("Thoi gian", Validator::thongBaoLoiThoiGian());

        if (!Validator::kiemTraTrongKhungGio(batDau) || !Validator::kiemTraTrongKhungGio(ketThuc))
            throw NghiepVuException("Thoi gian phai trong khung 6h-22h");

        // Kiểm tra sân tồn tại
        SanBong *san = timSanBong(maSan);
        if (san == nullptr)
            throw KhongTimThayException("San bong", maSan);

        if (san->getBaoTri())
            throw NghiepVuException("San dang bao tri, khong the dat");

        // Kiểm tra khách hàng
        if (timKhachHang(maKH) == nullptr)
            throw KhongTimThayException("Khach hang", maKH);

        // Kiểm tra trùng lịch
        if (kiemTraTrungLich(maSan, batDau, ketThuc))
            throw XungDotException("San da duoc dat trong khung gio nay");

        return datSan(maKH, maSan, batDau, ketThuc, maLichMoi);
    }

    // Override themNhanVien với validation
    bool themNhanVienValidated(const string &maNV, const string &hoTen, const string &sdt,
                               const string &viTri, double luong,
                               const string &taiKhoan = "", const string &matKhau = "")
    {
        if (!Validator::kiemTraMa(maNV))
            throw DuLieuKhongHopLeException("Ma NV", Validator::thongBaoLoiMa());

        if (!Validator::kiemTraTen(hoTen))
            throw DuLieuKhongHopLeException("Ho ten", Validator::thongBaoLoiTen());

        if (!Validator::kiemTraSDT(sdt))
            throw DuLieuKhongHopLeException("SDT", Validator::thongBaoLoiSDT());

        if (!Validator::kiemTraLuong(luong))
            throw DuLieuKhongHopLeException("Luong", Validator::thongBaoLoiLuong());

        if (!taiKhoan.empty() && !Validator::kiemTraMa(taiKhoan))
            throw DuLieuKhongHopLeException("Tai khoan", "Tai khoan khong hop le");

        if (!matKhau.empty() && !Validator::kiemTraPassword(matKhau))
            throw DuLieuKhongHopLeException("Mat khau", Validator::thongBaoLoiPassword());

        if (timNhanVien(maNV) != nullptr)
            throw TrungLapException("Nhan vien", maNV);

        return themNhanVien(maNV, hoTen, sdt, viTri, luong, taiKhoan, matKhau);
    }

    // Override themDichVu với validation
    bool themDichVuValidated(const string &maDV, const string &tenDV,
                             const string &loaiDV, double giaDV)
    {
        if (!Validator::kiemTraMa(maDV))
            throw DuLieuKhongHopLeException("Ma DV", Validator::thongBaoLoiMa());

        if (!Validator::kiemTraTen(tenDV))
            throw DuLieuKhongHopLeException("Ten DV", Validator::thongBaoLoiTen());

        if (!Validator::kiemTraGia(giaDV))
            throw DuLieuKhongHopLeException("Gia DV", Validator::thongBaoLoiGia());

        if (timDichVu(maDV) != nullptr)
            throw TrungLapException("Dich vu", maDV);

        return themDichVu(maDV, tenDV, loaiDV, giaDV);
    }

    // Override themDichVuVaoLich với validation
    bool themDichVuVaoLichValidated(const string &maLichDat, const string &maDV, int soLuong)
    {
        if (!Validator::kiemTraSoLuong(soLuong))
            throw DuLieuKhongHopLeException("So luong", Validator::thongBaoLoiSoLuong());

        if (timLichDat(maLichDat) == nullptr)
            throw KhongTimThayException("Lich dat", maLichDat);

        DichVu *dv = timDichVu(maDV);
        if (dv == nullptr)
            throw KhongTimThayException("Dich vu", maDV);

        if (!dv->getConHang())
            throw NghiepVuException("Dich vu het hang");

        return themDichVuVaoLich(maLichDat, maDV, soLuong);
    }
};

#endif // QUANLYTHUESAN_VALIDATED_H
