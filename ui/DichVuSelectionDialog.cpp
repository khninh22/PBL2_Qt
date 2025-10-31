#include "DichVuSelectionDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include <QFont>

DichVuSelectionDialog::DichVuSelectionDialog(QuanLyThueSan *quanLy, QWidget *parent)
    : QDialog(parent), quanLy(quanLy), tongTien(0.0)
{
    setWindowTitle("🎯 Chọn Dịch Vụ");
    setMinimumSize(700, 500);

    setupUI();
    loadDichVu();
}

void DichVuSelectionDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ============== HEADER ==============
    QLabel *lblTitle = new QLabel("🎯 CHỌN DỊCH VỤ CHO LỊCH ĐẶT SÂN");
    QFont titleFont = lblTitle->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    lblTitle->setFont(titleFont);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("color: #1976D2; padding: 10px;");
    mainLayout->addWidget(lblTitle);

    // ============== HƯỚNG DẪN ==============
    QLabel *lblGuide = new QLabel("📋 Chọn dịch vụ cần sử dụng và nhập số lượng. Bạn có thể bỏ qua nếu không cần.");
    lblGuide->setWordWrap(true);
    lblGuide->setStyleSheet("color: #555; padding: 5px; background-color: #E3F2FD; border-radius: 5px;");
    mainLayout->addWidget(lblGuide);

    // ============== TABLE DỊCH VỤ ==============
    QGroupBox *groupTable = new QGroupBox("📋 Danh sách dịch vụ khả dụng");
    QVBoxLayout *groupLayout = new QVBoxLayout(groupTable);

    tableDichVu = new QTableWidget(0, 5);
    tableDichVu->setHorizontalHeaderLabels({"Chọn", "Mã DV", "Tên dịch vụ", "Đơn giá", "Số lượng"});
    tableDichVu->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableDichVu->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableDichVu->setSelectionMode(QAbstractItemView::SingleSelection);
    tableDichVu->setAlternatingRowColors(true);
    tableDichVu->verticalHeader()->setVisible(false);

    // Set column widths
    tableDichVu->setColumnWidth(0, 60);  // Checkbox
    tableDichVu->setColumnWidth(1, 100); // Mã DV
    tableDichVu->setColumnWidth(2, 250); // Tên
    tableDichVu->setColumnWidth(3, 120); // Giá
    tableDichVu->setColumnWidth(4, 100); // Số lượng

    tableDichVu->horizontalHeader()->setStretchLastSection(false);
    tableDichVu->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    groupLayout->addWidget(tableDichVu);
    mainLayout->addWidget(groupTable);

    // ============== TỔNG TIỀN ==============
    QFrame *frameTongTien = new QFrame();
    frameTongTien->setFrameStyle(QFrame::Box | QFrame::Raised);
    frameTongTien->setStyleSheet("background-color: #FFF9C4; border: 2px solid #FBC02D; border-radius: 5px;");

    QHBoxLayout *layoutTongTien = new QHBoxLayout(frameTongTien);

    QLabel *lblTongTienLabel = new QLabel("💰 TỔNG TIỀN DỊCH VỤ:");
    QFont fontLabel = lblTongTienLabel->font();
    fontLabel.setPointSize(11);
    fontLabel.setBold(true);
    lblTongTienLabel->setFont(fontLabel);

    lblTongTien = new QLabel("0 VNĐ");
    QFont fontValue = lblTongTien->font();
    fontValue.setPointSize(12);
    fontValue.setBold(true);
    lblTongTien->setFont(fontValue);
    lblTongTien->setStyleSheet("color: #D32F2F;");

    layoutTongTien->addWidget(lblTongTienLabel);
    layoutTongTien->addWidget(lblTongTien);
    layoutTongTien->addStretch();

    mainLayout->addWidget(frameTongTien);

    // ============== LƯU Ý ==============
    QLabel *lblNote = new QLabel("💡 Lưu ý: Bạn có thể bỏ qua bước này nếu không cần sử dụng dịch vụ.");
    lblNote->setWordWrap(true);
    lblNote->setStyleSheet("color: #666; font-style: italic; padding: 5px;");
    mainLayout->addWidget(lblNote);

    // ============== BUTTONS ==============
    QHBoxLayout *layoutButtons = new QHBoxLayout();
    layoutButtons->addStretch();

    btnSkip = new QPushButton("🚫 Bỏ qua");
    btnSkip->setMinimumSize(120, 35);
    btnSkip->setStyleSheet(
        "QPushButton {"
        "   background-color: #9E9E9E;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #757575;"
        "}");
    connect(btnSkip, &QPushButton::clicked, this, &DichVuSelectionDialog::onSkip);
    layoutButtons->addWidget(btnSkip);

    btnConfirm = new QPushButton("✅ Xác nhận");
    btnConfirm->setMinimumSize(120, 35);
    btnConfirm->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}");
    connect(btnConfirm, &QPushButton::clicked, this, &DichVuSelectionDialog::onConfirm);
    layoutButtons->addWidget(btnConfirm);

    mainLayout->addLayout(layoutButtons);
}

void DichVuSelectionDialog::loadDichVu()
{
    dsDichVu = quanLy->getDsDichVu();

    if (dsDichVu.getKichThuoc() == 0)
    {
        QMessageBox::information(this, "Thông báo",
                                 "Không có dịch vụ nào trong hệ thống.\nBạn có thể bỏ qua bước này.");
        return;
    }

    tableDichVu->setRowCount(dsDichVu.getKichThuoc());

    for (int i = 0; i < dsDichVu.getKichThuoc(); i++)
    {
        const DichVu &dv = dsDichVu[i];

        // Column 0: Checkbox
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setStyleSheet("margin-left: 20px;");
        connect(checkBox, &QCheckBox::checkStateChanged, this, [this, i](Qt::CheckState state)
                {
            // Khi checkbox thay đổi, update spinbox
            QSpinBox *spinBox = qobject_cast<QSpinBox*>(tableDichVu->cellWidget(i, 4));
            if (spinBox)
            {
                if (state == Qt::Checked)
                {
                    if (spinBox->value() == 0)
                        spinBox->setValue(1); // Mặc định số lượng = 1
                }
                else
                {
                    spinBox->setValue(0); // Bỏ chọn = 0
                }
            }
            updateTongTien(); });
        tableDichVu->setCellWidget(i, 0, checkBox);

        // Column 1: Mã DV
        QTableWidgetItem *itemMaDV = new QTableWidgetItem(QString::fromStdString(dv.getMaDV()));
        itemMaDV->setTextAlignment(Qt::AlignCenter);
        tableDichVu->setItem(i, 1, itemMaDV);

        // Column 2: Tên DV
        QTableWidgetItem *itemTenDV = new QTableWidgetItem(QString::fromStdString(dv.getTenDV()));
        tableDichVu->setItem(i, 2, itemTenDV);

        // Column 3: Đơn giá
        QTableWidgetItem *itemGia = new QTableWidgetItem(QString("%L1 VNĐ").arg(dv.getGiaDV(), 0, 'f', 0));
        itemGia->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tableDichVu->setItem(i, 3, itemGia);

        // Column 4: Spinbox số lượng
        QSpinBox *spinBox = new QSpinBox();
        spinBox->setMinimum(0);
        spinBox->setMaximum(100);
        spinBox->setValue(0);
        spinBox->setAlignment(Qt::AlignCenter);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, i, checkBox](int value)
                {
            // Đồng bộ checkbox
            if (value > 0)
            {
                checkBox->setChecked(true);
            }
            else
            {
                checkBox->setChecked(false);
            }
            updateTongTien(); });
        tableDichVu->setCellWidget(i, 4, spinBox);
    }

    updateTongTien();
}

void DichVuSelectionDialog::updateTongTien()
{
    tongTien = 0.0;
    selectedDichVu.clear();

    for (int i = 0; i < tableDichVu->rowCount(); i++)
    {
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(tableDichVu->cellWidget(i, 0));
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(tableDichVu->cellWidget(i, 4));

        if (checkBox && checkBox->isChecked() && spinBox && spinBox->value() > 0)
        {
            const DichVu &dv = dsDichVu[i];
            int soLuong = spinBox->value();
            double thanhTien = dv.getGiaDV() * soLuong;

            selectedDichVu.append(DichVuSelected(
                dv.getMaDV(),
                dv.getTenDV(),
                dv.getGiaDV(),
                soLuong));

            tongTien += thanhTien;
        }
    }

    lblTongTien->setText(QString("%L1 VNĐ").arg(tongTien, 0, 'f', 0));
}

void DichVuSelectionDialog::onConfirm()
{
    // Có thể xác nhận với 0 dịch vụ (không bắt buộc)
    if (selectedDichVu.isEmpty())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "❓ Xác nhận",
            "Bạn chưa chọn dịch vụ nào.\n\nBạn có muốn tiếp tục không chọn dịch vụ?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No)
        {
            return; // Quay lại chọn dịch vụ
        }
    }

    accept();
}

void DichVuSelectionDialog::onSkip()
{
    // Bỏ qua = không chọn dịch vụ nào
    selectedDichVu.clear();
    tongTien = 0.0;
    accept();
}

QList<DichVuSelected> DichVuSelectionDialog::getSelectedDichVu() const
{
    return selectedDichVu;
}

double DichVuSelectionDialog::getTongTienDichVu() const
{
    return tongTien;
}

void DichVuSelectionDialog::onTableItemChanged(int row, int column)
{
    // Handle table item changes if needed
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void DichVuSelectionDialog::onSpinBoxValueChanged(int value)
{
    // Handle spinbox changes if needed
    Q_UNUSED(value);
}
