#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Day8 Part 1
//Given a list of instructions give the current value of the accumulator after one complete cycle.
//Instruction Opcodes
//ACC: increases or decreases a single global value called the accumulator by the value given in the argument.For example, acc + 7 would increase the 
//accumulator by 7. The accumulator starts at 0. After an acc instruction, the instruction immediately below it is executed next.
//JMP: jumps to a new instruction relative to itself.The next instruction to execute is found using the argument as an offset from the jmp instruction; for 
//example, jmp + 2 would skip the next instruction, jmp + 1 would continue to the instruction immediately below it, and jmp - 20 would cause the instruction 
//20 lines above to be executed next.
//NOP: stands for No OPeration - it does nothing.The instruction immediately below it is executed next.
//Instruction example:
//nop + 0 #1
//acc + 1 #2 acc = 1 #8 this would be the start of the next cycle
//jmp + 4 #3
//acc + 3 #6 acc = 5
//jmp - 3 #7
//acc - 99
//acc + 1 #4 acc = 2
//jmp - 4 #5
//acc + 6
//So 5 would be the value of the accumulator after 1 cycle.

//So there are a few things to consider, one we need a way to traverse the instructions and also extract the information from those instructions, two we
//need to keep track of which instructions we have visited, we can most likely use a boolean array to do this. So what we can do is store each line in a
//vector, then split on space, using a switch we can create a enum for the op codes, then do each operation based on that, once an index location has been
//used we can flip that index in our boolean array, an check if its true before running each op code.

//For part two, we can reuse our part one. We need to create two input sets one with the opcode left alone and one with the opcode swapped, we will call
//them inputO and inputSwap then we need to capture the next index for the opcode that we go to next in inputO which will call i, then we let 
//inputSwap run and watch its index values which we will call j, if j ever equals i then we know we haven't escaped our loop and we can continue to the 
//next code to swap and do the same thing. The part of our part one we can reuse is the part that checks if we return to a previous index, just like
//when we were checking the acc before we looped again.
enum OPCODE
{
	ACC,
	JMP,
	NOP
};

std::vector<std::string> getPuzzleInput()
{
	std::vector<std::string> input;

	std::ifstream file;
	std::string line;
	file.open("puzzle-input.txt");
	while (std::getline(file, line))
	{
		input.push_back(line);
	}

	return input;
}

std::vector<std::string> split(const std::string s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> tokens;
	while (getline(ss, item, delim))
	{
		tokens.push_back(item);
	}

	return tokens;
}

OPCODE getEnum(std::string s)
{
	if (s == "acc")
		return ACC;
	if (s == "jmp")
		return JMP;
	if (s == "nop")
		return NOP;
}

int findAnswerPart1(std::vector<std::string> input)
{
	int acc = 0;
	int index = 0;
	bool flags[611] = { 0 };

	while (flags[index] != true)
	{
		std::cout << index << ":" << input[index] << '\n';
		std::vector<std::string> parts = split(input[index], ' ');

		std::string opcode = parts[0];
		int operand = std::stoi(parts[1]);
		flags[index] = true;

		switch (getEnum(opcode))
		{
		case ACC:
			acc += operand;
			index++;
			break;
		case JMP:
			index += operand;
			break;
		case NOP:
			index++;
			break;
		}
	}

	return acc;
}


bool check(int indexOfSwap, std::string swap, std::vector<std::string> input)
{
	int acc = 0;
	int index = 0;
	bool flags[612] = { 0 };
	std::string opcode = "";
	int operand = 0;

	while (flags[index] != true)
	{	
		if (index >= 612)
		{
			break;
		}

		std::vector<std::string> parts = split(input[index], ' ');
		if (index == indexOfSwap)
		{
			opcode = swap;
		}
		else
		{
			opcode = parts[0];
		}

		operand = std::stoi(parts[1]);

		flags[index] = true;

		switch (getEnum(opcode))
		{
		case ACC:
			acc += operand;
			index++;
			break;
		case JMP:
			index += operand;
			break;
		case NOP:
			index++;
			break;
		}
	}

	if (flags[index] == true)
	{
		return false;
	}

	std::cout << "Acc: " << acc << '\n';
	return true;
}

void findAnswerPart2(std::vector<std::string> input)
{
	for (int i = 0; i < input.size(); i++)
	{
		std::vector<std::string> parts = split(input[i], ' ');
		std::string opcode = parts[0];
		int operand = std::stoi(parts[1]);

		if (opcode == "jmp")
		{
			if (check(i, "nop", input))
			{
				std::cout << "input[" << i << "] should be nop" << '\n';
				break;
			}
		}
		else if (opcode == "nop")
		{
			if (check(i, "jmp", input))
			{
				std::cout << "input[" << i << "] should be jmp" << '\n';
				break;
			}
		}
		else
		{
			continue;
		}
	}
}


int main()
{
	std::vector<std::string> input = getPuzzleInput();

	std::cout << findAnswerPart1(input) << '\n';

	findAnswerPart2(input);

	return 0;
}