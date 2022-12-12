// Day12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <list>

#include "Graph.h"

using namespace std;

struct Square
{
    int X;
    int Y;
    char Value;

    Square() : Square(0, 0, '\0') {}

    Square(int x, int y, char v) : X(x), Y(y), Value(v) {}

    bool operator==(const Square& p) const
    {
        return X == p.X && Y == p.Y;
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

void generateGraph(vector<string> &input, Graph<Square>& graph, Square& Start, Square& End)
{
    for (size_t i = 0; i < input.size(); ++i)
    {
        for (size_t j = 0; j < input[i].size(); ++j)
        {
            Square S(i, j, input[i][j]);
            graph.addVertex(S);

            if (input[i][j] == 'S')
            {
                Start = S;
                input[i][j] = 'a';
            }
            else if (input[i][j] == 'E')
            {
                End = S;
                input[i][j] = 'z';
            }
        }
    }

    for (size_t i = 0; i < input.size(); ++i)
    {
        for (size_t j = 0; j < input[i].size(); ++j)
        {
            char CurrentChar = input[i][j];
            Square CurrentSquare(i, j, CurrentChar);

            if (i < input.size() - 1)
            {
                if (CurrentChar - input[i + 1][j] >= -1 && CurrentChar - input[i + 1][j] <= 1)
                {
                    if (!graph.addEdge(CurrentSquare, Square(i + 1, j, input[i + 1][j]), 1))
                    {
                        cout << "Could not add edge between (" << i << ", " << j << ") and (" << (i + 1) << ", " << j << ")" << endl;
                    }

                    if (!graph.addEdge(Square(i + 1, j, input[i + 1][j]), CurrentSquare, 1))
                    {
                        cout << "Could not add edge between (" << (i + 1) << ", " << j << ") and (" << i << ", " << j << ")" << endl;
                    }
                }
                else if (CurrentChar - input[i + 1][j] > 1)
                {
                    if (!graph.addEdge(CurrentSquare, Square(i + 1, j, input[i + 1][j]), 1))
                    {
                        cout << "Could not add edge between (" << i << ", " << j << ") and (" << (i + 1) << ", " << j << ")" << endl;
                    }
                }
                else if (CurrentChar - input[i + 1][j] < -1)
                {
                    if (!graph.addEdge(Square(i + 1, j, input[i + 1][j]), CurrentSquare, 1))
                    {
                        cout << "Could not add edge between (" << i << ", " << j << ") and (" << (i + 1) << ", " << j << ")" << endl;
                    }
                }
            }

            if (j < input[i].size() - 1)
            {
                if (CurrentChar - input[i][j + 1] >= -1 && CurrentChar - input[i][j + 1] <= 1)
                {
                    if (!graph.addEdge(CurrentSquare, Square(i, j + 1, input[i][j + 1]), 1))
                    {
                        cout << "Could not add edge between (" << i << ", " << j << ") and (" << i << ", " << (j + 1) << ")" << endl;
                    }

                    if (!graph.addEdge(Square(i, j + 1, input[i][j + 1]), CurrentSquare, 1))
                    {
                        cout << "Could not add edge between (" << i << ", " << (j + 1) << ") and (" << i << ", " << j << ")" << endl;
                    }
                }
                else if (CurrentChar - input[i][j + 1] > 1)
                {
                    if (!graph.addEdge(CurrentSquare, Square(i, j + 1, input[i][j + 1]), 1))
                    {
                        cout << "Could not add edge between (" << i << ", " << (j + 1) << ") and (" << i << ", " << j << ")" << endl;
                    }
                }
                else if (CurrentChar - input[i][j + 1] < -1)
                {
                    if (!graph.addEdge(Square(i, j + 1, input[i][j + 1]), CurrentSquare, 1))
                    {
                        cout << "Could not add edge between (" << i << ", " << (j + 1) << ") and (" << i << ", " << j << ")" << endl;
                    }
                }
            }
        }
    }
}

int part1(vector<string> input)
{
    Graph<Square> graph;
    Square Start, End;

    generateGraph(input, graph, Start, End);

    graph.dijkstraShortestPath(Start);

    return graph.getPath(Start, End).size() - 1;
}

int part2(vector<string> input)
{
    Graph<Square> graph;
    Square Start, End;

    generateGraph(input, graph, Start, End);

    size_t MinSize = INT_MAX;
    for (size_t i = 0; i < input.size(); ++i)
    {
        for (size_t j = 0; j < input[i].size(); ++j)
        {
            if (input[i][j] == 'a')
            {
                Square CurrentSquare(i, j, input[i][j]);
                graph.dijkstraShortestPath(CurrentSquare);
                size_t Size = graph.getPath(CurrentSquare, End).size() - 1;
                if (Size > 0 && Size < MinSize)
                {
                    MinSize = Size;
                }
            }
        }
    }

    return MinSize;
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}