#ifndef DICHVU_H
#define DICHVU_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class DichVu
{
private:
    string maDV;
    string tenDV;
    string loaiDV; // "Do Uong" / "Do An" / "Khac"
    double giaDV;
    bool conHang;

public:
    // Constructor
    DichVu(string ma = "", string ten = "", string loai = "Khác", double gia = 0.0, bool hang = true);

    // Getters
    string getMaDV() const;
    string getTenDV() const;
    string getLoaiDV() const;
    double getGiaDV() const;
    bool getConHang() const;

    // Setters
    void setTenDV(const string &ten);
    void setLoaiDV(const string &loai);
    void setGiaDV(double gia);
    void setConHang(bool hang);

    // Hiển thị thông tin
    void hienThi() const;

    // Serialization - Lưu/đọc đối tượng theo hướng đối tượng
    void serialize(ofstream &out) const;
    void deserialize(ifstream &in);
};

#endif // DICHVU_H
