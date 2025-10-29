#include "KhachHang.h"
#include <iomanip>

// Constructor
KhachHang::KhachHang(string ma, string ten, string sdt_val, int diem, int cap)
    : maKH(ma), hoTen(ten), sdt(sdt_val), diemTichLuy(diem), capDoTV(cap) {}

// Getters
string KhachHang::getMaKH() const { return maKH; }
string KhachHang::getHoTen() const { return hoTen; }
string KhachHang::getSdt() const { return sdt; }
int KhachHang::getDiemTichLuy() const { return diemTichLuy; }
int KhachHang::getCapDoTV() const { return capDoTV; }

string KhachHang::getTenCapDo() const
{
    switch (capDoTV)
    {
    case 1:
        return "Dong";
    case 2:
        return "Bac";
    case 3:
        return "Vang";
    case 4:
        return "Kim Cuong";
    default:
        return "Dong";
    }
}

// Setters
void KhachHang::setHoTen(const string &ten) { hoTen = ten; }
void KhachHang::setSdt(const string &s) { sdt = s; }
void KhachHang::setDiemTichLuy(int diem) { diemTichLuy = diem; }
void KhachHang::setCapDoTV(int cap) { capDoTV = cap; }

// Hệ thống điểm thưởng
void KhachHang::tichDiem(double soTien)
{
    // Mỗi 10,000 VND = 1 điểm
    int diemMoi = (int)(soTien / 10000);
    diemTichLuy += diemMoi;
    capNhatCapDo();
}

bool KhachHang::doiDiem(int diem, double &voucher)
{
    if (diem > diemTichLuy)
    {
        return false; // Không đủ điểm
    }

    if (diem < 100)
    {
        return false; // Tối thiểu 100 điểm
    }

    // 100 điểm = 50,000 VND
    voucher = (diem / 100) * 50000;
    diemTichLuy -= diem;
    return true;
}

void KhachHang::capNhatCapDo()
{
    // Tự động nâng cấp dựa trên điểm
    if (diemTichLuy >= 1000)
        capDoTV = 4; // Kim Cương
    else if (diemTichLuy >= 500)
        capDoTV = 3; // Vàng
    else if (diemTichLuy >= 200)
        capDoTV = 2; // Bạc
    else
        capDoTV = 1; // Đồng
}

double KhachHang::tinhPhanTramGiam() const
{
    // Giảm giá theo cấp độ
    switch (capDoTV)
    {
    case 4:
        return 15.0; // Kim Cương: 15%
    case 3:
        return 10.0; // Vàng: 10%
    case 2:
        return 5.0; // Bạc: 5%
    case 1:
    default:
        return 0.0; // Đồng: 0%
    }
}

// Hiển thị thông tin khách hàng
void KhachHang::hienThi() const
{
    cout << left
         << setw(10) << maKH
         << setw(25) << hoTen
         << setw(15) << sdt
         << setw(10) << diemTichLuy
         << setw(12) << getTenCapDo()
         << endl;
}

// Serialization: Ghi đối tượng vào file binary
void KhachHang::serialize(ofstream &out) const
{
    // Ghi độ dài và nội dung của maKH
    size_t len = maKH.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maKH.c_str(), len);

    // Ghi độ dài và nội dung của hoTen
    len = hoTen.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(hoTen.c_str(), len);

    // Ghi độ dài và nội dung của sdt
    len = sdt.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(sdt.c_str(), len);

    // Ghi diemTichLuy
    out.write(reinterpret_cast<const char *>(&diemTichLuy), sizeof(diemTichLuy));

    // Ghi capDoTV
    out.write(reinterpret_cast<const char *>(&capDoTV), sizeof(capDoTV));
}

// Deserialization: Đọc đối tượng từ file binary
void KhachHang::deserialize(ifstream &in)
{
    // Đọc maKH
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maKH.resize(len);
    in.read(&maKH[0], len);

    // Đọc hoTen
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    hoTen.resize(len);
    in.read(&hoTen[0], len);

    // Đọc sdt
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    sdt.resize(len);
    in.read(&sdt[0], len);

    // Đọc diemTichLuy (nếu có, nếu không thì = 0)
    if (in.peek() != EOF)
    {
        in.read(reinterpret_cast<char *>(&diemTichLuy), sizeof(diemTichLuy));
        in.read(reinterpret_cast<char *>(&capDoTV), sizeof(capDoTV));
    }
    else
    {
        diemTichLuy = 0;
        capDoTV = 1;
    }
}
