#ifndef MANGDONG_H
#define MANGDONG_H

#include <iostream>
#include <string>
using namespace std;

// ============================
//  MẢNG ĐỘNG TỰ CÀI ĐẶT
// ============================

template <typename T>
class MangDong
{
private:
    T *data;       // con trỏ tới mảng dữ liệu
    int kichThuoc; // số phần tử hiện có
    int sucChua;   // dung lượng tối đa hiện tại

    void moRong()
    {
        int moi = (sucChua == 0) ? 2 : sucChua * 2;
        T *temp = new T[moi];
        for (int i = 0; i < kichThuoc; i++)
            temp[i] = data[i];
        delete[] data;
        data = temp;
        sucChua = moi;
    }

public:
    // 1) Constructor mặc định + đủ tham số
    MangDong(int sucChuaBanDau = 5)
        : data(new T[sucChuaBanDau > 0 ? sucChuaBanDau : 5]),
          kichThuoc(0),
          sucChua(sucChuaBanDau > 0 ? sucChuaBanDau : 5) {}

    // 2) Copy constructor (deep copy)
    MangDong(const MangDong &o)
        : data(new T[o.sucChua]),
          kichThuoc(o.kichThuoc),
          sucChua(o.sucChua)
    {
        for (int i = 0; i < kichThuoc; ++i)
            data[i] = o.data[i];
    }

    // 3) Copy assignment (deep copy + self-assign guard)
    MangDong &operator=(const MangDong &o)
    {
        if (this == &o)
            return *this;
        T *newData = new T[o.sucChua];
        for (int i = 0; i < o.kichThuoc; ++i)
            newData[i] = o.data[i];
        delete[] data;
        data = newData;
        kichThuoc = o.kichThuoc;
        sucChua = o.sucChua;
        return *this;
    }

    // 4) Destructor
    ~MangDong()
    {
        delete[] data;
    }

    // Thêm phần tử vào cuối mảng
    void them(const T &phanTu)
    {
        if (kichThuoc >= sucChua)
            moRong();
        data[kichThuoc++] = phanTu;
    }

    // Lấy kích thước hiện tại
    int getKichThuoc() const
    {
        return kichThuoc;
    }

    // Truy cập phần tử theo chỉ số
    T &operator[](int i)
    {
        return data[i];
    }

    const T &operator[](int i) const
    {
        return data[i];
    }

    // Xóa phần tử theo vị trí
    bool xoaTheoViTri(int index)
    {
        if (index < 0 || index >= kichThuoc)
            return false;
        for (int i = index; i < kichThuoc - 1; i++)
            data[i] = data[i + 1];
        kichThuoc--;
        return true;
    }
};

// ============================
//  HÀM TÌM KIẾM / KIỂM TRA TRÙNG
// ============================

// Tìm phần tử theo mã (dùng cho class có getMaKH() hoặc getMaSan())
template <typename T>
int timTheoMa(const MangDong<T> &ds, const string &ma)
{
    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        if ((ds[i].getMaKH() == ma) || (ds[i].getMaSan() == ma))
            return i;
    }
    return -1;
}

// Kiểm tra mã trùng
template <typename T>
bool maDaTonTai(const MangDong<T> &ds, const string &ma)
{
    return timTheoMa(ds, ma) != -1;
}

// ============================
//  HÀM SẮP XẾP (BUBBLE SORT) - CŨ
// ============================

template <typename T>
void sapXep(MangDong<T> &ds, bool (*soSanh)(const T &, const T &))
{
    int n = ds.getKichThuoc();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (soSanh(ds[j + 1], ds[j]))
            {
                T temp = ds[j];
                ds[j] = ds[j + 1];
                ds[j + 1] = temp;
            }
        }
    }
}

// ============================
//  QUICK SORT - MỚI (O(n log n))
// ============================

template <typename T>
int partition(MangDong<T> &ds, int low, int high, bool (*soSanh)(const T &, const T &))
{
    T pivot = ds[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (soSanh(ds[j], pivot))
        {
            i++;
            T temp = ds[i];
            ds[i] = ds[j];
            ds[j] = temp;
        }
    }

    T temp = ds[i + 1];
    ds[i + 1] = ds[high];
    ds[high] = temp;
    return i + 1;
}

template <typename T>
void quickSortHelper(MangDong<T> &ds, int low, int high, bool (*soSanh)(const T &, const T &))
{
    if (low < high)
    {
        int pi = partition(ds, low, high, soSanh);
        quickSortHelper(ds, low, pi - 1, soSanh);
        quickSortHelper(ds, pi + 1, high, soSanh);
    }
}

// Hàm gọi Quick Sort (Interface đơn giản)
template <typename T>
void quickSort(MangDong<T> &ds, bool (*soSanh)(const T &, const T &))
{
    if (ds.getKichThuoc() > 1)
    {
        quickSortHelper(ds, 0, ds.getKichThuoc() - 1, soSanh);
    }
}

// ============================
//  BINARY SEARCH - MỚI (O(log n))
//  Yêu cầu: Mảng đã sắp xếp
// ============================

// Binary Search với pointer to member function
template <typename T>
int binarySearch(const MangDong<T> &ds, const string &ma, string (T::*getMa)() const)
{
    int left = 0, right = ds.getKichThuoc() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        string midMa = (ds[mid].*getMa)();

        if (midMa == ma)
            return mid;
        else if (midMa < ma)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1; // Không tìm thấy
}

// Binary Search đơn giản hơn (dùng lambda hoặc function pointer)
template <typename T>
int binarySearchSimple(const MangDong<T> &ds, const string &ma, string (*getMa)(const T &))
{
    int left = 0, right = ds.getKichThuoc() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        string midMa = getMa(ds[mid]);

        if (midMa == ma)
            return mid;
        else if (midMa < ma)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

#endif // MANGDONG_H
