# 💡 PHÂN TÍCH: CHỨC NĂNG ĐẶT DỊCH VỤ

**Ngày:** 30/10/2025  
**Vấn đề:** Quyết định thời điểm đặt dịch vụ - Trong lúc đặt sân hay lúc thanh toán?

---

## 🤔 HAI LỰA CHỌN

### OPTION 1: Đặt dịch vụ TRONG LÚC ĐẶT SÂN

**Flow:**

```
1. Chọn sân + khung giờ
2. Chọn khách hàng
3. ✨ Chọn dịch vụ (nếu muốn)
4. Xác nhận đặt sân + dịch vụ
5. Lưu lịch đặt
```

**Ưu điểm:**

- ✅ **UX tốt**: Một luồng duy nhất, không phải quay lại
- ✅ **Tiết kiệm thời gian**: Đặt tất cả một lúc
- ✅ **Thông tin đầy đủ ngay**: Biết tổng chi phí ngay từ đầu
- ✅ **Dễ quản lý**: Tất cả thông tin gắn với lịch đặt
- ✅ **Tránh quên**: Khách hàng không quên đặt dịch vụ

**Nhược điểm:**

- ⚠️ Dialog đặt sân phức tạp hơn
- ⚠️ Khách có thể chưa quyết định dịch vụ
- ⚠️ Thời gian đặt sân lâu hơn

**Phù hợp khi:**

- Khách hàng thường đặt dịch vụ cùng sân
- Muốn tối ưu UX
- Dịch vụ không thay đổi nhiều

---

### OPTION 2: Đặt dịch vụ LÚC THANH TOÁN

**Flow:**

```
1. Chọn sân + khung giờ
2. Chọn khách hàng
3. Xác nhận đặt sân
4. Lưu lịch đặt
...
5. (Sau khi đá xong) Vào thanh toán
6. ✨ Chọn thêm dịch vụ đã sử dụng
7. Tính tổng tiền + thanh toán
```

**Ưu điểm:**

- ✅ **Dialog đặt sân đơn giản**: Tập trung vào sân
- ✅ **Linh hoạt**: Khách có thể quyết định dịch vụ sau
- ✅ **Thực tế**: Một số dịch vụ chỉ biết sau khi đá (đồ uống, đồ ăn thêm)
- ✅ **Chính xác**: Tính theo thực tế sử dụng

**Nhược điểm:**

- ⚠️ Khách có thể quên đặt dịch vụ
- ⚠️ Không biết tổng chi phí trước
- ⚠️ Phải quay lại để thêm dịch vụ
- ⚠️ Có thể gây nhầm lẫn

**Phù hợp khi:**

- Dịch vụ thay đổi linh hoạt
- Dịch vụ phụ thuộc vào thực tế sử dụng
- Muốn giữ flow đặt sân đơn giản

---

## 🎯 KHUYẾN NGHỊ: OPTION 1 (Đặt lúc đặt sân)

### Lý do:

1. **UX tốt hơn nhiều**

   - Khách hàng không phải nhớ và quay lại
   - Một flow hoàn chỉnh từ đầu đến cuối
   - Giảm friction

2. **Thực tế trong ngành sân bóng**

   - Hầu hết sân bóng cho phép đặt dịch vụ khi booking
   - Dịch vụ phổ biến: Nước uống, giày, bóng, áo đấu
   - Khách thường biết trước cần gì

3. **Dễ quản lý hơn**

   - Tất cả thông tin tập trung tại lịch đặt
   - Dễ tracking và báo cáo
   - Giảm việc phải cập nhật sau

4. **Tối ưu doanh thu**
   - Khách dễ đặt dịch vụ hơn khi có sẵn trong flow
   - Upselling tự nhiên
   - Tăng giá trị đơn hàng

---

## 💻 THIẾT KẾ ĐỀ XUẤT

### Flow đặt sân + dịch vụ:

```
┌─────────────────────────────────────┐
│  1. CHỌN SÂN + KHUNG GIỜ           │
│  - Chọn ngày, loại sân, thời lượng │
│  - Click vào ô sân                  │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  2. XÁC NHẬN ĐẶT SÂN               │
│  - Click "Xác nhận đặt sân"         │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  3. CHỌN KHÁCH HÀNG                │
│  - Chọn có sẵn / Thêm mới          │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  4. CHỌN DỊCH VỤ (MỚI)            │ ✨
│  - Danh sách dịch vụ có sẵn        │
│  - Số lượng cho mỗi dịch vụ        │
│  - Có thể bỏ qua (không bắt buộc)  │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  5. XÁC NHẬN TỔNG HỢP              │
│  - Thông tin khách hàng            │
│  - Thông tin sân + giờ             │
│  - Danh sách dịch vụ đã chọn       │
│  - TỔNG CHI PHÍ                    │
└──────────────┬──────────────────────┘
               │
               ▼
           [LƯU VÀO DB]
```

---

## 🛠️ IMPLEMENTATION PLAN

### Phase 1: Chuẩn bị dữ liệu (Đã có)

- ✅ Class `DichVu` - Đã có
- ✅ Class `ChiTietDichVu` - Đã có
- ✅ Danh sách dịch vụ trong `QuanLyThueSan` - Đã có

### Phase 2: Tạo UI Dialog chọn dịch vụ

File mới: `ui/DichVuSelectionDialog.h/cpp`

**Tính năng:**

```cpp
class DichVuSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    DichVuSelectionDialog(QuanLyThueSan *quanLy, QWidget *parent);

    // Lấy danh sách dịch vụ đã chọn
    QList<ChiTietDichVuInfo> getSelectedDichVu() const;

private:
    // UI: Table/List với checkbox + spinbox số lượng
    QTableWidget *tableDichVu;

    // Data
    MangDong<DichVu> dsDichVu;
};
```

**UI Layout:**

```
╔════════════════════════════════════════╗
║  🎯 CHỌN DỊCH VỤ                      ║
╠════════════════════════════════════════╣
║                                        ║
║  [✓] Nước suối (5,000đ)   [Số lượng: 2]║
║  [ ] Nước ngọt (10,000đ)  [Số lượng: 0]║
║  [✓] Thuê giày (20,000đ)  [Số lượng: 1]║
║  [ ] Thuê áo (15,000đ)    [Số lượng: 0]║
║  [ ] Thuê bóng (30,000đ)  [Số lượng: 0]║
║                                        ║
║  ─────────────────────────────────────║
║  Tổng dịch vụ: 40,000 VNĐ            ║
║                                        ║
║       [Bỏ qua]         [✓ Xác nhận]  ║
╚════════════════════════════════════════╝
```

### Phase 3: Tích hợp vào flow đặt sân

**File cần sửa:** `ui/SanBookingDialog.cpp`

```cpp
void SanBookingDialog::onDatSan()
{
    // ... code hiện tại ...

    // ✨ THÊM: Dialog chọn dịch vụ
    DichVuSelectionDialog dvDialog(quanLy, this);
    if (dvDialog.exec() == QDialog::Accepted)
    {
        QList<ChiTietDichVuInfo> dsDichVuDaChon = dvDialog.getSelectedDichVu();
        // Lưu vào lịch đặt
    }

    // Tiếp tục xác nhận và lưu...
}
```

### Phase 4: Cập nhật database model

**File cần sửa:** `core/LichDatSan.h/cpp`

Có thể cần thêm:

```cpp
class LichDatSan
{
private:
    // ... existing fields ...
    MangDong<ChiTietDichVu> dsDichVu; // ✨ Danh sách dịch vụ đã đặt

public:
    void themDichVu(const ChiTietDichVu &dv);
    MangDong<ChiTietDichVu> getDsDichVu() const;
    double tinhTongTienDichVu() const;
};
```

---

## 📋 CHECKLIST IMPLEMENTATION

### Bước 1: Tạo Dialog chọn dịch vụ

- [ ] Tạo file `ui/DichVuSelectionDialog.h`
- [ ] Tạo file `ui/DichVuSelectionDialog.cpp`
- [ ] Thiết kế UI với QTableWidget
- [ ] Thêm checkbox + spinbox cho mỗi dịch vụ
- [ ] Tính tổng tiền tự động
- [ ] Validation số lượng

### Bước 2: Cập nhật LichDatSan (nếu cần)

- [ ] Xem xét cấu trúc hiện tại
- [ ] Thêm field lưu dịch vụ (nếu chưa có)
- [ ] Thêm method tính tổng tiền dịch vụ

### Bước 3: Tích hợp vào flow

- [ ] Gọi dialog sau khi chọn khách hàng
- [ ] Lưu dịch vụ vào lịch đặt
- [ ] Cập nhật xác nhận cuối cùng (hiển thị dịch vụ)
- [ ] Test flow hoàn chỉnh

### Bước 4: Testing

- [ ] Test đặt sân không có dịch vụ
- [ ] Test đặt sân + 1 dịch vụ
- [ ] Test đặt sân + nhiều dịch vụ
- [ ] Test tính tiền chính xác
- [ ] Test lưu/đọc từ database

---

## 🎨 MOCKUP DIALOG

### Dialog chọn dịch vụ (Chi tiết):

```
┌────────────────────────────────────────────────────┐
│  🎯 Chọn Dịch Vụ Cho Lịch Đặt Sân                  │
├────────────────────────────────────────────────────┤
│                                                     │
│  📋 Danh sách dịch vụ khả dụng:                   │
│                                                     │
│  ┌─────────────────────────────────────────────┐  │
│  │ ☑ │ Tên DV      │ Giá       │ SL   │ Tổng   │  │
│  ├───┼─────────────┼───────────┼──────┼────────┤  │
│  │ ✓ │ Nước suối   │ 5,000đ   │ [2↕] │ 10,000 │  │
│  │ □ │ Nước ngọt   │ 10,000đ  │ [0↕] │ 0      │  │
│  │ ✓ │ Thuê giày   │ 20,000đ  │ [1↕] │ 20,000 │  │
│  │ □ │ Thuê áo     │ 15,000đ  │ [0↕] │ 0      │  │
│  │ □ │ Thuê bóng   │ 30,000đ  │ [0↕] │ 0      │  │
│  └─────────────────────────────────────────────┘  │
│                                                     │
│  ─────────────────────────────────────────────────│
│                                                     │
│  💰 TỔNG TIỀN DỊCH VỤ: 30,000 VNĐ                │
│                                                     │
│  💡 Lưu ý: Bạn có thể bỏ qua nếu không cần dịch vụ│
│                                                     │
│           [🚫 Bỏ qua]         [✅ Xác nhận]       │
└────────────────────────────────────────────────────┘
```

---

## 💰 ĐỀ XUẤT GIÁ TRÊN

**Dự kiến chi phí development:**

- Dialog UI: ~2-3 giờ
- Tích hợp flow: ~1-2 giờ
- Database update (nếu cần): ~1 giờ
- Testing: ~1 giờ
- **Tổng: ~5-7 giờ làm việc**

---

## 🎯 KẾT LUẬN

### Khuyến nghị: ✅ **ĐẶT DỊCH VỤ KHI ĐẶT SÂN**

**Lý do chính:**

1. UX tốt hơn (một flow duy nhất)
2. Phù hợp với thực tế ngành sân bóng
3. Tăng doanh thu (khách dễ đặt dịch vụ hơn)
4. Dễ quản lý và tracking

**Next step:**

- Review và approve design
- Implement Phase 1: DichVuSelectionDialog
- Integrate vào flow đặt sân
- Test và deploy

---

**Prepared by:** GitHub Copilot + Development Team  
**Date:** 30/10/2025  
**Status:** Waiting for approval ✋
