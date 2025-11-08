#include "NguoiDung.h"
#include <iostream>

// Constructors
NguoiDung::NguoiDung() : ConNguoi(), taiKhoan(""), matKhau(""), hoatDong(true) {}

NguoiDung::NguoiDung(const std::string &ht, const std::string &sdt, const std::string &dc,
                     const std::string &tk, const std::string &mk)
    : ConNguoi(ht, sdt, dc), taiKhoan(tk), matKhau(mk), hoatDong(true) {}

NguoiDung::NguoiDung(const NguoiDung &other)
    : ConNguoi(other), taiKhoan(other.taiKhoan), matKhau(other.matKhau), hoatDong(other.hoatDong) {}

NguoiDung::~NguoiDung() {}

// Methods
bool NguoiDung::dangNhap(const std::string &mk) const
{
    return hoatDong && (matKhau == mk);
}

bool NguoiDung::doiMatKhau(const std::string &mkCu, const std::string &mkMoi)
{
    if (matKhau == mkCu)
    {
        matKhau = mkMoi;
        return true;
    }
    return false;
}

// Virtual methods
void NguoiDung::hienThi() const
{
    ConNguoi::hienThi(); // Gọi base class
    std::cout << "Tai khoan: " << taiKhoan << std::endl;
    std::cout << "Trang thai: " << (hoatDong ? "Hoat dong" : "Bi khoa") << std::endl;
}

// ✅ NEW: Enhanced detailed display
void NguoiDung::hienThiThongTin() const
{
    ConNguoi::hienThiThongTin(); // Gọi base class
    std::cout << "Tai Khoan: " << taiKhoan << std::endl;
    std::cout << "Trang Thai: " << (hoatDong ? "Hoat Dong" : "Bi Khoa") << std::endl;
    std::cout << "==============================" << std::endl;
}

// Serialization
void NguoiDung::serialize(std::ofstream &out) const
{
    ConNguoi::serialize(out); // Gọi base class

    size_t len;

    // Tai khoan
    len = taiKhoan.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(taiKhoan.c_str(), len);

    // Mat khau
    len = matKhau.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(matKhau.c_str(), len);

    // Hoat dong
    out.write(reinterpret_cast<const char *>(&hoatDong), sizeof(hoatDong));
}

void NguoiDung::deserialize(std::ifstream &in)
{
    ConNguoi::deserialize(in); // Gọi base class

    size_t len;

    // Tai khoan
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    taiKhoan.resize(len);
    in.read(&taiKhoan[0], len);

    // Mat khau
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    matKhau.resize(len);
    in.read(&matKhau[0], len);

    // Hoat dong
    in.read(reinterpret_cast<char *>(&hoatDong), sizeof(hoatDong));
}

// ✅ NEW: Text file I/O
void NguoiDung::ghiFile(std::ofstream &out) const
{
    ConNguoi::ghiFile(out); // Gọi base class
    out << taiKhoan << "\n" << matKhau << "\n" << hoatDong << "\n";
}

void NguoiDung::docFile(std::ifstream &in)
{
    ConNguoi::docFile(in); // Gọi base class
    std::getline(in, taiKhoan);
    std::getline(in, matKhau);
    in >> hoatDong;
    in.ignore(); // Clear newline
}

// Operators
NguoiDung &NguoiDung::operator=(const NguoiDung &other)
{
    if (this != &other)
    {
        ConNguoi::operator=(other);
        taiKhoan = other.taiKhoan;
        matKhau = other.matKhau;
        hoatDong = other.hoatDong;
    }
    return *this;
}
