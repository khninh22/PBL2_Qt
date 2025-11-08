#ifndef CHITIETDICHVU_H
#define CHITIETDICHVU_H

#include "DichVu.h" // ✅ NEW: For object pointers
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ChiTietDichVu
{
private:
    string maLichDat; // Liên kết với lịch đặt sân
    DichVu *dichVu;   // ✅ NEW: Object pointer instead of string maDV
    int soLuong;      // Số lượng
    double donGia;    // Đơn giá tại thời điểm đặt
    double thanhTien; // Thành tiền = soLuong * donGia
    
    // ✅ INTERNAL: Temporary storage for deserialization
    string tempMaDV; // Used during loading to resolve dichVu pointer

public:
    // Constructor (NEW: Object pointer version)
    ChiTietDichVu(string maLD = "", DichVu *dv = nullptr, int sl = 0, double gia = 0.0);
    
    // Constructor (BACKWARD COMPATIBLE: string ID version)
    ChiTietDichVu(string maLD, string maDV, int sl, double gia);

    // Getters
    string getMaLichDat() const;
    DichVu* getDichVu() const;       // ✅ NEW: Return object pointer
    string getMaDV() const;          // Keep for backward compatibility
    string getTempMaDV() const;      // ✅ NEW: Get temporary ID for resolution
    int getSoLuong() const;
    double getDonGia() const;
    double getThanhTien() const;

    // Setters
    void setDichVu(DichVu *dv);      // ✅ NEW: Set object pointer (no price update)
    void setSoLuong(int sl);
    
    // ✅ NEW: Set DichVu and auto-update price (for new bookings only)
    void setDichVuAndSyncPrice(DichVu *dv);

    // Hiển thị thông tin
    void hienThi() const;

    // Serialization
    void serialize(ofstream &out) const;
    void deserialize(ifstream &in);
};

#endif // CHITIETDICHVU_H
