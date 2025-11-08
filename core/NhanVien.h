#ifndef NHANVIEN_H
#define NHANVIEN_H

#include "NguoiDung.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/**
 * @brief Lớp nhân viên - Kế thừa từ NguoiDung
 * @details Nhân viên CÓ tài khoản đăng nhập (khác KhachHang)
 *          Lớp cha: ConNguoi -> NguoiDung -> NhanVien (3 levels)
 */
class NhanVien : public NguoiDung
{
private:
    string maNV;
    string viTri;      // "Quản Lý", "Thu Ngân", "Bảo Vệ", "Bảo Trì"
    double luongCoBan; // Lương cơ bản/tháng
    bool dangLam;      // Đang làm hay đã nghỉ

public:
    // Constructors
    NhanVien();
    NhanVien(string ma, string ten = "", string sdt = "", string dc = "",
             string vt = "Thu Ngân", double luong = 5000000, bool lam = true,
             string tk = "", string mk = "");
    NhanVien(const NhanVien &other);

    // Getters
    string getMaNV() const;
    string getHoTen() const { return hoTen; }       // Từ ConNguoi
    string getSdt() const { return sdt; }           // Từ ConNguoi
    string getDiaChi() const { return diaChi; }     // Từ ConNguoi
    string getTaiKhoan() const { return taiKhoan; } // Từ NguoiDung
    string getMatKhau() const { return matKhau; }   // Từ NguoiDung
    string getViTri() const;
    double getLuongCoBan() const;
    bool getDangLam() const;

    // Setters
    void setHoTen(const string &ten) { hoTen = ten; }       // Từ ConNguoi
    void setSdt(const string &s) { sdt = s; }               // Từ ConNguoi
    void setDiaChi(const string &dc) { diaChi = dc; }       // Từ ConNguoi
    void setTaiKhoan(const string &tk) { taiKhoan = tk; }   // Từ NguoiDung
    void setMatKhau(const string &mk) { matKhau = mk; }     // Từ NguoiDung
    void setViTri(const string &vt);
    void setLuongCoBan(double luong);
    void setDangLam(bool lam);

    // Virtual methods override
    virtual void hienThi() const override;
    virtual void hienThiThongTin() const override; // ✅ NEW
    virtual std::string layLoai() const override { return "NhanVien"; }

    // Serialization override - Chain với NguoiDung -> ConNguoi
    virtual void serialize(ofstream &out) const override;
    virtual void deserialize(ifstream &in) override;
    virtual void ghiFile(ofstream &out) const override; // ✅ NEW
    virtual void docFile(ifstream &in) override;        // ✅ NEW

    // Operators
    NhanVien &operator=(const NhanVien &other);
};

#endif // NHANVIEN_H
