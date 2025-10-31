# 🖼️ Resources - Tài Nguyên Hình Ảnh# 🖼️ Thư Mục Resources - Tài Nguyên Hình Ảnh# Resources - Tài Nguyên

## 📁 Files

| File | Mô Tả |## 📋 Mô Tả## 📸 Ảnh Nền Đăng Nhập

|------|-------|

| `login_bg.jpg` | Ảnh nền đăng nhập |Thư mục này chứa tất cả tài nguyên hình ảnh và file tĩnh của ứng dụng.

| `download_image.cpp` | **C++ downloader** - Tải ảnh chất lượng cao ✅ |

| `build_downloader.bat` | Build script |Đặt ảnh nền sân bóng vào đây với tên:

| ~~`download_background.py`~~ | ❌ Deprecated - Ảnh bị mờ |

## 📁 Cấu Trúc

## 🎨 Tải Ảnh Chất Lượng Cao

- `login_bg.jpg` (khuyến nghị)

### ⭐ Phương Pháp 1: C++ Downloader (Khuyến Nghị)

| File | Mô Tả | Sử Dụng |- hoặc `login_bg.png`

✅ **Ưu điểm:** Tải ảnh gốc full quality, không bị nén

|------|-------|---------|

```bash

# Build| `login_bg.jpg` | Ảnh nền màn hình đăng nhập | LoginDialog |### Yêu Cầu:

build_downloader.bat

| `login_stadium.jpg` | Ảnh sân bóng (backup) | LoginDialog |

# Tải ảnh

download_image.exe| `download_background.py` | Script tải ảnh nền | Tự động |- **Kích thước**: 1920x1080 hoặc tương đương

```

- **Định dạng**: JPG hoặc PNG

### 📥 Phương Pháp 2: Tải Thủ Công (Tốt Nhất)

## 📍 Vị Trí Quan Trọng- **Nội dung**: Ảnh sân bóng, cầu thủ đang chơi bóng

**Nguồn chất lượng cao:**

- Unsplash: https://unsplash.com/s/photos/soccer-stadium (Download "Original")

- Pexels: https://www.pexels.com/search/soccer-field/

⚠️ **Chỉ có 1 thư mục resources duy nhất:** `build/bin/resources/`### Tải Ảnh Mẫu:

**Yêu cầu:**

- Resolution: 1920x1080+ (2K/4K càng tốt)

- Format: JPG (90-100% quality) hoặc PNG

- Size: 500KB - 3MB❌ **KHÔNG còn resources ở:**Bạn có thể tải ảnh từ:

### ❌ Tại Sao Python Bị Mờ?- `d:\PBL2_GUI\resources\` (đã xóa để tránh nhầm lẫn)

Python requests/urllib tải ảnh đã được optimize/resize → mất chất lượng. - Bất kỳ vị trí nào khác- Unsplash: https://unsplash.com/s/photos/football-field

C++ WinINet hoặc tải thủ công → Giữ nguyên quality.

- Pexels: https://www.pexels.com/search/soccer-field/

## 🔍 Debug Ảnh Mờ

## 🔧 Cách Hoạt Động- Pixabay: https://pixabay.com/images/search/soccer-stadium/

````bash

# Kiểm tra size

dir login_bg.jpg

### Working Directory### Ảnh Đã Cung Cấp:

# Nếu < 300KB → Quality thấp, tải lại!

```Chương trình tự động set working directory về `build/bin/`:



---```cppẢnh trong prompt của bạn (cầu thủ đá bóng trong sân vận động) rất phù hợp!

**Location:** `build/bin/resources/`

QDir::setCurrent(QCoreApplication::applicationDirPath());Hãy lưu nó vào đây với tên `login_bg.jpg`.

````

---

### Load Hình Ảnh

```cpp**Nếu không có ảnh**: Hệ thống sẽ tự động hiển thị gradient màu xanh đẹp mắt.

// Đường dẫn tương đối từ build/bin/
QPixmap bg("resources/login_bg.jpg");
```

## 🎨 Thêm Hình Ảnh Mới

Để thêm hình ảnh mới:

1. Copy file vào `build/bin/resources/`
2. Sử dụng đường dẫn `"resources/filename.jpg"` trong code
3. Không cần rebuild project

## 📦 Build System

CMakeLists.txt tự động tạo thư mục này:

```cmake
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)
```

## 📸 Yêu Cầu Ảnh Nền

- **Kích thước**: 1920x1080 hoặc tương đương
- **Định dạng**: JPG hoặc PNG
- **Nội dung**: Ảnh sân bóng, cầu thủ, sân vận động

## 🔗 Nguồn Ảnh Mẫu

- Unsplash: https://unsplash.com/s/photos/football-field
- Pexels: https://www.pexels.com/search/soccer-field/
- Pixabay: https://pixabay.com/images/search/soccer-stadium/

---

**Đường dẫn đầy đủ:** `d:\PBL2_GUI\build\bin\resources\`
