#include "instructionswidget.h"
#include "ui_instructionswidget.h"

InstructionsWidget::InstructionsWidget(QWidget *parent) : QWidget(parent), ui(new Ui::InstructionsWidget) {
    ui->setupUi(this);
}

InstructionsWidget::~InstructionsWidget() {
    delete ui;
}
