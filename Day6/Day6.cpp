// Day6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

string readFile()
{
    ifstream f;
    f.open("input.txt");

    string line;

    while (!f.eof())
    {
        getline(f, line);
    }

    f.close();

    return line;
}

int findPacketStart(string input, int length)
{
    bool visited[256] = { false };

    int left = 0, right = 0;
    for (; left < input.length(); ++left)
    {
        for (right = left; right < input.length() && right < left + length; ++right)
        {
            if (visited[input[right]])
            {
                break;
            }

            visited[input[right]] = true;
        }

        if (right == left + length)
        {
            break;
        }

        for (right = left; right < input.length() && right < left + length; ++right)
        {
            visited[input[right]] = false;
        }
    }

    return right;
}

int part1(string input)
{
    return findPacketStart(input, 4);
}

int part2(string input)
{
    return findPacketStart(input, 14);
}

int main()
{
    string input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}