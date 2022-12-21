// Day21.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

enum Operation
{
    None,
    Addition,
    Subtraction,
    Product,
    Division
};

struct Monkey
{
    Operation operation;
    string leftMonkey;
    string rightMonkey;
    long long value;

    Monkey(string InLeft, string InRight, Operation InOperation)
        : leftMonkey(InLeft)
        , rightMonkey(InRight)
        , operation(InOperation)
        , value(0)
    {}

    Monkey(long long InValue)
        : leftMonkey("")
        , rightMonkey("")
        , operation(Operation::None)
        , value(InValue)
    {}

    long long CalculateValue(const unordered_map<string, Monkey*>& InMonkeys)
    {
        if (operation == Operation::None)
        {
            return value;
        }

        auto MonkeyIT1 = InMonkeys.find(leftMonkey);
        auto MonkeyIT2 = InMonkeys.find(rightMonkey);

        switch (operation)
        {
            case Operation::Addition:
                return MonkeyIT1->second->CalculateValue(InMonkeys) + MonkeyIT2->second->CalculateValue(InMonkeys);
            case Operation::Subtraction:
                return MonkeyIT1->second->CalculateValue(InMonkeys) - MonkeyIT2->second->CalculateValue(InMonkeys);
            case Operation::Product:
                return MonkeyIT1->second->CalculateValue(InMonkeys) * MonkeyIT2->second->CalculateValue(InMonkeys);
            case Operation::Division:
                return MonkeyIT1->second->CalculateValue(InMonkeys) / MonkeyIT2->second->CalculateValue(InMonkeys);
        }

        cout << "Invalid monkey!" << endl;
        return 0;
    }

    string CalculateStringValue(const unordered_map<string, Monkey*>& InMonkeys)
    {
        if (operation == Operation::None)
        {
            return to_string(value);
        }

        auto MonkeyIT1 = InMonkeys.find(leftMonkey);
        auto MonkeyIT2 = InMonkeys.find(rightMonkey);

        string M1Value = leftMonkey == "humn" ? "x" : MonkeyIT1->second->CalculateStringValue(InMonkeys);
        string M2Value = rightMonkey == "humn" ? "x" : MonkeyIT2->second->CalculateStringValue(InMonkeys);

        bool IsM1Num = M1Value[0] >= '0' && M1Value[0] <= '0';
        bool IsM2Num = M2Value[0] >= '0' && M2Value[0] <= '0';

        switch (operation)
        {
        case Operation::Addition:
            if (IsM1Num && IsM2Num)
            {
                return to_string(stoll(M1Value) + stoll(M2Value));
            }
            return "(" + M1Value + " + " + M2Value + ")";
        case Operation::Subtraction:
            if (IsM1Num && IsM2Num)
            {
                return to_string(stoll(M1Value) - stoll(M2Value));
            }
            return "(" + M1Value + " - " + M2Value + ")";
        case Operation::Product:
            if (IsM1Num && IsM2Num)
            {
                return to_string(stoll(M1Value) * stoll(M2Value));
            }
            return "(" + M1Value + " * " + M2Value + ")";
        case Operation::Division:
            if (IsM1Num && IsM2Num)
            {
                return to_string(stoll(M1Value) / stoll(M2Value));
            }
            return "(" + M1Value + " / " + M2Value + ")";
        }

        cout << "Invalid monkey!" << endl;
        return 0;
    }
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

void CreateMonkeys(vector<string> input, unordered_map<string, Monkey*>& OutMonkeys)
{
    for (size_t i = 0; i < input.size(); ++i)
    {
        string line = input[i];

        long long StringIndex = line.find(':');
        string MonkeyName = line.substr(0, StringIndex);

        line = line.substr(StringIndex + 2);

        StringIndex = line.find(' ');
        if (StringIndex == string::npos)
        {
            long long MonkeyValue = stoi(line);
            OutMonkeys.insert({ MonkeyName, new Monkey(MonkeyValue) });
            continue;
        }

        string FirstMonkeyName = line.substr(0, StringIndex);
        Operation op = Operation::None;
        string SecondMonkeyName;

        line = line.substr(StringIndex + 1);
        switch (line[0])
        {
        case '*':
            op = Operation::Product;
            break;
        case '+':
            op = Operation::Addition;
            break;
        case '-':
            op = Operation::Subtraction;
            break;
        case '/':
            op = Operation::Division;
            break;
        default:
            break;
        }

        StringIndex = line.find(' ');
        line = line.substr(StringIndex + 1);
        SecondMonkeyName = line;

        OutMonkeys.insert({ MonkeyName, new Monkey(FirstMonkeyName, SecondMonkeyName, op) });
    }
}

long long part1(vector<string> input)
{
    unordered_map<string, Monkey*> Monkeys;
    CreateMonkeys(input, Monkeys);

    return Monkeys.find("root")->second->CalculateValue(Monkeys);
}

long long part2(vector<string> input)
{
    unordered_map<string, Monkey*> Monkeys;
    CreateMonkeys(input, Monkeys);

    Monkey* RootMonkey = Monkeys.find("root")->second;

    auto leftIT = Monkeys.find(RootMonkey->leftMonkey);
    auto rightIT = Monkeys.find(RootMonkey->rightMonkey);

    cout << leftIT->second->CalculateStringValue(Monkeys) << " = " << rightIT->second->CalculateStringValue(Monkeys) << endl;
    // ((33 + ((59078404933780 - (13 * ((((((2 * (((509 + (6 * (((300 + (((2 * (((905 + (((((168 + (((((671 + ((((527 + ((((74 + (((((2 * ((((((((3 * (((981 + ((9 * (((((((x - 365) * 32) + 496) / 12) + 330) / 2) - 405)) + 689)) / 2) - 489)) - 420) / 3) + 590) * 4) - 283) / 5) + 632)) + 678) * 3) - 877) / 5)) * 7) - 931) / 2)) / 5) - 530) * 3)) / 8) - 61) * 20) + 391)) + 632) / 3) - 724) * 2)) / 3) - 492)) - 346) * 2)) / 4) - 946))) / 5) + 952)) - 863) / 11) + 15) / 2) - 716))) * 3)) / 3) = 28379346560301
    // Partially solved by hand:
    // ((905 + (((((168 + (((((671 + ((((527 + ((((74 + (((((2 * ((((((((3 * (((981 + ((9 * (((((((x - 365) * 32) + 496) / 12) + 330) / 2) - 405)) + 689)) / 2) - 489)) - 420) / 3) + 590) * 4) - 283) / 5) + 632)) + 678) * 3) - 877) / 5)) * 7) - 931) / 2)) / 5) - 530) * 3)) / 8) - 61) * 20) + 391)) + 632) / 3) - 724) * 2)) / 3) = 21646771937537
    // Used https://www.mathpapa.com/equation-solver/ to obtain the final solution

    return 0;
}

int main()
{
    vector<string> input = readFile();

    long long solution = part1(input);
    //long long solution = part2(input);
    cout << solution << endl;

    return 0;
}