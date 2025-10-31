#include "SanBongDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>

SanBongDialog::SanBongDialog(QWidget *parent, SanBong *san)
    : QDialog(parent)
{
    setWindowTitle(san ? "Sửa Sân Bóng" : "Thêm Sân Bóng Mới");
    setMinimumWidth(400);
    
    // ✅ Cải thiện UI
    setStyleSheet(
        "QDialog { background-color: #f5f5f5; }"
        "QPushButton {"
        "    background-color: #2196F3; color: white; border: none;"
        "    padding: 10px 24px; border-radius: 5px; font-weight: bold;"
        "    font-size: 13px; min-width: 100px;"
        "}"
        "QPushButton:hover { background-color: #1976D2; }"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {"
        "    padding: 8px; border: 2px solid #BDBDBD; border-radius: 4px;"
        "    background-color: white; font-size: 13px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {"
        "    border: 2px solid #2196F3;"
        "}"
    );

    QFormLayout *formLayout = new QFormLayout();

    txtMaSan = new QLineEdit();
    txtTenSan = new QLineEdit();
    cboLoaiSan = new QComboBox();
    spnGiaThue = new QDoubleSpinBox();

    cboLoaiSan->addItems({"5", "7", "11"});
    spnGiaThue->setRange(0, 10000000);
    spnGiaThue->setSuffix(" VNĐ");
    spnGiaThue->setDecimals(0);

    if (san)
    {
        txtMaSan->setText(QString::fromStdString(san->getMaSan()));
        txtMaSan->setReadOnly(true);
        txtTenSan->setText(QString::fromStdString(san->getTenSan()));
        cboLoaiSan->setCurrentText(QString::number(san->getLoaiSan()));
        spnGiaThue->setValue(san->getGiaThue());
    }

    formLayout->addRow("Mã Sân:", txtMaSan);
    formLayout->addRow("Tên Sân:", txtTenSan);
    formLayout->addRow("Loại Sân:", cboLoaiSan);
    formLayout->addRow("Giá Thuê:", spnGiaThue);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}
