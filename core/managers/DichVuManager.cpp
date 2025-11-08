#include "DichVuManager.h"
#include "LichDatManager.h"
#include <fstream>

// Constructor
DichVuManager::DichVuManager()
{
    // Index sẽ được rebuild khi load data
}

// Tìm kiếm - O(1) với Hash Table
DichVu *DichVuManager::timDichVu(const string &maDV)
{
    return indexDichVu.search(maDV);
}

// Thêm dịch vụ
bool DichVuManager::themDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV)
{
    if (timDichVu(maDV) != nullptr)
        return false;

    DichVu dv(maDV, tenDV, loaiDV, giaDV, true); // conHang = true
    dsDichVu.them(dv);
    
    // Cập nhật index
    indexDichVu.insert(dsDichVu[dsDichVu.getKichThuoc() - 1].getMaDV(),
                      &dsDichVu[dsDichVu.getKichThuoc() - 1]);
    return true;
}

// Sửa dịch vụ
bool DichVuManager::suaDichVu(const string &maDV, const string &tenDV, const string &loaiDV, double giaDV)
{
    DichVu *dv = timDichVu(maDV);
    if (dv == nullptr)
        return false;

    dv->setTenDV(tenDV);
    dv->setLoaiDV(loaiDV);
    dv->setGiaDV(giaDV);
    return true;
}

// Xóa dịch vụ
bool DichVuManager::xoaDichVu(const string &maDV)
{
    for (int i = 0; i < dsDichVu.getKichThuoc(); i++)
    {
        if (dsDichVu[i].getMaDV() == maDV)
        {
            indexDichVu.remove(maDV);
            dsDichVu.xoaTheoViTri(i);
            rebuildIndex(); // Rebuild vì địa chỉ thay đổi
            return true;
        }
    }
    return false;
}

// Thêm dịch vụ vào lịch đặt
bool DichVuManager::themDichVuVaoLich(const string &maLichDat, const string &maDV, int soLuong,
                                      LichDatManager *lichMgr)
{
    // Kiểm tra lịch đặt (cần LichDatManager)
    if (lichMgr == nullptr)
        return false;
    
    LichDatSan *lich = lichMgr->timLichDat(maLichDat);
    if (lich == nullptr)
        return false;

    // Kiểm tra dịch vụ
    DichVu *dv = timDichVu(maDV);
    if (dv == nullptr || !dv->getConHang())
        return false;

    // ✅ NEW: Thêm chi tiết dịch vụ với OBJECT POINTER
    ChiTietDichVu ct(maLichDat, dv, soLuong, dv->getGiaDV());
    dsChiTietDichVu.them(ct);

    // Cập nhật tổng tiền lịch đặt
    double tongMoi = lich->getTongTien() + ct.getThanhTien();
    lich->setTongTien(tongMoi);

    return true;
}

// Rebuild index
void DichVuManager::rebuildIndex()
{
    indexDichVu.clear();
    for (int i = 0; i < dsDichVu.getKichThuoc(); i++)
        indexDichVu.insert(dsDichVu[i].getMaDV(), &dsDichVu[i]);
}

// Lưu file dịch vụ
void DichVuManager::luuFileDichVu(const string &filename)
{
    ofstream file(filename, ios::binary);
    if (file.is_open())
    {
        for (int i = 0; i < dsDichVu.getKichThuoc(); i++)
            dsDichVu[i].serialize(file);
        file.close();
    }
}

// Đọc file dịch vụ
void DichVuManager::docFileDichVu(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (file.is_open())
    {
        while (file.peek() != EOF)
        {
            DichVu dv;
            dv.deserialize(file);
            dsDichVu.them(dv);
        }
        file.close();
        rebuildIndex();
    }
}

// Lưu file chi tiết dịch vụ
void DichVuManager::luuFileChiTiet(const string &filename)
{
    ofstream file(filename, ios::binary);
    if (file.is_open())
    {
        for (int i = 0; i < dsChiTietDichVu.getKichThuoc(); i++)
            dsChiTietDichVu[i].serialize(file);
        file.close();
    }
}

// Đọc file chi tiết dịch vụ
void DichVuManager::docFileChiTiet(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (file.is_open())
    {
        while (file.peek() != EOF)
        {
            ChiTietDichVu ct;
            ct.deserialize(file);
            dsChiTietDichVu.them(ct);
        }
        file.close();
    }
}
