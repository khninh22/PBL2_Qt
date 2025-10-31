#ifndef NHANVIEN_H
#define NHANVIEN_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class NhanVien
{
private:
    string maNV;
    string hoTen;
    string sdt;
    string viTri;      // "Quản Lý", "Thu Ngân", "Bảo Vệ", "Bảo Trì"
    double luongCoBan; // Lương cơ bản/tháng
    bool dangLam;      // Đang làm hay đã nghỉ
    string taiKhoan;   // Tài khoản đăng nhập
    string matKhau;    // Mật khẩu đăng nhập

public:
    // Constructor
    NhanVien(string ma = "", string ten = "", string sdt = "",
             string vt = "Thu Ngân", double luong = 5000000, bool lam = true,
             string tk = "", string mk = "");

    // Getters
    string getMaNV() const;
    string getHoTen() const;
    string getSdt() const;
    string getViTri() const;
    double getLuongCoBan() const;
    bool getDangLam() const;
    string getTaiKhoan() const;
    string getMatKhau() const;

    // Setters
    void setHoTen(const string &ten);
    void setSdt(const string &sdt);
    void setViTri(const string &vt);
    void setLuongCoBan(double luong);
    void setDangLam(bool lam);
    void setTaiKhoan(const string &tk);
    void setMatKhau(const string &mk);

    // Hiển thị thông tin
    void hienThi() const;

    // Serialization
    void serialize(ofstream &out) const;
    void deserialize(ifstream &in);
};

#endif // NHANVIEN_H
