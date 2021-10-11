#ifndef DOCUMENTATIONWINDOW_H
#define DOCUMENTATIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class documentationWindow;
}

class documentationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit documentationWindow(QWidget *parent = nullptr);
    ~documentationWindow();

private:
    Ui::documentationWindow *ui;
    void * self_reference = nullptr;
};

#endif // DOCUMENTATIONWINDOW_H
