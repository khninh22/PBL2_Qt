#ifndef SANBONGMANAGER_H
#define SANBONGMANAGER_H

#include "../SanBong.h"
#include "../LichDatSan.h"
#include "../MangDong.h"
#include "../HashTable.h"
#include <string>
#include <ctime>
using namespace std;

// ============================
//  SAN BONG MANAGER
//  Quản lý toàn bộ nghiệp vụ liên quan đến sân bóng
// ============================

class SanBongManager
{
private:
    MangDong<SanBong> dsSanBong;
    HashTable<SanBong> indexSanBong;

public:
    // Constructor
    SanBongManager();

    // Getter cho danh sách
    const MangDong<SanBong> &getDsSanBong() const { return dsSanBong; }
    MangDong<SanBong> &getDsSanBongRef() { return dsSanBong; }

    // Tìm kiếm - O(1) với Hash Table
    SanBong *timSanBong(const string &maSan);

    // CRUD Operations
    bool themSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue);
    bool suaSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue);
    bool xoaSanBong(const string &maSan);

    // Bảo trì
    bool batBaoTriSan(const string &maSan);
    bool tatBaoTriSan(const string &maSan);

    // Tìm sân trống
    MangDong<SanBong> timSanTrong(int loaiSan, time_t batDau, time_t ketThuc, 
                                   const MangDong<LichDatSan> &dsLichDat);

    // Sắp xếp
    void sapXepTheoGia(bool tangDan = true);

    // Index Management
    void rebuildIndex();

    // File I/O
    void luuFile(const string &filename);
    void docFile(const string &filename);
};

#endif // SANBONGMANAGER_H
