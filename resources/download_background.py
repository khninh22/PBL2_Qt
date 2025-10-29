#!/usr/bin/env python3
"""
Script để tải ảnh nền đăng nhập
Sử dụng ảnh sân bóng từ prompt
"""

import os
import sys

def main():
    print("=" * 60)
    print("HƯỚNG DẪN: Thêm Ảnh Nền Đăng Nhập")
    print("=" * 60)
    print()
    print("📸 Ảnh nền cần thiết cho màn hình đăng nhập!")
    print()
    print("Các bước thực hiện:")
    print()
    print("1️⃣  Lưu ảnh sân bóng (cầu thủ đá bóng) đã gửi")
    print("    Tên file: login_bg.jpg")
    print("    Thư mục: resources/")
    print()
    print("2️⃣  Hoặc copy từ bất kỳ ảnh sân bóng nào:")
    print("    - Kích thước đề xuất: 1920x1080")
    print("    - Định dạng: JPG hoặc PNG")
    print()
    print("3️⃣  Đặt vào: d:\\PBL2_GUI\\resources\\login_bg.jpg")
    print()
    print("⚠️  Nếu không có ảnh, hệ thống sẽ dùng gradient xanh")
    print()
    print("=" * 60)
    
    # Kiểm tra file có tồn tại chưa
    current_dir = os.path.dirname(os.path.abspath(__file__))
    image_path = os.path.join(current_dir, "login_bg.jpg")
    
    if os.path.exists(image_path):
        print(f"✅ Đã tìm thấy ảnh nền: {image_path}")
        size_kb = os.path.getsize(image_path) / 1024
        print(f"   Kích thước: {size_kb:.2f} KB")
    else:
        print(f"❌ Chưa có ảnh nền tại: {image_path}")
        print(f"   Vui lòng thêm ảnh vào thư mục này!")
    
    print("=" * 60)

if __name__ == "__main__":
    main()
