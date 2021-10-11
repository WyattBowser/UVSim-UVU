#ifndef UVSIM_UVSIM_H
#define UVSIM_UVSIM_H

#include "dataType.h"
#include <string>

// uncomment the definition of UVSIM_IS_USING_GUI to remove all GUI related code
#define UVSIM_IS_USING_GUI
#ifdef UVSIM_IS_USING_GUI
#include <mutex>
#include <QTextBrowser>
#include <QString>
#include <QTextEdit>
#include <QTextStream>
#include <atomic>
#include "sharedMemory.h"

//#include "mainwindow.h"

bool updateTableFromMemory(int index);
#endif

using namespace std;

static const int MAIN_MEMORY_SIZE = 100;

/**
 * UVSim class is used as the main backbone of our simulator
 * 
 * This is were all the methods for the opcodes are defined
 * as well as having methods that help with the overall execution
 * of our simulator.
 * There are no accepted paramaters.
 * When the execute method is ran the expected output
 * is a completed run through all opcodes that are
 * in the specific program that is ran. The accumulator
 * as well as other registers should update to show that the
 * opcodes have ran correctly. The other methods within
 * the UVSim class are the opcodes that either return a boolean 
 * value or do not have a returned value (void). See outline
 * documentation for specific method descriptions. 
*/
class UVSim
{
    public:
        UVSim();
        ~UVSim();

        //METHODS

        //Main execute command that will start the main memory
        int execute();

        // inserts integer into memory index
        bool insertInstruction(int index, int data);

        // Reads a word from the keyboard into a specific memory location
        bool read(int index);

        //Branch to a specific location in memory if the accumulator is negative.
        bool branchNeg(int jump);

        //Branch to a specific location in memory if the accumulator is zero
        bool branchZero(int jump);

        //provides information on the registers and memory
        void dumpStateInfo();   

        // dumps all mainMemory as int for debugging. Is called by dumpStateInfo
        void dumpMemory();

        void output(const string & text);
        int getBoundedIntFromInput();

    protected:
        //Initialize the operations
        //Memory allocation of 100 all initialized to 0
        word_6 mainMemory[MAIN_MEMORY_SIZE] = { };

        //program counter
        word_6 pc = word_6(0);       //Holds the index for the next instruction to be executed from mainMemory

        //accumulator
        word_6 accumulator = word_6(0);        //This will hold data that will be used in some way

        //Vocam Definitions with values
            //i/o
        static const int READ = 10;        //Read a word from the keyboard into a specific location in memory.
        static const int WRITE = 11;       //Write a word from a specific location in memory to screen.
            //Load/Store
        static const int LOAD = 20;        //Load a word from a specific location in memory into the accumulator. 
        static const int STORE = 21;       //Store a word from the accumulator into a specific location in memory. 
            //Arithmetic
        static const int ADD = 30;         //Add a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator) 
        static const int SUBTRACT = 31;    //Subtract a word from a specific location in memory from the word in the accumulator (leave the result in the accumulator)
        static const int DIVIDE = 32;      //Divide the word in the accumulator by a word from a specific location in memory (leave the result in the accumulator).
        static const int MULTIPLY = 33;    //multiply a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator).
            //Control Operations
        static const int BRANCH = 40;      //Branch to a specific location in memory
        static const int BRANCHNEG = 41;   //Branch to a specific location in memory if the accumulator is negative.
        static const int BRANCHZERO = 42;  //Branch to a specific location in memory if the accumulator is zero.
        static const int HALT = 43;        //Pause the program

#ifdef UVSIM_IS_USING_GUI

        sharedMemory * sharedMem;

        QTextBrowser * guiOut = nullptr;
        QTextEdit * guiIn  = nullptr;

        friend class MainWindow;
#endif
};

#endif
