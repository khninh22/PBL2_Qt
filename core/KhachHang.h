#ifndef KHACHHANG_H
#define KHACHHANG_H

#include "ConNguoi.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/**
 * @brief Lớp khách hàng - Kế thừa từ ConNguoi
 * @details Khách hàng KHÔNG CÓ tài khoản đăng nhập (khác NhanVien)
 *          Có hệ thống điểm thưởng và cấp độ thành viên
 */
class KhachHang : public ConNguoi
{
private:
    string maKH;
    int diemTichLuy; // Điểm thưởng tích lũy
    int capDoTV;     // Cấp độ: 1=Đồng, 2=Bạc, 3=Vàng, 4=Kim Cương

public:
    // Constructors
    KhachHang();
    KhachHang(string ma, string ten = "", string sdt = "", string dc = "", int diem = 0, int cap = 1);
    KhachHang(const KhachHang &other);

    // Getters
    string getMaKH() const;
    string getHoTen() const { return hoTen; }   // Từ ConNguoi
    string getSdt() const { return sdt; }       // Từ ConNguoi
    string getDiaChi() const { return diaChi; } // Từ ConNguoi
    int getDiemTichLuy() const;
    int getCapDoTV() const;
    string getTenCapDo() const; // Trả về tên cấp độ (Đồng/Bạc/Vàng/Kim Cương)

    // Setters
    void setHoTen(const string &ten) { hoTen = ten; }  // Từ ConNguoi
    void setSdt(const string &s) { sdt = s; }          // Từ ConNguoi
    void setDiaChi(const string &dc) { diaChi = dc; }  // Từ ConNguoi
    void setDiemTichLuy(int diem);
    void setCapDoTV(int cap);

    // Hệ thống điểm thưởng
    void tichDiem(double soTien);            // Mỗi 10,000 VND = 1 điểm
    bool doiDiem(int diem, double &voucher); // 100 điểm = 50,000 VND
    void capNhatCapDo();                     // Tự động nâng cấp dựa trên điểm
    double tinhPhanTramGiam() const;         // Giảm giá theo cấp độ

    // Virtual methods override
    virtual void hienThi() const override;
    virtual void hienThiThongTin() const override; // ✅ NEW
    virtual std::string layLoai() const override { return "KhachHang"; }

    // Serialization override - Chain với ConNguoi
    virtual void serialize(ofstream &out) const override;
    virtual void deserialize(ifstream &in) override;
    virtual void ghiFile(ofstream &out) const override; // ✅ NEW
    virtual void docFile(ifstream &in) override;        // ✅ NEW

    // Operators
    KhachHang &operator=(const KhachHang &other);
};

#endif // KHACHHANG_H
