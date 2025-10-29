#include "ChiTietDichVu.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Constructor
ChiTietDichVu::ChiTietDichVu(string maLD, string maDV, int sl, double gia)
    : maLichDat(maLD), maDV(maDV), soLuong(sl), donGia(gia)
{
    thanhTien = soLuong * donGia;
}

// Getters
string ChiTietDichVu::getMaLichDat() const { return maLichDat; }
string ChiTietDichVu::getMaDV() const { return maDV; }
int ChiTietDichVu::getSoLuong() const { return soLuong; }
double ChiTietDichVu::getDonGia() const { return donGia; }
double ChiTietDichVu::getThanhTien() const { return thanhTien; }

// Setters
void ChiTietDichVu::setSoLuong(int sl)
{
    soLuong = sl;
    thanhTien = soLuong * donGia;
}

// Hiển thị thông tin
void ChiTietDichVu::hienThi() const
{
    cout << left
         << setw(12) << maLichDat
         << setw(10) << maDV
         << setw(10) << soLuong
         << setw(15) << fixed << setprecision(0) << donGia
         << setw(15) << thanhTien
         << endl;
}

// Serialization
void ChiTietDichVu::serialize(ofstream &out) const
{
    // Ghi maLichDat
    size_t len = maLichDat.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maLichDat.c_str(), len);

    // Ghi maDV
    len = maDV.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maDV.c_str(), len);

    // Ghi soLuong
    out.write(reinterpret_cast<const char *>(&soLuong), sizeof(soLuong));

    // Ghi donGia
    out.write(reinterpret_cast<const char *>(&donGia), sizeof(donGia));

    // Ghi thanhTien
    out.write(reinterpret_cast<const char *>(&thanhTien), sizeof(thanhTien));
}

// Deserialization
void ChiTietDichVu::deserialize(ifstream &in)
{
    // Đọc maLichDat
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maLichDat.resize(len);
    in.read(&maLichDat[0], len);

    // Đọc maDV
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maDV.resize(len);
    in.read(&maDV[0], len);

    // Đọc soLuong
    in.read(reinterpret_cast<char *>(&soLuong), sizeof(soLuong));

    // Đọc donGia
    in.read(reinterpret_cast<char *>(&donGia), sizeof(donGia));

    // Đọc thanhTien
    in.read(reinterpret_cast<char *>(&thanhTien), sizeof(thanhTien));
}
