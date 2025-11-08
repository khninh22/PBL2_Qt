#ifndef THONGKE_H
#define THONGKE_H

#include "../NgayThang.h"
#include <string>

/**
 * @brief Base class cho các loại thống kê
 * @details PRACTICAL APPLICATION:
 *          - Separation of concerns (tách logic thống kê ra khỏi manager)
 *          - Extensible (dễ thêm loại thống kê mới)
 *          - Polymorphic statistics generation
 */
class ThongKe
{
protected:
    NgayThang tuNgay;
    NgayThang denNgay;
    std::string tieuDe;

public:
    // Constructor
    ThongKe();
    ThongKe(const NgayThang &tu, const NgayThang &den, const std::string &td);
    virtual ~ThongKe();

    // Getters
    NgayThang getTuNgay() const { return tuNgay; }
    NgayThang getDenNgay() const { return denNgay; }
    std::string getTieuDe() const { return tieuDe; }

    // Setters
    void setTuNgay(const NgayThang &t) { tuNgay = t; }
    void setDenNgay(const NgayThang &d) { denNgay = d; }
    void setTieuDe(const std::string &td) { tieuDe = td; }

    // Pure virtual - must be implemented by derived classes
    virtual void tinhToan() = 0;
    virtual void hienThi() const = 0;
    virtual std::string toText() const = 0;
};

#endif // THONGKE_H
