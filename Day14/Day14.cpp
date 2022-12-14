// Day14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <list>

using namespace std;

struct Rock
{
    static int MAX_X;
    static int MIN_X;
    static int MAX_Y;
    static int MIN_Y;

    int X;
    int Y;

    Rock(int x, int y) : X(x), Y(y)
    {
        if (x < MIN_X) MIN_X = x;
        if (x > MAX_X) MAX_X = x;

        if (y < MIN_Y) MIN_Y = y;
        if (y > MAX_Y) MAX_Y = y;
    }
};

int Rock::MAX_X = INT_MIN;
int Rock::MIN_X = INT_MAX;
int Rock::MAX_Y = INT_MIN;
int Rock::MIN_Y = INT_MAX;

struct Path
{
    list<Rock*> Rocks;
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

void getRockPaths(vector<string> input, vector<Path>& OutPaths)
{
    for (size_t i = 0; i < input.size(); ++i)
    {
        string line = input[i];

        Path path;

        int CommaIndex = 0;
        int ArrowIndex = -3;
        do
        {
            line = line.substr(ArrowIndex + 3);

            CommaIndex = line.find(',');

            int X = stoi(line.substr(0, CommaIndex));
            line = line.substr(CommaIndex + 1);

            int Y = stoi(line.substr(0, line.find(' ')));
            ArrowIndex = line.find("->");

            if (!path.Rocks.empty())
            {
                const Rock* LastRock = path.Rocks.back();
                if (LastRock->X == X)
                {
                    int BeginY = LastRock->Y > Y ? Y : LastRock->Y;
                    int EndY = LastRock->Y > Y ? LastRock->Y : Y;
                    for (int NewY = BeginY + 1; NewY < EndY; ++NewY)
                    {
                        path.Rocks.push_back(new Rock(X, NewY));
                    }
                }
                else if (LastRock->Y == Y)
                {
                    int BeginX = LastRock->X > X ? X : LastRock->X;
                    int EndX = LastRock->X > X ? LastRock->X : X;
                    for (int NewX = BeginX + 1; NewX < EndX; ++NewX)
                    {
                        path.Rocks.push_back(new Rock(NewX, Y));
                    }
                }
                else
                {
                    cout << "Unexpected behaviour" << endl;
                }
            }

            path.Rocks.push_back(new Rock(X, Y));

        } while (ArrowIndex != -1);

        OutPaths.push_back(path);
    }
}

void buildCave(const vector<Path>& Paths, vector<vector<char>>& OutCave)
{
    OutCave = vector<vector<char>>(Rock::MAX_Y + 1, vector<char>(Rock::MAX_X - Rock::MIN_X + 1, '.'));

    for (const Path& P : Paths)
    {
        for (const Rock* R : P.Rocks)
        {
            OutCave[R->Y][R->X - Rock::MIN_X] = '#';
        }
    }
}

void printCave(const vector<vector<char>>& Cave)
{
    for (size_t i = 0; i < Cave.size(); ++i)
    {
        for (size_t j = 0; j < Cave[i].size(); ++j)
        {
            cout << Cave[i][j];
        }
        cout << endl;
    }
}

int calculateNumUnits(vector<vector<char>>& Cave)
{
    int NumSandUnits = 0;
    int StartX = 500 - Rock::MIN_X;

    bool CanPlaceSand = true;
    do
    {
        int SandX = StartX, SandY = 0;
        while (true)
        {
            if (SandY + 1 >= Cave.size() || Cave[SandY][SandX] == 'o')
            {
                CanPlaceSand = false;
                break;
            }

            if (Cave[SandY + 1][SandX] == '.')
            {
                ++SandY;
            }
            else if (SandX - 1 >= 0 && Cave[SandY + 1][SandX - 1] == '.')
            {
                --SandX;
                ++SandY;
            }
            else if (SandX + 1 < Cave[0].size() && Cave[SandY + 1][SandX + 1] == '.')
            {
                ++SandX;
                ++SandY;
            }
            else if (SandX - 1 < 0 || SandX + 1 >= Cave[0].size())
            {
                CanPlaceSand = false;
                break;
            }
            else
            {
                Cave[SandY][SandX] = 'o';
                ++NumSandUnits;
                break;
            }
        }
    } while (CanPlaceSand);

    return NumSandUnits;
}

int part1(vector<string> input)
{
    vector<Path> Paths;
    getRockPaths(input, Paths);

    vector<vector<char>> Cave;
    buildCave(Paths, Cave);

    return calculateNumUnits(Cave);
}

int part2(vector<string> input)
{
    vector<Path> Paths;
    getRockPaths(input, Paths);

    int Y = Rock::MAX_Y + 2;
    int MinX = 0, MaxX = Rock::MAX_X + Rock::MIN_X;

    Path bottomPath;
    for (int NewX = MinX; NewX <= MaxX; ++NewX)
    {
        bottomPath.Rocks.push_back(new Rock(NewX, Y));
    }

    Paths.push_back(bottomPath);

    vector<vector<char>> Cave;
    buildCave(Paths, Cave);

    return calculateNumUnits(Cave);
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}
