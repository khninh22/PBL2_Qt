#include "LichDatSan.h"
#include <iomanip>
#include <sstream>

// Hàm format thời gian
string formatTime(time_t t)
{
    if (t == 0)
        return "N/A";
    char buffer[20];
#ifdef _WIN32
    tm local_tm;
    localtime_s(&local_tm, &t);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &local_tm);
#else
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", localtime(&t));
#endif
    return string(buffer);
}

// Constructor
LichDatSan::LichDatSan(string ma, string maKH_, string maSan_,
                       time_t batDau, time_t ketThuc,
                       double tongTien_, string trangThaiDat_)
{
    maLichDat = ma;
    maKH = maKH_;
    maSan = maSan_;
    thoiGianBatDau = batDau;
    thoiGianKetThuc = ketThuc;
    tongTien = tongTien_;
    trangThaiDat = trangThaiDat_;
    trangThaiTT = "Chưa Thanh Toán";
    thoiGianThanhToan = 0;
}

// Getters
string LichDatSan::getMaLichDat() const
{
    return maLichDat;
}

string LichDatSan::getMaSan() const
{
    return maSan;
}

string LichDatSan::getMaKH() const
{
    return maKH;
}

time_t LichDatSan::getThoiGianBatDau() const
{
    return thoiGianBatDau;
}

time_t LichDatSan::getThoiGianKetThuc() const
{
    return thoiGianKetThuc;
}

double LichDatSan::getTongTien() const
{
    return tongTien;
}

string LichDatSan::getTrangThaiDat() const
{
    return trangThaiDat;
}

string LichDatSan::getTrangThaiTT() const
{
    return trangThaiTT;
}

time_t LichDatSan::getThoiGianThanhToan() const
{
    return thoiGianThanhToan;
}

// Setters
void LichDatSan::setTrangThaiDat(const string &s)
{
    trangThaiDat = s;
}

void LichDatSan::setTrangThaiTT(const string &s)
{
    trangThaiTT = s;
}

void LichDatSan::setThoiGianThanhToan(time_t t)
{
    thoiGianThanhToan = t;
}

void LichDatSan::setTongTien(double tien)
{
    tongTien = tien;
}

// Hiển thị thông tin lịch đặt sân
void LichDatSan::hienThi() const
{
    cout << left
         << setw(12) << maLichDat
         << setw(10) << maKH
         << setw(10) << maSan
         << setw(18) << formatTime(thoiGianBatDau)
         << setw(18) << formatTime(thoiGianKetThuc)
         << right << setw(12) << fixed << setprecision(0) << tongTien
         << left
         << setw(14) << trangThaiDat
         << setw(16) << trangThaiTT;

    if (thoiGianThanhToan != 0)
        cout << setw(20) << formatTime(thoiGianThanhToan);
    else
        cout << setw(20) << "N/A";

    cout << endl;
}

// Serialization: Ghi đối tượng vào file binary
void LichDatSan::serialize(ofstream &out) const
{
    // Ghi độ dài và nội dung của maLichDat
    size_t len = maLichDat.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maLichDat.c_str(), len);

    // Ghi độ dài và nội dung của maKH
    len = maKH.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maKH.c_str(), len);

    // Ghi độ dài và nội dung của maSan
    len = maSan.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maSan.c_str(), len);

    // Ghi thoiGianBatDau (time_t)
    out.write(reinterpret_cast<const char *>(&thoiGianBatDau), sizeof(thoiGianBatDau));

    // Ghi thoiGianKetThuc (time_t)
    out.write(reinterpret_cast<const char *>(&thoiGianKetThuc), sizeof(thoiGianKetThuc));

    // Ghi tongTien (double)
    out.write(reinterpret_cast<const char *>(&tongTien), sizeof(tongTien));

    // Ghi độ dài và nội dung của trangThaiDat
    len = trangThaiDat.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(trangThaiDat.c_str(), len);

    // Ghi độ dài và nội dung của trangThaiTT
    len = trangThaiTT.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(trangThaiTT.c_str(), len);

    // Ghi thoiGianThanhToan (time_t)
    out.write(reinterpret_cast<const char *>(&thoiGianThanhToan), sizeof(thoiGianThanhToan));
}

// Deserialization: Đọc đối tượng từ file binary
void LichDatSan::deserialize(ifstream &in)
{
    // Đọc maLichDat
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maLichDat.resize(len);
    in.read(&maLichDat[0], len);

    // Đọc maKH
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maKH.resize(len);
    in.read(&maKH[0], len);

    // Đọc maSan
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maSan.resize(len);
    in.read(&maSan[0], len);

    // Đọc thoiGianBatDau
    in.read(reinterpret_cast<char *>(&thoiGianBatDau), sizeof(thoiGianBatDau));

    // Đọc thoiGianKetThuc
    in.read(reinterpret_cast<char *>(&thoiGianKetThuc), sizeof(thoiGianKetThuc));

    // Đọc tongTien
    in.read(reinterpret_cast<char *>(&tongTien), sizeof(tongTien));

    // Đọc trangThaiDat
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    trangThaiDat.resize(len);
    in.read(&trangThaiDat[0], len);

    // Đọc trangThaiTT
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    trangThaiTT.resize(len);
    in.read(&trangThaiTT[0], len);

    // Đọc thoiGianThanhToan
    in.read(reinterpret_cast<char *>(&thoiGianThanhToan), sizeof(thoiGianThanhToan));
}
