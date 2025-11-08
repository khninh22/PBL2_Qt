#include "ChiTietDichVu.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Constructor (NEW: Object pointer version - RECOMMENDED)
ChiTietDichVu::ChiTietDichVu(string maLD, DichVu *dv, int sl, double gia)
    : maLichDat(maLD), dichVu(dv), soLuong(sl), donGia(gia), tempMaDV("")
{
    thanhTien = soLuong * donGia;
}

// Constructor (BACKWARD COMPATIBLE: string ID version - for loading old data)
ChiTietDichVu::ChiTietDichVu(string maLD, string maDV, int sl, double gia)
    : maLichDat(maLD), dichVu(nullptr), soLuong(sl), donGia(gia), tempMaDV(maDV)
{
    thanhTien = soLuong * donGia;
}

// Getters
string ChiTietDichVu::getMaLichDat() const { return maLichDat; }

DichVu* ChiTietDichVu::getDichVu() const { return dichVu; } // ✅ Return object pointer

string ChiTietDichVu::getMaDV() const { 
    return (dichVu != nullptr) ? dichVu->getMaDV() : tempMaDV; // ✅ Return pointer data or temp ID
}

string ChiTietDichVu::getTempMaDV() const { return tempMaDV; } // ✅ Get unresolved ID

int ChiTietDichVu::getSoLuong() const { return soLuong; }
double ChiTietDichVu::getDonGia() const { return donGia; }
double ChiTietDichVu::getThanhTien() const { return thanhTien; }

// Setters
void ChiTietDichVu::setDichVu(DichVu *dv)
{
    dichVu = dv; // ✅ Set object pointer
    // NOTE: Don't auto-update donGia during resolution - keep historical price
}

void ChiTietDichVu::setDichVuAndSyncPrice(DichVu *dv)
{
    dichVu = dv; // ✅ Set object pointer
    if (dv != nullptr) {
        donGia = dv->getGiaDV(); // ✅ Update price from DichVu (for NEW bookings)
        thanhTien = soLuong * donGia;
    }
}

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
         << setw(10) << (dichVu ? dichVu->getMaDV() : tempMaDV) // ✅ Navigate pointer
         << setw(10) << soLuong
         << setw(15) << fixed << setprecision(0) << donGia
         << setw(15) << thanhTien
         << endl;
}

// Serialization (SAVE as string IDs for compatibility)
void ChiTietDichVu::serialize(ofstream &out) const
{
    // Ghi maLichDat
    size_t len = maLichDat.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maLichDat.c_str(), len);

    // Ghi maDV (extract from object pointer, or tempMaDV if null)
    string maDV = (dichVu != nullptr) ? dichVu->getMaDV() : tempMaDV;
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

// Deserialization (LOAD as string IDs - will be resolved later)
void ChiTietDichVu::deserialize(ifstream &in)
{
    // NOTE: This reads string IDs from file (backward compatible)
    // QuanLyThueSan will call setDichVu() to resolve pointer after loading
    
    // Đọc maLichDat
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maLichDat.resize(len);
    in.read(&maLichDat[0], len);

    // Đọc maDV (store in tempMaDV, will be used to find DichVu* later)
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    tempMaDV.resize(len);
    in.read(&tempMaDV[0], len);
    dichVu = nullptr; // Will be resolved by QuanLyThueSan

    // Đọc soLuong
    in.read(reinterpret_cast<char *>(&soLuong), sizeof(soLuong));

    // Đọc donGia
    in.read(reinterpret_cast<char *>(&donGia), sizeof(donGia));

    // Đọc thanhTien
    in.read(reinterpret_cast<char *>(&thanhTien), sizeof(thanhTien));
}
