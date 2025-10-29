#ifndef KHACHHANG_H
#define KHACHHANG_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class KhachHang
{
private:
    string maKH;
    string hoTen;
    string sdt;
    int diemTichLuy; // Điểm thưởng tích lũy
    int capDoTV;     // Cấp độ: 1=Đồng, 2=Bạc, 3=Vàng, 4=Kim Cương

public:
    // Constructor
    KhachHang(string ma = "", string ten = "", string sdt = "", int diem = 0, int cap = 1);

    // Getters
    string getMaKH() const;
    string getHoTen() const;
    string getSdt() const;
    int getDiemTichLuy() const;
    int getCapDoTV() const;
    string getTenCapDo() const; // Trả về tên cấp độ (Đồng/Bạc/Vàng/Kim Cương)

    // Setters
    void setHoTen(const string &ten);
    void setSdt(const string &sdt);
    void setDiemTichLuy(int diem);
    void setCapDoTV(int cap);

    // Hệ thống điểm thưởng
    void tichDiem(double soTien);            // Mỗi 10,000 VND = 1 điểm
    bool doiDiem(int diem, double &voucher); // 100 điểm = 50,000 VND
    void capNhatCapDo();                     // Tự động nâng cấp dựa trên điểm
    double tinhPhanTramGiam() const;         // Giảm giá theo cấp độ

    // Hiển thị thông tin
    void hienThi() const;

    // Serialization - Lưu/đọc đối tượng theo hướng đối tượng
    void serialize(ofstream &out) const; // Ghi đối tượng vào file binary
    void deserialize(ifstream &in);      // Đọc đối tượng từ file binary
};

#endif // KHACHHANG_H
