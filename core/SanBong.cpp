#include "SanBong.h"
#include <iomanip>
#include <algorithm>

// Constructor
SanBong::SanBong(string ma, string ten, int loai, double gia, bool bt)
    : maSan(ma), tenSan(ten), loaiSan(loai), giaThue(gia), baoTri(bt) {}

// Getters
string SanBong::getMaSan() const
{
    return maSan;
}

string SanBong::getTenSan() const
{
    return tenSan;
}

int SanBong::getLoaiSan() const
{
    return loaiSan;
}

double SanBong::getGiaThue() const
{
    return giaThue;
}

bool SanBong::getBaoTri() const
{
    return baoTri;
}

// Setters
void SanBong::setTenSan(const string &ten)
{
    tenSan = ten;
}

void SanBong::setLoaiSan(int loai)
{
    if (loai == 5 || loai == 7 || loai == 11)
        loaiSan = loai;
}

void SanBong::setGiaThue(double gia)
{
    if (gia >= 0)
        giaThue = gia;
}

void SanBong::setBaoTri(bool bt)
{
    baoTri = bt;
}

// Hiển thị thông tin sân bóng
void SanBong::hienThi() const
{
    cout << left << setw(10) << maSan
         << left << setw(20) << tenSan
         << left << setw(15) << ("San " + to_string(loaiSan) + " nguoi")
         << right << setw(15) << fixed << setprecision(0) << giaThue
         << left << setw(15) << (baoTri ? "Dang Bao Tri" : "Hoat Dong")
         << '\n';
}

// Serialization: Ghi đối tượng vào file binary
void SanBong::serialize(ofstream &out) const
{
    // Ghi độ dài và nội dung của maSan
    size_t len = maSan.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maSan.c_str(), len);

    // Ghi độ dài và nội dung của tenSan
    len = tenSan.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(tenSan.c_str(), len);

    // Ghi loaiSan (int)
    out.write(reinterpret_cast<const char *>(&loaiSan), sizeof(loaiSan));

    // Ghi giaThue (double)
    out.write(reinterpret_cast<const char *>(&giaThue), sizeof(giaThue));

    // Ghi baoTri (bool)
    out.write(reinterpret_cast<const char *>(&baoTri), sizeof(baoTri));
}

// Deserialization: Đọc đối tượng từ file binary
void SanBong::deserialize(ifstream &in)
{
    // Đọc maSan
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maSan.resize(len);
    in.read(&maSan[0], len);

    // Đọc tenSan
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    tenSan.resize(len);
    in.read(&tenSan[0], len);

    // Đọc loaiSan
    in.read(reinterpret_cast<char *>(&loaiSan), sizeof(loaiSan));

    // Đọc giaThue
    in.read(reinterpret_cast<char *>(&giaThue), sizeof(giaThue));

    // Đọc baoTri
    in.read(reinterpret_cast<char *>(&baoTri), sizeof(baoTri));
}
