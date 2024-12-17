#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <QDialog>

namespace Ui {
class KeyBindings;
}

class KeyBindings : public QDialog
{
    Q_OBJECT

public:
    explicit KeyBindings(QWidget *parent = nullptr);
    ~KeyBindings();

private:
    Ui::KeyBindings *ui;
};

#endif // KEYBINDINGS_H
