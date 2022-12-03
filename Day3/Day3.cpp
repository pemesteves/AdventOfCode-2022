// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
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

int countPriorities(unordered_set<char> sharedItems)
{
    int prioritiesSum = 0;
    for (auto it = sharedItems.begin(); it != sharedItems.end(); ++it)
    {
        char c = *it;
        prioritiesSum += (
            c >= 'a' && c <= 'z' ? (c - 'a' + 1) : (c - 'A' + 27)
        );
    }
    return prioritiesSum;
}

int part1(vector<string> input)
{
    int solution = 0;
    for (size_t i = 0; i < input.size(); i++)
    {
        string line = input[i];

        // Set first rucksack
        unordered_set<char> firstRucksack;
        size_t halfSize = line.size() / 2;
        for (size_t j = 0; j < halfSize; ++j)
        {
            firstRucksack.insert(line[j]);
        }

        unordered_set<char> sharedItems;
        for (size_t j = halfSize; j < line.size(); ++j)
        {
            if (firstRucksack.find(line[j]) != firstRucksack.end())
            {
                sharedItems.insert(line[j]);
            }
        }

        solution += countPriorities(sharedItems);
    }

    return solution;
}

int part2(vector<string> input)
{
    int solution = 0;
    for (size_t i = 0; i < input.size(); i += 3)
    {
        string line = input[i];

        // Set first rucksack
        unordered_set<char> firstRucksack;
        for (size_t j = 0; j < line.size(); ++j)
        {
            firstRucksack.insert(line[j]);
        }

        line = input[i + 1];
        // Set shared items between the first and second rucksack
        unordered_set<char> secondRucksack;
        for (size_t j = 0; j < line.size(); ++j)
        {
            if (firstRucksack.find(line[j]) != firstRucksack.end())
            {
                secondRucksack.insert(line[j]);
            }
        }

        line = input[i + 2];
        // Set shared items
        unordered_set<char> sharedItems;
        for (size_t j = 0; j < line.size(); ++j)
        {
            if (secondRucksack.find(line[j]) != secondRucksack.end())
            {
                sharedItems.insert(line[j]);
            }
        }

        solution += countPriorities(sharedItems);
    }

    return solution;
}

int main()
{
    vector<string> input = readFile();

    // int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}

