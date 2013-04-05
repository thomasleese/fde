#include <QTimer>
#include <QDesktopWidget>
#include <QMessageBox>

#include "instructionswidget.h"
#include "component.h"
#include "loadprogramdialogue.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    mALU = new Component("Arithmetic Logic Unit",           0,      0,      0);
    mCIR = new Component("Current Instruction Register",    110,    0,      50);
    mPC = new Component("Program Counter",                  220,    0,      100);
    mAccumulator = new Component("Accumulator",             0,      110,    150);
    mCU = new Component("Control Unit",                     110,    110,    200);
    mMAR = new Component("Memory Address Register",         220,    110,    250);
    mMDR = new Component("Memory Data Register",            0,      220,    300);
    
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(mALU);
    scene->addItem(mCIR);
    scene->addItem(mPC);
    scene->addItem(mAccumulator);
    scene->addItem(mCU);
    scene->addItem(mMAR);
    scene->addItem(mMDR);
    
    ui->graphicsView->setScene(scene);
        
    on_actionStop_triggered();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::makeActive(Component *component) {
    mALU->setActive(false);
    mCIR->setActive(false);
    mPC->setActive(false);
    mAccumulator->setActive(false);
    mCU->setActive(false);
    mMAR->setActive(false);
    mMDR->setActive(false);
    
    if (component != 0) {
        component->setActive(true);
    }
}

void MainWindow::nextTick(const char *slot, bool force) {
    mCurrentTick = (char *) slot;
    
    if (force) {
        mTick--; // it gets ++ later
        QTimer::singleShot(0, this, slot);
    } else if (mContinous) {
        int timeout = ui->sliderTimePerTick->value();
        QTimer::singleShot(timeout, this, slot);
    }
    
    mTick++;
    ui->lblTick->setText(QString::number(mTick));
}

void MainWindow::fetch1() {
    ui->lblStatus->setText("Fetching");
    
    makeActive(mPC);
    
    nextTick(SLOT(fetch2()));
}

void MainWindow::fetch2() {
    makeActive(mMAR);
    mMAR->setRawValue(mPC->getRawValue());
    
    nextTick(SLOT(fetch3()));
}

void MainWindow::fetch3() {
    makeActive(mMDR);
    mMDR->setRawValue(ui->tblMemory->item(mMAR->getHexValue(), 0)->text());
    
    nextTick(SLOT(fetch4()));
}

void MainWindow::fetch4() {
    makeActive(mCIR);
    mCIR->setRawValue(mMDR->getRawValue());
    
    nextTick(SLOT(decode1()));
}

void MainWindow::decode1() {
    ui->lblStatus->setText("Decoding");
    
    makeActive(mCU);
    mCU->setRawValue(mCIR->getRawValue());
    
    QStringList instruction = mCU->getRawValue().split(QRegExp("\\s+"));
    QString op = instruction[0].toLower();
    
    if (instruction.length() >= 2) {
        mOperand = instruction[1];
    }
    
    if (op == "load" || op == "lod") {
        nextTick(SLOT(executeLoad1()));
    } else if (op == "add") {
        nextTick(SLOT(executeAdd1()));
    } else if (op == "sub") {
        nextTick(SLOT(executeSub1()));
    } else if (op == "store" || op == "sto") {
        nextTick(SLOT(executeStore1()));
    } else if (op == "brz") {
        nextTick(SLOT(executeBrz1()));
    } else if (op == "brn") {
        nextTick(SLOT(executeBrn1()));
    } else if (op == "jump" || op == "jmp") {
        nextTick(SLOT(executeJump1()));
    } else if (op == "stop" || op == "stp") {
        nextTick(SLOT(executeStop1()));
    } else {
        QMessageBox::critical(this, "Invalid Instruction", op + " is not a recognised operator! Execution stopped.");
        on_actionStop_triggered();
    }
}

void MainWindow::executeLoad1() {
    ui->lblStatus->setText("Executing");
    
    makeActive(mMAR);
    mMAR->setRawValue(mOperand);
    
    nextTick(SLOT(executeLoad2()));
}

void MainWindow::executeLoad2() {
    makeActive(mMDR);
    mMDR->setRawValue(ui->tblMemory->item(mMAR->getHexValue(), 0)->text());
    
    nextTick(SLOT(executeLoad3()));
}

void MainWindow::executeLoad3() {
    makeActive(mAccumulator);
    mAccumulator->setRawValue(mMDR->getRawValue());
    
    mPC->setHexValue(mPC->getHexValue() + 1);
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeAdd1() {
    ui->lblStatus->setText("Executing");
    
    makeActive(mMAR);
    mMAR->setRawValue(mOperand);
    
    nextTick(SLOT(executeAdd2()));
}

void MainWindow::executeAdd2() {
    makeActive(mMDR);
    mMDR->setRawValue(ui->tblMemory->item(mMAR->getHexValue(), 0)->text());
    
    nextTick(SLOT(executeAdd3()));
}

void MainWindow::executeAdd3() {
    makeActive(mALU);
    mALU->setDecValue(mAccumulator->getDecValue() + mMDR->getDecValue());
    
    nextTick(SLOT(executeAdd4()));
}

void MainWindow::executeAdd4() {
    makeActive(mAccumulator);
    mAccumulator->setRawValue(mALU->getRawValue());
    mALU->setRawValue("NULL");
    
    mPC->setHexValue(mPC->getHexValue() + 1);
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeSub1() {
    ui->lblStatus->setText("Executing");
    
    makeActive(mMAR);
    mMAR->setRawValue(mOperand);
    
    nextTick(SLOT(executeSub2()));
}

void MainWindow::executeSub2() {
    makeActive(mMDR);
    mMDR->setRawValue(ui->tblMemory->item(mMAR->getHexValue(), 0)->text());
    
    nextTick(SLOT(executeSub3()));
}

void MainWindow::executeSub3() {
    makeActive(mALU);
    mALU->setDecValue(mAccumulator->getDecValue() - mMDR->getDecValue());
    
    nextTick(SLOT(executeSub4()));
}

void MainWindow::executeSub4() {
    makeActive(mAccumulator);
    mAccumulator->setRawValue(mALU->getRawValue());
    mALU->setRawValue("NULL");
    
    mPC->setHexValue(mPC->getHexValue() + 1);
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeBrz1() {
    ui->lblStatus->setText("Executing");
    
    makeActive(mMAR);
    mMAR->setRawValue(mOperand);
    
    nextTick(SLOT(executeBrz2()));
}

void MainWindow::executeBrz2() {
    makeActive(mAccumulator);
    if (mAccumulator->getDecValue() == 0) {
        mPC->setHexValue(mMAR->getHexValue());
    } else {
        mPC->setHexValue(mPC->getHexValue() + 1);
    }
    
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeBrn1() {
    ui->lblStatus->setText("Executing");
    
    makeActive(mMAR);
    mMAR->setRawValue(mOperand);
    
    nextTick(SLOT(executeBrn2()));
}

void MainWindow::executeBrn2() {
    makeActive(mAccumulator);
    if (mAccumulator->getDecValue() < 0) {
        mPC->setHexValue(mMAR->getHexValue());
    } else {
        mPC->setHexValue(mPC->getHexValue() + 1);
    }
    
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeStore1() {
    ui->lblStatus->setText("Executing");
    
    makeActive(mMAR);
    mMAR->setRawValue(mOperand);
    
    nextTick(SLOT(executeStore2()));
}

void MainWindow::executeStore2() {
    makeActive(mAccumulator);
    
    nextTick(SLOT(executeStore3()));
}

void MainWindow::executeStore3() {
    makeActive(mAccumulator);
    
    int sum = mAccumulator->getDecValue();
    int row = mMAR->getHexValue();
    ui->tblMemory->setItem(row, 0, new QTableWidgetItem(QString::number(sum)));
        
    mPC->setHexValue(mPC->getHexValue() + 1);
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeJump1() {
    ui->lblStatus->setText("Executing");
    
    makeActive(mMAR);
    mMAR->setRawValue(mOperand);
    
    mPC->setHexValue(mMAR->getHexValue());
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeStop1() {
    ui->lblStatus->setText("Stopped");
    
    on_actionStop_triggered();
}

void MainWindow::on_actionLoadProgram_triggered() {
    LoadProgramDialogue d(this);
    if (d.exec() == QDialog::Accepted) {
        mCode = d.getProgramCode();
        on_actionReloadCode_triggered();
        on_actionStop_triggered();
    }
}

void MainWindow::on_actionStop_triggered() {
    ui->lblStatus->setText("Computer Reset");
    
    mALU->setRawValue("NULL");
    mCIR->setRawValue("NULL");
    mPC->setRawValue("0x00");
    mAccumulator->setRawValue("NULL");
    mCU->setRawValue("NULL");
    mMAR->setRawValue("NULL");
    mMDR->setRawValue("NULL");
    
    makeActive(0);
    
    ui->actionPlay->setEnabled(true);
    ui->actionStop->setEnabled(false);
    ui->actionReloadCode->setEnabled(false);
    ui->actionStep->setEnabled(true);
    ui->actionPause->setEnabled(false);
    
    mTick = 0;
    
    mCurrentTick = (char *) SLOT(fetch1());
    mContinous = false;
}

void MainWindow::on_actionPlay_triggered() {
    mContinous = true;
    nextTick(mCurrentTick);
    
    ui->actionPlay->setEnabled(false);
    ui->actionStop->setEnabled(true);
    ui->actionReloadCode->setEnabled(true);
    ui->actionStep->setEnabled(false);
    ui->actionPause->setEnabled(true);
}

void MainWindow::on_actionStep_triggered() {
    mContinous = false;
    nextTick(mCurrentTick, true);
    
    ui->actionPlay->setEnabled(true);
    ui->actionStop->setEnabled(true);
    ui->actionReloadCode->setEnabled(true);
    ui->actionStep->setEnabled(true);
    ui->actionPause->setEnabled(false);
}

void MainWindow::on_actionPause_triggered() {
    mContinous = false;
    
    ui->actionPlay->setEnabled(true);
    ui->actionStop->setEnabled(true);
    ui->actionReloadCode->setEnabled(true);
    ui->actionStep->setEnabled(true);
    ui->actionPause->setEnabled(false);
}

void MainWindow::on_actionInstructions_triggered() {
    InstructionsWidget *widget = new InstructionsWidget();
    widget->show();
    widget->setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                            Qt::AlignCenter,
                                            widget->size(),
                                            qApp->desktop()->availableGeometry()));
}

void MainWindow::on_actionReloadCode_triggered() {
    ui->tblMemory->clearContents();
    QStringList lines = mCode.split("\n");
    for (int i = 0; i < lines.length(); i++) {
        QString line = lines[i];
        QTableWidgetItem *item = new QTableWidgetItem(line);
        ui->tblMemory->setItem(i, 0, item);
    }
    
    on_actionStop_triggered();
}
