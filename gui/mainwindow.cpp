#include "mainwindow.h"                    // Your GUI header (declarations)
#include "ui_mainwindow.h"                 // Generated from mainwindow.ui (auto via AUTOUIC)
#include <QDebug>                          // For debug output
#include <fstream>                         // For dict loading
#include <unordered_set>                   // For weakPasswords (shared)
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
}

MainWindow::~MainWindow()
{
    delete ui;  // Safe now—full type defined
}

void MainWindow::onAnalyzeButtonClicked()
{
    qDebug() << "Analyze clicked (TBD—calls analyzePassword)";
    // Step 3: Get text from QLineEdit, call analyzePassword, display in QLabel
}

void MainWindow::onGenerateButtonClicked()
{
    qDebug() << "Generate clicked (TBD—calls generatePassword)";
    // Step 3: Call generate, set to QLineEdit or QLabel
}