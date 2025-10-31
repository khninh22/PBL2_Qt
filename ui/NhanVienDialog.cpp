#include "NhanVienDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>

NhanVienDialog::NhanVienDialog(QWidget *parent, NhanVien *nv)
    : QDialog(parent)
{
    setWindowTitle(nv ? "Sửa Nhân Viên" : "Thêm Nhân Viên Mới");
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
        "QLineEdit, QComboBox {"
        "    padding: 8px; border: 2px solid #BDBDBD; border-radius: 4px;"
        "    background-color: white; font-size: 13px;"
        "}"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #2196F3; }"
    );

    QFormLayout *formLayout = new QFormLayout();

    txtMaNV = new QLineEdit();
    txtHoTen = new QLineEdit();
    txtSdt = new QLineEdit();
    cboViTri = new QComboBox();
    spnLuong = new QDoubleSpinBox();
    txtTaiKhoan = new QLineEdit();
    txtMatKhau = new QLineEdit();

    cboViTri->addItems({"Quản Lý", "Thu Ngân", "Bảo Vệ", "Bảo Trì"});
    spnLuong->setRange(0, 100000000);
    spnLuong->setSuffix(" VNĐ");
    spnLuong->setDecimals(0);
    spnLuong->setValue(5000000);

    txtMatKhau->setEchoMode(QLineEdit::Password);

    if (nv)
    {
        txtMaNV->setText(QString::fromStdString(nv->getMaNV()));
        txtMaNV->setReadOnly(true);
        txtHoTen->setText(QString::fromStdString(nv->getHoTen()));
        txtSdt->setText(QString::fromStdString(nv->getSdt()));
        cboViTri->setCurrentText(QString::fromStdString(nv->getViTri()));
        spnLuong->setValue(nv->getLuongCoBan());
        txtTaiKhoan->setText(QString::fromStdString(nv->getTaiKhoan()));
        txtMatKhau->setText(QString::fromStdString(nv->getMatKhau()));
    }

    formLayout->addRow("📋 Mã Nhân Viên:", txtMaNV);
    formLayout->addRow("👤 Họ Tên:", txtHoTen);
    formLayout->addRow("📞 Số Điện Thoại:", txtSdt);
    formLayout->addRow("💼 Vị Trí:", cboViTri);
    formLayout->addRow("💰 Lương Cơ Bản:", spnLuong);
    formLayout->addRow("📧 Tài Khoản:", txtTaiKhoan);
    formLayout->addRow("🔒 Mật Khẩu:", txtMatKhau);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}
