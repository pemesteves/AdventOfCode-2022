// Day16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

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
    return 0;
}

int part2(vector<string> input)
{
    return 0;
}

int main()
{
    vector<string> input = readFile();

    int solution = part1(input);
    //int solution = part2(input);
    cout << solution << endl;
}