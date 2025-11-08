#ifndef CONNGUOI_H
#define CONNGUOI_H

#include <string>
#include <fstream>

/**
 * @brief Lớp cơ sở đại diện cho con người
 * @details Base class cho KhachHang, NhanVien, etc.
 *          Áp dụng OOP: Encapsulation, Inheritance, Polymorphism
 */
class ConNguoi
{
protected:
    std::string hoTen;
    std::string sdt;
    std::string diaChi;

public:
    // Constructors
    ConNguoi();
    ConNguoi(const std::string &ht, const std::string &sdt, const std::string &dc = "");
    ConNguoi(const ConNguoi &other);
    virtual ~ConNguoi();

    // Getters
    std::string getHoTen() const { return hoTen; }
    std::string getSdt() const { return sdt; }
    std::string getDiaChi() const { return diaChi; }

    // Setters
    void setHoTen(const std::string &ht) { hoTen = ht; }
    void setSdt(const std::string &s) { sdt = s; }
    void setDiaChi(const std::string &dc) { diaChi = dc; }

    // Virtual methods - Polymorphism (ENHANCED for strong OOP)
    virtual void hienThi() const;
    virtual void hienThiThongTin() const; // ✅ NEW: Display detailed info
    virtual std::string layLoai() const { return "ConNguoi"; }

    // Serialization - Template pattern (ENHANCED)
    virtual void serialize(std::ofstream &out) const;
    virtual void deserialize(std::ifstream &in);
    virtual void ghiFile(std::ofstream &out) const; // ✅ NEW: Write to text file
    virtual void docFile(std::ifstream &in);        // ✅ NEW: Read from text file

    // Operators
    ConNguoi &operator=(const ConNguoi &other);
};

#endif // CONNGUOI_H
