#include "SanBongManager.h"
#include <fstream>

// Constructor
SanBongManager::SanBongManager()
{
    // Index sẽ được rebuild khi load data
}

// Tìm kiếm - O(1) với Hash Table
SanBong *SanBongManager::timSanBong(const string &maSan)
{
    return indexSanBong.search(maSan);
}

// Thêm sân bóng
bool SanBongManager::themSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue)
{
    if (timSanBong(maSan) != nullptr)
        return false;

    SanBong san(maSan, tenSan, loaiSan, giaThue, false);
    dsSanBong.them(san);
    
    // Cập nhật index
    indexSanBong.insert(dsSanBong[dsSanBong.getKichThuoc() - 1].getMaSan(),
                       &dsSanBong[dsSanBong.getKichThuoc() - 1]);
    return true;
}

// Sửa sân bóng
bool SanBongManager::suaSanBong(const string &maSan, const string &tenSan, int loaiSan, double giaThue)
{
    SanBong *san = timSanBong(maSan);
    if (san == nullptr)
        return false;

    san->setTenSan(tenSan);
    san->setLoaiSan(loaiSan);
    san->setGiaThue(giaThue);
    return true;
}

// Xóa sân bóng
bool SanBongManager::xoaSanBong(const string &maSan)
{
    for (int i = 0; i < dsSanBong.getKichThuoc(); i++)
    {
        if (dsSanBong[i].getMaSan() == maSan)
        {
            indexSanBong.remove(maSan);
            dsSanBong.xoaTheoViTri(i);
            rebuildIndex(); // Rebuild vì địa chỉ thay đổi
            return true;
        }
    }
    return false;
}

// Bật bảo trì
bool SanBongManager::batBaoTriSan(const string &maSan)
{
    SanBong *san = timSanBong(maSan);
    if (san == nullptr)
        return false;

    san->setBaoTri(true);
    return true;
}

// Tắt bảo trì
bool SanBongManager::tatBaoTriSan(const string &maSan)
{
    SanBong *san = timSanBong(maSan);
    if (san == nullptr)
        return false;

    san->setBaoTri(false);
    return true;
}

// Tìm sân trống
MangDong<SanBong> SanBongManager::timSanTrong(int loaiSan, time_t batDau, time_t ketThuc,
                                                const MangDong<LichDatSan> &dsLichDat)
{
    MangDong<SanBong> kq;

    for (int i = 0; i < dsSanBong.getKichThuoc(); i++)
    {
        const SanBong &san = dsSanBong[i];

        // Kiểm tra loại sân và bảo trì
        if (san.getLoaiSan() != loaiSan || san.getBaoTri())
            continue;

        // Kiểm tra trùng lịch
        bool trung = false;
        for (int j = 0; j < dsLichDat.getKichThuoc(); j++)
        {
            const LichDatSan &lich = dsLichDat[j];
            if (lich.getMaSan() == san.getMaSan() && lich.getTrangThaiTT() != "Đã Hủy")
            {
                time_t lichBatDau = lich.getThoiGianBatDauTimeT();    // ✅ Backward compatible
                time_t lichKetThuc = lich.getThoiGianKetThucTimeT();  // ✅ Backward compatible

                // Kiểm tra chồng lấn thời gian
                if (!(ketThuc <= lichBatDau || batDau >= lichKetThuc))
                {
                    trung = true;
                    break;
                }
            }
        }

        if (!trung)
            kq.them(san);
    }

    return kq;
}

// Hàm so sánh cho Quick Sort
static bool soSanhGiaTang(const SanBong &a, const SanBong &b)
{
    return a.getGiaThue() < b.getGiaThue();
}

static bool soSanhGiaGiam(const SanBong &a, const SanBong &b)
{
    return a.getGiaThue() > b.getGiaThue();
}

// Sắp xếp theo giá (Quick Sort)
void SanBongManager::sapXepTheoGia(bool tangDan)
{
    if (tangDan)
        quickSort(dsSanBong, soSanhGiaTang);
    else
        quickSort(dsSanBong, soSanhGiaGiam);
    rebuildIndex();
}

// Rebuild index
void SanBongManager::rebuildIndex()
{
    indexSanBong.clear();
    for (int i = 0; i < dsSanBong.getKichThuoc(); i++)
        indexSanBong.insert(dsSanBong[i].getMaSan(), &dsSanBong[i]);
}

// Lưu file
void SanBongManager::luuFile(const string &filename)
{
    ofstream file(filename, ios::binary);
    if (file.is_open())
    {
        for (int i = 0; i < dsSanBong.getKichThuoc(); i++)
            dsSanBong[i].serialize(file);
        file.close();
    }
}

// Đọc file
void SanBongManager::docFile(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (file.is_open())
    {
        while (file.peek() != EOF)
        {
            SanBong san;
            san.deserialize(file);
            dsSanBong.them(san);
        }
        file.close();
        rebuildIndex();
    }
}
