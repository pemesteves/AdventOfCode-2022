// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <list>

using namespace std;

enum OpType
{
    Addition = 0,
    Product = 1
};

enum OpValueType
{
    Old = 0,
    Number = 1
};

struct Monkey
{
    // Items
    list<unsigned long long> Items;

    // Operation
    OpType OperationType;
    OpValueType OperationValueType;
    int OperationValue;

    // Test
    int DivisionValue;
    int TrueTarget;
    int FalseTarget;

    // Solution Variables
    unsigned long long NumItemsInspected = 0;
};

vector<string> readFile()
{
    ifstream f;
    f.open("input.txt");

    vector<string> input;

    string line;

    while (!f.eof())
    {
        getline(f, line);

        input.push_back(line);
    }

    f.close();

    return input;
}

void parseMonkeys(vector<string> input, vector<Monkey> &Monkeys)
{
    Monkeys = {};

    string Line;
    int StringIndex;
    for (size_t i = 0; i < input.size(); ++i)
    {
        Monkey M;

        // Starting items
        ++i;
        Line = input[i].substr(input[i].find(":") + 2);
        do
        {
            StringIndex = Line.find(',');
            M.Items.push_back(stoi(Line.substr(0, StringIndex)));
            Line = Line.substr(StringIndex + 2);
        } while (StringIndex != string::npos);

        // Operation
        ++i;
        Line = input[i].substr(input[i].find('=') + 2);
        Line = Line.substr(Line.find(' ') + 1);

        M.OperationType = Line[0] == '*' ? Product : Addition;

        Line = Line.substr(Line.find(' ') + 1);

        M.OperationValueType = Line == "old" ? Old : Number;
        if (M.OperationValueType == Number)
        {
            M.OperationValue = stoi(Line);
        }

        // Test
        ++i;
        Line = input[i].substr(input[i].find("by") + 3);
        M.DivisionValue = stoi(Line);

        ++i;
        Line = input[i].substr(input[i].find("monkey") + 7);
        M.TrueTarget = stoi(Line);

        ++i;
        Line = input[i].substr(input[i].find("monkey") + 7);
        M.FalseTarget = stoi(Line);

        // Empty Line
        ++i;

        Monkeys.push_back(M);
    }
}

void monkeyInspection(vector<Monkey>& Monkeys, const int& NumRounds, const bool& bDivide, const int& Modulo = 1)
{
    for (int Round = 0; Round < NumRounds; ++Round)
    {
        if ((Round > 0 && Round % 1000 == 0) || Round == 20)
        {
            int i;
            i = 10;
        }

        for (size_t i = 0; i < Monkeys.size(); ++i)
        {
            Monkey& M = Monkeys[i];
            while (!M.Items.empty())
            {
                unsigned long long WorryLevel = M.Items.front();
                if (M.OperationType == Product)
                {
                    WorryLevel = WorryLevel * (M.OperationValueType == Old ? WorryLevel : M.OperationValue);
                }
                else
                {
                    WorryLevel = WorryLevel + (M.OperationValueType == Old ? WorryLevel : M.OperationValue);
                }

                if (bDivide)
                {
                    WorryLevel /= 3;
                }
                else
                {
                    WorryLevel %= Modulo;
                }

                int TargetIndex = WorryLevel % M.DivisionValue == 0 ? M.TrueTarget : M.FalseTarget;
                M.Items.pop_front();
                Monkeys[TargetIndex].Items.push_back(WorryLevel);
                ++M.NumItemsInspected;
            }
        }
    }
}

void findBestMonkeys(vector<Monkey>& Monkeys, unsigned long long& MostActive, unsigned long long& SecondMostActive)
{
    for (size_t i = 0; i < Monkeys.size(); ++i)
    {
        Monkey& M = Monkeys[i];
        if (M.NumItemsInspected >= MostActive)
        {
            SecondMostActive = MostActive;
            MostActive = M.NumItemsInspected;
        }
        else if (M.NumItemsInspected >= SecondMostActive)
        {
            SecondMostActive = M.NumItemsInspected;
        }
    }
}

unsigned long long part1(vector<string> input)
{
    vector<Monkey> Monkeys;
    parseMonkeys(input, Monkeys);

    monkeyInspection(Monkeys, 20, true);

    unsigned long long MostActive = 0, SecondMostActive = 0;
    findBestMonkeys(Monkeys, MostActive, SecondMostActive);

    return MostActive * SecondMostActive;
}

unsigned long long part2(vector<string> input)
{
    vector<Monkey> Monkeys;
    parseMonkeys(input, Monkeys);

    unsigned long long DivisionValue = 1;
    for (size_t i = 0; i < Monkeys.size(); ++i)
    {
        DivisionValue *= Monkeys[i].DivisionValue;
    }

    monkeyInspection(Monkeys, 10000, false, DivisionValue);

    unsigned long long MostActive = 0, SecondMostActive = 0;
    findBestMonkeys(Monkeys, MostActive, SecondMostActive);

    return MostActive * SecondMostActive;
}

int main()
{
    vector<string> input = readFile();

    //unsigned long long solution = part1(input);
    unsigned long long solution = part2(input);
    cout << solution << endl;
}
