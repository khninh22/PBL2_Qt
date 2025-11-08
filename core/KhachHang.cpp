#include "KhachHang.h"
#include <iomanip>

// Constructors
KhachHang::KhachHang()
    : ConNguoi(), maKH(""), diemTichLuy(0), capDoTV(1) {}

KhachHang::KhachHang(string ma, string ten, string sdt_val, string dc, int diem, int cap)
    : ConNguoi(ten, sdt_val, dc), maKH(ma), diemTichLuy(diem), capDoTV(cap) {}

KhachHang::KhachHang(const KhachHang &other)
    : ConNguoi(other), maKH(other.maKH), diemTichLuy(other.diemTichLuy), capDoTV(other.capDoTV) {}

// Getters
string KhachHang::getMaKH() const { return maKH; }
int KhachHang::getDiemTichLuy() const { return diemTichLuy; }
int KhachHang::getCapDoTV() const { return capDoTV; }

string KhachHang::getTenCapDo() const
{
    switch (capDoTV)
    {
    case 1:
        return "Đồng";
    case 2:
        return "Bạc";
    case 3:
        return "Vàng";
    case 4:
        return "Kim Cương";
    default:
        return "Đồng";
    }
}

// Setters
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

// Hiển thị thông tin khách hàng - Override từ ConNguoi
void KhachHang::hienThi() const
{
    cout << left
         << setw(10) << maKH
         << setw(25) << hoTen // Từ ConNguoi
         << setw(15) << sdt   // Từ ConNguoi
         << setw(10) << diemTichLuy
         << setw(12) << getTenCapDo()
         << endl;
}

// ✅ NEW: Enhanced detailed display
void KhachHang::hienThiThongTin() const
{
    ConNguoi::hienThiThongTin(); // Gọi base class
    cout << "Ma Khach Hang: " << maKH << endl;
    cout << "Diem Tich Luy: " << diemTichLuy << " diem" << endl;
    cout << "Cap Do Thanh Vien: " << getTenCapDo() << " (Cap " << capDoTV << ")" << endl;
    cout << "Phan Tram Giam Gia: " << tinhPhanTramGiam() << "%" << endl;
    cout << "==============================" << endl;
}

// Serialization: Ghi đối tượng vào file binary - Chain với ConNguoi
void KhachHang::serialize(ofstream &out) const
{
    // Gọi serialize của ConNguoi (hoTen, sdt, diaChi)
    ConNguoi::serialize(out);

    // Ghi độ dài và nội dung của maKH
    size_t len = maKH.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maKH.c_str(), len);

    // Ghi diemTichLuy
    out.write(reinterpret_cast<const char *>(&diemTichLuy), sizeof(diemTichLuy));

    // Ghi capDoTV
    out.write(reinterpret_cast<const char *>(&capDoTV), sizeof(capDoTV));
}

// Deserialization: Đọc đối tượng từ file binary - Chain với ConNguoi
void KhachHang::deserialize(ifstream &in)
{
    // Gọi deserialize của ConNguoi (hoTen, sdt, diaChi)
    ConNguoi::deserialize(in);

    // Đọc maKH
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maKH.resize(len);
    in.read(&maKH[0], len);

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

// ✅ NEW: Text file I/O
void KhachHang::ghiFile(ofstream &out) const
{
    ConNguoi::ghiFile(out); // Gọi base class
    out << maKH << "\n" << diemTichLuy << "\n" << capDoTV << "\n";
}

void KhachHang::docFile(ifstream &in)
{
    ConNguoi::docFile(in); // Gọi base class
    getline(in, maKH);
    in >> diemTichLuy >> capDoTV;
    in.ignore(); // Clear newline
}

// Operator assignment
KhachHang &KhachHang::operator=(const KhachHang &other)
{
    if (this != &other)
    {
        ConNguoi::operator=(other); // Gọi base assignment
        maKH = other.maKH;
        diemTichLuy = other.diemTichLuy;
        capDoTV = other.capDoTV;
    }
    return *this;
}
