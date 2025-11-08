#ifndef NGUOIDUNG_H
#define NGUOIDUNG_H

#include "ConNguoi.h"

/**
 * @brief Lớp đại diện cho người dùng có tài khoản đăng nhập
 * @details Kế thừa từ ConNguoi, thêm tài khoản/mật khẩu
 *          Lớp cha cho NhanVien, QuanLy, Admin
 */
class NguoiDung : public ConNguoi
{
protected:
    std::string taiKhoan;
    std::string matKhau;
    bool hoatDong; // Tài khoản còn hoạt động không

public:
    // Constructors
    NguoiDung();
    NguoiDung(const std::string &ht, const std::string &sdt, const std::string &dc,
              const std::string &tk, const std::string &mk);
    NguoiDung(const NguoiDung &other);
    virtual ~NguoiDung();

    // Getters
    std::string getTaiKhoan() const { return taiKhoan; }
    std::string getMatKhau() const { return matKhau; }
    bool getHoatDong() const { return hoatDong; }

    // Setters
    void setTaiKhoan(const std::string &tk) { taiKhoan = tk; }
    void setMatKhau(const std::string &mk) { matKhau = mk; }
    void setHoatDong(bool hd) { hoatDong = hd; }

    // Methods
    bool dangNhap(const std::string &mk) const;
    bool doiMatKhau(const std::string &mkCu, const std::string &mkMoi);
    void khoaTaiKhoan() { hoatDong = false; }
    void moKhoaTaiKhoan() { hoatDong = true; }

    // Virtual methods override
    virtual void hienThi() const override;
    virtual void hienThiThongTin() const override; // ✅ NEW: Override for detailed info
    virtual std::string layLoai() const override { return "NguoiDung"; }

    // Serialization override
    virtual void serialize(std::ofstream &out) const override;
    virtual void deserialize(std::ifstream &in) override;
    virtual void ghiFile(std::ofstream &out) const override; // ✅ NEW
    virtual void docFile(std::ifstream &in) override;        // ✅ NEW

    // Operators
    NguoiDung &operator=(const NguoiDung &other);
};

#endif // NGUOIDUNG_H
