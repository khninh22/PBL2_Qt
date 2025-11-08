#include "KhachHangDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>

KhachHangDialog::KhachHangDialog(QuanLyThueSan *ql, QWidget *parent, KhachHang *kh)
    : QDialog(parent), quanLy(ql)
{
    setWindowTitle(kh ? "Sửa Khách Hàng" : "Thêm Khách Hàng Mới");
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
        "QLineEdit {"
        "    padding: 8px; border: 2px solid #BDBDBD; border-radius: 4px;"
        "    background-color: white; font-size: 13px;"
        "}"
        "QLineEdit:focus { border: 2px solid #2196F3; }"
    );

    QFormLayout *formLayout = new QFormLayout();

    txtMaKH = new QLineEdit();
    txtHoTen = new QLineEdit();
    txtSdt = new QLineEdit();

    if (kh)
    {
        txtMaKH->setText(QString::fromStdString(kh->getMaKH()));
        txtMaKH->setReadOnly(true);
        txtHoTen->setText(QString::fromStdString(kh->getHoTen()));
        txtSdt->setText(QString::fromStdString(kh->getSdt()));
    }
    else
    {
        // ✅ Auto-generate mã khách hàng mới
        QString suggestedMa = generateMaKH();
        txtMaKH->setText(suggestedMa);
        txtMaKH->setPlaceholderText("VD: KH001");
        
        // ✅ Gợi ý mã
        lblSuggestion = new QLabel(QString("💡 Gợi ý: <b>%1</b>").arg(suggestedMa));
        lblSuggestion->setStyleSheet("color: #1976D2; font-size: 12px;");
    }

    formLayout->addRow("Mã Khách Hàng:", txtMaKH);
    if (!kh && lblSuggestion)
        formLayout->addRow("", lblSuggestion);
    formLayout->addRow("Họ Tên:", txtHoTen);
    formLayout->addRow("Số Điện Thoại:", txtSdt);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}

QString KhachHangDialog::generateMaKH()
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
