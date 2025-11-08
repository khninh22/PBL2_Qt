#include "RestoreDialog.h"
#include <QMessageBox>
#include <QDateTime>

RestoreDialog::RestoreDialog(QuanLyThueSan *quanLy, QWidget *parent)
    : QDialog(parent), quanLy(quanLy)
{
    setupUI();
    loadBackupList();
}

void RestoreDialog::setupUI()
{
    setWindowTitle("Khôi Phục Dữ Liệu");
    setModal(true);
    setMinimumSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Info label
    lblInfo = new QLabel("Chọn bản sao lưu để khôi phục:");
    lblInfo->setStyleSheet("font-weight: bold; font-size: 14px; color: #2c3e50;");
    mainLayout->addWidget(lblInfo);

    // List widget
    listBackups = new QListWidget();
    listBackups->setStyleSheet(
        "QListWidget {"
        "    border: 2px solid #3498db;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "    background-color: white;"
        "    font-size: 12px;"
        "}"
        "QListWidget::item {"
        "    padding: 8px;"
        "    border-bottom: 1px solid #ecf0f1;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #3498db;"
        "    color: white;"
        "}");
    mainLayout->addWidget(listBackups);

    // Button layout
    QHBoxLayout *btnLayout = new QHBoxLayout();

    btnRefresh = new QPushButton("🔄 Làm Mới");
    btnRefresh->setStyleSheet(
        "QPushButton {"
        "    background-color: #95a5a6;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #7f8c8d;"
        "}");
    connect(btnRefresh, &QPushButton::clicked, this, &RestoreDialog::onRefresh);

    btnRestore = new QPushButton("✅ Khôi Phục");
    btnRestore->setStyleSheet(
        "QPushButton {"
        "    background-color: #27ae60;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #229954;"
        "}");
    btnRestore->setEnabled(false);
    connect(btnRestore, &QPushButton::clicked, this, &RestoreDialog::onRestore);
    connect(listBackups, &QListWidget::itemSelectionChanged, this, [this]() {
        btnRestore->setEnabled(listBackups->currentRow() >= 0);
    });

    btnCancel = new QPushButton("❌ Hủy");
    btnCancel->setStyleSheet(
        "QPushButton {"
        "    background-color: #e74c3c;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c0392b;"
        "}");
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    btnLayout->addWidget(btnRefresh);
    btnLayout->addStretch();
    btnLayout->addWidget(btnRestore);
    btnLayout->addWidget(btnCancel);

    mainLayout->addLayout(btnLayout);
}

void RestoreDialog::loadBackupList()
{
    listBackups->clear();
    
    if (!quanLy)
        return;

    auto backupList = quanLy->layDanhSachBackup();
    
    if (backupList.getKichThuoc() == 0)
    {
        listBackups->addItem("⚠️ Không có bản sao lưu nào");
        lblInfo->setText("Không tìm thấy bản sao lưu. Hãy tạo backup trước.");
        lblInfo->setStyleSheet("font-weight: bold; font-size: 14px; color: #e74c3c;");
        return;
    }

    lblInfo->setText(QString("Có %1 bản sao lưu. Chọn để khôi phục:").arg(backupList.getKichThuoc()));
    lblInfo->setStyleSheet("font-weight: bold; font-size: 14px; color: #27ae60;");

    for (int i = 0; i < backupList.getKichThuoc(); i++)
    {
        listBackups->addItem(QString::fromStdString("💾 " + backupList[i]));
    }
}

void RestoreDialog::onRefresh()
{
    loadBackupList();
}

void RestoreDialog::onRestore()
{
    int row = listBackups->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn bản sao lưu!");
        return;
    }

    QString itemText = listBackups->currentItem()->text();
    selectedBackup = itemText.mid(3); // Remove "💾 " prefix

    auto reply = QMessageBox::question(
        this,
        "Xác Nhận Khôi Phục",
        QString("Bạn có chắc muốn khôi phục dữ liệu từ:\n%1\n\n"
                "⚠️ Cảnh báo: Dữ liệu hiện tại sẽ bị ghi đè!")
            .arg(selectedBackup),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->khoiPhucDuLieu(selectedBackup.toStdString()))
        {
            QMessageBox::information(this, "Thành Công",
                                     "✅ Khôi phục dữ liệu thành công!\n\n"
                                     "Vui lòng khởi động lại ứng dụng để cập nhật.");
            accept();
        }
        else
        {
            QMessageBox::critical(this, "Lỗi",
                                  "❌ Khôi phục thất bại!\n\n"
                                  "File backup có thể bị hỏng hoặc không tồn tại.");
        }
    }
}

QString RestoreDialog::getSelectedBackupName() const
{
    return selectedBackup;
}
