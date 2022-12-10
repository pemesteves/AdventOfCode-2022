// Day10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

using namespace std;

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

int part1(vector<string> input)
{
    int X = 1;
    int NumCycles = 0;

    int Solution = 0;
    string Line;
    for (size_t i = 0; i < input.size(); ++i)
    {
        Line = input[i];
        if (Line == "noop")
        {
            ++NumCycles;

            if (NumCycles == 20 || (NumCycles - 20) % 40 == 0)
            {
                Solution += (NumCycles * X);
            }
        }
        else
        {
            Line = Line.substr(Line.find(' ') + 1);

            if (NumCycles == 19 || (NumCycles - 20) % 40 == 39)
            {
                Solution += ((NumCycles  + 1) * X);
            }

            NumCycles += 2;

            if (NumCycles == 20 || (NumCycles - 20) % 40 == 0)
            {
                Solution += (NumCycles * X);
            }

            X += stoi(Line);
        }
    }

    return Solution;
}

int part2(vector<string> input)
{
    int X = 1;
    int NumCycles = 0;

    vector<vector<char>> CRT(6, vector<char>(40, '.'));

    string Line;
    for (size_t i = 0; i < input.size(); ++i)
    {
        Line = input[i];
        if (Line == "noop")
        {
            if (NumCycles % 40 >= X - 1 && NumCycles % 40 <= X + 1)
            {
                CRT[NumCycles / 40][NumCycles % 40] = '#';
            }

            ++NumCycles;
        }
        else
        {
            Line = Line.substr(Line.find(' ') + 1);

            if (NumCycles % 40 >= X - 1 && NumCycles % 40 <= X + 1)
            {
                CRT[NumCycles / 40][NumCycles % 40] = '#';
            }

            ++NumCycles;

            if (NumCycles % 40 >= X - 1 && NumCycles % 40 <= X + 1)
            {
                CRT[NumCycles / 40][NumCycles % 40] = '#';
            }

            ++NumCycles;

            X += stoi(Line);
        }
    }

    for (size_t i = 0; i < CRT.size(); ++i)
    {
        for (size_t j = 0; j < CRT[i].size(); ++j)
        {
            cout << CRT[i][j];
        }
        cout << endl;
    }

    return 0;
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}