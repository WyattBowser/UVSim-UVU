/**
    CS-2450 Milestone 2: Takes care of main window of GUI for UVSim.
    @file mainwindow.cpp
    @author IED (Scrum Team 10)
    @version 1.1 3/21/21
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractItemModel>
#include <QTableView>
#include <QTableWidget>
#include <iostream>
#include <QDebug>
#include <thread>
#include <mutex>
#include <future>
#include <QThread>
#include <QFont>
#include <QFontDatabase>
#include <QFileDialog>
#include <QFile>
#include <QObject>
#include <QAction>





// ---------------
// FORMATTING INFO
// ---------------
//          2 blank lines
//          2 blank lines
//  Pretend_function()
//  Functions will be grouped in this format
//  Leave 5 blank lines in between each
//  "Header" and "Footer" for each group
//  and code outside of the group, or other
//  groups
//          2 blank lines
//          2 blank lines
//  Pretend_function()
//  Leave 2 blank lines in between each function
//  Within the group, and between the functions
//  and their group headers like this
//          2 blank lines
//          2 blank lines
// -------------------
// END FORMATTING INFO
// -------------------





// -----------------
// SETUP AND CLEANUP
// -----------------

    const int TABLE_MEM_DATA_COLUMN = 0;
    const int TABLE_OP_STRING_COLUMN = 1;

MainWindow::MainWindow(QApplication * application, QWidget *parent)
    : QMainWindow(parent)
    , application(application)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    this->output = ui->output_window;
    this->input = ui->input_window;

    // connect the menu file and save to the saveFile and loadFile functions
    connect(ui->menu_save, &QAction::triggered, this, &MainWindow::save_mem_to_file);
    connect(ui->menu_load, &QAction::triggered, this, &MainWindow::load_mem_from_file);

    // Create UVSim and Connect the UVSim's pointers to the mainWindow
    simulator = new UVSim();
    simulator->guiOut = this->output;
    simulator->guiIn = this->input;
    simulator->sharedMem = &sharedMem;

    // Timer is needed for syncing output between the two threads. After these three lines the timer shouldn't be touched
    QTimer *output_timer = new QTimer(this);
    connect(output_timer, &QTimer::timeout, this, &MainWindow::updateOutput);
    output_timer->start();

    // Timer for syncing the Registers shown. Runs less frequently because humans are slow :P
    QTimer *stateinfo_timer = new QTimer(this);
    connect(stateinfo_timer, &QTimer::timeout, this, &MainWindow::populateStateInfo);
    stateinfo_timer->start(10);

   // Changes the font for the output window to be fixed. This way output is consistent and each char takes the same amount of space
   const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
   output->setFont(fixedFont);



    // Sets the table up properly for startup
    QStringList row_labels;

    auto row_list = ui->memoryTable->selectedItems();

    for (int i = 0; i < 100; i++)
    {
        ui->memoryTable->setItem(i, TABLE_MEM_DATA_COLUMN, new QTableWidgetItem());
        ui->memoryTable->setItem(i, TABLE_OP_STRING_COLUMN, new QTableWidgetItem());
        updateTableCellFromMemory(i);
        row_labels << QString("%1").arg(i);
    }

    ui->memoryTable->setVerticalHeaderLabels(row_labels);
    tableSetupFinished = true;



   output->append(welcomeMessage);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete simulator;
}

// ---------------------
// END SETUP AND CLEANUP
// ---------------------





// -------------------
// GETTERS AND SETTERS
// -------------------


bool MainWindow::setSimulatorPC(int num)
{
    bool result = simulator->pc.writeWord(num);

    populateStateInfo();

    return result;
}

bool MainWindow::setSimulatorAccumulator(int num)
{
    bool result = simulator->accumulator.writeWord(num);

    populateStateInfo();

    return result;
}

bool MainWindow::setSimulatorMem(int index, int num)
{
    if (index < 0 || index > MAIN_MEMORY_SIZE)
        return false;

    simulator->mainMemory[index].writeWord(num);

    updateTableCellFromMemory(index);

    return true;
}

int MainWindow::getSimulatorMem(int index)
{
    if (index < 0 && index > MAIN_MEMORY_SIZE)
        return -1;

    return simulator->mainMemory[index].readWord();
}

int MainWindow::getSimulatorMem(int index, bool * errorFlag)
{
    if (index < 0 && index > MAIN_MEMORY_SIZE)
    {
        *errorFlag = true;
        return -1;
    }

    return simulator->mainMemory[index].readWord();
}


word_6 MainWindow::getSimulatorMemWord(int index)
{
    if (index < 0 && index > MAIN_MEMORY_SIZE)
        return word_6();

    return simulator->mainMemory[index];
}

int MainWindow::getSimulatorPC()
{
    return simulator->pc.readWord();
}


int MainWindow::getSimulatorAccumulator()
{
    return simulator->accumulator.readWord();
}


// -----------------------
// END GETTERS AND SETTERS
// -----------------------




// ------------------------
// Responsive GUI functions
// ------------------------


void MainWindow::on_memoryTable_cellChanged(int row, int column)
{

    if (!tableSetupFinished)
        return;


    if (column == TABLE_MEM_DATA_COLUMN)
    {
        setMemoryWordFromTable(row);
        // updateTableCellFromMemory called in case a number out of bounds for the word datatype
        // was entered. If that happens, the table cell goes to the upper or lower bound
        // of the word datatype
        updateTableCellFromMemory(row);
    }

        word_6 tableMemValue = getSimulatorMemWord(row);

        QString newCellOpCode;

        switch (tableMemValue.getOpCode()){

        case UVSim::READ:
                newCellOpCode = "READ ";
            break;
        case UVSim::WRITE:
                newCellOpCode = "WRITE ";
            break;
        case UVSim::LOAD:
                newCellOpCode = "LOAD ";
            break;
        case UVSim::STORE:
                newCellOpCode = "STORE ";
            break;
        case UVSim::ADD:
                newCellOpCode = "ADD ";
            break;
        case UVSim::SUBTRACT:
                newCellOpCode = "SUBTRACT ";
            break;
        case UVSim::DIVIDE:
                newCellOpCode = "DIVIDE ";
            break;
        case UVSim::MULTIPLY:
                newCellOpCode = "MULTIPLY ";
            break;
        case UVSim::BRANCH:
                newCellOpCode = "BRANCH ";
            break;
        case UVSim::BRANCHNEG:
                newCellOpCode = "BRANCHNEG ";
            break;
        case UVSim::BRANCHZERO:
                newCellOpCode = "BRANCHZERO ";
            break;
        case UVSim::HALT:
                newCellOpCode = "HALT ";
            break;

        }


        if (newCellOpCode.size() > 0)
        {
            QString newCellOperand = QString::number(tableMemValue.getOperand());
            ui->memoryTable->item(row, TABLE_OP_STRING_COLUMN)->setText(newCellOpCode + ":" + newCellOperand);
        }
}

  
void MainWindow::populateStateInfo()
{
    const int RANDOM_OUT_OF_WORD_RANGE_NUMBER {INT_MIN};
    static int current_accumulator_text {RANDOM_OUT_OF_WORD_RANGE_NUMBER};
    static int current_pc_text {RANDOM_OUT_OF_WORD_RANGE_NUMBER};
    static int current_pc_at_text {RANDOM_OUT_OF_WORD_RANGE_NUMBER};

    // Since this is called often via a timer, we avoid the costly procedure of actually changing elements by checking if they've been changed

    if (current_accumulator_text != getSimulatorAccumulator())
    {
        current_accumulator_text = getSimulatorAccumulator();
        ui->accumulator_text->setText(QString::number(current_accumulator_text));
    }

    if (current_pc_text != getSimulatorPC())
    {
        current_pc_text = getSimulatorPC();
        ui->pc_text->setText(QString::number(current_pc_text));
    }

    if (current_pc_at_text != getSimulatorMem(getSimulatorPC()))
    {
        current_pc_at_text = getSimulatorMem(getSimulatorPC());
        ui->pc_data_text->setText(QString::number(current_pc_at_text));
    }

}


bool MainWindow::setMemoryWordFromTable(int index)
{
    if (index < 0 || index >= MAIN_MEMORY_SIZE)
    {
        return false;
    }

    int new_num = ui->memoryTable->item(index, TABLE_MEM_DATA_COLUMN)->text().toInt();

    simulator->mainMemory[index].writeWord(new_num);

    return true;
}

bool MainWindow::updateTableCellFromMemory(int index)
{
    if (index < MIN_WORD_6_VALUE || index > MAX_WORD_6_VALUE)
    {
        return false;
    }

    QString new_text = QString::number(simulator->mainMemory[index].readWord());

    if (new_text.at(0) == '-')
    {
        while (new_text.length() < WORD_6_NUM_DIGITS + 1)
        {
           new_text.insert(1, '0');
        }
    }
    else
    {
        while (new_text.length() < WORD_6_NUM_DIGITS)
        {
            new_text.prepend('0');
        }
            new_text.prepend('+');
    }

    ui->memoryTable->item(index, TABLE_MEM_DATA_COLUMN)->setText(new_text);

    return true;
}


void MainWindow::on_output_clear_button_clicked()
{
    if (!sharedMem.simulator_running)
        output->clear();
}


void MainWindow::on_help_button_clicked()
{
    if (!docs)
    {
        docs = new documentationWindow(this);
        docs->setAttribute(Qt::WA_DeleteOnClose);
        docs->setFixedSize(docs->size());
        docs->show();
    }
}



void MainWindow::save_mem_to_file()
{

    QString save_file_name = QFileDialog::getSaveFileName(this, "Save memory to a .uvm file", application->applicationDirPath(), "UVSim memory files (*.uvm)");
    QFile * save_file = new QFile(save_file_name);
    save_file->open(QIODevice::WriteOnly | QIODevice::Text);

    if (save_file->isOpen())
    {
        QTextStream stream(save_file);

        for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
        {
            int mem_int = getSimulatorMem(i);
            string mem_string = to_string(mem_int);

            if (mem_int >= 0)
                mem_string.insert(0, "+");

            while(mem_string.length() < WORD_6_NUM_DIGITS + 1)
            {
                mem_string.insert(1, "0");
            }

            stream << mem_string.c_str() << '\n';
        }
    }

}

void MainWindow::load_mem_from_file()
{
    const int READ_LINE_ERROR = 0;
    const int BUFF_SIZE = 20;
    char buffer[BUFF_SIZE];
    memset(buffer, 0, BUFF_SIZE);


    // Asks for a .uvm file from the file dialog
    QPointer<QFileDialog> temp = new QFileDialog(this);
    temp->setFileMode(QFileDialog::ExistingFile);
    QFile * read_file = new QFile(temp->getOpenFileName(this, "Select a memory file. It should have the extension .uvm", application->applicationDirPath(), "UVSim memory files (*.uvm)"));


    if (read_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {

        for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
        {
            if (read_file->readLine(buffer, BUFF_SIZE) != READ_LINE_ERROR)
            {
                bool ok;
                int insertInt = QString(buffer).toInt(&ok);

                if (ok == false)
                    return;

                setSimulatorMem(i, insertInt);

            }
            else {
                return;
            }
        }
    }
}


// ------------------------
// Responsive GUI functions
// ------------------------





// -------------------
// THREADING FOR INPUT
// -------------------


sharedMemory MainWindow::sharedMem;


void runSimulator(MainWindow * gui, sharedMemory * sharedMem)
{

    atomic<bool> simulator_thread_joinable = ATOMIC_VAR_INIT(false);

    std::thread simulator_thread([&]{
        gui->simulator->execute();
        simulator_thread_joinable = true;
    });


    while (1)
    {
        QCoreApplication::processEvents();

        if (simulator_thread_joinable)
        {
            //simulator_qthread.wait();
            simulator_thread.join();

            sharedMem->kill_simulator_thread = false;
            if (sharedMem->reset_again)
            {
                MainWindow::reset_gui(gui);
                sharedMem->reset_again = false;
            }
            sharedMem->simulator_running = false;
            return;
        }
    }

}


void MainWindow::on_run_button_clicked()
{

    if (sharedMem.simulator_running == true)
    {
        return;
    }

    output->insertPlainText("*** Program loading completed ***\n"
                            "*** Program execution begins  ***\n");

    sharedMem.simulator_running = true;
    ui->run_button->setText("RUNNING");

    setSimulatorPC(0);
    setSimulatorAccumulator(0);
    runSimulator(this, &sharedMem);
    populateStateInfo();

    ui->run_button->setText("RUN");
}


void MainWindow::on_input_button_clicked()
{
    if (sharedMem.input_needed == true)
    {
        sharedMem.input_available = true;

        while (sharedMem.input_available == true) {}

        ui->input_window->setPlainText("");

        if (sharedMem.mem_index_to_update != DONT_UPDATE_MEM)
        {
            updateTableCellFromMemory(sharedMem.mem_index_to_update);
            sharedMem.mem_index_to_update = DONT_UPDATE_MEM;
        }
    }
}

void MainWindow::updateOutput()
{
    if (sharedMem.shared_string_filled)
    {
        output->insertPlainText (QString::fromStdString(sharedMem.shared_string));
        sharedMem.shared_string_filled = false;
    }
}


// --------------------------
// END OF THREADING FOR INPUT
// --------------------------





// ---------------------------------------------------
// OTHER FUNCTIONS WHICH NEED TO WORRY ABOUT THREADING
// ---------------------------------------------------


void MainWindow::on_reset_button_clicked()
{

    if (sharedMem.simulator_running)
    {
        sharedMem.kill_simulator_thread = true;
        sharedMem.reset_again = true;
        // wait for after thread join to reset
    }
    else
    {
        reset_gui(this);
    }

}

void MainWindow::reset_gui(MainWindow * gui)
{
    gui->output->clear();
    gui->input->clear();

    gui->ui->run_button->setText("RUN");

    for (int i = 0; i < MAIN_MEMORY_SIZE; i++)
    {
        gui->setSimulatorMem(i, 0);
        gui->updateTableCellFromMemory(i);
        gui->ui->memoryTable->item(i, TABLE_OP_STRING_COLUMN)->setText("");
    }

    gui->setSimulatorPC(0);
    gui->setSimulatorAccumulator(0);

    gui->output->append(gui->welcomeMessage);
}

// ---------------------------------------------------
// OTHER FUNCTIONS WHICH NEED TO WORRY ABOUT THREADING
// ---------------------------------------------------
