#include "DichVu.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Constructor
DichVu::DichVu(string ma, string ten, string loai, double gia, bool hang)
    : maDV(ma), tenDV(ten), loaiDV(loai), giaDV(gia), conHang(hang) {}

// Getters
string DichVu::getMaDV() const { return maDV; }
string DichVu::getTenDV() const { return tenDV; }
string DichVu::getLoaiDV() const { return loaiDV; }
double DichVu::getGiaDV() const { return giaDV; }
bool DichVu::getConHang() const { return conHang; }

// Setters
void DichVu::setTenDV(const string &ten) { tenDV = ten; }
void DichVu::setLoaiDV(const string &loai) { loaiDV = loai; }
void DichVu::setGiaDV(double gia) { giaDV = gia; }
void DichVu::setConHang(bool hang) { conHang = hang; }

// Hiển thị thông tin
void DichVu::hienThi() const
{
    cout << left
         << setw(10) << maDV
         << setw(25) << tenDV
         << setw(12) << loaiDV
         << setw(15) << fixed << setprecision(0) << giaDV
         << setw(12) << (conHang ? "Con Hang" : "Het Hang")
         << endl;
}

// Serialization: Ghi đối tượng vào file binary
void DichVu::serialize(ofstream &out) const
{
    // Ghi maDV
    size_t len = maDV.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maDV.c_str(), len);

    // Ghi tenDV
    len = tenDV.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(tenDV.c_str(), len);

    // Ghi loaiDV
    len = loaiDV.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(loaiDV.c_str(), len);

    // Ghi giaDV
    out.write(reinterpret_cast<const char *>(&giaDV), sizeof(giaDV));

    // Ghi conHang
    out.write(reinterpret_cast<const char *>(&conHang), sizeof(conHang));
}

// Deserialization: Đọc đối tượng từ file binary
void DichVu::deserialize(ifstream &in)
{
    // Đọc maDV
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maDV.resize(len);
    in.read(&maDV[0], len);

    // Đọc tenDV
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    tenDV.resize(len);
    in.read(&tenDV[0], len);

    // Đọc loaiDV
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    loaiDV.resize(len);
    in.read(&loaiDV[0], len);

    // Đọc giaDV
    in.read(reinterpret_cast<char *>(&giaDV), sizeof(giaDV));

    // Đọc conHang
    in.read(reinterpret_cast<char *>(&conHang), sizeof(conHang));
}
