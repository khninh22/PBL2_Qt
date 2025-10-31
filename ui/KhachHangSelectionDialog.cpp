#include "KhachHangSelectionDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QHeaderView>
#include <QScrollArea>

KhachHangSelectionDialog::KhachHangSelectionDialog(QuanLyThueSan *ql, QWidget *parent)
    : QDialog(parent), quanLy(ql), selectedMaKH(""), tongTienDichVu(0.0), 
      tongTienSan(0.0), bookingConfirmed(false)
{
    setWindowTitle("📋 Thông Tin Đặt Sân");
    setMinimumSize(900, 650);
    setupUI();
}

void KhachHangSelectionDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // HEADER
    QLabel *lblTitle = new QLabel("📋 THÔNG TIN ĐẶT SÂN");
    QFont titleFont = lblTitle->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    lblTitle->setFont(titleFont);
    lblTitle->setStyleSheet("color: #1976D2; padding: 10px; background-color: #E3F2FD; border-radius: 5px;");
    lblTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblTitle);

    mainLayout->addSpacing(10);

    // TAB WIDGET
    tabWidget = new QTabWidget();
    tabWidget->setStyleSheet(
        "QTabWidget::pane { border: 2px solid #ddd; border-radius: 5px; }"
        "QTabBar::tab { padding: 10px 20px; font-weight: bold; font-size: 12px; }"
        "QTabBar::tab:selected { background-color: #1976D2; color: white; }"
        "QTabBar::tab:!selected { background-color: #f0f0f0; color: #555; }"
    );

    setupTabKhachHang();
    setupTabDichVu();
    setupTabTomTat(); // ✅ Tab 3

    tabWidget->addTab(tabKhachHang, "👤 1. Khách Hàng");
    tabWidget->addTab(tabDichVu, "🎯 2. Dịch Vụ");
    tabWidget->addTab(tabTomTat, "📋 3. Tóm Tắt");
    
    // ✅ Vô hiệu hóa tab 3 ban đầu (chỉ mở khi có booking info)
    tabWidget->setTabEnabled(2, false);

    mainLayout->addWidget(tabWidget);
    mainLayout->addSpacing(15);

    // BUTTONS
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    btnCancel = new QPushButton("❌ Hủy");
    btnCancel->setMinimumSize(120, 40);
    btnCancel->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 5px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #c0392b; }"
    );
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    btnNext = new QPushButton("Tiếp tục →");
    btnNext->setMinimumSize(150, 40);
    btnNext->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; border: none; border-radius: 5px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #2980b9; }"
    );
    connect(btnNext, &QPushButton::clicked, this, &KhachHangSelectionDialog::onNextToSummary);

    btnConfirm = new QPushButton("✅ Xác Nhận Đặt Sân");
    btnConfirm->setMinimumSize(180, 40);
    btnConfirm->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 5px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #229954; }"
    );
    btnConfirm->setVisible(false); // Chỉ hiện ở tab 3
    connect(btnConfirm, &QPushButton::clicked, this, &KhachHangSelectionDialog::onConfirmBooking);

    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(btnNext);
    buttonLayout->addWidget(btnConfirm);

    mainLayout->addLayout(buttonLayout);
    
    // ✅ Thay đổi nút khi chuyển tab
    connect(tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        btnNext->setVisible(index < 2);
        btnConfirm->setVisible(index == 2);
        
        if (index == 2) {
            updateSummary(); // Cập nhật tóm tắt khi vào tab 3
        }
    });
}

void KhachHangSelectionDialog::setupTabKhachHang()
{
    tabKhachHang = new QWidget();
    QVBoxLayout *mainTabLayout = new QVBoxLayout(tabKhachHang);

    // MODE SELECTION
    QHBoxLayout *modeLayout = new QHBoxLayout();
    rbChonCoSan = new QRadioButton("🔍 Chọn khách hàng có sẵn");
    rbThemMoi = new QRadioButton("➕ Thêm khách hàng mới");
    rbChonCoSan->setChecked(true);

    rbChonCoSan->setStyleSheet("font-size: 13px; font-weight: bold;");
    rbThemMoi->setStyleSheet("font-size: 13px; font-weight: bold;");

    modeLayout->addWidget(rbChonCoSan);
    modeLayout->addSpacing(30);
    modeLayout->addWidget(rbThemMoi);
    modeLayout->addStretch();
    mainTabLayout->addLayout(modeLayout);
    mainTabLayout->addSpacing(10);

    // STACKED WIDGET
    stackedWidget = new QStackedWidget();

    // PAGE 1: Chọn khách hàng
    pageChonKH = new QWidget();
    QVBoxLayout *page1Layout = new QVBoxLayout(pageChonKH);

    QGroupBox *grpTimKiem = new QGroupBox("🔎 Tìm kiếm khách hàng");
    QVBoxLayout *grpLayout1 = new QVBoxLayout(grpTimKiem);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Tìm kiếm:"));
    txtTimKiem = new QLineEdit();
    txtTimKiem->setPlaceholderText("Nhập tên hoặc SĐT...");
    searchLayout->addWidget(txtTimKiem);
    QPushButton *btnSearch = new QPushButton("🔍");
    btnSearch->setStyleSheet("background-color: #3498db; color: white; padding: 8px 15px; border-radius: 5px;");
    searchLayout->addWidget(btnSearch);
    grpLayout1->addLayout(searchLayout);

    cboKhachHang = new QComboBox();
    cboKhachHang->setStyleSheet("padding: 8px;");
    grpLayout1->addWidget(new QLabel("Danh sách:"));
    grpLayout1->addWidget(cboKhachHang);

    // Load khách hàng
    const MangDong<KhachHang> &dsKH = quanLy->getDsKhachHang();
    if (dsKH.getKichThuoc() == 0)
    {
        cboKhachHang->addItem("(Chưa có khách hàng)");
        cboKhachHang->setEnabled(false);
    }
    else
    {
        for (int i = 0; i < dsKH.getKichThuoc(); i++)
        {
            QString item = QString("%1 - %2 - %3")
                               .arg(QString::fromStdString(dsKH[i].getMaKH()))
                               .arg(QString::fromStdString(dsKH[i].getHoTen()))
                               .arg(QString::fromStdString(dsKH[i].getSdt()));
            cboKhachHang->addItem(item, QString::fromStdString(dsKH[i].getMaKH()));
        }
    }

    lblThongTinKH = new QLabel();
    lblThongTinKH->setStyleSheet("background-color: #f8f9fa; padding: 15px; border: 1px solid #ddd; border-radius: 5px;");
    lblThongTinKH->setWordWrap(true);
    grpLayout1->addWidget(new QLabel("📄 Thông tin:"));
    grpLayout1->addWidget(lblThongTinKH);

    page1Layout->addWidget(grpTimKiem);
    page1Layout->addStretch();

    connect(cboKhachHang, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]() {
        QString maKH = cboKhachHang->currentData().toString();
        if (maKH.isEmpty()) return;
        
        KhachHang *kh = quanLy->timKhachHang(maKH.toStdString());
        if (kh) {
            QString info = QString("👤 <b>Mã:</b> %1<br>📝 <b>Họ tên:</b> %2<br>📞 <b>SĐT:</b> %3<br>⭐ <b>Điểm:</b> %4<br>🎖️ <b>Cấp độ:</b> %5 (Giảm %6%)")
                            .arg(QString::fromStdString(kh->getMaKH()))
                            .arg(QString::fromStdString(kh->getHoTen()))
                            .arg(QString::fromStdString(kh->getSdt()))
                            .arg(kh->getDiemTichLuy())
                            .arg(QString::fromStdString(kh->getTenCapDo()))
                            .arg(kh->tinhPhanTramGiam());
            lblThongTinKH->setText(info);
        }
    });

    if (cboKhachHang->count() > 0 && cboKhachHang->isEnabled())
        cboKhachHang->setCurrentIndex(0);

    connect(btnSearch, &QPushButton::clicked, this, &KhachHangSelectionDialog::onSearchKhachHang);

    // PAGE 2: Thêm mới
    pageThemMoi = new QWidget();
    QVBoxLayout *page2Layout = new QVBoxLayout(pageThemMoi);

    QGroupBox *grpThongTin = new QGroupBox("📝 Thông tin khách hàng mới");
    QFormLayout *formLayout = new QFormLayout(grpThongTin);

    txtHoTen = new QLineEdit();
    txtHoTen->setPlaceholderText("Nhập họ và tên đầy đủ");
    formLayout->addRow("Họ tên: *", txtHoTen);

    txtSDT = new QLineEdit();
    txtSDT->setPlaceholderText("VD: 0912345678");
    QRegularExpression phoneRegex("^[0-9]{10,11}$");
    txtSDT->setValidator(new QRegularExpressionValidator(phoneRegex, this));
    formLayout->addRow("SĐT: *", txtSDT);

    page2Layout->addWidget(grpThongTin);
    page2Layout->addStretch();

    stackedWidget->addWidget(pageChonKH);
    stackedWidget->addWidget(pageThemMoi);

    mainTabLayout->addWidget(stackedWidget);

    connect(rbChonCoSan, &QRadioButton::toggled, this, &KhachHangSelectionDialog::onModeChanged);
    connect(rbThemMoi, &QRadioButton::toggled, this, &KhachHangSelectionDialog::onModeChanged);
}

void KhachHangSelectionDialog::setupTabDichVu()
{
    tabDichVu = new QWidget();
    QVBoxLayout *mainTabLayout = new QVBoxLayout(tabDichVu);

    QLabel *lblGuide = new QLabel("🎯 Chọn dịch vụ cần sử dụng (không bắt buộc)");
    lblGuide->setStyleSheet("color: #555; padding: 5px; background-color: #E3F2FD; border-radius: 5px;");
    mainTabLayout->addWidget(lblGuide);

    // TABLE
    tableDichVu = new QTableWidget(0, 5);
    tableDichVu->setHorizontalHeaderLabels({"Chọn", "Mã DV", "Tên dịch vụ", "Đơn giá", "Số lượng"});
    tableDichVu->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableDichVu->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableDichVu->setAlternatingRowColors(true);
    tableDichVu->verticalHeader()->setVisible(false);

    tableDichVu->setColumnWidth(0, 60);
    tableDichVu->setColumnWidth(1, 100);
    tableDichVu->setColumnWidth(2, 250);
    tableDichVu->setColumnWidth(3, 120);
    tableDichVu->setColumnWidth(4, 100);

    tableDichVu->horizontalHeader()->setStretchLastSection(false);
    tableDichVu->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    mainTabLayout->addWidget(tableDichVu);

    // TỔNG TIỀN
    QFrame *frameTong = new QFrame();
    frameTong->setFrameStyle(QFrame::Box);
    frameTong->setStyleSheet("background-color: #FFF9C4; border: 2px solid #FBC02D; border-radius: 5px; padding: 10px;");
    
    QHBoxLayout *layoutTong = new QHBoxLayout(frameTong);
    QLabel *lblTongLabel = new QLabel("💰 TỔNG TIỀN DỊCH VỤ:");
    lblTongLabel->setStyleSheet("font-weight: bold; font-size: 11pt;");
    
    lblTongTienDichVu = new QLabel("0 VNĐ");
    lblTongTienDichVu->setStyleSheet("color: #D32F2F; font-weight: bold; font-size: 12pt;");
    
    layoutTong->addWidget(lblTongLabel);
    layoutTong->addWidget(lblTongTienDichVu);
    layoutTong->addStretch();
    
    mainTabLayout->addWidget(frameTong);

    loadDichVu();
}

void KhachHangSelectionDialog::loadDichVu()
{
    dsDichVu = quanLy->getDsDichVu();
    
    if (dsDichVu.getKichThuoc() == 0)
    {
        QMessageBox::information(this, "Thông báo", "Không có dịch vụ nào trong hệ thống.");
        return;
    }

    tableDichVu->setRowCount(dsDichVu.getKichThuoc());

    for (int i = 0; i < dsDichVu.getKichThuoc(); i++)
    {
        const DichVu &dv = dsDichVu[i];

        // Checkbox
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setStyleSheet("margin-left: 20px;");
        connect(checkBox, &QCheckBox::checkStateChanged, this, &KhachHangSelectionDialog::onDichVuCheckChanged);
        tableDichVu->setCellWidget(i, 0, checkBox);

        // Mã
        tableDichVu->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(dv.getMaDV())));

        // Tên
        tableDichVu->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(dv.getTenDV())));

        // Giá
        QTableWidgetItem *itemGia = new QTableWidgetItem(QString("%L1 VNĐ").arg(dv.getGiaDV(), 0, 'f', 0));
        itemGia->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tableDichVu->setItem(i, 3, itemGia);

        // Spinbox
        QSpinBox *spinBox = new QSpinBox();
        spinBox->setMinimum(0);
        spinBox->setMaximum(100);
        spinBox->setValue(0);
        spinBox->setAlignment(Qt::AlignCenter);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &KhachHangSelectionDialog::onDichVuSpinChanged);
        tableDichVu->setCellWidget(i, 4, spinBox);
    }

    updateDichVuTongTien();
}

void KhachHangSelectionDialog::updateDichVuTongTien()
{
    tongTienDichVu = 0.0;
    selectedDichVu.clear();

    for (int i = 0; i < tableDichVu->rowCount(); i++)
    {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(tableDichVu->cellWidget(i, 0));
        QSpinBox *spinBox = qobject_cast<QSpinBox*>(tableDichVu->cellWidget(i, 4));

        if (checkBox && checkBox->isChecked() && spinBox && spinBox->value() > 0)
        {
            const DichVu &dv = dsDichVu[i];
            int soLuong = spinBox->value();
            double thanhTien = dv.getGiaDV() * soLuong;

            selectedDichVu.append(DichVuInfo(
                dv.getMaDV(),
                dv.getTenDV(),
                dv.getGiaDV(),
                soLuong
            ));

            tongTienDichVu += thanhTien;
        }
    }

    lblTongTienDichVu->setText(QString("%L1 VNĐ").arg(tongTienDichVu, 0, 'f', 0));
}

void KhachHangSelectionDialog::onModeChanged()
{
    stackedWidget->setCurrentIndex(rbThemMoi->isChecked() ? 1 : 0);
    if (rbThemMoi->isChecked())
        txtHoTen->setFocus();
}

void KhachHangSelectionDialog::onSearchKhachHang()
{
    QString keyword = txtTimKiem->text().trimmed().toLower();
    if (keyword.isEmpty())
        return;

    const MangDong<KhachHang> &dsKH = quanLy->getDsKhachHang();
    for (int i = 0; i < cboKhachHang->count(); i++)
    {
        QString itemText = cboKhachHang->itemText(i).toLower();
        if (itemText.contains(keyword))
        {
            cboKhachHang->setCurrentIndex(i);
            return;
        }
    }

    QMessageBox::information(this, "Không tìm thấy", "Không tìm thấy khách hàng phù hợp.");
}

void KhachHangSelectionDialog::onDichVuCheckChanged(Qt::CheckState state)
{
    QCheckBox *sender = qobject_cast<QCheckBox*>(QObject::sender());
    if (!sender) return;

    for (int i = 0; i < tableDichVu->rowCount(); i++)
    {
        if (tableDichVu->cellWidget(i, 0) == sender)
        {
            QSpinBox *spinBox = qobject_cast<QSpinBox*>(tableDichVu->cellWidget(i, 4));
            if (spinBox)
            {
                if (state == Qt::Checked && spinBox->value() == 0)
                    spinBox->setValue(1);
                else if (state == Qt::Unchecked)
                    spinBox->setValue(0);
            }
            break;
        }
    }

    updateDichVuTongTien();
}

void KhachHangSelectionDialog::onDichVuSpinChanged(int value)
{
    QSpinBox *sender = qobject_cast<QSpinBox*>(QObject::sender());
    if (!sender) return;

    for (int i = 0; i < tableDichVu->rowCount(); i++)
    {
        if (tableDichVu->cellWidget(i, 4) == sender)
        {
            QCheckBox *checkBox = qobject_cast<QCheckBox*>(tableDichVu->cellWidget(i, 0));
            if (checkBox)
            {
                checkBox->setChecked(value > 0);
            }
            break;
        }
    }

    updateDichVuTongTien();
}

bool KhachHangSelectionDialog::validateNewCustomer()
{
    QString hoTen = txtHoTen->text().trimmed();
    QString sdt = txtSDT->text().trimmed();

    if (hoTen.isEmpty())
    {
        QMessageBox::warning(this, "⚠️ Thiếu thông tin", "Vui lòng nhập họ tên!");
        txtHoTen->setFocus();
        return false;
    }

    if (sdt.length() < 10 || sdt.length() > 11)
    {
        QMessageBox::warning(this, "⚠️ SĐT không hợp lệ", "Số điện thoại phải có 10-11 chữ số!");
        txtSDT->setFocus();
        return false;
    }

    // Check duplicate
    const MangDong<KhachHang> &dsKH = quanLy->getDsKhachHang();
    for (int i = 0; i < dsKH.getKichThuoc(); i++)
    {
        if (QString::fromStdString(dsKH[i].getSdt()) == sdt)
        {
            QMessageBox::warning(this, "⚠️ Trùng lặp", QString("Số điện thoại %1 đã tồn tại!\n\nKhách hàng: %2")
                .arg(sdt)
                .arg(QString::fromStdString(dsKH[i].getHoTen())));
            return false;
        }
    }

    // Tạo mã KH
    QString maKH = generateMaKH();
    
    // Thêm vào hệ thống
    if (quanLy->themKhachHang(maKH.toStdString(), hoTen.toStdString(), sdt.toStdString()))
    {
        selectedMaKH = maKH;
        QMessageBox::information(this, "✅ Thành công", QString("Đã thêm khách hàng mới:\n\nMã: %1\nHọ tên: %2\nSĐT: %3")
            .arg(maKH).arg(hoTen).arg(sdt));
        return true;
    }
    else
    {
        QMessageBox::critical(this, "❌ Lỗi", "Không thể thêm khách hàng!");
        return false;
    }
}

QString KhachHangSelectionDialog::generateMaKH()
{
    const MangDong<KhachHang> &dsKH = quanLy->getDsKhachHang();
    int maxNum = 0;

    for (int i = 0; i < dsKH.getKichThuoc(); i++)
    {
        QString maKH = QString::fromStdString(dsKH[i].getMaKH());
        if (maKH.startsWith("KH"))
        {
            bool ok;
            int num = maKH.mid(2).toInt(&ok);
            if (ok && num > maxNum)
                maxNum = num;
        }
    }

    return QString("KH%1").arg(maxNum + 1, 3, 10, QChar('0'));
}

// ===== TAB 3: TÓM TẮT =====
void KhachHangSelectionDialog::setupTabTomTat()
{
    tabTomTat = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tabTomTat);
    layout->setSpacing(15);
    
    QLabel *lblHeader = new QLabel("📋 XÁC NHẬN THÔNG TIN ĐẶT SÂN");
    lblHeader->setAlignment(Qt::AlignCenter);
    lblHeader->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: #1976D2; "
        "padding: 12px; background-color: #E3F2FD; border-radius: 5px;"
    );
    layout->addWidget(lblHeader);
    
    // Nội dung tóm tắt
    lblTomTatNoiDung = new QLabel();
    lblTomTatNoiDung->setWordWrap(true);
    lblTomTatNoiDung->setTextFormat(Qt::RichText);
    lblTomTatNoiDung->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    lblTomTatNoiDung->setStyleSheet(
        "background-color: white; padding: 20px; border: 2px solid #ddd; "
        "border-radius: 8px; font-size: 13px; line-height: 1.6;"
    );
    
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(lblTomTatNoiDung);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(350);
    layout->addWidget(scrollArea);
    
    // Tổng thanh toán (nổi bật)
    lblTongThanhToan = new QLabel();
    lblTongThanhToan->setAlignment(Qt::AlignCenter);
    lblTongThanhToan->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: white; "
        "background-color: #27ae60; padding: 15px; border-radius: 8px;"
    );
    layout->addWidget(lblTongThanhToan);
}

void KhachHangSelectionDialog::setBookingInfo(const QDate &ngay, const QList<BookingSlot> &slotList)
{
    ngayDat = ngay;
    bookingSlots = slotList;
    
    // Tính tổng tiền sân
    tongTienSan = 0;
    for (const BookingSlot &slot : slotList) {
        tongTienSan += slot.giaTien;
    }
    
    // ✅ Kích hoạt tab 3
    tabWidget->setTabEnabled(2, true);
}

void KhachHangSelectionDialog::onNextToSummary()
{
    int currentTab = tabWidget->currentIndex();
    
    if (currentTab == 0) {
        // Validate khách hàng trước khi chuyển tab
        if (!validateCustomerSelection()) {
            return;
        }
        tabWidget->setCurrentIndex(1); // Chuyển sang tab Dịch vụ
    }
    else if (currentTab == 1) {
        tabWidget->setCurrentIndex(2); // Chuyển sang tab Tóm tắt
    }
}

bool KhachHangSelectionDialog::validateCustomerSelection()
{
    if (rbChonCoSan->isChecked())
    {
        if (!cboKhachHang->isEnabled() || cboKhachHang->currentData().toString().isEmpty())
        {
            QMessageBox::warning(this, "⚠️ Cảnh báo", "Vui lòng chọn khách hàng!");
            return false;
        }
        selectedMaKH = cboKhachHang->currentData().toString();
        return true;
    }
    else
    {
        return validateNewCustomer();
    }
}

void KhachHangSelectionDialog::updateSummary()
{
    if (selectedMaKH.isEmpty() || bookingSlots.isEmpty()) {
        lblTomTatNoiDung->setText("<p style='color:red;'>⚠️ Chưa có thông tin đặt sân</p>");
        lblTongThanhToan->setText("TỔNG: 0 VNĐ");
        return;
    }
    
    // Lấy thông tin khách hàng
    KhachHang *kh = quanLy->timKhachHang(selectedMaKH.toStdString());
    if (!kh) {
        lblTomTatNoiDung->setText("<p style='color:red;'>⚠️ Không tìm thấy thông tin khách hàng</p>");
        return;
    }
    
    // Tính toán
    double giamGia = tongTienSan * kh->tinhPhanTramGiam() / 100.0;
    double thanhTienSan = tongTienSan - giamGia;
    double tongThanhToan = thanhTienSan + tongTienDichVu;
    
    // Tạo HTML tóm tắt
    QString html = "<div style='font-family: Segoe UI, Arial;'>";
    
    // KHÁCH HÀNG
    html += "<h3 style='color: #1976D2; border-bottom: 2px solid #1976D2; padding-bottom: 5px;'>👤 KHÁCH HÀNG</h3>";
    html += "<table style='width:100%; margin-bottom: 15px;'>";
    html += QString("<tr><td width='150'><b>Mã KH:</b></td><td>%1</td></tr>").arg(QString::fromStdString(kh->getMaKH()));
    html += QString("<tr><td><b>Họ tên:</b></td><td>%1</td></tr>").arg(QString::fromStdString(kh->getHoTen()));
    html += QString("<tr><td><b>SĐT:</b></td><td>%1</td></tr>").arg(QString::fromStdString(kh->getSdt()));
    html += QString("<tr><td><b>Điểm tích lũy:</b></td><td>%1 điểm</td></tr>").arg(kh->getDiemTichLuy());
    html += QString("<tr><td><b>Cấp độ:</b></td><td><span style='color:#e67e22; font-weight:bold;'>%1</span> (Giảm %2%)</td></tr>")
                .arg(QString::fromStdString(kh->getTenCapDo())).arg(kh->tinhPhanTramGiam());
    html += "</table>";
    
    // THÔNG TIN ĐẶT SÂN
    html += "<h3 style='color: #1976D2; border-bottom: 2px solid #1976D2; padding-bottom: 5px;'>📅 THÔNG TIN ĐẶT SÂN</h3>";
    html += QString("<p><b>Ngày:</b> %1</p>").arg(ngayDat.toString("dd/MM/yyyy (dddd)"));
    html += "<table style='width:100%; border-collapse: collapse; margin-bottom: 15px;' border='1' cellpadding='5'>";
    html += "<tr style='background-color: #E3F2FD; font-weight: bold;'>"
            "<th>Sân</th><th>Thời gian</th><th>Số giờ</th><th>Giá tiền</th></tr>";
    
    for (const BookingSlot &slot : bookingSlots) {
        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%L4 VNĐ</td></tr>")
                    .arg(slot.maSan)
                    .arg(slot.thoiGian)
                    .arg(slot.soGio)
                    .arg(slot.giaTien, 0, 'f', 0);
    }
    html += "</table>";
    
    // DỊCH VỤ (nếu có)
    if (!selectedDichVu.isEmpty()) {
        html += "<h3 style='color: #1976D2; border-bottom: 2px solid #1976D2; padding-bottom: 5px;'>🎯 DỊCH VỤ ĐÃ CHỌN</h3>";
        html += "<table style='width:100%; border-collapse: collapse; margin-bottom: 15px;' border='1' cellpadding='5'>";
        html += "<tr style='background-color: #E3F2FD; font-weight: bold;'>"
                "<th>Dịch vụ</th><th>Đơn giá</th><th>Số lượng</th><th>Thành tiền</th></tr>";
        
        for (const DichVuInfo &dv : selectedDichVu) {
            html += QString("<tr><td>%1</td><td>%L2 VNĐ</td><td>%3</td><td><b>%L4 VNĐ</b></td></tr>")
                        .arg(QString::fromStdString(dv.tenDichVu))
                        .arg(dv.donGia, 0, 'f', 0)
                        .arg(dv.soLuong)
                        .arg(dv.thanhTien, 0, 'f', 0);
        }
        html += "</table>";
    }
    
    // CHI PHÍ
    html += "<h3 style='color: #27ae60; border-bottom: 2px solid #27ae60; padding-bottom: 5px;'>💰 CHI PHÍ</h3>";
    html += "<table style='width:100%; font-size: 14px;'>";
    html += QString("<tr><td width='200'><b>Tiền sân:</b></td><td align='right'>%L1 VNĐ</td></tr>").arg(tongTienSan, 0, 'f', 0);
    
    if (giamGia > 0) {
        html += QString("<tr><td><b>Giảm giá (%1%):</b></td><td align='right' style='color:#e74c3c;'>-%L2 VNĐ</td></tr>")
                    .arg(kh->tinhPhanTramGiam()).arg(giamGia, 0, 'f', 0);
    }
    
    if (tongTienDichVu > 0) {
        html += QString("<tr><td><b>Dịch vụ:</b></td><td align='right'>+%L1 VNĐ</td></tr>").arg(tongTienDichVu, 0, 'f', 0);
    }
    
    html += QString("<tr style='font-size: 16px; color: #27ae60;'><td><b>TỔNG THANH TOÁN:</b></td><td align='right'><b>%L1 VNĐ</b></td></tr>")
                .arg(tongThanhToan, 0, 'f', 0);
    html += "</table>";
    
    html += "</div>";
    
    lblTomTatNoiDung->setText(html);
    lblTongThanhToan->setText(QString("💰 TỔNG THANH TOÁN: %L1 VNĐ").arg(tongThanhToan, 0, 'f', 0));
}

void KhachHangSelectionDialog::onConfirmBooking()
{
    if (selectedMaKH.isEmpty() || bookingSlots.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Lỗi", "Thiếu thông tin đặt sân!");
        return;
    }
    
    // Đánh dấu đã xác nhận
    bookingConfirmed = true;
    accept();
}
