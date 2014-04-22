#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Component;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void makeActive(Component *component);
    void nextTick(const char *slot, bool force = false);

private slots:
    void fetch1();
    void fetch2();
    void fetch3();
    void fetch4();

    void decode1();

    void executeLoad1();
    void executeLoad2();
    void executeLoad3();

    void executeAdd1();
    void executeAdd2();
    void executeAdd3();
    void executeAdd4();

    void executeSub1();
    void executeSub2();
    void executeSub3();
    void executeSub4();

    void executeBrz1();
    void executeBrz2();

    void executeBrn1();
    void executeBrn2();

    void executeStore1();
    void executeStore2();
    void executeStore3();

    void executeJump1();

    void executeStop1();

    void on_actionLoadProgram_triggered();
    void on_actionStop_triggered();
    void on_actionPlay_triggered();

    void on_actionStep_triggered();

    void on_actionPause_triggered();

    void on_actionInstructions_triggered();

    void on_actionReloadCode_triggered();

private:
    Ui::MainWindow *ui;
    class MainWindowPrivate *d;
};

#endif // MAINWINDOW_H
