/**
    CS-2450 Milestone 2: Contains the main switch case with all nessisary functionality.
    @file UVsim.cpp
    @author IED (Scrum Team 10)
    @version 1.1 3/21/21
*/

#include <UVsim.h>
#include <iostream>
#include <cstdio>
#include <string>

#ifdef UVSIM_IS_USING_GUI
#include <QDebug>
#endif


using namespace std;

UVSim::UVSim()
{
	//Constructor crap
}

UVSim::~UVSim()
{
	//Deconstructor crap
}

bool UVSim::read(int index)
{
    output("Enter an integer\n");
	
    int insertNum = getBoundedIntFromInput();
    bool returnVal = this->insertInstruction(index, insertNum);

#ifdef UVSIM_IS_USING_GUI
    sharedMem->mem_index_to_update = index;
#endif

    return returnVal;
}

bool UVSim::branchNeg(int jump)
{
    if (accumulator.readWord() < 0)
        {
            pc.writeWord(jump);
			return true;
        }
	else
	{
		return false;
	}
}

bool UVSim::branchZero(int jump)
{
    if(jump > MAIN_MEMORY_SIZE - 1 && accumulator.readWord() == 0)
	{
		pc.writeWord(jump);
		return true;
	}
	else {
		return false;
	}
}

void UVSim::dumpStateInfo()
{
    output("REGISTERS: \n");
    output("Accumulator:            " + to_string(accumulator.readWord()) + "\n");
    output("InstructionCounter:     " + to_string(pc.readWord()) + "\n");
    output("InstructionRegister:    " + to_string(mainMemory[pc.readWord()].readWord()) + "\n");
    output("OperationCode:          " + to_string(mainMemory[pc.readWord()].getOpCode()) + "\n");
    output("Operand:                " + to_string(mainMemory[pc.readWord()].getOperand())	+"\n\n");
    dumpMemory();
}




int UVSim::execute() {


    bool stillRunning = true;



    while (stillRunning) {



        word_6 instruction = this->mainMemory[pc.readWord()];



        switch (instruction.getOpCode()) {



        // IO

        case READ:

        {

            output("Reading an integer into memory location " + to_string(instruction.getOperand()) + "\n");
            this->read(instruction.getOperand());

            pc.increment();
            break;

        }



        case WRITE:

        {

            output("Writing an integer from memory location " + to_string(instruction.getOperand()) + "\n");

            output("Outputted number is :" + to_string(mainMemory[instruction.getOperand()].readWord()) + "\n");

            pc.increment();
            break;

        }



        // Load/Store

        case LOAD:

        {

            accumulator.writeWord(mainMemory[instruction.getOperand()].readWord());

            pc.increment();
            output("Loading an integer from memory location " + to_string(instruction.getOperand()) + " into the accumulator \n");
            break;

        }



        case STORE:

        {

            int valueToWrite = accumulator.readWord();

            mainMemory[instruction.getOperand()].writeWord(valueToWrite);

            pc.increment();
            output("Storing the integer from the accumulator into memory location " + to_string(instruction.getOperand()) + "\n");
            break;

        }





        // Arithmetic

        case ADD:

        {

            accumulator.writeWord(mainMemory[instruction.getOperand()].readWord() + accumulator.readWord());

            pc.increment();
            output("Adding an integer from memory location " + to_string(instruction.getOperand()) + " to the accumulator\n");
            break;

        }



        case SUBTRACT:

        {

            accumulator.writeWord(mainMemory[instruction.getOperand()].readWord() - accumulator.readWord());



            pc.increment();
             output("Subtracting an integer from memory location " + to_string(instruction.getOperand()) + " from the integer in the accumulator\n");
            break;

        }



        case DIVIDE:

        {

            accumulator.writeWord(mainMemory[instruction.getOperand()].readWord() / accumulator.readWord());

            pc.increment();
             output("Dividing the integer in the accumulator by an integer from memory location " + to_string(instruction.getOperand()) + "\n");
            break;

        }



        case MULTIPLY:

        {

            accumulator.writeWord(mainMemory[instruction.getOperand()].readWord() * accumulator.readWord());

            pc.increment();
            output("Multiplying an integer from memory location " + to_string(instruction.getOperand()) + " to the accumulator\n");
            break;

        }





        // Control Operations

        case BRANCH:

        {

            if (instruction.getOperand() > MAIN_MEMORY_SIZE - 1)
            {
                goto out_of_bounds_jump; // ends execution of UVSim with unique error message
            }

            pc.writeWord(instruction.getOperand());
            output("Branching to memory location " + to_string(instruction.getOperand()) + "\n");
            break;

        }



        case BRANCHNEG:

        {

            if (instruction.getOperand() > MAIN_MEMORY_SIZE - 1)
            {
                goto out_of_bounds_jump;
            }

            if (this->branchNeg(instruction.getOperand()))
            {
                output("BranchNeging to memory location " + to_string(instruction.getOperand()) + "\n");
            }
            else
            {
                this->pc.increment();
                output("BranchNeg did not pass. Moving to next memory location\n");
            }
            break;

        }



        case BRANCHZERO:

        {

            if (instruction.getOperand() > MAIN_MEMORY_SIZE - 1)
            {
                goto out_of_bounds_jump;
            }

            if (this->branchZero(instruction.getOperand()))
            {
                output("BranchZeroing to memory location " + to_string(instruction.getOperand()) + "\n");
            }
            else
            {
                this->pc.increment();
                output("BranchZero did not pass. Moving to next memory location\n");
            }
            break;

        }



        case HALT:

        {

            this->dumpStateInfo();

            stillRunning = false;

            break;

        }



        default:

        {

            std::cerr << "Error: Improper or unimplemented opcode [" << instruction.getOpCode() << "] found. PC has a value of [" << pc.readWord() << "]\n";

            return -1;

out_of_bounds_jump: std::cerr << "Error: Attempted to jump out of bounds of memory. Memory has valid indexes from [0," << to_string(MAIN_MEMORY_SIZE) << "] inclusive. Jump location was " << instruction.getOperand() << '\n';
                    return -2;
        }

        }

    }

    return 0;

}

bool UVSim::insertInstruction(int index, int data)
{
	if (index >= MAIN_MEMORY_SIZE || index < 0)
		return false;

	return mainMemory[index].writeWord(data);
}

void UVSim::dumpMemory()
{
    const int BUF_SIZE = 30;
    char buffer[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);
	static const int numColumns = 10;
	static const int numRows = 10;
    output("\nMEMORY:\n");

	// top row with column numbers
    output("  ");

    for (int i = 0; i < numColumns; i++)
    {
        snprintf(buffer, BUF_SIZE, "%6d%01d", 0, i);
        output(buffer);
        memset(buffer, 0, BUF_SIZE);
        //printf("%6d%01d", 0, i);
    }
    output("\n");

	
	// printing rows
	for (int i = 0; i < MAIN_MEMORY_SIZE; i += numColumns) 
	{
        snprintf(buffer, BUF_SIZE, "%02d", i);
        output(buffer);
        memset(buffer, 0, BUF_SIZE);
        //printf("%02d", i);

        for (int j = 0; j < numColumns; j++)
        {
            int temp_int = this->mainMemory[i + j].readWord();
            snprintf(buffer, BUF_SIZE, "  %07d", temp_int);
            if (temp_int < 0)
                buffer[2] = '-';
            else
                buffer[2] = '+';
            output(buffer);
            memset(buffer, 0, BUF_SIZE);
            //printf("  %+05d", this->mainMemory[i + j].readWord());
        }

        output("\n");
	}

}

void UVSim::output(const string & text)
{
#ifdef UVSIM_IS_USING_GUI
    if (guiOut)
    {
        while (sharedMem->shared_string_filled) {}

        sharedMem->shared_string = text;

        sharedMem->shared_string_filled = true;
    }
    else
#endif
    {
        output(text);
    }
}

int UVSim::getBoundedIntFromInput()
{
#ifdef UVSIM_IS_USING_GUI
    if (guiIn)
    {
        sharedMem->input_needed = true;

        while (1)
        {
            while (sharedMem->input_available == false) { if (sharedMem->kill_simulator_thread) return -1;}

            QString text = guiIn->toPlainText();

            sharedMem->input_available = false;

            bool conversionSuccessful = false;
            int inputNum = text.toInt(&conversionSuccessful);

            if (conversionSuccessful && MIN_WORD_6_VALUE <= inputNum && inputNum <= MAX_WORD_6_VALUE)
            {
                sharedMem->input_needed = false;
                return inputNum;
            }
            else
            {
                output("*** Please input a valid number between " + to_string(MIN_WORD_6_VALUE) + " and +" + to_string(MAX_WORD_6_VALUE) + " ***\n");
            }
        }
    }
    else
#endif
    {
        while (1)
        {

            int inputNum = 0;
            bool inputFailed = !(std::cin >> inputNum);

            // lets you see the file input as if it were input normally while debugging

            if (inputFailed)
            {
                output("*** Please input a valid number between -999999 and +999999 ***\n");
                output("*** Enter -9999999 to exit                                    ***\n");
                cin.clear();
            }
            //
            // bounds checking on input
            else if (inputNum > MAX_WORD_6_VALUE || inputNum < MIN_WORD_6_VALUE)
            {
                output("*** Please input a valid number between -999999 and +999999 ***\n");
                output("*** Enter -9999999 to exit                                    ***\n");
            }
            else {

                return inputNum;

            }
        }
    }

}
