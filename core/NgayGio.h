#ifndef NGAYGIO_H
#define NGAYGIO_H

#include "NgayThang.h"
#include <string>
#include <fstream>
#include <ctime>

/**
 * @brief Lớp quản lý ngày giờ đầy đủ (DD/MM/YYYY HH:MM:SS)
 * @details PRACTICAL APPLICATION:
 *          - Replace time_t in LichDatSan for better booking display
 *          - Vietnamese datetime format (DD/MM/YYYY HH:MM)
 *          - Easier hour/minute filtering in UI
 *          - Better timezone handling
 */
class NgayGio
{
private:
    NgayThang ngay;
    int gio;  // 0-23
    int phut; // 0-59
    int giay; // 0-59

public:
    // Constructors
    NgayGio();
    NgayGio(const NgayThang &n, int h, int m, int s = 0);
    NgayGio(int d, int mon, int y, int h, int m, int s = 0);
    NgayGio(const NgayGio &other);
    NgayGio(time_t timestamp); // Convert from time_t

    // Getters
    NgayThang getNgay() const { return ngay; }
    int getGio() const { return gio; }
    int getPhut() const { return phut; }
    int getGiay() const { return giay; }

    // Setters với validation
    bool setNgay(const NgayThang &n);
    bool setGio(int h);
    bool setPhut(int m);
    bool setGiay(int s);
    bool setNgayGio(const NgayThang &n, int h, int m, int s = 0);

    // Utility methods
    std::string toString() const;                   // "DD/MM/YYYY HH:MM"
    std::string toStringDay() const;                // "DD/MM/YYYY"
    std::string toStringTime() const;               // "HH:MM"
    std::string toStringFull() const;               // "DD/MM/YYYY HH:MM:SS"
    time_t toTimeT() const;                         // Convert to time_t
    int soSanhVoi(const NgayGio &other) const;      // -1: <, 0: ==, 1: >
    static NgayGio layThoiGianHienTai();            // Lấy thời gian hiện tại
    
    // Date/Time calculations
    long long soGiayChenhLech(const NgayGio &other) const; // Khoảng cách (giây)
    double soGioChenhLech(const NgayGio &other) const;     // Khoảng cách (giờ)

    // Operators
    bool operator==(const NgayGio &other) const;
    bool operator!=(const NgayGio &other) const;
    bool operator<(const NgayGio &other) const;
    bool operator>(const NgayGio &other) const;
    bool operator<=(const NgayGio &other) const;
    bool operator>=(const NgayGio &other) const;

    // Serialization
    void ghiFile(std::ofstream &out) const;
    void docFile(std::ifstream &in);
};

#endif // NGAYGIO_H
