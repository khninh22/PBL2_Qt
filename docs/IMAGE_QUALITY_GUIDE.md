# 🖼️ Hướng Dẫn Tải Ảnh Nền Chất Lượng Cao

## ❓ Vấn Đề

**Python downloader (`download_background.py`) tải ảnh bị MỜ so với ảnh gốc**

### Nguyên Nhân:

- Python `requests`/`urllib` thường tải ảnh đã được server optimize/resize
- Một số website tự động giảm quality cho fast loading
- Không kiểm soát được compression level

## ✅ Giải Pháp

### 1️⃣ C++ Downloader (Tốt Nhất - Tự Động)

**File:** `build/bin/resources/download_image.cpp`

**Ưu điểm:**

- ✅ Sử dụng Windows WinINet API
- ✅ Tải ảnh gốc 100% quality
- ✅ Không cần cài thêm gì (built-in Windows)
- ✅ Nhanh và ổn định

**Cách dùng:**

```bash
# 1. Mở PowerShell/CMD tại: d:\PBL2_GUI\build\bin\resources\

# 2. Build
.\build_downloader.bat

# 3. Chạy (interactive mode)
.\download_image.exe

# Hoặc direct download:
.\download_image.exe https://source.unsplash.com/1920x1080/?soccer-stadium login_bg.jpg
```

**Output:** File `login_bg.jpg` chất lượng cao, không bị mờ

### 2️⃣ Tải Thủ Công (Tốt Nhất - Manual)

**Nguồn ảnh chất lượng cao:**

1. **Unsplash** (Khuyến nghị):

   - Link: https://unsplash.com/s/photos/soccer-stadium
   - Search: "soccer stadium", "football match", "soccer field night"
   - **QUAN TRỌNG:** Click vào ảnh → Nhấn "Download" → Chọn **"Original"** (không chọn Small/Medium)

2. **Pexels**:

   - Link: https://www.pexels.com/search/soccer-field/
   - Download size lớn nhất có thể

3. **Pixabay**:
   - Link: https://pixabay.com/images/search/soccer-stadium/
   - Chọn resolution cao nhất

**Sau khi tải:**

1. Đổi tên thành `login_bg.jpg`
2. Copy vào `d:\PBL2_GUI\build\bin\resources\`
3. Restart ứng dụng

### 3️⃣ So Sánh Chất Lượng

| Phương Pháp   | Quality    | Speed    | Dễ Dùng  | Khuyến Nghị |
| ------------- | ---------- | -------- | -------- | ----------- |
| Python script | ⭐⭐ (Mờ)  | ⭐⭐⭐   | ⭐⭐⭐   | ❌          |
| C++ WinINet   | ⭐⭐⭐⭐   | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ✅          |
| Tải thủ công  | ⭐⭐⭐⭐⭐ | ⭐⭐     | ⭐⭐⭐   | ✅✅        |

## 📸 Yêu Cầu Ảnh

**Để có ảnh nền ĐẸP và NÉT:**

### Kỹ Thuật:

- **Resolution**: Tối thiểu 1920x1080, khuyến nghị 2K (2560x1440) hoặc 4K (3840x2160)
- **Format**: JPG với quality 90-100%, hoặc PNG
- **File size**: 500KB - 5MB (càng lớn càng nét, nhưng < 5MB)
- **Color depth**: 24-bit hoặc cao hơn

### Nội Dung:

- ✅ Sân bóng rộng, góc chụp đẹp
- ✅ Sân vận động có đèn, khán giả
- ✅ Cầu thủ đang chơi bóng (action shots)
- ✅ Ánh sáng đẹp (golden hour hoặc đèn sân vào ban đêm)
- ✅ Màu sắc sống động
- ❌ Tránh: Ảnh tối, mờ, pixelated, watermark lớn

## 🔍 Kiểm Tra Quality

### Trên Windows:

```bash
# Xem thông tin file
dir login_bg.jpg
```

**Đánh giá:**

- < 300KB → ⚠️ Quality thấp, nên tải lại
- 300KB - 800KB → ⭐⭐⭐ OK
- 800KB - 2MB → ⭐⭐⭐⭐ Tốt
- 2MB - 5MB → ⭐⭐⭐⭐⭐ Xuất sắc

### Xem Trực Tiếp:

1. Double-click `login_bg.jpg` để mở bằng Photos
2. Zoom 100% → Nếu vẫn nét → OK
3. Nếu thấy pixelated/blurry → Tải lại ảnh quality cao hơn

## 🛠️ Troubleshooting

### Vấn Đề: Ảnh Vẫn Bị Mờ Sau Khi Tải

**Kiểm tra:**

1. **File size quá nhỏ?**

   ```bash
   dir login_bg.jpg
   # Nếu < 300KB → Tải lại!
   ```

2. **Ảnh gốc bị mờ?**

   - Mở ảnh bằng Photos/Paint
   - Zoom 100% xem có nét không
   - Nếu ảnh gốc đã mờ → Tìm ảnh khác

3. **Qt scaling issue?**
   - Kiểm tra code trong `LoginDialog.cpp`
   - Đảm bảo không dùng `Qt::SmoothTransformation` với ảnh nhỏ

### Vấn Đề: C++ Downloader Không Build

**Error: "g++ is not recognized"**

```bash
# Cài MinGW hoặc dùng Visual Studio
# Hoặc download pre-built binary (nếu có)
```

**Giải pháp:** Dùng phương pháp tải thủ công (vẫn cho quality tốt nhất)

## 💡 Tips Chọn Ảnh Đẹp

1. **Tìm kiếm đúng keyword:**

   - ✅ "soccer stadium night lights"
   - ✅ "football match action"
   - ✅ "soccer field aerial view"
   - ❌ "soccer" (quá chung chung)

2. **Chọn góc chụp:**

   - ✅ Wide angle (góc rộng)
   - ✅ Aerial view (từ trên cao)
   - ✅ Action shots (cầu thủ đang chơi)

3. **Màu sắc:**

   - ✅ Sân cỏ xanh tươi
   - ✅ Đèn sân vàng ấm
   - ✅ Contrast tốt

4. **Composition:**
   - ✅ Rule of thirds
   - ✅ Leading lines
   - ✅ Depth (có foreground, middle, background)

## 📚 Recommended Images

**Ảnh gợi ý từ Unsplash (tải "Original"):**

1. Soccer stadium at night with lights
2. Football match in progress - wide shot
3. Aerial view of soccer field
4. Soccer players in action - golden hour
5. Modern stadium with green field

**Search terms hiệu quả:**

- `soccer stadium lights night`
- `football match action wide`
- `soccer field aerial drone`
- `stadium grass green beautiful`

---

**TL;DR:** Dùng C++ downloader hoặc tải thủ công từ Unsplash (chọn "Original"). Python bị mờ vì tải ảnh đã optimize.
