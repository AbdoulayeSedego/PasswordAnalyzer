//
// Created by Abdoulaye Sedego on 10/6/25.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../common/PasswordAnalyzer.h"  // Shared core

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }  // UI class (capital 'M')
QT_END_NAMESPACE

class MainWindow : public QMainWindow  // Capital 'M' class name
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAnalyzeButtonClicked();
    void onGenerateButtonClicked();

private:
    Ui::MainWindow *ui;  // Matches UI namespace
};
#endif // MAINWINDOW_H