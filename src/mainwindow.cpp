#include <QTimer>
#include <QDesktopWidget>
#include <QMessageBox>

#include "instructionswidget.h"
#include "component.h"
#include "loadprogramdialogue.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


class MainWindowPrivate {
public:
    Component *alu, *cir;
    Component *pc, *accumulator;
    Component *cu, *mar, *mdr;

    QString operand, code;
    int tick;
    char *currentTick;
    bool continous;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), d(new MainWindowPrivate), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    d->alu = new Component("Arithmetic Logic Unit",           0,      0,      0);
    d->cir = new Component("Current Instruction Register",    110,    0,      50);
    d->pc = new Component("Program Counter",                  220,    0,      100);
    d->accumulator = new Component("Accumulator",             0,      110,    150);
    d->cu = new Component("Control Unit",                     110,    110,    200);
    d->mar = new Component("Memory Address Register",         220,    110,    250);
    d->mdr = new Component("Memory Data Register",            0,      220,    300);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(d->alu);
    scene->addItem(d->cir);
    scene->addItem(d->pc);
    scene->addItem(d->accumulator);
    scene->addItem(d->cu);
    scene->addItem(d->mar);
    scene->addItem(d->mdr);

    ui->graphicsView->setScene(scene);

    on_actionStop_triggered();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::makeActive(Component *component) {
    d->alu->setActive(false);
    d->cir->setActive(false);
    d->pc->setActive(false);
    d->accumulator->setActive(false);
    d->cu->setActive(false);
    d->mar->setActive(false);
    d->mdr->setActive(false);

    if (component != 0) {
        component->setActive(true);
    }
}

void MainWindow::nextTick(const char *slot, bool force) {
    d->currentTick = (char *) slot;

    if (force) {
        d->tick--; // it gets ++ later
        QTimer::singleShot(0, this, slot);
    } else if (d->continous) {
        int timeout = ui->sliderTimePerTick->value();
        QTimer::singleShot(timeout, this, slot);
    }

    d->tick++;
    ui->lblTick->setText(QString::number(d->tick));
}

void MainWindow::fetch1() {
    ui->lblStatus->setText("Fetching");

    makeActive(d->pc);

    nextTick(SLOT(fetch2()));
}

void MainWindow::fetch2() {
    makeActive(d->mar);
    d->mar->setRawValue(d->pc->getRawValue());

    nextTick(SLOT(fetch3()));
}

void MainWindow::fetch3() {
    makeActive(d->mdr);
    d->mdr->setRawValue(ui->tblMemory->item(d->mar->getHexValue(), 0)->text());

    nextTick(SLOT(fetch4()));
}

void MainWindow::fetch4() {
    makeActive(d->cir);
    d->cir->setRawValue(d->mdr->getRawValue());

    nextTick(SLOT(decode1()));
}

void MainWindow::decode1() {
    ui->lblStatus->setText("Decoding");

    makeActive(d->cu);
    d->cu->setRawValue(d->cir->getRawValue());

    QStringList instruction = d->cu->getRawValue().split(QRegExp("\\s+"));
    QString op = instruction[0].toLower();

    if (instruction.length() >= 2) {
        d->operand = instruction[1];
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

    makeActive(d->mar);
    d->mar->setRawValue(d->operand);

    nextTick(SLOT(executeLoad2()));
}

void MainWindow::executeLoad2() {
    makeActive(d->mdr);
    d->mdr->setRawValue(ui->tblMemory->item(d->mar->getHexValue(), 0)->text());

    nextTick(SLOT(executeLoad3()));
}

void MainWindow::executeLoad3() {
    makeActive(d->accumulator);
    d->accumulator->setRawValue(d->mdr->getRawValue());

    d->pc->setHexValue(d->pc->getHexValue() + 1);
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeAdd1() {
    ui->lblStatus->setText("Executing");

    makeActive(d->mar);
    d->mar->setRawValue(d->operand);

    nextTick(SLOT(executeAdd2()));
}

void MainWindow::executeAdd2() {
    makeActive(d->mdr);
    d->mdr->setRawValue(ui->tblMemory->item(d->mar->getHexValue(), 0)->text());

    nextTick(SLOT(executeAdd3()));
}

void MainWindow::executeAdd3() {
    makeActive(d->alu);
    d->alu->setDecValue(d->accumulator->getDecValue() + d->mdr->getDecValue());

    nextTick(SLOT(executeAdd4()));
}

void MainWindow::executeAdd4() {
    makeActive(d->accumulator);
    d->accumulator->setRawValue(d->alu->getRawValue());
    d->alu->setRawValue("NULL");

    d->pc->setHexValue(d->pc->getHexValue() + 1);
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeSub1() {
    ui->lblStatus->setText("Executing");

    makeActive(d->mar);
    d->mar->setRawValue(d->operand);

    nextTick(SLOT(executeSub2()));
}

void MainWindow::executeSub2() {
    makeActive(d->mdr);
    d->mdr->setRawValue(ui->tblMemory->item(d->mar->getHexValue(), 0)->text());

    nextTick(SLOT(executeSub3()));
}

void MainWindow::executeSub3() {
    makeActive(d->alu);
    d->alu->setDecValue(d->accumulator->getDecValue() - d->mdr->getDecValue());

    nextTick(SLOT(executeSub4()));
}

void MainWindow::executeSub4() {
    makeActive(d->accumulator);
    d->accumulator->setRawValue(d->alu->getRawValue());
    d->alu->setRawValue("NULL");

    d->pc->setHexValue(d->pc->getHexValue() + 1);
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeBrz1() {
    ui->lblStatus->setText("Executing");

    makeActive(d->mar);
    d->mar->setRawValue(d->operand);

    nextTick(SLOT(executeBrz2()));
}

void MainWindow::executeBrz2() {
    makeActive(d->accumulator);
    if (d->accumulator->getDecValue() == 0) {
        d->pc->setHexValue(d->mar->getHexValue());
    } else {
        d->pc->setHexValue(d->pc->getHexValue() + 1);
    }

    nextTick(SLOT(fetch1()));
}

void MainWindow::executeBrn1() {
    ui->lblStatus->setText("Executing");

    makeActive(d->mar);
    d->mar->setRawValue(d->operand);

    nextTick(SLOT(executeBrn2()));
}

void MainWindow::executeBrn2() {
    makeActive(d->accumulator);
    if (d->accumulator->getDecValue() < 0) {
        d->pc->setHexValue(d->mar->getHexValue());
    } else {
        d->pc->setHexValue(d->pc->getHexValue() + 1);
    }

    nextTick(SLOT(fetch1()));
}

void MainWindow::executeStore1() {
    ui->lblStatus->setText("Executing");

    makeActive(d->mar);
    d->mar->setRawValue(d->operand);

    nextTick(SLOT(executeStore2()));
}

void MainWindow::executeStore2() {
    makeActive(d->accumulator);

    nextTick(SLOT(executeStore3()));
}

void MainWindow::executeStore3() {
    makeActive(d->accumulator);

    int sum = d->accumulator->getDecValue();
    int row = d->mar->getHexValue();
    ui->tblMemory->setItem(row, 0, new QTableWidgetItem(QString::number(sum)));

    d->pc->setHexValue(d->pc->getHexValue() + 1);
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeJump1() {
    ui->lblStatus->setText("Executing");

    makeActive(d->mar);
    d->mar->setRawValue(d->operand);

    d->pc->setHexValue(d->mar->getHexValue());
    nextTick(SLOT(fetch1()));
}

void MainWindow::executeStop1() {
    ui->lblStatus->setText("Stopped");

    on_actionStop_triggered();
}

void MainWindow::on_actionLoadProgram_triggered() {
    LoadProgramDialogue dialogue(this);
    if (dialogue.exec() == QDialog::Accepted) {
        d->code = dialogue.getProgramCode();
        on_actionReloadCode_triggered();
        on_actionStop_triggered();
    }
}

void MainWindow::on_actionStop_triggered() {
    ui->lblStatus->setText("Computer Reset");

    d->alu->setRawValue("NULL");
    d->cir->setRawValue("NULL");
    d->pc->setRawValue("0x00");
    d->accumulator->setRawValue("NULL");
    d->cu->setRawValue("NULL");
    d->mar->setRawValue("NULL");
    d->mdr->setRawValue("NULL");

    makeActive(0);

    ui->actionPlay->setEnabled(true);
    ui->actionStop->setEnabled(false);
    ui->actionReloadCode->setEnabled(false);
    ui->actionStep->setEnabled(true);
    ui->actionPause->setEnabled(false);

    d->tick = 0;

    d->currentTick = (char *) SLOT(fetch1());
    d->continous = false;
}

void MainWindow::on_actionPlay_triggered() {
    d->continous = true;
    nextTick(d->currentTick);

    ui->actionPlay->setEnabled(false);
    ui->actionStop->setEnabled(true);
    ui->actionReloadCode->setEnabled(true);
    ui->actionStep->setEnabled(false);
    ui->actionPause->setEnabled(true);
}

void MainWindow::on_actionStep_triggered() {
    d->continous = false;
    nextTick(d->currentTick, true);

    ui->actionPlay->setEnabled(true);
    ui->actionStop->setEnabled(true);
    ui->actionReloadCode->setEnabled(true);
    ui->actionStep->setEnabled(true);
    ui->actionPause->setEnabled(false);
}

void MainWindow::on_actionPause_triggered() {
    d->continous = false;

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
    QStringList lines = d->code.split("\n");
    for (int i = 0; i < lines.length(); i++) {
        QString line = lines[i];
        QTableWidgetItem *item = new QTableWidgetItem(line);
        ui->tblMemory->setItem(i, 0, item);
    }

    on_actionStop_triggered();
}
