#ifndef LICHDATSAN_H
#define LICHDATSAN_H

#include "NgayGio.h"
#include "KhachHang.h" // ✅ NEW: For object pointers
#include "SanBong.h"    // ✅ NEW: For object pointers
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

class LichDatSan
{
private:
    string maLichDat;
    KhachHang *khachHang;     // ✅ NEW: Object pointer instead of string maKH
    SanBong *sanBong;         // ✅ NEW: Object pointer instead of string maSan
    NgayGio thoiGianBatDau;   // ✅ NEW: NgayGio instead of time_t
    NgayGio thoiGianKetThuc;  // ✅ NEW: NgayGio instead of time_t
    double tongTien;
    string trangThaiDat;      // "Đã Đặt" / "Đã Hủy"
    string trangThaiTT;       // "Chưa Thanh Toán" / "Đã Thanh Toán"
    NgayGio thoiGianThanhToan; // ✅ NEW: NgayGio instead of time_t
    
    // ✅ INTERNAL: Temporary storage for deserialization (will be resolved to pointers)
    string tempMaKH;   // Used during loading to resolve khachHang pointer
    string tempMaSan;  // Used during loading to resolve sanBong pointer

public:
    // Constructor (NEW: Object pointer version)
    LichDatSan(string maLD = "", KhachHang *kh = nullptr, SanBong *san = nullptr,
               const NgayGio &batDau = NgayGio(), const NgayGio &ketThuc = NgayGio(), 
               double tien = 0.0, string status = "Đã Đặt");
    
    // Constructor (BACKWARD COMPATIBLE: string ID version for loading old data)
    LichDatSan(string maLD, string maKH, string maSan,
               const NgayGio &batDau, const NgayGio &ketThuc, 
               double tien, string status);
    
    // Constructor (BACKWARD COMPATIBLE: time_t version)
    LichDatSan(string maLD, string maKH, string maSan,
               time_t batDau, time_t ketThuc, double tien, string status);

    // Getters (NEW: Object pointer version)
    string getMaLichDat() const;
    KhachHang* getKhachHang() const;      // ✅ NEW: Return object pointer
    SanBong* getSanBong() const;          // ✅ NEW: Return object pointer
    string getMaSan() const;              // Keep for backward compatibility
    string getMaKH() const;               // Keep for backward compatibility
    string getTempMaKH() const;           // ✅ NEW: Get temporary ID for resolution
    string getTempMaSan() const;          // ✅ NEW: Get temporary ID for resolution
    NgayGio getThoiGianBatDau() const;
    NgayGio getThoiGianKetThuc() const;
    double getTongTien() const;
    string getTrangThaiDat() const;
    string getTrangThaiTT() const;
    NgayGio getThoiGianThanhToan() const;
    
    // Getters (BACKWARD COMPATIBLE: time_t version)
    time_t getThoiGianBatDauTimeT() const;
    time_t getThoiGianKetThucTimeT() const;
    time_t getThoiGianThanhToanTimeT() const;

    // Setters
    void setKhachHang(KhachHang *kh);     // ✅ NEW: Set object pointer
    void setSanBong(SanBong *san);        // ✅ NEW: Set object pointer
    void setTrangThaiDat(const string &s);
    void setTrangThaiTT(const string &s);
    void setThoiGianThanhToan(const NgayGio &t);
    void setThoiGianThanhToan(time_t t); // BACKWARD COMPATIBLE
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
