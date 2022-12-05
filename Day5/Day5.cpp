// Day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <stack>

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

string part1(vector<string> input)
{
    int numStacks = input[0].size() / 3 - (input[0].size() / 9 - 1);

    vector<list<char>> stacks(numStacks, list<char>());

    string solution;
    string line;
    size_t lineIndex = 0;
    bool foundStackValues = false;
    for (; lineIndex < input.size(); ++lineIndex)
    {
        line = input[lineIndex];
        for (size_t j = 1; j < line.size(); j += 4)
        {
            if (line[j] >= '1' && line[j] <= '9')
            {
                foundStackValues = true;
                break;
            }
            else if (line[j] != ' ')
            {
                stacks[(j - 1) / 4].push_back(line[j]);
            }
        }

        if (foundStackValues)
        {
            lineIndex += 2;
            break;
        }
    }

    for (; lineIndex < input.size(); ++lineIndex)
    {
        line = input[lineIndex];
        line = line.substr(line.find(' ') + 1);
        int quantity = stoi(line.substr(0, line.find(' ')));
        line = line.substr(line.find(' ') + 1);
        line = line.substr(line.find(' ') + 1);
        int fromStack = stoi(line.substr(0, line.find(' '))) - 1;
        line = line.substr(line.find(' ') + 1);
        line = line.substr(line.find(' ') + 1);
        int toStack = stoi(line.substr(0, line.find(' '))) - 1;

        while (quantity > 0)
        {
            char c = stacks[fromStack].front();
            stacks[fromStack].pop_front();

            stacks[toStack].push_front(c);

            quantity--;
        }
    }

    for (size_t i = 0; i < stacks.size(); ++i)
    {
        solution.push_back(stacks[i].front());
    }

    return solution;
}

string part2(vector<string> input)
{
    int numStacks = input[0].size() / 3 - (input[0].size() / 9 - 1);

    vector<list<char>> stacks(numStacks, list<char>());

    string solution;
    string line;
    size_t lineIndex = 0;
    bool foundStackValues = false;
    for (; lineIndex < input.size(); ++lineIndex)
    {
        line = input[lineIndex];
        for (size_t j = 1; j < line.size(); j += 4)
        {
            if (line[j] >= '1' && line[j] <= '9')
            {
                foundStackValues = true;
                break;
            }
            else if (line[j] != ' ')
            {
                stacks[(j - 1) / 4].push_back(line[j]);
            }
        }

        if (foundStackValues)
        {
            lineIndex += 2;
            break;
        }
    }

    for (; lineIndex < input.size(); ++lineIndex)
    {
        line = input[lineIndex];
        line = line.substr(line.find(' ') + 1);
        int quantity = stoi(line.substr(0, line.find(' ')));
        line = line.substr(line.find(' ') + 1);
        line = line.substr(line.find(' ') + 1);
        int fromStack = stoi(line.substr(0, line.find(' '))) - 1;
        line = line.substr(line.find(' ') + 1);
        line = line.substr(line.find(' ') + 1);
        int toStack = stoi(line.substr(0, line.find(' '))) - 1;

        stack<char> crates;
        while (quantity > 0)
        {
            crates.push(stacks[fromStack].front());
            stacks[fromStack].pop_front();
            quantity--;
        }

        while (!crates.empty())
        {
            stacks[toStack].push_front(crates.top());
            crates.pop();
        }
    }

    for (size_t i = 0; i < stacks.size(); ++i)
    {
        solution.push_back(stacks[i].front());
    }

    return solution;
}

int main()
{
    vector<string> input = readFile();

    //string solution = part1(input);
    string solution = part2(input);
    cout << solution << endl;
}