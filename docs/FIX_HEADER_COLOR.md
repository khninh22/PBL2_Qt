# 🎨 FIX: Đổi Màu Header Của Bảng Đặt Sân

## 🐛 Vấn Đề

Header (cột giờ và cột sân) trong bảng đặt sân có **màu mặc định** (thường là màu xám nhạt hoặc trắng), dễ nhầm lẫn với các ô sân trống (màu xanh lá #4CAF50).

**Trước khi sửa:**

```
┌────────────┬──────┬──────┬──────┐
│ Sân / Giờ  │ 6:00 │ 7:00 │ 8:00 │  ← Màu nhạt, khó phân biệt
├────────────┼──────┼──────┼──────┤
│ Sân 1      │  🟢  │  🟢  │  🔴  │
│ (5 người)  │      │      │      │
├────────────┼──────┼──────┼──────┤
│ Sân 2      │  🟢  │  🔴  │  🔴  │
│ (7 người)  │      │      │      │
└────────────┴──────┴──────┴──────┘
```

## ✅ Giải Pháp

Đổi màu header thành **màu xám xanh đậm** để dễ phân biệt với các ô sân.

**File:** `ui/SanBookingDialog.cpp` (setupUI())

```cpp
// ===== BOOKING GRID =====
bookingGrid = new QTableWidget();
bookingGrid->setEditTriggers(QAbstractItemView::NoEditTriggers);
bookingGrid->setSelectionMode(QAbstractItemView::NoSelection);
bookingGrid->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
bookingGrid->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
bookingGrid->setMinimumHeight(500);

// ✅ THÊM MỚI: Đổi màu header
bookingGrid->setStyleSheet(
    "QHeaderView::section {"
    "    background-color: #34495e;"  // Màu xám xanh đậm (mặc định)
    "    color: white;"
    "    padding: 8px;"
    "    border: 1px solid #2c3e50;"
    "    font-weight: bold;"
    "    font-size: 13px;"
    "}"
    "QHeaderView::section:horizontal {"
    "    background-color: #2c3e50;"  // Cột giờ: xám đậm hơn
    "}"
    "QHeaderView::section:vertical {"
    "    background-color: #34495e;"  // Cột sân: xám nhạt hơn
    "}"
);

mainLayout->addWidget(bookingGrid);
```

## 🎨 Bảng Màu

| Phần tử                  | Màu          | Mã màu    | Mô tả                  |
| ------------------------ | ------------ | --------- | ---------------------- |
| **Cột giờ** (Horizontal) | Xám xanh đậm | `#2c3e50` | Đậm hơn để làm nổi bật |
| **Cột sân** (Vertical)   | Xám xanh     | `#34495e` | Nhạt hơn một chút      |
| **Text header**          | Trắng        | `white`   | Tương phản tốt         |
| **Border**               | Xám xanh đen | `#2c3e50` | Đồng bộ với cột giờ    |
|                          |              |           |                        |
| 🟢 **Sân trống**         | Xanh lá      | `#4CAF50` | Không đổi              |
| 🔴 **Sân đã đặt**        | Đỏ           | `#EF5350` | Không đổi              |
| 🟡 **Đang chọn**         | Vàng         | `#FFC107` | Không đổi              |

## 📊 So Sánh Trước/Sau

### **Trước:**

```
Header: 🤍 Màu nhạt (xám/trắng)
Sân trống: 🟢 Xanh lá
→ Khó phân biệt, dễ nhầm lẫn
```

### **Sau:**

```
Header: 🔵 Xám xanh đậm (#2c3e50 / #34495e)
Sân trống: 🟢 Xanh lá (#4CAF50)
→ Rõ ràng, dễ phân biệt
```

## 🎯 Kết Quả

**Sau khi sửa:**

```
┌────────────┬──────┬──────┬──────┐
│ 🔵 Sân/Giờ │ 6:00 │ 7:00 │ 8:00 │  ← Màu xám xanh đậm, dễ phân biệt
├────────────┼──────┼──────┼──────┤
│ 🔵 Sân 1   │  🟢  │  🟢  │  🔴  │
│   (5 người)│      │      │      │
├────────────┼──────┼──────┼──────┤
│ 🔵 Sân 2   │  🟢  │  🔴  │  🔴  │
│   (7 người)│      │      │      │
└────────────┴──────┴──────┴──────┘
```

**Cải thiện:**

- ✅ Header có màu **xám xanh đậm** rõ ràng
- ✅ Text header **màu trắng** tương phản cao
- ✅ **Không nhầm lẫn** với sân trống (xanh lá)
- ✅ **Phân biệt** giữa cột giờ (đậm hơn) và cột sân (nhạt hơn)
- ✅ Font chữ **in đậm** dễ đọc
- ✅ Viền **1px solid** phân cách rõ ràng

## 🚀 Cách Test

1. **Chạy ứng dụng:**

   ```bash
   cd d:\PBL2_GUI\build
   .\bin\QuanLyThueSanQt.exe
   ```

2. **Kiểm tra:**
   - Đăng nhập
   - Click "📅 Đặt Sân"
   - Quan sát header:
     - **Cột giờ** (6:00, 7:00, ...): Màu xám xanh đậm (#2c3e50)
     - **Cột sân** (Sân 1, Sân 2, ...): Màu xám xanh (#34495e)
     - **Các ô sân trống**: Màu xanh lá (#4CAF50) - dễ phân biệt với header

## 💡 Tùy Chỉnh Màu Khác (Tùy Chọn)

### **Nếu muốn màu xanh dương cho header:**

```cpp
"QHeaderView::section {"
"    background-color: #1976D2;"  // Xanh dương Material Design
"    color: white;"
"}"
```

### **Nếu muốn màu tím cho header:**

```cpp
"QHeaderView::section {"
"    background-color: #7B1FA2;"  // Tím Material Design
"    color: white;"
"}"
```

### **Nếu muốn màu đen cho header:**

```cpp
"QHeaderView::section {"
"    background-color: #212121;"  // Đen gần như tuyệt đối
"    color: white;"
"}"
```

## ✅ Tổng Kết

**Thay đổi:**

- ✅ Thêm stylesheet cho QTableWidget header
- ✅ Màu xám xanh đậm (#2c3e50) cho cột giờ
- ✅ Màu xám xanh (#34495e) cho cột sân
- ✅ Text trắng, font in đậm 13px
- ✅ Viền 1px solid

**Lợi ích:**

- ✅ Phân biệt rõ ràng header vs ô sân
- ✅ Không nhầm lẫn với sân trống (xanh lá)
- ✅ Giao diện chuyên nghiệp hơn
- ✅ Dễ đọc, dễ sử dụng

---

**Ngày sửa:** 28/10/2025  
**Tác giả:** GitHub Copilot  
**Status:** ✅ Đã sửa xong
