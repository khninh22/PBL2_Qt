#include "MainWindow.h"
#include "SanBongDialog.h"
#include "KhachHangDialog.h"
#include "SanBookingDialog.h"
#include "DichVuDialog.h"
#include "NhanVienDialog.h"
#include "PaymentDialog.h"
#include "ThongKeDialog.h"
#include "RestoreDialog.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QIcon>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QGroupBox>
#include <QGridLayout>
#include <QApplication>
#include <QMouseEvent>

// Helper class for clickable widgets
class ClickableWidget : public QObject
{
public:
    ClickableWidget(QObject *parent, std::function<void()> callback)
        : QObject(parent), m_callback(callback) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                m_callback();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    std::function<void()> m_callback;
};

MainWindow::MainWindow(AuthManager *auth, QWidget *parent)
    : QMainWindow(parent), authManager(auth)
{
    quanLy = new QuanLyThueSan();
    setupUI();
    applyAccessControl();

    // Navigate to first page
    navigateToPage(0);

    // Update all tables
    updateSanBongTable();
    updateKhachHangTable();
    updateLichDatTable();
    updateDichVuTable();
    updateNhanVienTable();
    updateThongKeDisplay();
}

MainWindow::~MainWindow()
{
    delete quanLy;
}

void MainWindow::setupUI()
{
    setWindowTitle("⚽ Hệ Thống Quản Lý Thuê Sân Bóng");
    setMinimumSize(1400, 800);

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Setup sidebar and content
    setupSidebar();
    setupContent();

    // Add to main layout
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget, 1);

    // Global stylesheet
    setStyleSheet(
        "QMainWindow {"
        "    background-color: #f5f5f5;"
        "}"
        "QPushButton {"
        "    background-color: #e3f2fd;"
        "    color: #1976d2;"
        "    border: 2px solid #2196F3;"  /* ✅ Viền rõ ngay từ đầu */
        "    padding: 12px 24px;"  /* ✅ Padding lớn hơn */
        "    border-radius: 8px;"  /* ✅ Bo góc rõ hơn */
        "    font-weight: bold;"
        "    font-size: 14px;"
        "    min-height: 40px;"  /* ✅ Nút cao hơn */
        "    min-width: 140px;"  /* ✅ Nút rộng hơn */
        "    outline: none;"  /* ✅ Không vòng tròn */
        "}"
        "QPushButton:hover {"
        "    background-color: #bbdefb;"
        "    border: 3px solid #1976D2;"  /* ✅ Viền dày hơn khi hover */
        "    transform: translateY(-2px);"  /* ✅ Nút nâng lên */
        "}"
        "QPushButton:pressed {"
        "    background-color: #90caf9;"
        "    border: 2px solid #0d47a1;"
        "    transform: translateY(0px);"  /* ✅ Nút ấn xuống */
        "}"
        "QPushButton:focus {"
        "    outline: none;"  /* ✅ KHÔNG vòng tròn khi focus */
        "}"
        "QPushButton[class='delete'] {"
        "    background-color: #ffebee;"
        "    color: #c62828;"
        "    border: 2px solid #ef5350;"  /* ✅ Viền đỏ rõ */
        "}"
        "QPushButton[class='delete']:hover {"
        "    background-color: #ffcdd2;"
        "    border: 3px solid #c62828;"  /* ✅ Viền dày hơn */
        "    transform: translateY(-2px);"
        "}"
        "QPushButton[class='delete']:pressed {"
        "    background-color: #ef9a9a;"
        "    border: 2px solid #b71c1c;"
        "    transform: translateY(0px);"
        "}"
        "QPushButton[class='delete']:focus {"
        "    outline: none;"  /* ✅ KHÔNG vòng tròn */
        "}"
        "QTableWidget {"
        "    border: 1px solid #ddd;"
        "    gridline-color: #ddd;"
        "    selection-background-color: #0078d7;"
        "    selection-color: white;"
        "    background-color: white;"
        "    outline: none;"  /* ✅ KHÔNG vòng tròn cho table */
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #0078d7;"
        "    color: white;"
        "}"
        "QTableWidget:focus {"
        "    outline: none;"  /* ✅ KHÔNG vòng tròn khi table focus */
        "}"
        "QHeaderView::section {"
        "    background-color: #f0f0f0;"
        "    padding: 5px;"
        "    border: 1px solid #ddd;"
        "    font-weight: bold;"
        "}");
}

void MainWindow::setupSidebar()
{
    sidebar = new QFrame();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(250); // Start visible with full width
    sidebar->setStyleSheet(
        "#sidebar {"
        "    background-color: #2c3e50;"
        "    border-right: 1px solid #1a252f;"
        "}");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // Header with logo only
    QWidget *headerWidget = new QWidget();
    headerWidget->setStyleSheet("background-color: #1a252f;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(15, 20, 15, 20);
    headerLayout->setSpacing(0);

    QLabel *lblLogo = new QLabel("⚽ QUẢN LÝ SÂN BÓNG");
    lblLogo->setStyleSheet(
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "background-color: transparent;");
    lblLogo->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(lblLogo);

    sidebarLayout->addWidget(headerWidget);

    // Menu items
    QWidget *menuWidget = new QWidget();
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setContentsMargins(10, 20, 10, 10);
    menuLayout->setSpacing(5);

    btnMenuSanBong = createMenuButton("⚽", "Sân Bóng", 0);
    btnMenuKhachHang = createMenuButton("👥", "Khách Hàng", 1);
    btnMenuLichDat = createMenuButton("📅", "Đặt Sân", 2);
    btnMenuDichVu = createMenuButton("🍔", "Dịch Vụ", 3);
    btnMenuNhanVien = createMenuButton("👔", "Nhân Viên", 4);
    btnMenuThongKe = createMenuButton("📊", "Thống Kê", 5);

    menuLayout->addWidget(btnMenuSanBong.button);
    menuLayout->addWidget(btnMenuKhachHang.button);
    menuLayout->addWidget(btnMenuLichDat.button);
    menuLayout->addWidget(btnMenuDichVu.button);
    menuLayout->addWidget(btnMenuNhanVien.button);
    menuLayout->addWidget(btnMenuThongKe.button);
    menuLayout->addStretch();

    // ✅ Backup button - using MenuButton struct
    btnMenuBackup = createSpecialButton("💾", "Sao Lưu & Khôi Phục", "#f39c12", [this]() {
        // Show menu on click
        QMenu *backupMenu = new QMenu(this);
        QAction *actBackup = new QAction("💾 Tạo Bản Sao Lưu", this);
        QAction *actRestore = new QAction("♻️ Khôi Phục Dữ Liệu", this);
        backupMenu->addAction(actBackup);
        backupMenu->addAction(actRestore);
        connect(actBackup, &QAction::triggered, this, &MainWindow::onBackupData);
        connect(actRestore, &QAction::triggered, this, &MainWindow::onRestoreData);
        backupMenu->exec(QCursor::pos());
    });
    menuLayout->addWidget(btnMenuBackup.button);

    // ✅ Logout button - using MenuButton struct
    btnMenuLogout = createSpecialButton("🚪", "Đăng Xuất", "#e74c3c", [this]() {
        onLogout();
    });
    menuLayout->addWidget(btnMenuLogout.button);

    sidebarLayout->addWidget(menuWidget, 1);
}

void MainWindow::setupContent()
{
    // Create stacked widget for pages directly (no wrapper)
    stackedWidget = new QStackedWidget();
    stackedWidget->setStyleSheet("background-color: #ecf0f1;");

    stackedWidget->addWidget(createSanBongPage());
    stackedWidget->addWidget(createKhachHangPage());
    stackedWidget->addWidget(createLichDatPage());
    stackedWidget->addWidget(createDichVuPage());
    stackedWidget->addWidget(createNhanVienPage());
    stackedWidget->addWidget(createThongKePage());
}

MainWindow::MenuButton MainWindow::createMenuButton(const QString &icon, const QString &text, int pageIndex)
{
    MenuButton menuBtn;
    
    // Single button with icon and text together
    menuBtn.button = new QPushButton(icon + "  " + text);
    menuBtn.button->setMinimumHeight(50);
    menuBtn.button->setCursor(Qt::PointingHandCursor);
    menuBtn.button->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #bdc3c7;"
        "    border: none;"
        "    border-radius: 8px;"
        "    text-align: left;"
        "    padding-left: 15px;"
        "    font-size: 14px;"
        "    font-weight: normal;"
        "}"
        "QPushButton:hover {"
        "    background-color: #34495e;"
        "}");
    
    connect(menuBtn.button, &QPushButton::clicked, [this, pageIndex]() {
        navigateToPage(pageIndex);
    });
    
    menuBtn.container = menuBtn.button; // Container is the button itself
    menuBtn.label = nullptr; // No separate label
    
    menuButtons.append(menuBtn);
    return menuBtn;
}

// Helper function for special buttons (Backup, Logout)
MainWindow::MenuButton MainWindow::createSpecialButton(const QString &icon, const QString &text, const QString &color, std::function<void()> callback)
{
    MenuButton menuBtn;
    
    // Single button with icon and text together
    menuBtn.button = new QPushButton(icon + "  " + text);
    menuBtn.button->setMinimumHeight(54);
    menuBtn.button->setCursor(Qt::PointingHandCursor);
    menuBtn.button->setStyleSheet(
        QString("QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    text-align: left;"
        "    padding-left: 15px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: %2;"
        "}").arg(color).arg(color == "#f39c12" ? "#e67e22" : "#c0392b"));
    
    connect(menuBtn.button, &QPushButton::clicked, callback);
    
    menuBtn.container = menuBtn.button; // Container is the button itself
    menuBtn.label = nullptr; // No separate label
    
    return menuBtn;
}

void MainWindow::navigateToPage(int index)
{
    stackedWidget->setCurrentIndex(index);

    // Highlight active menu item
    for (int i = 0; i < menuButtons.size(); i++)
    {
        if (i == index)
        {
            // Active item - green background with left border
            menuButtons[i].button->setStyleSheet(
                "QPushButton {"
                "    background-color: #1abc9c;"
                "    color: white;"
                "    border: none;"
                "    border-left: 4px solid #ffffff;"
                "    border-radius: 8px;"
                "    text-align: left;"
                "    padding-left: 15px;"
                "    font-size: 14px;"
                "    font-weight: bold;"
                "}"
                "QPushButton:hover {"
                "    background-color: #16a085;"
                "}");
        }
        else
        {
            // Inactive item - transparent with hover
            menuButtons[i].button->setStyleSheet(
                "QPushButton {"
                "    background-color: transparent;"
                "    color: #bdc3c7;"
                "    border: none;"
                "    border-radius: 8px;"
                "    text-align: left;"
                "    padding-left: 15px;"
                "    font-size: 14px;"
                "    font-weight: normal;"
                "}"
                "QPushButton:hover {"
                "    background-color: #34495e;"
                "}");
        }
    }
}

QWidget *MainWindow::createSanBongPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons - Chỉ giữ nút Thêm
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnThemSan = new QPushButton("➕ Thêm Sân");
    btnLayout->addWidget(btnThemSan);
    btnLayout->addStretch();

    // Table
    tableSanBong = new QTableWidget();
    tableSanBong->setColumnCount(5);
    tableSanBong->setHorizontalHeaderLabels({"Mã Sân", "Tên Sân", "Loại Sân", "Giá Thuê", "Trạng Thái"});
    
    // Thiết lập độ rộng cột
    tableSanBong->setColumnWidth(0, 80);   // Mã Sân
    tableSanBong->setColumnWidth(1, 200);  // Tên Sân
    tableSanBong->setColumnWidth(2, 150);  // Loại Sân
    tableSanBong->setColumnWidth(3, 120);  // Giá Thuê
    tableSanBong->setColumnWidth(4, 120);  // Trạng Thái
    
    tableSanBong->horizontalHeader()->setStretchLastSection(true);
    tableSanBong->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableSanBong->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableSanBong);

    // Connections
    connect(btnThemSan, &QPushButton::clicked, this, &MainWindow::onThemSanBong);
    // Double-click vào row để xem chi tiết
    connect(tableSanBong, &QTableWidget::cellDoubleClicked, this, &MainWindow::onSanBongRowDoubleClicked);

    return widget;
}

QWidget *MainWindow::createKhachHangPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons - Chỉ giữ nút Thêm
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnThemKH = new QPushButton("➕ Thêm Khách Hàng");
    btnLayout->addWidget(btnThemKH);
    btnLayout->addStretch();

    // Table
    tableKhachHang = new QTableWidget();
    tableKhachHang->setColumnCount(5);
    tableKhachHang->setHorizontalHeaderLabels({"Mã KH", "Họ Tên", "SĐT", "Điểm Tích Lũy", "Cấp Độ"});
    
    // Thiết lập độ rộng cột
    tableKhachHang->setColumnWidth(0, 80);   // Mã KH
    tableKhachHang->setColumnWidth(1, 200);  // Họ Tên
    tableKhachHang->setColumnWidth(2, 120);  // SĐT
    tableKhachHang->setColumnWidth(3, 120);  // Điểm
    tableKhachHang->setColumnWidth(4, 150);  // Cấp Độ
    
    tableKhachHang->horizontalHeader()->setStretchLastSection(true);
    tableKhachHang->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableKhachHang->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableKhachHang);

    // Connections
    connect(btnThemKH, &QPushButton::clicked, this, &MainWindow::onThemKhachHang);
    // Double-click vào row để xem chi tiết
    connect(tableKhachHang, &QTableWidget::cellDoubleClicked, this, &MainWindow::onKhachHangRowDoubleClicked);

    return widget;
}

QWidget *MainWindow::createLichDatPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnDatSan = new QPushButton("📅 Đặt Sân");
    btnHuyLich = new QPushButton("🚫 Hủy Lịch");
    btnThanhToan = new QPushButton("💰 Thanh Toán");

    btnLayout->addWidget(btnDatSan);
    btnLayout->addWidget(btnHuyLich);
    btnLayout->addWidget(btnThanhToan);
    btnLayout->addStretch();

    // Table
    tableLichDat = new QTableWidget();
    tableLichDat->setColumnCount(10);
    tableLichDat->setHorizontalHeaderLabels({"Mã Lịch", "Mã KH", "Tên KH", "SĐT", "Mã Sân", 
                                             "Bắt Đầu", "Kết Thúc", "Tổng Tiền", "TT Đặt", "TT Thanh Toán"});
    
    // Thiết lập độ rộng cột
    tableLichDat->setColumnWidth(0, 70);   // Mã Lịch
    tableLichDat->setColumnWidth(1, 60);   // Mã KH
    tableLichDat->setColumnWidth(2, 130);  // Tên KH
    tableLichDat->setColumnWidth(3, 100);  // SĐT
    tableLichDat->setColumnWidth(4, 70);   // Mã Sân
    tableLichDat->setColumnWidth(5, 120);  // Bắt Đầu
    tableLichDat->setColumnWidth(6, 120);  // Kết Thúc
    tableLichDat->setColumnWidth(7, 100);  // Tổng Tiền
    tableLichDat->setColumnWidth(8, 90);   // TT Đặt (Trạng thái đặt)
    tableLichDat->setColumnWidth(9, 120);  // TT Thanh Toán
    
    tableLichDat->horizontalHeader()->setStretchLastSection(true);
    tableLichDat->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLichDat->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableLichDat);

    // Connections
    connect(btnDatSan, &QPushButton::clicked, this, &MainWindow::onDatSanTrucQuan);
    connect(btnHuyLich, &QPushButton::clicked, this, &MainWindow::onHuyLich);
    connect(btnThanhToan, &QPushButton::clicked, this, &MainWindow::onThanhToan);

    return widget;
}

QWidget *MainWindow::createDichVuPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons - Chỉ giữ nút Thêm
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnThemDV = new QPushButton("➕ Thêm Dịch Vụ");
    btnLayout->addWidget(btnThemDV);
    btnLayout->addStretch();

    // Table
    tableDichVu = new QTableWidget();
    tableDichVu->setColumnCount(5);
    tableDichVu->setHorizontalHeaderLabels({"Mã DV", "Tên DV", "Loại DV", "Giá", "Trạng Thái"});
    
    // Thiết lập độ rộng cột
    tableDichVu->setColumnWidth(0, 80);   // Mã DV
    tableDichVu->setColumnWidth(1, 250);  // Tên DV
    tableDichVu->setColumnWidth(2, 150);  // Loại DV
    tableDichVu->setColumnWidth(3, 120);  // Giá
    tableDichVu->setColumnWidth(4, 120);  // Trạng Thái
    
    tableDichVu->horizontalHeader()->setStretchLastSection(true);
    tableDichVu->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableDichVu->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableDichVu);

    // Connections
    connect(btnThemDV, &QPushButton::clicked, this, &MainWindow::onThemDichVu);
    // Double-click vào row để xem chi tiết
    connect(tableDichVu, &QTableWidget::cellDoubleClicked, this, &MainWindow::onDichVuRowDoubleClicked);

    return widget;
}

QWidget *MainWindow::createNhanVienPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons - Chỉ giữ nút Thêm
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnThemNV = new QPushButton("➕ Thêm Nhân Viên");
    btnLayout->addWidget(btnThemNV);
    btnLayout->addStretch();

    // Table
    tableNhanVien = new QTableWidget();
    tableNhanVien->setColumnCount(6);
    tableNhanVien->setHorizontalHeaderLabels({"Mã NV", "Họ Tên", "SĐT", "Vị Trí", "Lương", "Trạng Thái"});
    
    // Thiết lập độ rộng cột
    tableNhanVien->setColumnWidth(0, 80);   // Mã NV
    tableNhanVien->setColumnWidth(1, 200);  // Họ Tên
    tableNhanVien->setColumnWidth(2, 120);  // SĐT
    tableNhanVien->setColumnWidth(3, 150);  // Vị Trí
    tableNhanVien->setColumnWidth(4, 120);  // Lương
    tableNhanVien->setColumnWidth(5, 120);  // Trạng Thái
    
    tableNhanVien->horizontalHeader()->setStretchLastSection(true);
    tableNhanVien->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableNhanVien->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableNhanVien);

    // Connections
    connect(btnThemNV, &QPushButton::clicked, this, &MainWindow::onThemNhanVien);
    // Double-click vào row để xem chi tiết
    connect(tableNhanVien, &QTableWidget::cellDoubleClicked, this, &MainWindow::onNhanVienRowDoubleClicked);

    return widget;
}

QWidget *MainWindow::createThongKePage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Group box thống kê
    QGroupBox *groupBox = new QGroupBox("📈 Thống Kê Hệ Thống");
    QGridLayout *grid = new QGridLayout(groupBox);

    // Labels
    lblDoanhThuNgay = new QLabel("0 VNĐ");
    lblDoanhThuThang = new QLabel("0 VNĐ");
    lblTongSan = new QLabel("0");
    lblTongKhach = new QLabel("0");

    lblDoanhThuNgay->setStyleSheet("font-size: 24px; font-weight: bold; color: #4CAF50;");
    lblDoanhThuThang->setStyleSheet("font-size: 24px; font-weight: bold; color: #2196F3;");
    lblTongSan->setStyleSheet("font-size: 24px; font-weight: bold; color: #FF9800;");
    lblTongKhach->setStyleSheet("font-size: 24px; font-weight: bold; color: #9C27B0;");

    grid->addWidget(new QLabel("💰 Doanh Thu Hôm Nay:"), 0, 0);
    grid->addWidget(lblDoanhThuNgay, 0, 1);

    grid->addWidget(new QLabel("💰 Doanh Thu Tháng Này:"), 1, 0);
    grid->addWidget(lblDoanhThuThang, 1, 1);

    grid->addWidget(new QLabel("⚽ Tổng Số Sân:"), 2, 0);
    grid->addWidget(lblTongSan, 2, 1);

    grid->addWidget(new QLabel("👥 Tổng Khách Hàng:"), 3, 0);
    grid->addWidget(lblTongKhach, 3, 1);

    layout->addWidget(groupBox);
    
    // ✅ MỚI: Button mở thống kê nâng cao
    QPushButton *btnThongKeNangCao = new QPushButton("📊 Xem Thống Kê Nâng Cao");
    btnThongKeNangCao->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4CAF50, stop:1 #2E7D32);"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   padding: 15px 30px;"
        "   border-radius: 8px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #66BB6A, stop:1 #388E3C);"
        "}"
        "QPushButton:pressed {"
        "   background: #1B5E20;"
        "}"
    );
    btnThongKeNangCao->setCursor(Qt::PointingHandCursor);
    
    connect(btnThongKeNangCao, &QPushButton::clicked, this, [this]() {
        ThongKeDialog *dialog = new ThongKeDialog(quanLy, this);
        dialog->exec();
        delete dialog;
    });
    
    layout->addWidget(btnThongKeNangCao, 0, Qt::AlignCenter);
    layout->addStretch();

    return widget;
}

void MainWindow::updateSanBongTable()
{
    tableSanBong->setRowCount(0);
    const MangDong<SanBong> &ds = quanLy->getDsSanBong();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const SanBong &san = ds[i];
        int row = tableSanBong->rowCount();
        tableSanBong->insertRow(row);

        tableSanBong->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(san.getMaSan().c_str())));
        tableSanBong->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(san.getTenSan().c_str())));
        tableSanBong->setItem(row, 2, new QTableWidgetItem(QString("Sân %1 người").arg(san.getLoaiSan())));
        tableSanBong->setItem(row, 3, new QTableWidgetItem(QString::number(san.getGiaThue(), 'f', 0) + " VNĐ"));
        tableSanBong->setItem(row, 4, new QTableWidgetItem(san.getBaoTri() ? "Đang Bảo Trì" : "Hoạt Động"));
    }
}

void MainWindow::updateKhachHangTable()
{
    tableKhachHang->setRowCount(0);
    const MangDong<KhachHang> &ds = quanLy->getDsKhachHang();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const KhachHang &kh = ds[i];
        int row = tableKhachHang->rowCount();
        tableKhachHang->insertRow(row);

        tableKhachHang->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(kh.getMaKH().c_str())));
        tableKhachHang->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(kh.getHoTen().c_str())));
        tableKhachHang->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(kh.getSdt().c_str())));
        tableKhachHang->setItem(row, 3, new QTableWidgetItem(QString::number(kh.getDiemTichLuy())));
        tableKhachHang->setItem(row, 4, new QTableWidgetItem(QString::fromUtf8(kh.getTenCapDo().c_str())));
    }
}

void MainWindow::updateLichDatTable()
{
    tableLichDat->setRowCount(0);
    const MangDong<LichDatSan> &ds = quanLy->getDsLichDatSan();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const LichDatSan &lich = ds[i];
        int row = tableLichDat->rowCount();
        tableLichDat->insertRow(row);

        // Lấy thông tin khách hàng
        KhachHang *kh = quanLy->timKhachHang(lich.getMaKH());
        QString tenKH = kh ? QString::fromUtf8(kh->getHoTen().c_str()) : "N/A";
        QString sdtKH = kh ? QString::fromUtf8(kh->getSdt().c_str()) : "N/A";

        tableLichDat->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(lich.getMaLichDat().c_str())));
        tableLichDat->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(lich.getMaKH().c_str())));
        tableLichDat->setItem(row, 2, new QTableWidgetItem(tenKH));
        tableLichDat->setItem(row, 3, new QTableWidgetItem(sdtKH));
        tableLichDat->setItem(row, 4, new QTableWidgetItem(QString::fromUtf8(lich.getMaSan().c_str())));

        // ✅ NEW: Use NgayGio::toString() instead of formatTime(time_t)
        QString batDau = QString::fromUtf8(lich.getThoiGianBatDau().toString().c_str());
        QString ketThuc = QString::fromUtf8(lich.getThoiGianKetThuc().toString().c_str());

        tableLichDat->setItem(row, 5, new QTableWidgetItem(batDau));
        tableLichDat->setItem(row, 6, new QTableWidgetItem(ketThuc));
        tableLichDat->setItem(row, 7, new QTableWidgetItem(QString("%L1 VNĐ").arg(lich.getTongTien(), 0, 'f', 0)));
        
        // Trạng thái đặt với màu
        QTableWidgetItem *itemTTDat = new QTableWidgetItem(QString::fromUtf8(lich.getTrangThaiDat().c_str()));
        if (lich.getTrangThaiDat() == "Đã Hủy")
            itemTTDat->setForeground(QBrush(QColor("#f44336")));
        else
            itemTTDat->setForeground(QBrush(QColor("#4CAF50")));
        tableLichDat->setItem(row, 8, itemTTDat);
        
        // Trạng thái thanh toán với màu
        QTableWidgetItem *itemTTTT = new QTableWidgetItem(QString::fromUtf8(lich.getTrangThaiTT().c_str()));
        if (lich.getTrangThaiTT() == "Đã Thanh Toán")
        {
            itemTTTT->setForeground(QBrush(QColor("#4CAF50")));
            itemTTTT->setBackground(QBrush(QColor("#E8F5E9")));
        }
        else
        {
            itemTTTT->setForeground(QBrush(QColor("#FF9800")));
            itemTTTT->setBackground(QBrush(QColor("#FFF3E0")));
        }
        tableLichDat->setItem(row, 9, itemTTTT);
    }
}

void MainWindow::updateDichVuTable()
{
    tableDichVu->setRowCount(0);
    const MangDong<DichVu> &ds = quanLy->getDsDichVu();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const DichVu &dv = ds[i];
        int row = tableDichVu->rowCount();
        tableDichVu->insertRow(row);

        tableDichVu->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(dv.getMaDV().c_str())));
        tableDichVu->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(dv.getTenDV().c_str())));
        tableDichVu->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(dv.getLoaiDV().c_str())));
        tableDichVu->setItem(row, 3, new QTableWidgetItem(QString::number(dv.getGiaDV(), 'f', 0) + " VNĐ"));
        tableDichVu->setItem(row, 4, new QTableWidgetItem(dv.getConHang() ? "Còn Hàng" : "Hết Hàng"));
    }
}

void MainWindow::updateNhanVienTable()
{
    tableNhanVien->setRowCount(0);
    const MangDong<NhanVien> &ds = quanLy->getDsNhanVien();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const NhanVien &nv = ds[i];
        int row = tableNhanVien->rowCount();
        tableNhanVien->insertRow(row);

        tableNhanVien->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(nv.getMaNV().c_str())));
        tableNhanVien->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(nv.getHoTen().c_str())));
        tableNhanVien->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(nv.getSdt().c_str())));
        tableNhanVien->setItem(row, 3, new QTableWidgetItem(QString::fromUtf8(nv.getViTri().c_str())));
        tableNhanVien->setItem(row, 4, new QTableWidgetItem(QString::number(nv.getLuongCoBan(), 'f', 0) + " VNĐ"));
        tableNhanVien->setItem(row, 5, new QTableWidgetItem(nv.getDangLam() ? "Đang Làm" : "Đã Nghỉ"));
    }
}

void MainWindow::updateThongKeDisplay()
{
    // Doanh thu hôm nay
    time_t now = time(nullptr);
    double doanhThuNgay = quanLy->tinhDoanhThuNgay(now);
    lblDoanhThuNgay->setText(QString::number(doanhThuNgay, 'f', 0) + " VNĐ");

    // Doanh thu tháng này
    tm *t = localtime(&now);
    double doanhThuThang = quanLy->tinhDoanhThuThang(t->tm_mon + 1, t->tm_year + 1900);
    lblDoanhThuThang->setText(QString::number(doanhThuThang, 'f', 0) + " VNĐ");

    // Tổng số sân
    lblTongSan->setText(QString::number(quanLy->getDsSanBong().getKichThuoc()));

    // Tổng khách hàng
    lblTongKhach->setText(QString::number(quanLy->getDsKhachHang().getKichThuoc()));
}

// Slot implementations (continue in next part due to length)
void MainWindow::onThemSanBong()
{
    SanBongDialog dialog(quanLy, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (quanLy->themSanBong(dialog.getMaSan(), dialog.getTenSan(),
                                dialog.getLoaiSan(), dialog.getGiaThue()))
        {
            updateSanBongTable();
            QMessageBox::information(this, "Thành công", "Đã thêm sân bóng!");
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Mã sân đã tồn tại!");
        }
    }
}

void MainWindow::onSuaSanBong()
{
    int row = tableSanBong->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn sân cần sửa!");
        return;
    }

    QString maSan = tableSanBong->item(row, 0)->text();
    SanBong *san = quanLy->timSanBong(maSan.toStdString());

    if (san)
    {
        SanBongDialog dialog(quanLy, this, san);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (quanLy->suaSanBong(maSan.toStdString(), dialog.getTenSan(),
                                   dialog.getLoaiSan(), dialog.getGiaThue()))
            {
                updateSanBongTable();
                QMessageBox::information(this, "Thành công", "Đã cập nhật sân bóng!");
            }
        }
    }
}

void MainWindow::onXoaSanBong()
{
    int row = tableSanBong->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn sân cần xóa!");
        return;
    }

    QString maSan = tableSanBong->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn xóa sân này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->xoaSanBong(maSan.toStdString()))
        {
            updateSanBongTable();
            QMessageBox::information(this, "Thành công", "Đã xóa sân bóng!");
        }
    }
}

void MainWindow::onBaoTriSan()
{
    int row = tableSanBong->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn sân!");
        return;
    }

    QString maSan = tableSanBong->item(row, 0)->text();
    SanBong *san = quanLy->timSanBong(maSan.toStdString());

    if (san)
    {
        if (san->getBaoTri())
        {
            quanLy->tatBaoTriSan(maSan.toStdString());
            QMessageBox::information(this, "Thành công", "Đã tắt chế độ bảo trì!");
        }
        else
        {
            quanLy->batBaoTriSan(maSan.toStdString());
            QMessageBox::information(this, "Thành công", "Đã bật chế độ bảo trì!");
        }
        updateSanBongTable();
    }
}

void MainWindow::onRefreshSanBong()
{
    updateSanBongTable();
}

// NEW: Xử lý double-click vào row sân bóng
void MainWindow::onSanBongRowDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    if (row < 0) return;

    QString maSan = tableSanBong->item(row, 0)->text();
    SanBong *san = quanLy->timSanBong(maSan.toStdString());

    if (!san) return;

    // Tạo dialog chi tiết với các nút chức năng
    QDialog *detailDialog = new QDialog(this);
    detailDialog->setWindowTitle("Chi Tiết Sân Bóng");
    detailDialog->setMinimumWidth(500);
    
    QVBoxLayout *layout = new QVBoxLayout(detailDialog);
    
    // Hiển thị thông tin chi tiết
    QGroupBox *infoGroup = new QGroupBox("📋 Thông Tin Sân");
    QGridLayout *grid = new QGridLayout(infoGroup);
    
    grid->addWidget(new QLabel("<b>Mã Sân:</b>"), 0, 0);
    grid->addWidget(new QLabel(QString::fromUtf8(san->getMaSan().c_str())), 0, 1);
    
    grid->addWidget(new QLabel("<b>Tên Sân:</b>"), 1, 0);
    grid->addWidget(new QLabel(QString::fromUtf8(san->getTenSan().c_str())), 1, 1);
    
    grid->addWidget(new QLabel("<b>Loại Sân:</b>"), 2, 0);
    grid->addWidget(new QLabel(QString("Sân %1 người").arg(san->getLoaiSan())), 2, 1);
    
    grid->addWidget(new QLabel("<b>Giá Thuê:</b>"), 3, 0);
    grid->addWidget(new QLabel(QString::number(san->getGiaThue(), 'f', 0) + " VNĐ"), 3, 1);
    
    grid->addWidget(new QLabel("<b>Trạng Thái:</b>"), 4, 0);
    QLabel *statusLabel = new QLabel(san->getBaoTri() ? "🔧 Đang Bảo Trì" : "✅ Hoạt Động");
    statusLabel->setStyleSheet(san->getBaoTri() ? "color: #f39c12;" : "color: #27ae60;");
    grid->addWidget(statusLabel, 4, 1);
    
    layout->addWidget(infoGroup);
    
    // Các nút chức năng
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnEdit = new QPushButton("✏️ Sửa");
    QPushButton *btnDelete = new QPushButton("❌ Xóa");
    QPushButton *btnMaintenance = new QPushButton(san->getBaoTri() ? "🔓 Tắt Bảo Trì" : "🔧 Bật Bảo Trì");
    QPushButton *btnClose = new QPushButton("Đóng");
    
    btnDelete->setProperty("class", "delete");
    
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addWidget(btnMaintenance);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);
    
    layout->addLayout(btnLayout);
    
    // Kết nối các nút
    connect(btnEdit, &QPushButton::clicked, [this, maSan, detailDialog]() {
        detailDialog->accept();
        SanBong *s = quanLy->timSanBong(maSan.toStdString());
        if (s) {
            SanBongDialog dialog(quanLy, this, s);
            if (dialog.exec() == QDialog::Accepted) {
                if (quanLy->suaSanBong(maSan.toStdString(), dialog.getTenSan(),
                                       dialog.getLoaiSan(), dialog.getGiaThue())) {
                    updateSanBongTable(); // Reload dữ liệu ngay
                    QMessageBox::information(this, "Thành công", "Đã cập nhật sân bóng!");
                }
            }
        }
    });
    
    connect(btnDelete, &QPushButton::clicked, [this, maSan, detailDialog]() {
        auto reply = QMessageBox::question(detailDialog, "Xác nhận",
                                           "Bạn có chắc muốn xóa sân này?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (quanLy->xoaSanBong(maSan.toStdString())) {
                updateSanBongTable(); // Reload dữ liệu ngay
                QMessageBox::information(detailDialog, "Thành công", "Đã xóa sân bóng!");
                detailDialog->accept();
            }
        }
    });
    
    connect(btnMaintenance, &QPushButton::clicked, [this, maSan, detailDialog]() {
        SanBong *s = quanLy->timSanBong(maSan.toStdString());
        if (s) {
            if (s->getBaoTri()) {
                quanLy->tatBaoTriSan(maSan.toStdString());
                QMessageBox::information(detailDialog, "Thành công", "Đã tắt chế độ bảo trì!");
            } else {
                quanLy->batBaoTriSan(maSan.toStdString());
                QMessageBox::information(detailDialog, "Thành công", "Đã bật chế độ bảo trì!");
            }
            updateSanBongTable(); // Reload dữ liệu ngay
            detailDialog->accept();
        }
    });
    
    connect(btnClose, &QPushButton::clicked, detailDialog, &QDialog::accept);
    
    detailDialog->exec();
    delete detailDialog;
}

// Similar implementations for other slot methods...
void MainWindow::onThemKhachHang()
{
    KhachHangDialog dialog(quanLy, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (quanLy->themKhachHang(dialog.getMaKH(), dialog.getHoTen(), dialog.getSdt()))
        {
            updateKhachHangTable();
            QMessageBox::information(this, "Thành công", "Đã thêm khách hàng!");
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Mã khách hàng đã tồn tại!");
        }
    }
}

void MainWindow::onSuaKhachHang()
{
    int row = tableKhachHang->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn khách hàng cần sửa!");
        return;
    }

    QString maKH = tableKhachHang->item(row, 0)->text();
    KhachHang *kh = quanLy->timKhachHang(maKH.toStdString());

    if (kh)
    {
        KhachHangDialog dialog(quanLy, this, kh);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (quanLy->suaKhachHang(maKH.toStdString(), dialog.getHoTen(), dialog.getSdt()))
            {
                updateKhachHangTable();
                QMessageBox::information(this, "Thành công", "Đã cập nhật khách hàng!");
            }
        }
    }
}

void MainWindow::onXoaKhachHang()
{
    int row = tableKhachHang->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn khách hàng cần xóa!");
        return;
    }

    QString maKH = tableKhachHang->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn xóa khách hàng này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->xoaKhachHang(maKH.toStdString()))
        {
            updateKhachHangTable();
            QMessageBox::information(this, "Thành công", "Đã xóa khách hàng!");
        }
    }
}

void MainWindow::onRefreshKhachHang()
{
    updateKhachHangTable();
}

void MainWindow::onKhachHangRowDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    
    if (row < 0 || row >= tableKhachHang->rowCount()) {
        return;
    }
    
    // Lấy thông tin khách hàng từ row được chọn
    QString maKH = tableKhachHang->item(row, 0)->text();
    QString hoTen = tableKhachHang->item(row, 1)->text();
    QString sdt = tableKhachHang->item(row, 2)->text();
    QString diemTichLuy = tableKhachHang->item(row, 3)->text();
    QString capDo = tableKhachHang->item(row, 4)->text();
    
    // Tạo dialog chi tiết
    QDialog *detailDialog = new QDialog(this);
    detailDialog->setWindowTitle("Chi Tiết Khách Hàng");
    detailDialog->setMinimumWidth(450);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(detailDialog);
    
    // Group box hiển thị thông tin
    QGroupBox *infoGroup = new QGroupBox("Thông Tin Khách Hàng", detailDialog);
    QGridLayout *infoLayout = new QGridLayout(infoGroup);
    
    infoLayout->addWidget(new QLabel("<b>Mã Khách Hàng:</b>"), 0, 0);
    infoLayout->addWidget(new QLabel(maKH), 0, 1);
    
    infoLayout->addWidget(new QLabel("<b>Họ Tên:</b>"), 1, 0);
    infoLayout->addWidget(new QLabel(hoTen), 1, 1);
    
    infoLayout->addWidget(new QLabel("<b>Số Điện Thoại:</b>"), 2, 0);
    infoLayout->addWidget(new QLabel(sdt), 2, 1);
    
    infoLayout->addWidget(new QLabel("<b>Điểm Tích Lũy:</b>"), 3, 0);
    infoLayout->addWidget(new QLabel(diemTichLuy), 3, 1);
    
    infoLayout->addWidget(new QLabel("<b>Cấp Độ:</b>"), 4, 0);
    infoLayout->addWidget(new QLabel(capDo), 4, 1);
    
    mainLayout->addWidget(infoGroup);
    
    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *btnEdit = new QPushButton("✏️  Sửa", detailDialog);
    btnEdit->setStyleSheet("QPushButton { padding: 8px 15px; background: #4CAF50; color: white; border-radius: 4px; }"
                           "QPushButton:hover { background: #45a049; }");
    
    QPushButton *btnDelete = new QPushButton("🗑️  Xóa", detailDialog);
    btnDelete->setStyleSheet("QPushButton { padding: 8px 15px; background: #f44336; color: white; border-radius: 4px; }"
                             "QPushButton:hover { background: #da190b; }");
    
    QPushButton *btnClose = new QPushButton("❌  Đóng", detailDialog);
    btnClose->setStyleSheet("QPushButton { padding: 8px 15px; background: #808080; color: white; border-radius: 4px; }"
                            "QPushButton:hover { background: #606060; }");
    
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);
    
    mainLayout->addLayout(btnLayout);
    
    // Kết nối sự kiện Edit
    connect(btnEdit, &QPushButton::clicked, detailDialog, [this, maKH, detailDialog]() {
        KhachHang *kh = quanLy->timKhachHang(maKH.toStdString());
        if (!kh) {
            QMessageBox::warning(this, "Lỗi", "Không tìm thấy khách hàng!");
            return;
        }
        KhachHangDialog editDialog(quanLy, this, kh);
        if (editDialog.exec() == QDialog::Accepted) {
            updateKhachHangTable();  // ✅ Load lại dữ liệu ngay
            detailDialog->close();
            QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin khách hàng!");
        }
    });
    
    // Kết nối sự kiện Delete
    connect(btnDelete, &QPushButton::clicked, detailDialog, [this, maKH, hoTen, detailDialog]() {
        auto reply = QMessageBox::question(this, "Xác nhận", 
                                          QString("Bạn có chắc muốn xóa khách hàng '%1'?").arg(hoTen),
                                          QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            try {
                quanLy->xoaKhachHang(maKH.toStdString());
                updateKhachHangTable();  // ✅ Load lại dữ liệu ngay
                detailDialog->close();
                QMessageBox::information(this, "Thành công", "Đã xóa khách hàng thành công!");
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Lỗi", QString("Không thể xóa khách hàng: %1").arg(e.what()));
            }
        }
    });
    
    // Kết nối sự kiện Close
    connect(btnClose, &QPushButton::clicked, detailDialog, &QDialog::close);
    
    detailDialog->exec();
    delete detailDialog;
}

void MainWindow::onDatSanTrucQuan()
{
    SanBookingDialog dialog(quanLy, this);

    // ✅ MỚI: Connect signal để refresh khách hàng khi có khách hàng mới
    connect(&dialog, &SanBookingDialog::khachHangAdded,
            this, &MainWindow::updateKhachHangTable);
    
    // ✅ MỚI: Connect signal để refresh lịch đặt ngay khi đặt xong
    connect(&dialog, &SanBookingDialog::lichDatAdded,
            this, &MainWindow::updateLichDatTable);
    
    // ✅ MỚI: Refresh thống kê khi đặt xong
    connect(&dialog, &SanBookingDialog::lichDatAdded,
            this, &MainWindow::updateThongKeDisplay);

    if (dialog.exec() == QDialog::Accepted)
    {
        updateLichDatTable();
        updateThongKeDisplay();
        QMessageBox::information(this, "Thành công", "Đã đặt sân thành công!");
    }
}

void MainWindow::onHuyLich()
{
    int row = tableLichDat->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn lịch cần hủy!");
        return;
    }

    QString maLich = tableLichDat->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn hủy lịch đặt này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->huyLichDat(maLich.toStdString()))
        {
            updateLichDatTable();
            QMessageBox::information(this, "Thành công", "Đã hủy lịch đặt!");
        }
    }
}

void MainWindow::onThanhToan()
{
    // ✅ Mở PaymentDialog mới với hiển thị hóa đơn đầy đủ và xuất PDF
    PaymentDialog dialog(quanLy, this);
    
    // ✅ Connect signal để refresh ngay khi thanh toán thành công
    connect(&dialog, &PaymentDialog::paymentCompleted, this, [this]() {
        updateLichDatTable();
        updateKhachHangTable();
        updateThongKeDisplay();
    });
    
    dialog.exec();
}

void MainWindow::onRefreshLichDat()
{
    updateLichDatTable();
}

void MainWindow::onThemDichVu()
{
    DichVuDialog dialog(quanLy, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (quanLy->themDichVu(dialog.getMaDV(), dialog.getTenDV(),
                               dialog.getLoaiDV(), dialog.getGiaDV()))
        {
            updateDichVuTable();
            QMessageBox::information(this, "Thành công", "Đã thêm dịch vụ!");
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Mã dịch vụ đã tồn tại!");
        }
    }
}

void MainWindow::onSuaDichVu()
{
    int row = tableDichVu->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn dịch vụ cần sửa!");
        return;
    }

    QString maDV = tableDichVu->item(row, 0)->text();
    DichVu *dv = quanLy->timDichVu(maDV.toStdString());

    if (dv)
    {
        DichVuDialog dialog(quanLy, this, dv);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (quanLy->suaDichVu(maDV.toStdString(), dialog.getTenDV(),
                                  dialog.getLoaiDV(), dialog.getGiaDV()))
            {
                updateDichVuTable();
                QMessageBox::information(this, "Thành công", "Đã cập nhật dịch vụ!");
            }
        }
    }
}

void MainWindow::onXoaDichVu()
{
    int row = tableDichVu->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn dịch vụ cần xóa!");
        return;
    }

    QString maDV = tableDichVu->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn xóa dịch vụ này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->xoaDichVu(maDV.toStdString()))
        {
            updateDichVuTable();
            QMessageBox::information(this, "Thành công", "Đã xóa dịch vụ!");
        }
    }
}

void MainWindow::onRefreshDichVu()
{
    updateDichVuTable();
}

void MainWindow::onDichVuRowDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    
    if (row < 0 || row >= tableDichVu->rowCount()) {
        return;
    }
    
    // Lấy thông tin dịch vụ từ row được chọn
    QString maDV = tableDichVu->item(row, 0)->text();
    QString tenDV = tableDichVu->item(row, 1)->text();
    QString loaiDV = tableDichVu->item(row, 2)->text();
    QString giaDV = tableDichVu->item(row, 3)->text();
    QString trangThai = tableDichVu->item(row, 4)->text();
    
    // Tạo dialog chi tiết
    QDialog *detailDialog = new QDialog(this);
    detailDialog->setWindowTitle("Chi Tiết Dịch Vụ");
    detailDialog->setMinimumWidth(450);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(detailDialog);
    
    // Group box hiển thị thông tin
    QGroupBox *infoGroup = new QGroupBox("Thông Tin Dịch Vụ", detailDialog);
    QGridLayout *infoLayout = new QGridLayout(infoGroup);
    
    infoLayout->addWidget(new QLabel("<b>Mã Dịch Vụ:</b>"), 0, 0);
    infoLayout->addWidget(new QLabel(maDV), 0, 1);
    
    infoLayout->addWidget(new QLabel("<b>Tên Dịch Vụ:</b>"), 1, 0);
    infoLayout->addWidget(new QLabel(tenDV), 1, 1);
    
    infoLayout->addWidget(new QLabel("<b>Loại Dịch Vụ:</b>"), 2, 0);
    infoLayout->addWidget(new QLabel(loaiDV), 2, 1);
    
    infoLayout->addWidget(new QLabel("<b>Giá:</b>"), 3, 0);
    infoLayout->addWidget(new QLabel(giaDV), 3, 1);
    
    infoLayout->addWidget(new QLabel("<b>Trạng Thái:</b>"), 4, 0);
    infoLayout->addWidget(new QLabel(trangThai), 4, 1);
    
    mainLayout->addWidget(infoGroup);
    
    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *btnEdit = new QPushButton("✏️  Sửa", detailDialog);
    btnEdit->setStyleSheet("QPushButton { padding: 8px 15px; background: #4CAF50; color: white; border-radius: 4px; }"
                           "QPushButton:hover { background: #45a049; }");
    
    QPushButton *btnDelete = new QPushButton("🗑️  Xóa", detailDialog);
    btnDelete->setStyleSheet("QPushButton { padding: 8px 15px; background: #f44336; color: white; border-radius: 4px; }"
                             "QPushButton:hover { background: #da190b; }");
    
    QPushButton *btnClose = new QPushButton("❌  Đóng", detailDialog);
    btnClose->setStyleSheet("QPushButton { padding: 8px 15px; background: #808080; color: white; border-radius: 4px; }"
                            "QPushButton:hover { background: #606060; }");
    
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);
    
    mainLayout->addLayout(btnLayout);
    
    // Kết nối sự kiện Edit
    connect(btnEdit, &QPushButton::clicked, detailDialog, [this, maDV, detailDialog]() {
        DichVu *dv = quanLy->timDichVu(maDV.toStdString());
        if (!dv) {
            QMessageBox::warning(this, "Lỗi", "Không tìm thấy dịch vụ!");
            return;
        }
        DichVuDialog editDialog(quanLy, this, dv);
        if (editDialog.exec() == QDialog::Accepted) {
            updateDichVuTable();  // ✅ Load lại dữ liệu ngay
            detailDialog->close();
            QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin dịch vụ!");
        }
    });
    
    // Kết nối sự kiện Delete
    connect(btnDelete, &QPushButton::clicked, detailDialog, [this, maDV, tenDV, detailDialog]() {
        auto reply = QMessageBox::question(this, "Xác nhận", 
                                          QString("Bạn có chắc muốn xóa dịch vụ '%1'?").arg(tenDV),
                                          QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (quanLy->xoaDichVu(maDV.toStdString())) {
                updateDichVuTable();  // ✅ Load lại dữ liệu ngay
                detailDialog->close();
                QMessageBox::information(this, "Thành công", "Đã xóa dịch vụ thành công!");
            } else {
                QMessageBox::critical(this, "Lỗi", "Không thể xóa dịch vụ!");
            }
        }
    });
    
    // Kết nối sự kiện Close
    connect(btnClose, &QPushButton::clicked, detailDialog, &QDialog::close);
    
    detailDialog->exec();
    delete detailDialog;
}

void MainWindow::onThemNhanVien()
{
    NhanVienDialog dialog(quanLy, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (quanLy->themNhanVien(dialog.getMaNV(), dialog.getHoTen(), dialog.getSdt(),
                                 dialog.getViTri(), dialog.getLuong(),
                                 dialog.getTaiKhoan(), dialog.getMatKhau()))
        {
            updateNhanVienTable();
            QMessageBox::information(this, "✅ Thành công",
                                     "Đã thêm nhân viên!\n\n"
                                     "Tài khoản: " +
                                         QString::fromStdString(dialog.getTaiKhoan()) + "\n"
                                                                                        "Mật khẩu: " +
                                         QString::fromStdString(dialog.getMatKhau()));
        }
        else
        {
            QMessageBox::warning(this, "❌ Lỗi", "Mã nhân viên đã tồn tại!");
        }
    }
}

void MainWindow::onSuaNhanVien()
{
    int row = tableNhanVien->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "⚠️ Cảnh báo", "Vui lòng chọn nhân viên cần sửa!");
        return;
    }

    QString maNV = tableNhanVien->item(row, 0)->text();
    NhanVien *nv = quanLy->timNhanVien(maNV.toStdString());

    if (nv)
    {
        NhanVienDialog dialog(quanLy, this, nv);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (quanLy->suaNhanVien(maNV.toStdString(), dialog.getHoTen(), dialog.getSdt(),
                                    dialog.getViTri(), dialog.getLuong(),
                                    dialog.getTaiKhoan(), dialog.getMatKhau()))
            {
                updateNhanVienTable();
                QMessageBox::information(this, "✅ Thành công", "Đã cập nhật nhân viên!");
            }
        }
    }
}

void MainWindow::onXoaNhanVien()
{
    int row = tableNhanVien->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn nhân viên cần xóa!");
        return;
    }

    QString maNV = tableNhanVien->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn xóa nhân viên này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->xoaNhanVien(maNV.toStdString()))
        {
            updateNhanVienTable();
            QMessageBox::information(this, "Thành công", "Đã xóa nhân viên!");
        }
    }
}

void MainWindow::onRefreshNhanVien()
{
    updateNhanVienTable();
}

void MainWindow::onNhanVienRowDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    
    if (row < 0 || row >= tableNhanVien->rowCount()) {
        return;
    }
    
    // Lấy thông tin nhân viên từ row được chọn
    QString maNV = tableNhanVien->item(row, 0)->text();
    QString hoTen = tableNhanVien->item(row, 1)->text();
    QString sdt = tableNhanVien->item(row, 2)->text();
    QString viTri = tableNhanVien->item(row, 3)->text();
    QString luong = tableNhanVien->item(row, 4)->text();
    QString trangThai = tableNhanVien->item(row, 5)->text();
    
    // Tạo dialog chi tiết
    QDialog *detailDialog = new QDialog(this);
    detailDialog->setWindowTitle("Chi Tiết Nhân Viên");
    detailDialog->setMinimumWidth(450);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(detailDialog);
    
    // Group box hiển thị thông tin
    QGroupBox *infoGroup = new QGroupBox("Thông Tin Nhân Viên", detailDialog);
    QGridLayout *infoLayout = new QGridLayout(infoGroup);
    
    infoLayout->addWidget(new QLabel("<b>Mã Nhân Viên:</b>"), 0, 0);
    infoLayout->addWidget(new QLabel(maNV), 0, 1);
    
    infoLayout->addWidget(new QLabel("<b>Họ Tên:</b>"), 1, 0);
    infoLayout->addWidget(new QLabel(hoTen), 1, 1);
    
    infoLayout->addWidget(new QLabel("<b>Số Điện Thoại:</b>"), 2, 0);
    infoLayout->addWidget(new QLabel(sdt), 2, 1);
    
    infoLayout->addWidget(new QLabel("<b>Vị Trí:</b>"), 3, 0);
    infoLayout->addWidget(new QLabel(viTri), 3, 1);
    
    infoLayout->addWidget(new QLabel("<b>Lương:</b>"), 4, 0);
    infoLayout->addWidget(new QLabel(luong), 4, 1);
    
    infoLayout->addWidget(new QLabel("<b>Trạng Thái:</b>"), 5, 0);
    infoLayout->addWidget(new QLabel(trangThai), 5, 1);
    
    mainLayout->addWidget(infoGroup);
    
    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *btnEdit = new QPushButton("✏️  Sửa", detailDialog);
    btnEdit->setStyleSheet("QPushButton { padding: 8px 15px; background: #4CAF50; color: white; border-radius: 4px; }"
                           "QPushButton:hover { background: #45a049; }");
    
    QPushButton *btnDelete = new QPushButton("🗑️  Xóa", detailDialog);
    btnDelete->setStyleSheet("QPushButton { padding: 8px 15px; background: #f44336; color: white; border-radius: 4px; }"
                             "QPushButton:hover { background: #da190b; }");
    
    QPushButton *btnClose = new QPushButton("❌  Đóng", detailDialog);
    btnClose->setStyleSheet("QPushButton { padding: 8px 15px; background: #808080; color: white; border-radius: 4px; }"
                            "QPushButton:hover { background: #606060; }");
    
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);
    
    mainLayout->addLayout(btnLayout);
    
    // Kết nối sự kiện Edit
    connect(btnEdit, &QPushButton::clicked, detailDialog, [this, maNV, detailDialog]() {
        NhanVien *nv = quanLy->timNhanVien(maNV.toStdString());
        if (!nv) {
            QMessageBox::warning(this, "Lỗi", "Không tìm thấy nhân viên!");
            return;
        }
        NhanVienDialog editDialog(quanLy, this, nv);
        if (editDialog.exec() == QDialog::Accepted) {
            updateNhanVienTable();  // ✅ Load lại dữ liệu ngay
            detailDialog->close();
            QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin nhân viên!");
        }
    });
    
    // Kết nối sự kiện Delete
    connect(btnDelete, &QPushButton::clicked, detailDialog, [this, maNV, hoTen, detailDialog]() {
        auto reply = QMessageBox::question(this, "Xác nhận", 
                                          QString("Bạn có chắc muốn xóa nhân viên '%1'?").arg(hoTen),
                                          QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            try {
                quanLy->xoaNhanVien(maNV.toStdString());
                updateNhanVienTable();  // ✅ Load lại dữ liệu ngay
                detailDialog->close();
                QMessageBox::information(this, "Thành công", "Đã xóa nhân viên thành công!");
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Lỗi", QString("Không thể xóa nhân viên: %1").arg(e.what()));
            }
        }
    });
    
    // Kết nối sự kiện Close
    connect(btnClose, &QPushButton::clicked, detailDialog, &QDialog::close);
    
    detailDialog->exec();
    delete detailDialog;
}

void MainWindow::onXemThongKe()
{
    updateThongKeDisplay();
}

// ===== AUTHENTICATION METHODS =====

void MainWindow::applyAccessControl()
{
    bool isAdmin = authManager->hasFullAccess();

    // User info removed from sidebar - no longer displaying user details

    // Nhân viên không được:
    // 1. Xóa sân bóng, khách hàng, dịch vụ
    // 2. Quản lý nhân viên

    if (!isAdmin)
    {
        // Ẩn/vô hiệu hóa các nút xóa
        if (btnXoaSan)
        {
            btnXoaSan->setEnabled(false);
            btnXoaSan->setStyleSheet("background-color: #999; color: #666;");
            btnXoaSan->setToolTip("⚠️ Chỉ Admin mới có quyền xóa sân");
        }

        if (btnXoaKH)
        {
            btnXoaKH->setEnabled(false);
            btnXoaKH->setStyleSheet("background-color: #999; color: #666;");
            btnXoaKH->setToolTip("⚠️ Chỉ Admin mới có quyền xóa khách hàng");
        }

        if (btnXoaDV)
        {
            btnXoaDV->setEnabled(false);
            btnXoaDV->setStyleSheet("background-color: #999; color: #666;");
            btnXoaDV->setToolTip("⚠️ Chỉ Admin mới có quyền xóa dịch vụ");
        }

        // Ẩn menu Nhân viên
        btnMenuNhanVien.container->setVisible(false);

        // Ẩn page Nhân viên (page index 4)
        if (stackedWidget && stackedWidget->count() > 4)
        {
            // Không ẩn page, chỉ ẩn button menu
        }
    }
}

void MainWindow::onLogout()
{
    auto reply = QMessageBox::question(this, "🚪 Đăng Xuất",
                                       "Bạn có chắc muốn đăng xuất?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Lưu dữ liệu trước khi đăng xuất
        quanLy->luuDuLieu();
        authManager->logout();

        // Emit signal to return to login instead of closing app
        emit logoutRequested();

        // Close this window
        close();
    }
}

void MainWindow::onChangePassword()
{
    if (!authManager->hasFullAccess())
    {
        QMessageBox::warning(this, "⚠️ Không có quyền",
                             "Chỉ Admin mới có thể đổi mật khẩu!");
        return;
    }

    bool ok;
    QString oldPass = QInputDialog::getText(this, "🔑 Đổi Mật Khẩu Admin",
                                            "Nhập mật khẩu hiện tại:",
                                            QLineEdit::Password, "", &ok);

    if (!ok || oldPass.isEmpty())
        return;

    QString newPass = QInputDialog::getText(this, "🔑 Đổi Mật Khẩu Admin",
                                            "Nhập mật khẩu mới:",
                                            QLineEdit::Password, "", &ok);

    if (!ok || newPass.isEmpty())
        return;

    QString confirmPass = QInputDialog::getText(this, "🔑 Đổi Mật Khẩu Admin",
                                                "Xác nhận mật khẩu mới:",
                                                QLineEdit::Password, "", &ok);

    if (!ok || confirmPass.isEmpty())
        return;

    if (newPass != confirmPass)
    {
        QMessageBox::warning(this, "❌ Lỗi", "Mật khẩu xác nhận không khớp!");
        return;
    }

    if (authManager->changeAdminPassword(oldPass.toStdString(), newPass.toStdString()))
    {
        QMessageBox::information(this, "✅ Thành công",
                                 "Đã đổi mật khẩu Admin thành công!");
    }
    else
    {
        QMessageBox::critical(this, "❌ Thất bại",
                              "Mật khẩu hiện tại không đúng!");
    }
}

// =====================================================
// ✅ BACKUP & RESTORE FUNCTIONS
// =====================================================

void MainWindow::onBackupData()
{
    auto reply = QMessageBox::question(
        this,
        "💾 Sao Lưu Dữ Liệu",
        "Bạn có muốn tạo bản sao lưu toàn bộ dữ liệu?\n\n"
        "File backup sẽ được lưu tại: data/backup/\n"
        "Tên file: backup_YYYYMMDD_HHMMSS_*.dat",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        if (quanLy->taoBackupToanBo())
        {
            QMessageBox::information(
                this,
                "✅ Thành Công",
                "Đã tạo bản sao lưu thành công!\n\n"
                "Vị trí: data/backup/\n"
                "Tất cả dữ liệu đã được sao lưu an toàn.");
        }
        else
        {
            QMessageBox::critical(
                this,
                "❌ Lỗi",
                "Không thể tạo bản sao lưu!\n\n"
                "Hãy kiểm tra quyền ghi file.");
        }
    }
}

void MainWindow::onRestoreData()
{
    RestoreDialog dialog(quanLy, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        // Data has been restored - refresh all tables
        updateSanBongTable();
        updateKhachHangTable();
        updateLichDatTable();
        updateDichVuTable();
        updateNhanVienTable();
        updateThongKeDisplay();
        
        QMessageBox::information(
            this,
            "🔄 Đã Khôi Phục",
            "Dữ liệu đã được khôi phục!\n\n"
            "Tất cả bảng đã được cập nhật.");
    }
}



