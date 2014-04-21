#ifndef INSTRUCTIONSWIDGET_H
#define INSTRUCTIONSWIDGET_H

#include <QWidget>

namespace Ui {
    class InstructionsWidget;
}

class InstructionsWidget : public QWidget {
    Q_OBJECT

public:
    explicit InstructionsWidget(QWidget *parent = 0);
    ~InstructionsWidget();

private:
    Ui::InstructionsWidget *ui;

};

#endif // INSTRUCTIONSWIDGET_H
