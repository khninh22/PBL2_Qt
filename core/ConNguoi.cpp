#include "ConNguoi.h"
#include <iostream>

// Constructors
ConNguoi::ConNguoi() : hoTen(""), sdt(""), diaChi("") {}

ConNguoi::ConNguoi(const std::string &ht, const std::string &s, const std::string &dc)
    : hoTen(ht), sdt(s), diaChi(dc) {}

ConNguoi::ConNguoi(const ConNguoi &other)
    : hoTen(other.hoTen), sdt(other.sdt), diaChi(other.diaChi) {}

ConNguoi::~ConNguoi() {}

// Virtual methods
void ConNguoi::hienThi() const
{
    std::cout << "Ho ten: " << hoTen << std::endl;
    std::cout << "SDT: " << sdt << std::endl;
    std::cout << "Dia chi: " << diaChi << std::endl;
}

// ✅ NEW: Enhanced display with more details
void ConNguoi::hienThiThongTin() const
{
    std::cout << "=== THONG TIN " << layLoai() << " ===" << std::endl;
    std::cout << "Ho va Ten: " << hoTen << std::endl;
    std::cout << "So Dien Thoai: " << sdt << std::endl;
    std::cout << "Dia Chi: " << diaChi << std::endl;
}

// Serialization
void ConNguoi::serialize(std::ofstream &out) const
{
    size_t len;

    // Ho ten
    len = hoTen.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(hoTen.c_str(), len);

    // SDT
    len = sdt.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(sdt.c_str(), len);

    // Dia chi
    len = diaChi.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(diaChi.c_str(), len);
}

void ConNguoi::deserialize(std::ifstream &in)
{
    size_t len;

    // Ho ten
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    hoTen.resize(len);
    in.read(&hoTen[0], len);

    // SDT
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    sdt.resize(len);
    in.read(&sdt[0], len);

    // Dia chi
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    diaChi.resize(len);
    in.read(&diaChi[0], len);
}

// ✅ NEW: Text file I/O for polymorphic file operations
void ConNguoi::ghiFile(std::ofstream &out) const
{
    out << hoTen << "\n" << sdt << "\n" << diaChi << "\n";
}

void ConNguoi::docFile(std::ifstream &in)
{
    std::getline(in, hoTen);
    std::getline(in, sdt);
    std::getline(in, diaChi);
}

// Operators
ConNguoi &ConNguoi::operator=(const ConNguoi &other)
{
    if (this != &other)
    {
        hoTen = other.hoTen;
        sdt = other.sdt;
        diaChi = other.diaChi;
    }
    return *this;
}
