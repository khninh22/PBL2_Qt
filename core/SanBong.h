#ifndef SANBONG_H
#define SANBONG_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class SanBong
{
private:
    string maSan;
    string tenSan;
    int loaiSan; // 5, 7, 11
    double giaThue;
    bool baoTri;

public:
    // Constructor
    SanBong(string ma = "", string ten = "", int loai = 0, double gia = 0.0, bool bt = false);

    // Getters
    string getMaSan() const;
    string getTenSan() const;
    int getLoaiSan() const;
    double getGiaThue() const;
    bool getBaoTri() const;

    // Setters
    void setTenSan(const string &ten);
    void setLoaiSan(int loai);
    void setGiaThue(double gia);
    void setBaoTri(bool bt);

    // Hiển thị thông tin
    void hienThi() const;

    // Serialization - Lưu/đọc đối tượng theo hướng đối tượng
    void serialize(ofstream &out) const; // Ghi đối tượng vào file binary
    void deserialize(ifstream &in);      // Đọc đối tượng từ file binary
};

#endif // SANBONG_H
