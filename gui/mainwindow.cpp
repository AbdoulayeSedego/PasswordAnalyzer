#include "mainwindow.h"                    // Your GUI header (declarations)
#include "ui_mainwindow.h"                 // Generated from mainwindow.ui (auto via AUTOUIC)
#include <QDebug>                          // For debug output
#include <fstream>                         // For dict loading
#include <unordered_set>
#include <QMessageBox>
#include "../common/PasswordAnalyzer.h"    // Shared core (generatePassword, etc.)

// External global from common
extern std::unordered_set<std::string> weakPasswords;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)  // Full type now available from ui_mainwindow.h
{
    ui->setupUi(this);  // Load the .ui file (widgets, layout)—full type available

    setWindowTitle("Password Strength Analyzer GUI");
    resize(500, 400);   // Initial size for fields/labels

    // Load shared dict once (mirrors CLI—ensures O(1) checks)
    std::ifstream dict("../common_passwords.txt");  // Relative to gui/
    if (dict.is_open()) {
        std::string badPass;
        while (std::getline(dict, badPass)) {
            weakPasswords.insert(badPass);
        }
        dict.close();
        qDebug() << "Dict loaded in GUI:" << weakPasswords.size() << "entries.";
    } else {
        qDebug() << "Warning: Dict missing in GUI.";
    }

    // Test shared func (remove after Step 1)
    qDebug() << "GUI ready—shared gen example:" << generatePassword(16).c_str();
    connect(ui->analyzeButton, &QPushButton::clicked, this, &MainWindow::onAnalyzeButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;  // Safe now—full type defined
}

// Slot: Connected to analyzeButton clicked signal
void MainWindow::onAnalyzeButtonClicked()
{
    qDebug() << "Analyze clicked (calls analyzePassword)";
    QString input = ui->passwordLineEdit->text();
    if (input.isEmpty()) {
        ui->scoreLabel->setText("Enter a password first!");
        QMessageBox::warning(this, "Input Error", "Please enter a password.");  // Popup alert
        return;
    }

    std::string password = input.toStdString();
    int score = analyzePassword(password);  // Console output

    ui->scoreLabel->setText(QString("Score: %1/100").arg(score));
    if (score < 50) {
        ui->scoreLabel->setStyleSheet("color: red; font-weight: bold;");
        QMessageBox::warning(this, "Weak Password", "Vulnerable—use generator!");  // Alert
    } else if (score < 80) {
        ui->scoreLabel->setStyleSheet("color: orange;");
    } else {
        ui->scoreLabel->setStyleSheet("color: green; font-weight: bold;");
        QMessageBox::information(this, "Strong Password", "Resists cracking!");  // Alert
    }
}

void MainWindow::onGenerateButtonClicked()
{
    qDebug() << "Generate clicked (TBD—calls generatePassword)";
    // Step 3: Call generate, set to QLineEdit or QLabel TODO
}
