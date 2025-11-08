#include "NgayThang.h"
#include <sstream>
#include <iomanip>
#include <ctime>

// Constructors
NgayThang::NgayThang() : ngay(1), thang(1), nam(2000) {}

NgayThang::NgayThang(int d, int m, int y) : ngay(d), thang(m), nam(y)
{
    // Validation trong setters nếu cần
}

NgayThang::NgayThang(const NgayThang &other)
    : ngay(other.ngay), thang(other.thang), nam(other.nam) {}

NgayThang::NgayThang(time_t timestamp)
{
    struct tm *timeinfo = localtime(&timestamp);
    ngay = timeinfo->tm_mday;
    thang = timeinfo->tm_mon + 1;
    nam = timeinfo->tm_year + 1900;
}

// Setters với validation
bool NgayThang::setNgay(int d)
{
    if (d >= 1 && d <= soNgayTrongThang())
    {
        ngay = d;
        return true;
    }
    return false;
}

bool NgayThang::setThang(int m)
{
    if (m >= 1 && m <= 12)
    {
        thang = m;
        return true;
    }
    return false;
}

bool NgayThang::setNam(int y)
{
    if (y >= 1900 && y <= 2100)
    {
        nam = y;
        return true;
    }
    return false;
}

bool NgayThang::setNgayThang(int d, int m, int y)
{
    return setNam(y) && setThang(m) && setNgay(d);
}

// toString: "DD/MM/YYYY"
std::string NgayThang::toString() const
{
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << ngay << "/"
        << std::setw(2) << thang << "/"
        << nam;
    return oss.str();
}

// hienThiDayDu: "Ngày DD tháng MM năm YYYY"
std::string NgayThang::hienThiDayDu() const
{
    std::ostringstream oss;
    oss << "Ngày " << ngay << " tháng " << thang << " năm " << nam;
    return oss.str();
}

// Convert to time_t (midnight)
time_t NgayThang::toTimeT() const
{
    struct tm timeinfo = {};
    timeinfo.tm_mday = ngay;
    timeinfo.tm_mon = thang - 1;
    timeinfo.tm_year = nam - 1900;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    return mktime(&timeinfo);
}

// So sánh
int NgayThang::soSanhVoi(const NgayThang &other) const
{
    if (nam != other.nam)
        return (nam < other.nam) ? -1 : 1;
    if (thang != other.thang)
        return (thang < other.thang) ? -1 : 1;
    if (ngay != other.ngay)
        return (ngay < other.ngay) ? -1 : 1;
    return 0;
}

// Năm nhuận
bool NgayThang::laNamNhuan() const
{
    return (nam % 4 == 0 && nam % 100 != 0) || (nam % 400 == 0);
}

// Số ngày trong tháng
int NgayThang::soNgayTrongThang() const
{
    static const int ngayTrongThang[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (thang == 2 && laNamNhuan())
        return 29;
    return ngayTrongThang[thang - 1];
}

// Lấy ngày hiện tại
NgayThang NgayThang::layNgayHienTai()
{
    time_t now = time(nullptr);
    return NgayThang(now);
}

// Operators
bool NgayThang::operator==(const NgayThang &other) const
{
    return soSanhVoi(other) == 0;
}

bool NgayThang::operator!=(const NgayThang &other) const
{
    return soSanhVoi(other) != 0;
}

bool NgayThang::operator<(const NgayThang &other) const
{
    return soSanhVoi(other) < 0;
}

bool NgayThang::operator>(const NgayThang &other) const
{
    return soSanhVoi(other) > 0;
}

bool NgayThang::operator<=(const NgayThang &other) const
{
    return soSanhVoi(other) <= 0;
}

bool NgayThang::operator>=(const NgayThang &other) const
{
    return soSanhVoi(other) >= 0;
}

// Serialization
void NgayThang::ghiFile(std::ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&ngay), sizeof(ngay));
    out.write(reinterpret_cast<const char *>(&thang), sizeof(thang));
    out.write(reinterpret_cast<const char *>(&nam), sizeof(nam));
}

void NgayThang::docFile(std::ifstream &in)
{
    in.read(reinterpret_cast<char *>(&ngay), sizeof(ngay));
    in.read(reinterpret_cast<char *>(&thang), sizeof(thang));
    in.read(reinterpret_cast<char *>(&nam), sizeof(nam));
}
