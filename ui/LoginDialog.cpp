#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>

LoginDialog::LoginDialog(QuanLyThueSan *qlts, QWidget *parent)
    : QDialog(parent), quanLy(qlts)
{
    authManager = new AuthManager();
    setupUI();
    applyStyles();

    // Load background image từ thư mục resources (root dự án), không dùng Qt resource
    QString appDir = QCoreApplication::applicationDirPath();
    QList<QString> candidates = {
        // Khi chạy từ build/.../bin (Qt Creator): đi lên 3 cấp tới root dự án
        appDir + "/../../../resources/login_bg.jpg",
        // Khi đã copy resources vào cùng thư mục exe (bin)
        appDir + "/resources/login_bg.jpg",
        // Khi chạy từ VS Code với working dir là root dự án
        QStringLiteral("resources/login_bg.jpg"),
        // Fallback theo tên khác trong repo
        appDir + "/../../../resources/login_stadium.jpg",
        appDir + "/resources/login_stadium.jpg",
        QStringLiteral("resources/login_stadium.jpg"),
        // Fallback bổ sung: cùng thư mục app
        appDir + "/login_bg.jpg"};

    for (const QString &p : candidates)
    {
        if (QFile::exists(p))
        {
            if (backgroundImage.load(p))
            {
                // Background loaded successfully
                break;
            }
        }
    }

    // Silently handle if background not found
}

LoginDialog::~LoginDialog()
{
    delete authManager;
}

void LoginDialog::setupUI()
{
    setWindowTitle("Đăng Nhập - Hệ Thống Quản Lý Thuê Sân Bóng");
    setFixedSize(1000, 600); // Kích thước phù hợp hơn
    setModal(true);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Container widget để căn giữa form
    QWidget *centerWidget = new QWidget(this);
    centerWidget->setStyleSheet("background: transparent;");

    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setAlignment(Qt::AlignCenter);

    // Login form container với nền đen
    QWidget *formContainer = new QWidget(centerWidget);
    formContainer->setFixedWidth(380);
    formContainer->setStyleSheet(
        "background-color: rgba(0, 0, 0, 0.85);"
        "border-radius: 12px;"
        "padding: 20px;");

    QVBoxLayout *formLayout = new QVBoxLayout(formContainer);
    formLayout->setSpacing(20);
    formLayout->setContentsMargins(30, 40, 30, 40);

    // Title - Bỏ nền đen
    lblTitle = new QLabel("ĐĂNG NHẬP", formContainer);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "color: white;"
        "background: transparent;"
        "border: none;"
        "padding: 0;"
        "margin-bottom: 10px;");
    formLayout->addWidget(lblTitle);

    formLayout->addSpacing(10);

    // Role selection - Không có label
    cmbRole = new QComboBox(formContainer);
    cmbRole->addItem("ADMIN", ROLE_ADMIN);
    cmbRole->addItem("NHÂN VIÊN", ROLE_EMPLOYEE);
    cmbRole->setCurrentIndex(0);
    cmbRole->setMinimumHeight(45);
    formLayout->addWidget(cmbRole);

    // Username (ẩn mặc định với admin) - Không có label
    txtUsername = new QLineEdit(formContainer);
    txtUsername->setPlaceholderText("Tài khoản");
    txtUsername->setMinimumHeight(45);
    txtUsername->setVisible(false);
    formLayout->addWidget(txtUsername);

    // Password - Không có label
    txtPassword = new QLineEdit(formContainer);
    txtPassword->setPlaceholderText("Mật khẩu");
    txtPassword->setEchoMode(QLineEdit::Password);
    txtPassword->setMinimumHeight(45);
    formLayout->addWidget(txtPassword);

    formLayout->addSpacing(10);

    // Login button - BỎ dấu tích
    btnLogin = new QPushButton("Đăng Nhập", formContainer);
    btnLogin->setMinimumHeight(50);
    btnLogin->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(btnLogin);

    centerLayout->addWidget(formContainer);
    mainLayout->addWidget(centerWidget);

    // Connect signals
    connect(cmbRole, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LoginDialog::onRoleChanged);
    connect(btnLogin, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(txtPassword, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(txtUsername, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
}

void LoginDialog::applyStyles()
{
    // ComboBox style - Ẩn mũi tên (không hiển thị drop-down arrow)
    cmbRole->setStyleSheet(
        "QComboBox {"
        "    background-color: #2a2a2a;"
        "    color: white;"
        "    border: 1px solid #444;"
        "    border-radius: 6px;"
        "    padding: 12px 15px;" // padding đều, không chừa chỗ cho mũi tên
        "    padding-right: 15px;"
        "    font-size: 14px;"
        "}"
        "QComboBox:hover {"
        "    border: 1px solid #4CAF50;"
        "    background-color: #333;"
        "}"
        "QComboBox:focus {"
        "    border: 1px solid #4CAF50;"
        "}"
        "QComboBox::drop-down {"
        "    width: 0px;" // Ẩn hoàn toàn vùng drop-down
        "    border: none;"
        "    background: transparent;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    width: 0;"
        "    height: 0;"
        "    border: none;"
        "}"
        "QComboBox:hover::down-arrow { border: none; }"
        "QComboBox QAbstractItemView {"
        "    background-color: #2a2a2a;"
        "    color: white;"
        "    selection-background-color: #4CAF50;"
        "    border: 1px solid #444;"
        "    outline: none;"
        "}");

    // LineEdit style - Tối giản
    QString lineEditStyle =
        "QLineEdit {"
        "    background-color: #2a2a2a;"
        "    color: white;"
        "    border: 1px solid #444;"
        "    border-radius: 6px;"
        "    padding: 12px 15px;"
        "    font-size: 14px;"
        "}"
        "QLineEdit:hover {"
        "    border: 1px solid #4CAF50;"
        "    background-color: #333;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #4CAF50;"
        "    background-color: #333;"
        "}"
        "QLineEdit::placeholder {"
        "    color: #666;"
        "}";

    txtUsername->setStyleSheet(lineEditStyle);

    // Password field - Không có kẻ chia, border radius đầy đủ
    txtPassword->setStyleSheet(
        "QLineEdit {"
        "    background-color: #2a2a2a;"
        "    color: white;"
        "    border: 1px solid #444;"
        "    border-radius: 6px;"
        "    padding: 12px 15px;"
        "    font-size: 14px;"
        "}"
        "QLineEdit:hover {"
        "    border: 1px solid #4CAF50;"
        "    background-color: #333;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #4CAF50;"
        "    background-color: #333;"
        "}"
        "QLineEdit::placeholder {"
        "    color: #666;"
        "}");

    // Button style - Xanh lá, không có icon
    btnLogin->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 12px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}");
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (!backgroundImage.isNull())
    {
        // Vẽ ảnh nền gốc (không resize) - scale để fit
        QPixmap scaledBg = backgroundImage.scaled(
            size(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation);

        // Căn giữa ảnh
        int x = (width() - scaledBg.width()) / 2;
        int y = (height() - scaledBg.height()) / 2;
        painter.drawPixmap(x, y, scaledBg);
    }
    else
    {
        // Fallback: Gradient tối
        QLinearGradient gradient(0, 0, 0, height());
        gradient.setColorAt(0, QColor(20, 20, 20));
        gradient.setColorAt(1, QColor(40, 40, 40));
        painter.fillRect(rect(), gradient);
    }

    QDialog::paintEvent(event);
}

void LoginDialog::onRoleChanged(int index)
{
    UserRole role = static_cast<UserRole>(cmbRole->itemData(index).toInt());

    if (role == ROLE_ADMIN)
    {
        // Admin chỉ cần mật khẩu
        txtUsername->setVisible(false);
        txtUsername->clear();
    }
    else
    {
        // Nhân viên cần cả tài khoản và mật khẩu
        txtUsername->setVisible(true);
    }
}

void LoginDialog::onLoginClicked()
{
    UserRole role = static_cast<UserRole>(cmbRole->currentData().toInt());
    QString password = txtPassword->text().trimmed();

    if (password.isEmpty())
    {
        QMessageBox::warning(this, "Thiếu thông tin",
                             "Vui lòng nhập mật khẩu!");
        txtPassword->setFocus();
        return;
    }

    bool success = false;

    if (role == ROLE_ADMIN)
    {
        // Đăng nhập với quyền Admin
        success = authManager->loginAsAdmin(password.toStdString());

        if (!success)
        {
            QMessageBox::critical(this, "Đăng nhập thất bại",
                                  "Mật khẩu Admin không đúng!\n\n"
                                  "Mật khẩu mặc định: 123");
            txtPassword->clear();
            txtPassword->setFocus();
            return;
        }
    }
    else
    {
        // Đăng nhập với tài khoản nhân viên
        QString username = txtUsername->text().trimmed();

        if (username.isEmpty())
        {
            QMessageBox::warning(this, "Thiếu thông tin",
                                 "Vui lòng nhập tài khoản!");
            txtUsername->setFocus();
            return;
        }

        success = authManager->loginAsEmployee(username.toStdString(),
                                               password.toStdString(),
                                               quanLy);

        if (!success)
        {
            QMessageBox::critical(this, "Đăng nhập thất bại",
                                  "Tài khoản hoặc mật khẩu không đúng!\n"
                                  "Hoặc tài khoản đã nghỉ việc.");
            txtPassword->clear();
            txtPassword->setFocus();
            return;
        }
    }

    // Đăng nhập thành công
    QString roleText = (role == ROLE_ADMIN) ? "ADMIN (Chủ)" : "Nhân viên";
    QMessageBox::information(this, "Đăng nhập thành công",
                             QString("Chào mừng %1!\n\nVai trò: %2")
                                 .arg(QString::fromStdString(authManager->getCurrentUser()))
                                 .arg(roleText));

    accept(); // Đóng dialog và trả về Accepted
}
