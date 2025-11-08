#ifndef NGAYTHANG_H
#define NGAYTHANG_H

#include <string>
#include <fstream>
#include <ctime>

/**
 * @brief Lớp quản lý ngày tháng (DD/MM/YYYY)
 * @details PRACTICAL APPLICATION:
 *          - Replace time_t for better UI display
 *          - Easier date comparison and filtering
 *          - Vietnamese date format support
 */
class NgayThang
{
private:
    int ngay;
    int thang;
    int nam;

public:
    // Constructors
    NgayThang();
    NgayThang(int d, int m, int y);
    NgayThang(const NgayThang &other);
    NgayThang(time_t timestamp); // Convert from time_t

    // Getters
    int getNgay() const { return ngay; }
    int getThang() const { return thang; }
    int getNam() const { return nam; }

    // Setters với validation
    bool setNgay(int d);
    bool setThang(int m);
    bool setNam(int y);
    bool setNgayThang(int d, int m, int y);

    // Utility methods
    std::string toString() const;                        // "DD/MM/YYYY"
    std::string hienThiDayDu() const;                    // "Thứ X, ngày DD tháng MM năm YYYY"
    time_t toTimeT() const;                              // Convert to time_t
    int soSanhVoi(const NgayThang &other) const;         // -1: <, 0: ==, 1: >
    bool laNamNhuan() const;                             // Kiểm tra năm nhuận
    int soNgayTrongThang() const;                        // Số ngày trong tháng
    static NgayThang layNgayHienTai();                   // Lấy ngày hiện tại

    // Operators
    bool operator==(const NgayThang &other) const;
    bool operator!=(const NgayThang &other) const;
    bool operator<(const NgayThang &other) const;
    bool operator>(const NgayThang &other) const;
    bool operator<=(const NgayThang &other) const;
    bool operator>=(const NgayThang &other) const;

    // Serialization
    void ghiFile(std::ofstream &out) const;
    void docFile(std::ifstream &in);
};

#endif // NGAYTHANG_H
