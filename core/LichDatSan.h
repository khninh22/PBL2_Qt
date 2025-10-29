#ifndef LICHDATSAN_H
#define LICHDATSAN_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

class LichDatSan
{
private:
    string maLichDat;
    string maKH;
    string maSan;
    time_t thoiGianBatDau;
    time_t thoiGianKetThuc;
    double tongTien;
    string trangThaiDat;      // "Da Dat" / "Da Huy"
    string trangThaiTT;       // "Chua Thanh Toan" / "Da Thanh Toan"
    time_t thoiGianThanhToan; // 0 nếu chưa thanh toán

public:
    // Constructor
    LichDatSan(string maLD = "", string maKH = "", string maSan = "",
               time_t batDau = 0, time_t ketThuc = 0, double tien = 0.0,
               string status = "Da Dat");

    // Getters
    string getMaLichDat() const;
    string getMaSan() const;
    string getMaKH() const;
    time_t getThoiGianBatDau() const;
    time_t getThoiGianKetThuc() const;
    double getTongTien() const;
    string getTrangThaiDat() const;
    string getTrangThaiTT() const;
    time_t getThoiGianThanhToan() const;

    // Setters
    void setTrangThaiDat(const string &s);
    void setTrangThaiTT(const string &s);
    void setThoiGianThanhToan(time_t t);
    void setTongTien(double tien);

    // Hiển thị thông tin
    void hienThi() const;

    // Serialization - Lưu/đọc đối tượng theo hướng đối tượng
    void serialize(ofstream &out) const; // Ghi đối tượng vào file binary
    void deserialize(ifstream &in);      // Đọc đối tượng từ file binary
};

// Hàm tiện ích để format time_t thành string
string formatTime(time_t t);

#endif // LICHDATSAN_H
