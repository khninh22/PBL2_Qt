#include "NhanVien.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Constructor
NhanVien::NhanVien(string ma, string ten, string sdt, string vt, double luong, bool lam, string tk, string mk)
    : maNV(ma), hoTen(ten), sdt(sdt), viTri(vt), luongCoBan(luong), dangLam(lam), taiKhoan(tk), matKhau(mk) {}

// Getters
string NhanVien::getMaNV() const { return maNV; }
string NhanVien::getHoTen() const { return hoTen; }
string NhanVien::getSdt() const { return sdt; }
string NhanVien::getViTri() const { return viTri; }
double NhanVien::getLuongCoBan() const { return luongCoBan; }
bool NhanVien::getDangLam() const { return dangLam; }
string NhanVien::getTaiKhoan() const { return taiKhoan; }
string NhanVien::getMatKhau() const { return matKhau; }

// Setters
void NhanVien::setHoTen(const string &ten) { hoTen = ten; }
void NhanVien::setSdt(const string &s) { sdt = s; }
void NhanVien::setViTri(const string &vt) { viTri = vt; }
void NhanVien::setLuongCoBan(double luong) { luongCoBan = luong; }
void NhanVien::setDangLam(bool lam) { dangLam = lam; }
void NhanVien::setTaiKhoan(const string &tk) { taiKhoan = tk; }
void NhanVien::setMatKhau(const string &mk) { matKhau = mk; }

// Hiển thị thông tin
void NhanVien::hienThi() const
{
    cout << left
         << setw(10) << maNV
         << setw(25) << hoTen
         << setw(15) << sdt
         << setw(15) << viTri
         << setw(15) << fixed << setprecision(0) << luongCoBan
         << setw(12) << (dangLam ? "Dang Lam" : "Da Nghi")
         << endl;
}

// Serialization
void NhanVien::serialize(ofstream &out) const
{
    // Ghi maNV
    size_t len = maNV.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maNV.c_str(), len);

    // Ghi hoTen
    len = hoTen.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(hoTen.c_str(), len);

    // Ghi sdt
    len = sdt.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(sdt.c_str(), len);

    // Ghi viTri
    len = viTri.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(viTri.c_str(), len);

    // Ghi luongCoBan
    out.write(reinterpret_cast<const char *>(&luongCoBan), sizeof(luongCoBan));

    // Ghi dangLam
    out.write(reinterpret_cast<const char *>(&dangLam), sizeof(dangLam));

    // Ghi taiKhoan
    len = taiKhoan.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(taiKhoan.c_str(), len);

    // Ghi matKhau
    len = matKhau.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(matKhau.c_str(), len);
}

// Deserialization
void NhanVien::deserialize(ifstream &in)
{
    // Đọc maNV
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maNV.resize(len);
    in.read(&maNV[0], len);

    // Đọc hoTen
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    hoTen.resize(len);
    in.read(&hoTen[0], len);

    // Đọc sdt
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    sdt.resize(len);
    in.read(&sdt[0], len);

    // Đọc viTri
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    viTri.resize(len);
    in.read(&viTri[0], len);

    // Đọc luongCoBan
    in.read(reinterpret_cast<char *>(&luongCoBan), sizeof(luongCoBan));

    // Đọc dangLam
    in.read(reinterpret_cast<char *>(&dangLam), sizeof(dangLam));

    // Đọc taiKhoan
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    taiKhoan.resize(len);
    in.read(&taiKhoan[0], len);

    // Đọc matKhau
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    matKhau.resize(len);
    in.read(&matKhau[0], len);
}
