#include "NhanVienDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>

NhanVienDialog::NhanVienDialog(QuanLyThueSan *ql, QWidget *parent, NhanVien *nv)
    : QDialog(parent), quanLy(ql)
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
    else
    {
        // ✅ Auto-generate mã nhân viên mới
        QString suggestedMa = generateMaNV();
        txtMaNV->setText(suggestedMa);
        txtMaNV->setPlaceholderText("VD: NV001");
        
        // ✅ Gợi ý mã
        lblSuggestion = new QLabel(QString("💡 Gợi ý: <b>%1</b>").arg(suggestedMa));
        lblSuggestion->setStyleSheet("color: #1976D2; font-size: 12px;");
    }

    formLayout->addRow("📋 Mã Nhân Viên:", txtMaNV);
    if (!nv && lblSuggestion)
        formLayout->addRow("", lblSuggestion);
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

QString NhanVienDialog::generateMaNV()
{
    const MangDong<NhanVien> &dsNV = quanLy->getDsNhanVien();
    int maxNum = 0;

    for (int i = 0; i < dsNV.getKichThuoc(); i++)
    {
        QString maNV = QString::fromStdString(dsNV[i].getMaNV());
        if (maNV.startsWith("NV"))
        {
            bool ok;
            int num = maNV.mid(2).toInt(&ok);
            if (ok && num > maxNum)
                maxNum = num;
        }
    }

    return QString("NV%1").arg(maxNum + 1, 3, 10, QChar('0'));
}
