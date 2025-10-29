#ifndef CHITIETDICHVU_H
#define CHITIETDICHVU_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ChiTietDichVu
{
private:
    string maLichDat; // Liên kết với lịch đặt sân
    string maDV;      // Mã dịch vụ
    int soLuong;      // Số lượng
    double donGia;    // Đơn giá tại thời điểm đặt
    double thanhTien; // Thành tiền = soLuong * donGia

public:
    // Constructor
    ChiTietDichVu(string maLD = "", string maDV = "", int sl = 0, double gia = 0.0);

    // Getters
    string getMaLichDat() const;
    string getMaDV() const;
    int getSoLuong() const;
    double getDonGia() const;
    double getThanhTien() const;

    // Setters
    void setSoLuong(int sl);

    // Hiển thị thông tin
    void hienThi() const;

    // Serialization
    void serialize(ofstream &out) const;
    void deserialize(ifstream &in);
};

#endif // CHITIETDICHVU_H
