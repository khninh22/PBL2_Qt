#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <regex>
using namespace std;

// ============================
//  VALIDATOR CLASS
//  Kiểm tra tính hợp lệ của dữ liệu
// ============================

class Validator
{
public:
    // Kiểm tra số điện thoại (10 số, bắt đầu bằng 0)
    static bool kiemTraSDT(const string &sdt)
    {
        if (sdt.empty() || sdt.length() != 10)
            return false;
        if (sdt[0] != '0')
            return false;
        for (char c : sdt)
        {
            if (!isdigit(c))
                return false;
        }
        return true;
    }

    // Kiểm tra mã (không rỗng, độ dài hợp lý)
    static bool kiemTraMa(const string &ma)
    {
        return !ma.empty() && ma.length() >= 3 && ma.length() <= 20;
    }

    // Kiểm tra tên (không rỗng, không quá dài)
    static bool kiemTraTen(const string &ten)
    {
        return !ten.empty() && ten.length() >= 2 && ten.length() <= 100;
    }

    // Kiểm tra giá (dương, trong khoảng hợp lý)
    static bool kiemTraGia(double gia)
    {
        return gia > 0 && gia <= 10000000; // Tối đa 10 triệu
    }

    // Kiểm tra loại sân (chỉ 5, 7, 11)
    static bool kiemTraLoaiSan(int loaiSan)
    {
        return loaiSan == 5 || loaiSan == 7 || loaiSan == 11;
    }

    // Kiểm tra giờ (trong khung 6-22)
    static bool kiemTraGio(int gio)
    {
        return gio >= 6 && gio <= 22;
    }

    // Kiểm tra khoảng thời gian hợp lý
    static bool kiemTraThoiGian(time_t batDau, time_t ketThuc)
    {
        if (ketThuc <= batDau)
            return false;

        // Kiểm tra thời gian không quá xa trong quá khứ
        time_t now = time(nullptr);
        if (batDau < now - 86400)
            return false; // Không cho đặt quá 1 ngày trước

        // Kiểm tra thời gian không quá xa trong tương lai
        if (batDau > now + 86400 * 365)
            return false; // Không cho đặt quá 1 năm sau

        // Kiểm tra độ dài hợp lý (1-8 giờ)
        double soGio = difftime(ketThuc, batDau) / 3600.0;
        return soGio >= 1.0 && soGio <= 8.0;
    }

    // Kiểm tra trong khung giờ mở cửa
    static bool kiemTraTrongKhungGio(time_t thoiGian)
    {
        tm t = *localtime(&thoiGian);
        return t.tm_hour >= 6 && t.tm_hour < 22;
    }

    // Kiểm tra lương hợp lý
    static bool kiemTraLuong(double luong)
    {
        return luong >= 3000000 && luong <= 100000000; // 3tr - 100tr
    }

    // Kiểm tra số lượng dịch vụ
    static bool kiemTraSoLuong(int soLuong)
    {
        return soLuong > 0 && soLuong <= 100;
    }

    // Kiểm tra email (nếu có)
    static bool kiemTraEmail(const string &email)
    {
        if (email.empty())
            return true; // Email không bắt buộc
        regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return regex_match(email, pattern);
    }

    // Kiểm tra password (tối thiểu 6 ký tự)
    static bool kiemTraPassword(const string &pass)
    {
        return pass.length() >= 6 && pass.length() <= 50;
    }

    // Message lỗi tiếng Việt
    static string thongBaoLoiSDT()
    {
        return "So dien thoai khong hop le! (10 so, bat dau bang 0)";
    }

    static string thongBaoLoiMa()
    {
        return "Ma khong hop le! (3-20 ky tu)";
    }

    static string thongBaoLoiTen()
    {
        return "Ten khong hop le! (2-100 ky tu)";
    }

    static string thongBaoLoiGia()
    {
        return "Gia khong hop le! (0 - 10,000,000 VND)";
    }

    static string thongBaoLoiLoaiSan()
    {
        return "Loai san khong hop le! (Chi co: 5, 7, 11 nguoi)";
    }

    static string thongBaoLoiThoiGian()
    {
        return "Thoi gian khong hop le! (1-8 gio, trong khung 6h-22h)";
    }

    static string thongBaoLoiLuong()
    {
        return "Luong khong hop le! (3,000,000 - 100,000,000 VND)";
    }

    static string thongBaoLoiSoLuong()
    {
        return "So luong khong hop le! (1-100)";
    }

    static string thongBaoLoiPassword()
    {
        return "Mat khau khong hop le! (6-50 ky tu)";
    }
};

#endif // VALIDATOR_H
