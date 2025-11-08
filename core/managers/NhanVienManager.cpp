#include "NhanVienManager.h"
#include <fstream>

// Constructor
NhanVienManager::NhanVienManager()
{
    // Index sẽ được rebuild khi load data
}

// Tìm kiếm - O(1) với Hash Table
NhanVien *NhanVienManager::timNhanVien(const string &maNV)
{
    return indexNhanVien.search(maNV);
}

// Thêm nhân viên
bool NhanVienManager::themNhanVien(const string &maNV, const string &hoTen, const string &sdt,
                                    const string &viTri, double luong,
                                    const string &taiKhoan, const string &matKhau)
{
    if (timNhanVien(maNV) != nullptr)
        return false;

    // ✅ FIXED: Thêm diaChi="" parameter (từ ConNguoi inheritance)
    NhanVien nv(maNV, hoTen, sdt, "", viTri, luong, true, taiKhoan, matKhau); // dangLam = true, diaChi = ""
    dsNhanVien.them(nv);
    
    // Cập nhật index
    indexNhanVien.insert(dsNhanVien[dsNhanVien.getKichThuoc() - 1].getMaNV(),
                        &dsNhanVien[dsNhanVien.getKichThuoc() - 1]);
    return true;
}

// Sửa nhân viên
bool NhanVienManager::suaNhanVien(const string &maNV, const string &hoTen, const string &sdt,
                                   const string &viTri, double luong,
                                   const string &taiKhoan, const string &matKhau)
{
    NhanVien *nv = timNhanVien(maNV);
    if (nv == nullptr)
        return false;

    nv->setHoTen(hoTen);
    nv->setSdt(sdt);
    nv->setViTri(viTri);
    nv->setLuongCoBan(luong);
    
    if (!taiKhoan.empty())
        nv->setTaiKhoan(taiKhoan);
    if (!matKhau.empty())
        nv->setMatKhau(matKhau);
    
    return true;
}

// Xóa nhân viên
bool NhanVienManager::xoaNhanVien(const string &maNV)
{
    for (int i = 0; i < dsNhanVien.getKichThuoc(); i++)
    {
        if (dsNhanVien[i].getMaNV() == maNV)
        {
            indexNhanVien.remove(maNV);
            dsNhanVien.xoaTheoViTri(i);
            rebuildIndex(); // Rebuild vì địa chỉ thay đổi
            return true;
        }
    }
    return false;
}

// Rebuild index
void NhanVienManager::rebuildIndex()
{
    indexNhanVien.clear();
    for (int i = 0; i < dsNhanVien.getKichThuoc(); i++)
        indexNhanVien.insert(dsNhanVien[i].getMaNV(), &dsNhanVien[i]);
}

// Lưu file
void NhanVienManager::luuFile(const string &filename)
{
    ofstream file(filename, ios::binary);
    if (file.is_open())
    {
        for (int i = 0; i < dsNhanVien.getKichThuoc(); i++)
            dsNhanVien[i].serialize(file);
        file.close();
    }
}

// Đọc file
void NhanVienManager::docFile(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (file.is_open())
    {
        while (file.peek() != EOF)
        {
            NhanVien nv;
            nv.deserialize(file);
            dsNhanVien.them(nv);
        }
        file.close();
        rebuildIndex();
    }
}
