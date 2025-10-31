#include "PaymentDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QPrinter>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrintDialog>

PaymentDialog::PaymentDialog(QuanLyThueSan *ql, QWidget *parent)
    : QDialog(parent), quanLy(ql)
{
    setWindowTitle("💰 Thanh Toán Hóa Đơn");
    setMinimumSize(1200, 700);
    setupUI();
    loadLichDatChuaThanhToan();
}

void PaymentDialog::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    
    // ========== LEFT SIDE: Danh sách lịch đặt ==========
    QVBoxLayout *leftLayout = new QVBoxLayout();
    
    QLabel *lblTitle = new QLabel("📋 Các lịch đặt chưa thanh toán");
    lblTitle->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #2196F3;"
        "padding: 10px;"
    );
    leftLayout->addWidget(lblTitle);
    
    tableLichDat = new QTableWidget();
    tableLichDat->setColumnCount(6);
    tableLichDat->setHorizontalHeaderLabels({"Mã Lịch", "Mã KH", "Tên KH", "Bắt đầu", "Kết thúc", "Tổng tiền"});
    tableLichDat->horizontalHeader()->setStretchLastSection(true);
    tableLichDat->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLichDat->setSelectionMode(QAbstractItemView::SingleSelection);
    tableLichDat->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLichDat->setStyleSheet(
        "QTableWidget {"
        "    border: 2px solid #2196F3;"
        "    gridline-color: #e0e0e0;"
        "    selection-background-color: #2196F3;"
        "    background-color: white;"
        "    border-radius: 8px;"
        "}"
        "QHeaderView::section {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 8px;"
        "}"
    );
    leftLayout->addWidget(tableLichDat);
    
    mainLayout->addLayout(leftLayout, 2);
    
    // ========== RIGHT SIDE: Hóa đơn ==========
    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    QLabel *lblInvoiceTitle = new QLabel("🧾 CHI TIẾT HÓA ĐƠN");
    lblInvoiceTitle->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #4CAF50;"
        "padding: 10px;"
        "text-align: center;"
    );
    lblInvoiceTitle->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(lblInvoiceTitle);
    
    // Text edit for invoice preview
    txtHoaDon = new QTextEdit();
    txtHoaDon->setReadOnly(true);
    txtHoaDon->setStyleSheet(
        "QTextEdit {"
        "    border: 2px solid #4CAF50;"
        "    border-radius: 8px;"
        "    background-color: white;"
        "    padding: 15px;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 12px;"
        "}"
    );
    rightLayout->addWidget(txtHoaDon);
    
    // Tổng tiền section
    QVBoxLayout *tongTienLayout = new QVBoxLayout();
    
    QHBoxLayout *tongTienRow = new QHBoxLayout();
    tongTienRow->addWidget(new QLabel("Tổng tiền:"));
    lblTongTien = new QLabel("0 VNĐ");
    lblTongTien->setStyleSheet("font-weight: bold; color: #2196F3;");
    tongTienRow->addStretch();
    tongTienRow->addWidget(lblTongTien);
    tongTienLayout->addLayout(tongTienRow);
    
    QHBoxLayout *giamGiaRow = new QHBoxLayout();
    giamGiaRow->addWidget(new QLabel("Giảm giá:"));
    lblGiamGia = new QLabel("0 VNĐ");
    lblGiamGia->setStyleSheet("font-weight: bold; color: #FF9800;");
    giamGiaRow->addStretch();
    giamGiaRow->addWidget(lblGiamGia);
    tongTienLayout->addLayout(giamGiaRow);
    
    // Separator
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #4CAF50; height: 2px;");
    tongTienLayout->addWidget(line);
    
    QHBoxLayout *thanhToanRow = new QHBoxLayout();
    QLabel *lblThanhToanText = new QLabel("THANH TOÁN:");
    lblThanhToanText->setStyleSheet("font-size: 16px; font-weight: bold;");
    thanhToanRow->addWidget(lblThanhToanText);
    lblThanhToan = new QLabel("0 VNĐ");
    lblThanhToan->setStyleSheet("font-size: 20px; font-weight: bold; color: #4CAF50;");
    thanhToanRow->addStretch();
    thanhToanRow->addWidget(lblThanhToan);
    tongTienLayout->addLayout(thanhToanRow);
    
    rightLayout->addLayout(tongTienLayout);
    
    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    btnXacNhan = new QPushButton("✅ Xác nhận thanh toán");
    btnXacNhan->setEnabled(false);
    btnXacNhan->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 12px 20px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDBDBD;"
        "}"
    );
    
    btnInHoaDon = new QPushButton("🖨️ In hóa đơn");
    btnInHoaDon->setEnabled(false);
    btnInHoaDon->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    padding: 12px 20px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDBDBD;"
        "}"
    );
    
    btnLuuPDF = new QPushButton("📄 Lưu PDF");
    btnLuuPDF->setEnabled(false);
    btnLuuPDF->setStyleSheet(
        "QPushButton {"
        "    background-color: #FF9800;"
        "    color: white;"
        "    border: none;"
        "    padding: 12px 20px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #F57C00;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDBDBD;"
        "}"
    );
    
    QPushButton *btnDong = new QPushButton("❌ Đóng");
    btnDong->setStyleSheet(
        "QPushButton {"
        "    background-color: #757575;"
        "    color: white;"
        "    border: none;"
        "    padding: 12px 20px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #616161;"
        "}"
    );
    
    btnLayout->addWidget(btnXacNhan);
    btnLayout->addWidget(btnInHoaDon);
    btnLayout->addWidget(btnLuuPDF);
    btnLayout->addStretch();
    btnLayout->addWidget(btnDong);
    
    rightLayout->addLayout(btnLayout);
    
    mainLayout->addLayout(rightLayout, 3);
    
    // ========== Connections ==========
    connect(tableLichDat, &QTableWidget::itemSelectionChanged, 
            this, &PaymentDialog::onLichDatSelected);
    connect(btnXacNhan, &QPushButton::clicked, this, &PaymentDialog::onXacNhanThanhToan);
    connect(btnInHoaDon, &QPushButton::clicked, this, &PaymentDialog::onInHoaDon);
    connect(btnLuuPDF, &QPushButton::clicked, this, &PaymentDialog::onLuuPDF);
    connect(btnDong, &QPushButton::clicked, this, &QDialog::reject);
}

void PaymentDialog::loadLichDatChuaThanhToan()
{
    tableLichDat->setRowCount(0);
    const MangDong<LichDatSan> &ds = quanLy->getDsLichDatSan();
    
    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const LichDatSan &lich = ds[i];
        
        // Chỉ hiển thị lịch chưa thanh toán và chưa bị hủy
        if (lich.getTrangThaiTT() == "Chưa Thanh Toán" && lich.getTrangThaiDat() == "Đã Đặt")
        {
            KhachHang *kh = quanLy->timKhachHang(lich.getMaKH());
            
            int row = tableLichDat->rowCount();
            tableLichDat->insertRow(row);
            
            tableLichDat->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(lich.getMaLichDat())));
            tableLichDat->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(lich.getMaKH())));
            tableLichDat->setItem(row, 2, new QTableWidgetItem(
                kh ? QString::fromStdString(kh->getHoTen()) : "N/A"));
            tableLichDat->setItem(row, 3, new QTableWidgetItem(
                QString::fromStdString(formatTime(lich.getThoiGianBatDau()))));
            tableLichDat->setItem(row, 4, new QTableWidgetItem(
                QString::fromStdString(formatTime(lich.getThoiGianKetThuc()))));
            tableLichDat->setItem(row, 5, new QTableWidgetItem(
                QString("%L1 VNĐ").arg(lich.getTongTien(), 0, 'f', 0)));
        }
    }
    
    if (tableLichDat->rowCount() == 0)
    {
        QMessageBox::information(this, "Thông báo", 
            "Không có lịch đặt nào cần thanh toán!");
    }
}

void PaymentDialog::onLichDatSelected()
{
    if (tableLichDat->currentRow() < 0)
    {
        maLichDatDangChon = "";
        btnXacNhan->setEnabled(false);
        btnInHoaDon->setEnabled(false);
        btnLuuPDF->setEnabled(false);
        txtHoaDon->clear();
        return;
    }
    
    maLichDatDangChon = tableLichDat->item(tableLichDat->currentRow(), 0)->text();
    btnXacNhan->setEnabled(true);
    btnInHoaDon->setEnabled(true);
    btnLuuPDF->setEnabled(true);
    
    generateInvoicePreview();
}

void PaymentDialog::generateInvoicePreview()
{
    QString html = generateInvoiceHTML();
    txtHoaDon->setHtml(html);
}

QString PaymentDialog::generateInvoiceHTML()
{
    LichDatSan *lich = quanLy->timLichDat(maLichDatDangChon.toStdString());
    if (!lich) return "";
    
    KhachHang *kh = quanLy->timKhachHang(lich->getMaKH());
    SanBong *san = quanLy->timSanBong(lich->getMaSan());
    
    if (!kh || !san) return "";
    
    // Tính toán
    double tongTien = lich->getTongTien();
    double phanTramGiam = kh->tinhPhanTramGiam();
    double giamGia = tongTien * phanTramGiam / 100.0;
    double thanhToan = tongTien - giamGia;
    
    // Update labels
    lblTongTien->setText(QString("%L1 VNĐ").arg(tongTien, 0, 'f', 0));
    lblGiamGia->setText(QString("%L1 VNĐ (%2%)").arg(giamGia, 0, 'f', 0).arg(phanTramGiam, 0, 'f', 0));
    lblThanhToan->setText(QString("%L1 VNĐ").arg(thanhToan, 0, 'f', 0));
    
    // Get dịch vụ đã đặt
    const MangDong<ChiTietDichVu> &dsChiTiet = quanLy->getDsChiTietDichVu();
    QString dichVuHTML = "";
    double tongTienDichVu = 0;
    
    for (int i = 0; i < dsChiTiet.getKichThuoc(); i++)
    {
        if (dsChiTiet[i].getMaLichDat() == lich->getMaLichDat())
        {
            DichVu *dv = quanLy->timDichVu(dsChiTiet[i].getMaDV());
            if (dv)
            {
                dichVuHTML += QString(
                    "<tr>"
                    "<td>%1</td>"
                    "<td>%L2</td>"
                    "<td>%3</td>"
                    "<td style='text-align: right;'><b>%L4</b></td>"
                    "</tr>"
                ).arg(QString::fromStdString(dv->getTenDV()))
                 .arg(dv->getGiaDV(), 0, 'f', 0)
                 .arg(dsChiTiet[i].getSoLuong())
                 .arg(dsChiTiet[i].getThanhTien(), 0, 'f', 0);
                
                tongTienDichVu += dsChiTiet[i].getThanhTien();
            }
        }
    }
    
    double tienSan = tongTien - tongTienDichVu;
    
    QString html = QString(
        "<html>"
        "<head>"
        "<style>"
        "body { font-family: Arial, sans-serif; }"
        "h2 { color: #4CAF50; text-align: center; border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }"
        "table { width: 100%%; border-collapse: collapse; margin: 15px 0; }"
        "th, td { padding: 10px; text-align: left; }"
        "th { background-color: #E8F5E9; color: #2E7D32; font-weight: bold; }"
        "tr:nth-child(even) { background-color: #f9f9f9; }"
        ".info { background-color: #E3F2FD; padding: 15px; border-radius: 8px; margin: 10px 0; }"
        ".total { background-color: #FFF3E0; padding: 10px; border-radius: 8px; margin: 10px 0; font-size: 14px; }"
        ".grand-total { background-color: #E8F5E9; padding: 15px; border-radius: 8px; margin: 10px 0; font-size: 16px; font-weight: bold; }"
        "</style>"
        "</head>"
        "<body>"
        "<h2>🏟️ HỆ THỐNG QUẢN LÝ THUÊ SÂN BÓNG</h2>"
        "<div class='info'>"
        "<p><b>📋 Mã hóa đơn:</b> %1</p>"
        "<p><b>📅 Ngày lập:</b> %2</p>"
        "<p><b>👤 Khách hàng:</b> %3 (Mã: %4)</p>"
        "<p><b>📞 Số điện thoại:</b> %5</p>"
        "<p><b>💎 Cấp độ:</b> %6 (%7 điểm)</p>"
        "</div>"
        "<h3 style='color: #2196F3;'>📊 THÔNG TIN ĐẶT SÂN</h3>"
        "<table border='1' style='border-color: #ddd;'>"
        "<tr>"
        "<th>Sân</th>"
        "<th>Thời gian bắt đầu</th>"
        "<th>Thời gian kết thúc</th>"
        "<th style='text-align: right;'>Thành tiền</th>"
        "</tr>"
        "<tr>"
        "<td>%8</td>"
        "<td>%9</td>"
        "<td>%10</td>"
        "<td style='text-align: right;'><b>%L11 VNĐ</b></td>"
        "</tr>"
        "</table>"
    ).arg(maLichDatDangChon)
     .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"))
     .arg(QString::fromStdString(kh->getHoTen()))
     .arg(QString::fromStdString(kh->getMaKH()))
     .arg(QString::fromStdString(kh->getSdt()))
     .arg(QString::fromStdString(kh->getTenCapDo()))
     .arg(kh->getDiemTichLuy())
     .arg(QString::fromStdString(san->getTenSan()))
     .arg(QString::fromStdString(formatTime(lich->getThoiGianBatDau())))
     .arg(QString::fromStdString(formatTime(lich->getThoiGianKetThuc())))
     .arg(tienSan, 0, 'f', 0);
    
    // Dịch vụ (nếu có)
    if (!dichVuHTML.isEmpty())
    {
        html += QString(
            "<h3 style='color: #FF9800;'>🍔 DỊCH VỤ ĐÃ ĐẶT</h3>"
            "<table border='1' style='border-color: #ddd;'>"
            "<tr>"
            "<th>Tên dịch vụ</th>"
            "<th>Đơn giá</th>"
            "<th>Số lượng</th>"
            "<th style='text-align: right;'>Thành tiền</th>"
            "</tr>"
            "%1"
            "</table>"
        ).arg(dichVuHTML);
    }
    
    // Tổng cộng
    html += QString(
        "<div class='total'>"
        "<p><b>Tổng tiền sân:</b> %L1 VNĐ</p>"
        "<p><b>Tổng tiền dịch vụ:</b> %L2 VNĐ</p>"
        "<p><b>Tổng cộng:</b> %L3 VNĐ</p>"
        "<p><b>Giảm giá (%4%%):</b> -%L5 VNĐ</p>"
        "</div>"
        "<div class='grand-total' style='text-align: right;'>"
        "<p style='font-size: 18px; color: #4CAF50;'>💰 TỔNG THANH TOÁN: %L6 VNĐ</p>"
        "</div>"
        "<p style='text-align: center; color: #757575; margin-top: 30px;'>"
        "Cảm ơn quý khách! Hẹn gặp lại! ⚽"
        "</p>"
        "</body>"
        "</html>"
    ).arg(tienSan, 0, 'f', 0)
     .arg(tongTienDichVu, 0, 'f', 0)
     .arg(tongTien, 0, 'f', 0)
     .arg(phanTramGiam, 0, 'f', 0)
     .arg(giamGia, 0, 'f', 0)
     .arg(thanhToan, 0, 'f', 0);
    
    return html;
}

void PaymentDialog::onXacNhanThanhToan()
{
    if (maLichDatDangChon.isEmpty())
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn lịch đặt cần thanh toán!");
        return;
    }
    
    auto reply = QMessageBox::question(this, "Xác nhận",
        "Bạn có chắc muốn thanh toán hóa đơn này?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply != QMessageBox::Yes)
        return;
    
    double tongTien, giamGia;
    if (quanLy->thanhToan(maLichDatDangChon.toStdString(), tongTien, giamGia))
    {
        QString msg = QString(
            "✅ Thanh toán thành công!\n\n"
            "💰 Tổng tiền: %L1 VNĐ\n"
            "🎁 Giảm giá: %L2 VNĐ\n"
            "💵 Khách trả: %L3 VNĐ\n\n"
            "Cảm ơn quý khách!"
        ).arg(tongTien + giamGia, 0, 'f', 0)
         .arg(giamGia, 0, 'f', 0)
         .arg(tongTien, 0, 'f', 0);
        
        QMessageBox::information(this, "Thành công", msg);
        
        // ✅ Emit signal để MainWindow refresh
        emit paymentCompleted();
        
        // Reload
        loadLichDatChuaThanhToan();
        txtHoaDon->clear();
        maLichDatDangChon = "";
        btnXacNhan->setEnabled(false);
        btnInHoaDon->setEnabled(false);
        btnLuuPDF->setEnabled(false);
        
        lblTongTien->setText("0 VNĐ");
        lblGiamGia->setText("0 VNĐ");
        lblThanhToan->setText("0 VNĐ");
    }
    else
    {
        QMessageBox::critical(this, "Lỗi", "Không thể thanh toán!\n\nVui lòng thử lại.");
    }
}

void PaymentDialog::onInHoaDon()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize::A4);
    
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QTextDocument doc;
        doc.setHtml(generateInvoiceHTML());
        doc.print(&printer);
        
        QMessageBox::information(this, "Thành công", "Đã gửi lệnh in hóa đơn!");
    }
}

void PaymentDialog::onLuuPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Lưu hóa đơn PDF",
        QString("HoaDon_%1.pdf").arg(maLichDatDangChon),
        "PDF Files (*.pdf)");
    
    if (fileName.isEmpty())
        return;
    
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    
    QTextDocument doc;
    doc.setHtml(generateInvoiceHTML());
    doc.print(&printer);
    
    QMessageBox::information(this, "Thành công", 
        QString("Đã lưu hóa đơn thành công!\n\nVị trí: %1").arg(fileName));
}
