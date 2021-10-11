#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QMutex>
#include <UVsim.h>
#include <mutex>
#include <QThread>
#include <QTimer>
#include <QPointer>
#include "documentationwindow.h"
#include "./sharedMemory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/**
 * MainWindows purpose is to set up and run the GUI window
 * 
 * There are methods used for setup and cleanup.
 * Methods used as getters and setters.
 * Methods used for event handling.
 * Methods used for threading.
 * Preconditions: Will be documented in the outline document.
 * Postconditions: Methods either return a boolean or
 * do not have a return (void). Will also explain further
 * in outline documentation. 
*/


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //MainWindow(UVSim * machine) : simulator(machine) { QMainWindow(nullptr); };
    MainWindow(QApplication * application, QWidget *parent = nullptr);
    ~MainWindow();

    UVSim * simulator = nullptr;
    QTextBrowser * output = nullptr;
    QTextEdit * input = nullptr;

    QApplication * application = nullptr;

    bool setMemoryWordFromTable(int index);
    bool updateTableCellFromMemory(int index);

    static void reset_gui   (MainWindow * gui);

private slots:
    void on_memoryTable_cellChanged(int row, int column);

    void on_run_button_clicked();

    void on_input_button_clicked();

    void on_help_button_clicked();

    void on_output_clear_button_clicked();

    void on_reset_button_clicked();

private:

    Ui::MainWindow *ui;
    QString welcomeMessage = "*** Welcome to UVSim!                         ***\n"
                            "*** Please enter your program one instruction ***\n"
                            "*** ( or data word ) at a time into the input ***\n"
                            "*** table. Hit the RUN button to start your   ***\n"
                            "*** program. Hit the RESET button to reset    ***\n"
                            "*** everything to as if you had just opened   ***\n"
                            "*** This window again                         ***\n";

    QPointer<documentationWindow> docs = nullptr;

    bool tableSetupFinished = false;

    static sharedMemory sharedMem;

    void populateStateInfo();

    void load_mem_from_file();
    void save_mem_to_file();


    bool setSimulatorPC(int num);
    bool setSimulatorAccumulator(int num);
    bool setSimulatorMem(int index, int num);
    word_6 getSimulatorMemWord(int index);
    int getSimulatorPC();
    int getSimulatorAccumulator();
    int getSimulatorMem(int index);
    int getSimulatorMem(int index, bool * errorFlag);

    void updateOutput();

};
#endif // MAINWINDOW_H
