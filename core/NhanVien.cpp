#include "NhanVien.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Constructors
NhanVien::NhanVien()
    : NguoiDung(), maNV(""), viTri("Thu Ngân"), luongCoBan(5000000), dangLam(true) {}

NhanVien::NhanVien(string ma, string ten, string sdt, string dc,
                   string vt, double luong, bool lam, string tk, string mk)
    : NguoiDung(ten, sdt, dc, tk, mk), maNV(ma), viTri(vt), luongCoBan(luong), dangLam(lam) {}

NhanVien::NhanVien(const NhanVien &other)
    : NguoiDung(other), maNV(other.maNV), viTri(other.viTri),
      luongCoBan(other.luongCoBan), dangLam(other.dangLam) {}

// Getters
string NhanVien::getMaNV() const { return maNV; }
string NhanVien::getViTri() const { return viTri; }
double NhanVien::getLuongCoBan() const { return luongCoBan; }
bool NhanVien::getDangLam() const { return dangLam; }

// Setters
void NhanVien::setViTri(const string &vt) { viTri = vt; }
void NhanVien::setLuongCoBan(double luong) { luongCoBan = luong; }
void NhanVien::setDangLam(bool lam) { dangLam = lam; }

// Hiển thị thông tin - Override từ NguoiDung
void NhanVien::hienThi() const
{
    cout << left
         << setw(10) << maNV
         << setw(25) << hoTen // Từ ConNguoi
         << setw(15) << sdt   // Từ ConNguoi
         << setw(15) << viTri
         << setw(15) << fixed << setprecision(0) << luongCoBan
         << setw(12) << (dangLam ? "Dang Lam" : "Da Nghi")
         << endl;
}

// ✅ NEW: Enhanced detailed display
void NhanVien::hienThiThongTin() const
{
    NguoiDung::hienThiThongTin(); // Gọi base class
    cout << "Ma Nhan Vien: " << maNV << endl;
    cout << "Vi Tri: " << viTri << endl;
    cout << "Luong Co Ban: " << fixed << setprecision(0) << luongCoBan << " VND" << endl;
    cout << "Trang Thai Lam Viec: " << (dangLam ? "Dang Lam" : "Da Nghi") << endl;
    cout << "==============================" << endl;
}

// Serialization - Chain với NguoiDung -> ConNguoi
void NhanVien::serialize(ofstream &out) const
{
    // Gọi serialize của NguoiDung (hoTen, sdt, diaChi, taiKhoan, matKhau)
    NguoiDung::serialize(out);

    // Ghi maNV
    size_t len = maNV.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maNV.c_str(), len);

    // Ghi viTri
    len = viTri.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(viTri.c_str(), len);

    // Ghi luongCoBan
    out.write(reinterpret_cast<const char *>(&luongCoBan), sizeof(luongCoBan));

    // Ghi dangLam
    out.write(reinterpret_cast<const char *>(&dangLam), sizeof(dangLam));
}

// Deserialization - Chain với NguoiDung -> ConNguoi
void NhanVien::deserialize(ifstream &in)
{
    // Gọi deserialize của NguoiDung (hoTen, sdt, diaChi, taiKhoan, matKhau)
    NguoiDung::deserialize(in);

    // Đọc maNV
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maNV.resize(len);
    in.read(&maNV[0], len);

    // Đọc viTri
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    viTri.resize(len);
    in.read(&viTri[0], len);

    // Đọc luongCoBan
    in.read(reinterpret_cast<char *>(&luongCoBan), sizeof(luongCoBan));

    // Đọc dangLam
    in.read(reinterpret_cast<char *>(&dangLam), sizeof(dangLam));
}

// ✅ NEW: Text file I/O
void NhanVien::ghiFile(ofstream &out) const
{
    NguoiDung::ghiFile(out); // Gọi base class
    out << maNV << "\n" << viTri << "\n" << luongCoBan << "\n" << dangLam << "\n";
}

void NhanVien::docFile(ifstream &in)
{
    NguoiDung::docFile(in); // Gọi base class
    getline(in, maNV);
    getline(in, viTri);
    in >> luongCoBan >> dangLam;
    in.ignore(); // Clear newline
}

// Operator assignment
NhanVien &NhanVien::operator=(const NhanVien &other)
{
    if (this != &other)
    {
        NguoiDung::operator=(other); // Gọi base assignment
        maNV = other.maNV;
        viTri = other.viTri;
        luongCoBan = other.luongCoBan;
        dangLam = other.dangLam;
    }
    return *this;
}
