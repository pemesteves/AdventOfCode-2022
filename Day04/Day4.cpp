// Day4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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

bool oneFullyContainOther(int& min_first, int& max_first, int& min_second, int& max_second)
{
    return (min_first <= min_second && max_first >= max_second)
        || (min_first >= min_second && max_first <= max_second);
}

int part1(vector<string> input)
{
    int solution = 0;
    int min_first = 0, max_first = 0, min_second = 0, max_second = 0;
    char blank;
    for (size_t i = 0; i < input.size(); ++i)
    {
        istringstream iss(input[i]);

        iss >> min_first >> blank >> max_first >> blank >> min_second >> blank >> max_second;

        if (oneFullyContainOther(min_first, max_first, min_second, max_second))
        {
            solution++;
        }
    }

    return solution;
}

int part2(vector<string> input)
{
    int solution = 0;
    int min_first = 0, max_first = 0, min_second = 0, max_second = 0;
    char blank;
    for (size_t i = 0; i < input.size(); ++i)
    {
        istringstream iss(input[i]);

        iss >> min_first >> blank >> max_first >> blank >> min_second >> blank >> max_second;

        if (oneFullyContainOther(min_first, max_first, min_second, max_second)
            || (min_first <= min_second && min_second <= max_first)
            || (min_first <= max_second && max_second <= max_first))
        {
            solution++;
        }
    }

    return solution;
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}