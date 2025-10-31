#include "KhachHangManager.h"
#include <fstream>

// Constructor
KhachHangManager::KhachHangManager()
{
    // Index sẽ được rebuild khi load data
}

// Tìm kiếm - O(1) với Hash Table
KhachHang *KhachHangManager::timKhachHang(const string &maKH)
{
    return indexKhachHang.search(maKH);
}

// Thêm khách hàng
bool KhachHangManager::themKhachHang(const string &maKH, const string &hoTen, const string &sdt)
{
    if (timKhachHang(maKH) != nullptr)
        return false;

    KhachHang kh(maKH, hoTen, sdt, 0, 1); // Điểm = 0, Cấp độ = 1
    dsKhachHang.them(kh);
    
    // Cập nhật index
    indexKhachHang.insert(dsKhachHang[dsKhachHang.getKichThuoc() - 1].getMaKH(),
                         &dsKhachHang[dsKhachHang.getKichThuoc() - 1]);
    return true;
}

// Sửa khách hàng
bool KhachHangManager::suaKhachHang(const string &maKH, const string &hoTen, const string &sdt)
{
    KhachHang *kh = timKhachHang(maKH);
    if (kh == nullptr)
        return false;

    kh->setHoTen(hoTen);
    kh->setSdt(sdt);
    return true;
}

// Xóa khách hàng
bool KhachHangManager::xoaKhachHang(const string &maKH)
{
    for (int i = 0; i < dsKhachHang.getKichThuoc(); i++)
    {
        if (dsKhachHang[i].getMaKH() == maKH)
        {
            indexKhachHang.remove(maKH);
            dsKhachHang.xoaTheoViTri(i);
            rebuildIndex(); // Rebuild vì địa chỉ thay đổi
            return true;
        }
    }
    return false;
}

// Hàm so sánh cho Quick Sort
static bool soSanhDiemTang(const KhachHang &a, const KhachHang &b)
{
    return a.getDiemTichLuy() < b.getDiemTichLuy();
}

static bool soSanhDiemGiam(const KhachHang &a, const KhachHang &b)
{
    return a.getDiemTichLuy() > b.getDiemTichLuy();
}

// Sắp xếp theo điểm tích lũy
void KhachHangManager::sapXepTheoDiem(bool tangDan)
{
    if (tangDan)
        quickSort(dsKhachHang, soSanhDiemTang);
    else
        quickSort(dsKhachHang, soSanhDiemGiam);
    rebuildIndex();
}

// Rebuild index
void KhachHangManager::rebuildIndex()
{
    indexKhachHang.clear();
    for (int i = 0; i < dsKhachHang.getKichThuoc(); i++)
        indexKhachHang.insert(dsKhachHang[i].getMaKH(), &dsKhachHang[i]);
}

// Lưu file
void KhachHangManager::luuFile(const string &filename)
{
    ofstream file(filename, ios::binary);
    if (file.is_open())
    {
        for (int i = 0; i < dsKhachHang.getKichThuoc(); i++)
            dsKhachHang[i].serialize(file);
        file.close();
    }
}

// Đọc file
void KhachHangManager::docFile(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (file.is_open())
    {
        while (file.peek() != EOF)
        {
            KhachHang kh;
            kh.deserialize(file);
            dsKhachHang.them(kh);
        }
        file.close();
        rebuildIndex();
    }
}
