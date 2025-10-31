#ifndef DICHVUMANAGER_H
#define DICHVUMANAGER_H

#include "../DichVu.h"
#include "../ChiTietDichVu.h"
#include "../LichDatSan.h"
#include "../MangDong.h"
#include "../HashTable.h"
#include <string>
using namespace std;

// Forward declaration
class LichDatManager;

// ============================
//  DICH VU MANAGER
//  Quản lý toàn bộ nghiệp vụ liên quan đến dịch vụ
// ============================

class DichVuManager
{
private:
    MangDong<DichVu> dsDichVu;
    MangDong<ChiTietDichVu> dsChiTietDichVu;
    HashTable<DichVu> indexDichVu;

public:
    // Constructor
    DichVuManager();

    // Getter cho danh sách
    const MangDong<DichVu> &getDsDichVu() const { return dsDichVu; }
    MangDong<DichVu> &getDsDichVuRef() { return dsDichVu; }
    
    const MangDong<ChiTietDichVu> &getDsChiTietDichVu() const { return dsChiTietDichVu; }
    MangDong<ChiTietDichVu> &getDsChiTietDichVuRef() { return dsChiTietDichVu; }

    // Tìm kiếm - O(1) với Hash Table
    DichVu *timDichVu(const string &maDV);

    // CRUD Operations - Dịch vụ
    bool themDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV);
    bool suaDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV);
    bool xoaDichVu(const string &maDV);

    // Thêm dịch vụ vào lịch (cần access LichDatManager)
    bool themDichVuVaoLich(const string &maLichDat, const string &maDV, int soLuong,
                           LichDatManager *lichMgr);

    // Index Management
    void rebuildIndex();

    // File I/O
    void luuFileDichVu(const string &filename);
    void docFileDichVu(const string &filename);
    void luuFileChiTiet(const string &filename);
    void docFileChiTiet(const string &filename);
};

#endif // DICHVUMANAGER_H
