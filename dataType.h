#ifndef UVSIM_DATATYPE_H
#define UVSIM_DATATYPE_H


const int WORD_6_NUM_DIGITS = 6;
const int MAX_WORD_6_VALUE = 999999;
const int MIN_WORD_6_VALUE = -999999;

const int WORD_4_NUM_DIGITS = 4;
const int MAX_WORD_4_VALUE = 9999;
const int MIN_WORD_4_VALUE = -9999;

/**
 * The word struct is used to read/write words
 * and interpret them into their opcode
 * and operand and increment through them.
 * 
 * The expected input is a word that is read in
 * from the program that was entered by the user.
 * It will return false if the word is out of the
 * bounds expected.
 * The expected output for writeWord is a boolean,
 * for readWord it returns the int value within the
 * word.
 * getOpCode returns the OpCode in the word.
 * getOperand returns the Operand in the word.
 * increment returns a boolean in the same
 * manner as writeWord.
 */



struct word_4 {
public:
    word_4() : data(0) {}
    word_4(int newData) : data(newData) {}
    ~word_4() = default;
	
	bool writeWord(int data); // Changes the value of the word to data provided. Returns true if the written value was within bounds. Returns false if the written value was out of bounds, and sets the data to max (if over) or min (if under) value
	int readWord(); // Returns the integer value contained within word. Will be within the bounds of [MIN_WORD_VALUE, MAX_WORD_VALUE] inclusive

	int getOpCode(); // returns the OpCode of the data contained (high two digits in the 4 digit number. Thousand's and hundred's digits)
	int getOperand(); // returns the Operand of the data contained (low two digits in the 4 digit number. One's and ten's digits)

	bool increment(); // Increases the value within word by 1. Returns a bool in the exact same manner as writeWord

protected:
	int data;

    word_4& operator=(const int&) = delete; // Disables the assignment operator for int. Whenever assigning an int value to a word, do it through the writeWord() method
};

struct word_6 {
public:
    word_6() : left_word(word_4(0)), right_word(word_4(0)) {};
    word_6(int newData)
	{
		word_4* t = split(newData);
		left_word = t[0];
		right_word = t[1];
	};
    ~word_6() = default;

    bool writeWord(int data); // Changes the value of the word to data provided. Returns true if the written value was within bounds. Returns false if the written value was out of bounds, and sets the data to max (if over) or min (if under) value
    int readWord(); // Returns the integer value contained within word. Will be within the bounds of [MIN_WORD_VALUE, MAX_WORD_VALUE] inclusive

    int getOpCode(); // returns the OpCode of the data contained (high two digits in the 4 digit number. Thousand's and hundred's digits)
    int getOperand(); // returns the Operand of the data contained (low two digits in the 4 digit number. One's and ten's digits)

    bool increment(); // Increases the value within word by 1. Returns a bool in the exact same manner as writeWord

protected:
    word_4 left_word;
    word_4 right_word;

    word_6& operator=(const int&) = delete; // Disables the assignment operator for int. Whenever assigning an int value to a word, do it through the writeWord() method

private:
    word_4* split(int data);

};
#endif
