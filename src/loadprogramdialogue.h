#ifndef LOADPROGRAMDIALOGUE_H
#define LOADPROGRAMDIALOGUE_H

#include <QDialog>

namespace Ui {
    class LoadProgramDialogue;
}

class LoadProgramDialogue : public QDialog {
    
    Q_OBJECT
    
public:
    explicit LoadProgramDialogue(QWidget *parent = 0);
    ~LoadProgramDialogue();
    
    QString getProgramCode();
    
private slots:
    void on_comboExamples_currentIndexChanged(int index);
    
private:
    Ui::LoadProgramDialogue *ui;

};

#endif // LOADPROGRAMDIALOGUE_H
