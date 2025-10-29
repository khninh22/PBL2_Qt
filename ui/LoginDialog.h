#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include "../core/AuthManager.h"
#include "../core/QuanLyThueSan.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QuanLyThueSan *qlts, QWidget *parent = nullptr);
    ~LoginDialog();

    AuthManager *getAuthManager() const { return authManager; }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onLoginClicked();
    void onRoleChanged(int index);

private:
    void setupUI();
    void applyStyles();

    QuanLyThueSan *quanLy;
    AuthManager *authManager;

    // UI Components
    QComboBox *cmbRole;
    QLineEdit *txtUsername;
    QLineEdit *txtPassword;
    QPushButton *btnLogin;
    QLabel *lblTitle;

    // Background
    QPixmap backgroundImage;
};

#endif // LOGINDIALOG_H
