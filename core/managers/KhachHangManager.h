#ifndef KHACHHANGMANAGER_H
#define KHACHHANGMANAGER_H

#include "../KhachHang.h"
#include "../MangDong.h"
#include "../HashTable.h"
#include <string>
using namespace std;

// ============================
//  KHACH HANG MANAGER
//  Quản lý toàn bộ nghiệp vụ liên quan đến khách hàng
// ============================

class KhachHangManager
{
private:
    MangDong<KhachHang> dsKhachHang;
    HashTable<KhachHang> indexKhachHang;

public:
    // Constructor
    KhachHangManager();

    // Getter cho danh sách
    const MangDong<KhachHang> &getDsKhachHang() const { return dsKhachHang; }
    MangDong<KhachHang> &getDsKhachHangRef() { return dsKhachHang; }

    // Tìm kiếm - O(1) với Hash Table
    KhachHang *timKhachHang(const string &maKH);

    // CRUD Operations
    bool themKhachHang(const string &maKH, const string &hoTen, const string &sdt);
    bool suaKhachHang(const string &maKH, const string &hoTen, const string &sdt);
    bool xoaKhachHang(const string &maKH);

    // Sắp xếp
    void sapXepTheoDiem(bool tangDan = false);

    // Index Management
    void rebuildIndex();

    // File I/O
    void luuFile(const string &filename);
    void docFile(const string &filename);
};

#endif // KHACHHANGMANAGER_H
