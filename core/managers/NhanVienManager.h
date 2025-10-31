#ifndef NHANVIENMANAGER_H
#define NHANVIENMANAGER_H

#include "../NhanVien.h"
#include "../MangDong.h"
#include "../HashTable.h"
#include <string>
using namespace std;

// ============================
//  NHAN VIEN MANAGER
//  Quản lý toàn bộ nghiệp vụ liên quan đến nhân viên
// ============================

class NhanVienManager
{
private:
    MangDong<NhanVien> dsNhanVien;
    HashTable<NhanVien> indexNhanVien;

public:
    // Constructor
    NhanVienManager();

    // Getter cho danh sách
    const MangDong<NhanVien> &getDsNhanVien() const { return dsNhanVien; }
    MangDong<NhanVien> &getDsNhanVienRef() { return dsNhanVien; }

    // Tìm kiếm - O(1) với Hash Table
    NhanVien *timNhanVien(const string &maNV);

    // CRUD Operations
    bool themNhanVien(const string &maNV, const string &hoTen, const string &sdt,
                      const string &viTri, double luong,
                      const string &taiKhoan = "", const string &matKhau = "");
    bool suaNhanVien(const string &maNV, const string &hoTen, const string &sdt,
                     const string &viTri, double luong,
                     const string &taiKhoan = "", const string &matKhau = "");
    bool xoaNhanVien(const string &maNV);

    // Index Management
    void rebuildIndex();

    // File I/O
    void luuFile(const string &filename);
    void docFile(const string &filename);
};

#endif // NHANVIENMANAGER_H
