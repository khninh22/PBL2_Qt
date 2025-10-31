#include <QApplication>
#include <QDir>
#include "ui/MainWindow.h"
#include "ui/LoginDialog.h"
#include "core/QuanLyThueSan.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Đặt working directory về thư mục chứa executable
    // Đảm bảo đọc/ghi file data/ từ đúng vị trí (build/bin/)
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // Khởi tạo hệ thống quản lý
    QuanLyThueSan quanLy;

    while (true)
    {
        // Hiển thị màn hình đăng nhập
        LoginDialog loginDialog(&quanLy);

        if (loginDialog.exec() != QDialog::Accepted)
        {
            // Người dùng hủy đăng nhập hoặc đóng dialog
            return 0;
        }

        // Đăng nhập thành công, mở MainWindow
        MainWindow *window = new MainWindow(loginDialog.getAuthManager());

        // Connect logout signal to restart login loop
        bool shouldContinue = false;
        QObject::connect(window, &MainWindow::logoutRequested, [&shouldContinue]()
                         { shouldContinue = true; });

        window->showMaximized(); // ✅ Mở toàn màn hình
        app.exec();

        // Clean up
        delete window;

        // If logout was requested, continue loop to show login again
        if (shouldContinue)
        {
            continue;
        }
        else
        {
            // Window was closed normally, exit application
            break;
        }
    }

    return 0;
}