#include "NgayGio.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>

// Constructors
NgayGio::NgayGio() : ngay(), gio(0), phut(0), giay(0) {}

NgayGio::NgayGio(const NgayThang &n, int h, int m, int s)
    : ngay(n), gio(h), phut(m), giay(s) {}

NgayGio::NgayGio(int d, int mon, int y, int h, int m, int s)
    : ngay(d, mon, y), gio(h), phut(m), giay(s) {}

NgayGio::NgayGio(const NgayGio &other)
    : ngay(other.ngay), gio(other.gio), phut(other.phut), giay(other.giay) {}

NgayGio::NgayGio(time_t timestamp)
{
    struct tm *timeinfo = localtime(&timestamp);
    ngay = NgayThang(timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);
    gio = timeinfo->tm_hour;
    phut = timeinfo->tm_min;
    giay = timeinfo->tm_sec;
}

// Setters với validation
bool NgayGio::setNgay(const NgayThang &n)
{
    ngay = n;
    return true;
}

bool NgayGio::setGio(int h)
{
    if (h >= 0 && h <= 23)
    {
        gio = h;
        return true;
    }
    return false;
}

bool NgayGio::setPhut(int m)
{
    if (m >= 0 && m <= 59)
    {
        phut = m;
        return true;
    }
    return false;
}

bool NgayGio::setGiay(int s)
{
    if (s >= 0 && s <= 59)
    {
        giay = s;
        return true;
    }
    return false;
}

bool NgayGio::setNgayGio(const NgayThang &n, int h, int m, int s)
{
    return setNgay(n) && setGio(h) && setPhut(m) && setGiay(s);
}

// toString: "DD/MM/YYYY HH:MM"
std::string NgayGio::toString() const
{
    std::ostringstream oss;
    oss << ngay.toString() << " "
        << std::setfill('0') << std::setw(2) << gio << ":"
        << std::setw(2) << phut;
    return oss.str();
}

// toStringDay: "DD/MM/YYYY"
std::string NgayGio::toStringDay() const
{
    return ngay.toString();
}

// toStringTime: "HH:MM"
std::string NgayGio::toStringTime() const
{
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << gio << ":"
        << std::setw(2) << phut;
    return oss.str();
}

// toStringFull: "DD/MM/YYYY HH:MM:SS"
std::string NgayGio::toStringFull() const
{
    std::ostringstream oss;
    oss << ngay.toString() << " "
        << std::setfill('0') << std::setw(2) << gio << ":"
        << std::setw(2) << phut << ":"
        << std::setw(2) << giay;
    return oss.str();
}

// Convert to time_t
time_t NgayGio::toTimeT() const
{
    struct tm timeinfo = {};
    timeinfo.tm_mday = ngay.getNgay();
    timeinfo.tm_mon = ngay.getThang() - 1;
    timeinfo.tm_year = ngay.getNam() - 1900;
    timeinfo.tm_hour = gio;
    timeinfo.tm_min = phut;
    timeinfo.tm_sec = giay;
    return mktime(&timeinfo);
}

// So sánh
int NgayGio::soSanhVoi(const NgayGio &other) const
{
    int cmpNgay = ngay.soSanhVoi(other.ngay);
    if (cmpNgay != 0)
        return cmpNgay;

    if (gio != other.gio)
        return (gio < other.gio) ? -1 : 1;
    if (phut != other.phut)
        return (phut < other.phut) ? -1 : 1;
    if (giay != other.giay)
        return (giay < other.giay) ? -1 : 1;
    return 0;
}

// Lấy thời gian hiện tại
NgayGio NgayGio::layThoiGianHienTai()
{
    time_t now = time(nullptr);
    return NgayGio(now);
}

// Khoảng cách (giây)
long long NgayGio::soGiayChenhLech(const NgayGio &other) const
{
    time_t t1 = this->toTimeT();
    time_t t2 = other.toTimeT();
    return static_cast<long long>(difftime(t1, t2));
}

// Khoảng cách (giờ)
double NgayGio::soGioChenhLech(const NgayGio &other) const
{
    return soGiayChenhLech(other) / 3600.0;
}

// Operators
bool NgayGio::operator==(const NgayGio &other) const
{
    return soSanhVoi(other) == 0;
}

bool NgayGio::operator!=(const NgayGio &other) const
{
    return soSanhVoi(other) != 0;
}

bool NgayGio::operator<(const NgayGio &other) const
{
    return soSanhVoi(other) < 0;
}

bool NgayGio::operator>(const NgayGio &other) const
{
    return soSanhVoi(other) > 0;
}

bool NgayGio::operator<=(const NgayGio &other) const
{
    return soSanhVoi(other) <= 0;
}

bool NgayGio::operator>=(const NgayGio &other) const
{
    return soSanhVoi(other) >= 0;
}

// Serialization
void NgayGio::ghiFile(std::ofstream &out) const
{
    ngay.ghiFile(out);
    out.write(reinterpret_cast<const char *>(&gio), sizeof(gio));
    out.write(reinterpret_cast<const char *>(&phut), sizeof(phut));
    out.write(reinterpret_cast<const char *>(&giay), sizeof(giay));
}

void NgayGio::docFile(std::ifstream &in)
{
    ngay.docFile(in);
    in.read(reinterpret_cast<char *>(&gio), sizeof(gio));
    in.read(reinterpret_cast<char *>(&phut), sizeof(phut));
    in.read(reinterpret_cast<char *>(&giay), sizeof(giay));
}
