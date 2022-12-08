// Day8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
    int height = input.size();
    int length = input[0].size();

    int solution = height * length - ((height - 2) * (length - 2));

    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < length - 1; ++j)
        {
            bool isTaller = true;
            char tree = input[i][j];

            for (int k = i - 1; k >= 0; --k)
            {
                if (input[k][j] >= tree)
                {
                    isTaller = false;
                    break;
                }
            }
            if (isTaller)
            {
                solution++;
                continue;
            }

            isTaller = true;
            for (int k = i + 1; k < height; ++k)
            {
                if (input[k][j] >= tree)
                {
                    isTaller = false;
                    break;
                }
            }
            if (isTaller)
            {
                solution++;
                continue;
            }

            isTaller = true;
            for (int k = j - 1; k >= 0; --k)
            {
                if (input[i][k] >= tree)
                {
                    isTaller = false;
                    break;
                }
            }
            if (isTaller)
            {
                solution++;
                continue;
            }

            isTaller = true;
            for (int k = j + 1; k < length; ++k)
            {
                if (input[i][k] >= tree)
                {
                    isTaller = false;
                    break;
                }
            }
            if (isTaller)
            {
                solution++;
            }
        }
    }

    return solution;
}

int part2(vector<string> input)
{
    int height = input.size();
    int length = input[0].size();

    int solution = 0;
    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < length - 1; ++j)
        {
            char tree = input[i][j];
            int numTrees = 0;
            int score = 1;

            for (int k = i - 1; k >= 0; --k)
            {
                numTrees++;
                if (input[k][j] >= tree)
                {
                    break;
                }
            }

            score *= numTrees;
            numTrees = 0;
            for (int k = i + 1; k < height; ++k)
            {
                numTrees++;
                if (input[k][j] >= tree)
                {
                    break;
                }
            }

            score *= numTrees;
            numTrees = 0;
            for (int k = j - 1; k >= 0; --k)
            {
                numTrees++;
                if (input[i][k] >= tree)
                {
                    break;
                }
            }

            score *= numTrees;
            numTrees = 0;
            for (int k = j + 1; k < length; ++k)
            {
                numTrees++;
                if (input[i][k] >= tree)
                {
                    break;
                }
            }

            score *= numTrees;
            if (score > solution)
            {
                solution = score;
            }
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
