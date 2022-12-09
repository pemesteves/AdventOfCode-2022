// Day9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

using namespace std;

struct Pair
{
    int Line;
    int Column;

    Pair(int l, int c) : Line(l), Column(c) {}

    bool operator==(const Pair& p) const
    {
        return Line == p.Line && Column == p.Column;
    }
};

namespace std
{
    template <> struct hash<Pair>
    {
        std::size_t operator()(Pair const& p) const noexcept
        {
            return p.Line * p.Column + p.Line;
        }
    };
}

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

bool updatePair(Pair& previous, Pair& current)
{
    if (previous.Column - current.Column > 1)
    {
        ++current.Column;
        if (previous.Line > current.Line)
        {
            ++current.Line;
        }
        else if (previous.Line < current.Line)
        {
            --current.Line;
        }

        return true;
    }

    if (previous.Column - current.Column < -1)
    {
        --current.Column;
        if (previous.Line > current.Line)
        {
            ++current.Line;
        }
        else if (previous.Line < current.Line)
        {
            --current.Line;
        }

        return true;
    }

    if (previous.Line - current.Line > 1)
    {
        ++current.Line;
        if (previous.Column > current.Column)
        {
            ++current.Column;
        }
        else if (previous.Column < current.Column)
        {
            --current.Column;
        }

        return true;
    }

    if (previous.Line - current.Line < -1)
    {
        --current.Line;
        if (previous.Column > current.Column)
        {
            ++current.Column;
        }
        else if (previous.Column < current.Column)
        {
            --current.Column;
        }

        return true;
    }

    return false;
}

int part1(vector<string> input)
{
    Pair HeadPos = Pair(0, 0);
    Pair TailPos = Pair(0, 0);
 
    unordered_set<Pair> VisitedPositions = { TailPos };

    char op;
    int times;
    string line;
    for (size_t i = 0; i < input.size(); ++i)
    {
        line = input[i];
        op = line[0];
        times = stoi(line.substr(line.find(' ') + 1));

        for (int j = 0; j < times; ++j)
        {
            switch (op)
            {
                case 'R':
                    ++HeadPos.Column;
                    if (HeadPos.Column - TailPos.Column > 1)
                    {
                        TailPos.Line = HeadPos.Line;
                        ++TailPos.Column;
                    }
                    break;
                case 'L':
                    --HeadPos.Column;
                    if (HeadPos.Column - TailPos.Column < -1)
                    {
                        TailPos.Line = HeadPos.Line;
                        --TailPos.Column;
                    }
                    break;
                case 'D':
                    ++HeadPos.Line;
                    if (HeadPos.Line - TailPos.Line > 1)
                    {
                        TailPos.Column = HeadPos.Column;
                        ++TailPos.Line;
                    }
                    break;
                case 'U':
                    --HeadPos.Line;
                    if (HeadPos.Line - TailPos.Line < -1)
                    {
                        TailPos.Column = HeadPos.Column;
                        --TailPos.Line;
                    }
                    break;
                default:
                    break;
            }

            VisitedPositions.insert(TailPos);
        }
    }

    return VisitedPositions.size();
}

int part2(vector<string> input)
{
    vector<Pair> RopePos = vector<Pair>(10, Pair(0, 0));

    unordered_set<Pair> VisitedPositions = { RopePos[9] };

    char op;
    int times;
    string line;
    for (size_t i = 0; i < input.size(); ++i)
    {
        line = input[i];
        op = line[0];
        times = stoi(line.substr(line.find(' ') + 1));

        for (int j = 0; j < times; ++j)
        {
            switch (op)
            {
            case 'R':
                ++RopePos[0].Column;
                for (size_t k = 1; k < RopePos.size(); ++k)
                {
                    if (!updatePair(RopePos[k - 1], RopePos[k]))
                    {
                        break;
                    }
                }
                break;
            case 'L':
                --RopePos[0].Column;
                for (size_t k = 1; k < RopePos.size(); ++k)
                {
                    if (!updatePair(RopePos[k - 1], RopePos[k]))
                    {
                        break;
                    }
                }
                break;
            case 'D':
                ++RopePos[0].Line;
                for (size_t k = 1; k < RopePos.size(); ++k)
                {
                    if (!updatePair(RopePos[k - 1], RopePos[k]))
                    {
                        break;
                    }
                }
                break;
            case 'U':
                --RopePos[0].Line;
                for (size_t k = 1; k < RopePos.size(); ++k)
                {
                    if (!updatePair(RopePos[k - 1], RopePos[k]))
                    {
                        break;
                    }
                }
                break;
            default:
                break;
            }

            VisitedPositions.insert(RopePos[9]);
        }
    }

    return VisitedPositions.size();
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}