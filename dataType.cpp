/**
    CS-2450 Milestone 2: Basic data type for UVSim.
    @file dataType.cpp
    @author IED (Scrum Team 10)
    @version 1.1 3/21/21
*/
#include "dataType.h"

bool word_4::writeWord(int newData) {

    if (newData > MAX_WORD_4_VALUE)
	{
        this->data = MAX_WORD_4_VALUE;
		return false;
	}
    else if (newData < MIN_WORD_4_VALUE)
	{
        this->data = MIN_WORD_4_VALUE;
		return false;
	}

	this->data = newData;
	return true;
}

int word_4::readWord() {
	return this->data;
}


int word_4::getOpCode() {
	// use integer division to get the high 2 digits of the 4 digit number
	return data / 100;
}

int word_4::getOperand() {
	// use modulus division to get the low 2 digits of the 4 digit number
	return data % 100;
}

bool word_4::increment() {
	return writeWord(this->readWord() + 1);
}

word_4* word_6::split(int i) {
	word_4 * word4_arr = new word_4[2];
	int lWord = (int) i/10000;
	word4_arr[0] = word_4(lWord);
	word4_arr[1] = word_4(i - (lWord * 10000));
	return word4_arr;
}

int word_6::getOpCode() {
	return left_word.readWord();
}

int word_6::getOperand() {
	return right_word.readWord();
}

int word_6::readWord() {
	return this->getOpCode() * 10000 + this->getOperand();
}

bool word_6::writeWord(int newData) {

    if (newData > MAX_WORD_6_VALUE)
    {
        // 2 high digits of MAX_WORD_6_VALUE
        left_word.writeWord(MAX_WORD_6_VALUE / 10000);
        // 2 low digits of MAX_WORD_6_VALUE
        right_word.writeWord(MAX_WORD_6_VALUE % 10000);

        return false;
    }
    else if (newData < MIN_WORD_6_VALUE)
    {
        // 2 high digits of MAX_WORD_6_VALUE
        left_word.writeWord(MIN_WORD_6_VALUE / 10000);
        // 2 low digits of MAX_WORD_6_VALUE
        right_word.writeWord(MIN_WORD_6_VALUE % 10000);

        return false;
    }


	word_4* t = split(newData);
	left_word = t[0];
	right_word = t[1];

    return true;

}

bool word_6::increment() {
    return writeWord(this->readWord() + 1);
}
