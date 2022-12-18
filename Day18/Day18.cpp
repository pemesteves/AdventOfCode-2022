// Day18.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

struct Position
{
    static int MaxX, MaxY, MaxZ;
    static int MinX, MinY, MinZ;
    static Position ZeroPosition;

    int X;
    int Y;
    int Z;

    Position(int x, int y, int z)
        : X(x)
        , Y(y)
        , Z(z)
    {
        if (x > MaxX) MaxX = x;
        if (y > MaxY) MaxY = y;
        if (z > MaxZ) MaxZ = z;

        if (x < MinX) MinX = x;
        if (y < MinY) MinY = y;
        if (z < MinZ) MinZ = z;
    }

    int Distance(const Position& p)
    {
        return abs(X - p.X) + abs(Y - p.Y) + abs(Z - p.Z);
    }

    bool operator==(const Position& p) const
    {
        return X == p.X && Y == p.Y && Z == p.Z;
    }
};

int Position::MaxX = 0;
int Position::MaxY = 0;
int Position::MaxZ = 0;
int Position::MinX = INT_MAX;
int Position::MinY = INT_MAX;
int Position::MinZ = INT_MAX;
Position Position::ZeroPosition = Position(0, 0, 0);

namespace std {

    template <>
    struct hash<Position>
    {
        std::size_t operator()(const Position& p) const
        {
            using std::hash;

            return ((hash<int>()(p.X) ^ (hash<int>()(p.Y) << 1)) >> 1) ^ (hash<int>()(p.Z) << 1);
        }
    };

}

struct Cube
{
    Position Pos;

    Cube(const Position& p)
        : Pos(p)
    {
    }

    vector<Cube*> AdjacentCubes;
};

void FindAdjacentCubes(vector<Cube*> InCubes)
{
    for (size_t i = 0; i < InCubes.size(); ++i)
    {
        Cube* Current = InCubes[i];
        for (size_t j = i + 1; j < InCubes.size(); ++j)
        {
            Cube* Previous = InCubes[j];

            if (Current->Pos.Distance(Previous->Pos) == 1)
            {
                Current->AdjacentCubes.push_back(Previous);
                Previous->AdjacentCubes.push_back(Current);
            }
        }
    }
}

bool RayCastX(const unordered_map<Position, Cube*>& InCubes, const Position& InPosition, bool InForward)
{
    for (int x = InForward ? InPosition.X + 1 : InPosition.X - 1; InForward ? x <= Position::MaxX : x >= Position::MinX; x += InForward ? 1 : -1)
    {
        if (InCubes.find(Position(x, InPosition.Y, InPosition.Z)) != InCubes.end())
        {
            return true;
        }
    }

    return false;
}

bool RayCastY(const unordered_map<Position, Cube*>& InCubes, const Position& InPosition, bool InForward)
{
    for (int y = InForward ? InPosition.Y + 1 : InPosition.Y - 1; InForward ? y <= Position::MaxY : y >= Position::MinY; y += InForward ? 1 : -1)
    {
        if (InCubes.find(Position(InPosition.X, y, InPosition.Z)) != InCubes.end())
        {
            return true;
        }
    }

    return false;
}

bool RayCastZ(const unordered_map<Position, Cube*>& InCubes, const Position& InPosition, bool InForward)
{
    for (int z = InForward ? InPosition.Z + 1 : InPosition.Z - 1; InForward ? z <= Position::MaxZ : z >= Position::MinZ; z += InForward ? 1 : -1)
    {
        if (InCubes.find(Position(InPosition.X, InPosition.Y, z)) != InCubes.end())
        {
            return true;
        }
    }

    return false;
}

int part1(vector<string> input)
{
    vector<Cube*> Cubes;

    string line;
    for (size_t i = 0; i < input.size(); ++i)
    {
        line = input[i];

        int CommaIndex = line.find(',');
        int x = stoi(line.substr(0, CommaIndex));
    
        line = line.substr(CommaIndex + 1);
        CommaIndex = line.find(',');
        int y = stoi(line.substr(0, CommaIndex));

        line = line.substr(CommaIndex + 1);
        int z = stoi(line);

        Cubes.push_back(new Cube(Position(x, y, z)));
    }

    FindAdjacentCubes(Cubes);

    int Solution = 0;
    for (size_t i = 0; i < Cubes.size(); ++i)
    {
        Solution += (6 - Cubes[i]->AdjacentCubes.size());
    }

    for (size_t i = 0; i < Cubes.size(); ++i)
    {
        delete Cubes[i];
    }

    return Solution;
}

int part2(vector<string> input)
{
    vector<Cube*> Cubes;
    unordered_map<Position, Cube*> CubesPositions;

    string line;
    for (size_t i = 0; i < input.size(); ++i)
    {
        line = input[i];

        int CommaIndex = line.find(',');
        int x = stoi(line.substr(0, CommaIndex));

        line = line.substr(CommaIndex + 1);
        CommaIndex = line.find(',');
        int y = stoi(line.substr(0, CommaIndex));

        line = line.substr(CommaIndex + 1);
        int z = stoi(line);

        Cube* cube = new Cube(Position(x, y, z));
        Cubes.push_back(cube);
        CubesPositions.insert({ cube->Pos, cube });
    }

    for (int x = Position::MinX; x <= Position::MaxX; ++x)
    {
        for (int y = Position::MinY; y <= Position::MaxY; ++y)
        {
            for (int z = Position::MinZ; z <= Position::MaxZ; ++z)
            {
                Position p(x, y, z);

                if (CubesPositions.find(p) != CubesPositions.end())
                {
                    continue;
                }

                unordered_set<Position> CheckedPositions;
                queue<Position> PositionsToCheck({ p });
                
                bool IsCubeInside = true;
                do
                {
                    const Position Pos = PositionsToCheck.front();
                    PositionsToCheck.pop();

                    if (CheckedPositions.find(Pos) != CheckedPositions.end())
                    {
                        continue;
                    }

                    if (CubesPositions.find(Pos) != CubesPositions.end())
                    {
                        continue;
                    }

                    CheckedPositions.insert(Pos);

                    if (Pos.X - 1 < Position::MinX || Pos.X + 1 > Position::MaxX ||
                        Pos.Y - 1 < Position::MinY || Pos.Y + 1 > Position::MaxY ||
                        Pos.Z - 1 < Position::MinZ || Pos.Z + 1 > Position::MaxZ)
                    {
                        IsCubeInside = false;
                        break;
                    }

                    PositionsToCheck.push(Position(Pos.X - 1, Pos.Y, Pos.Z));
                    PositionsToCheck.push(Position(Pos.X + 1, Pos.Y, Pos.Z));
                    PositionsToCheck.push(Position(Pos.X, Pos.Y - 1, Pos.Z));
                    PositionsToCheck.push(Position(Pos.X, Pos.Y + 1, Pos.Z));
                    PositionsToCheck.push(Position(Pos.X, Pos.Y, Pos.Z - 1));
                    PositionsToCheck.push(Position(Pos.X, Pos.Y, Pos.Z + 1));

                } while (!PositionsToCheck.empty());

                if (IsCubeInside)
                {
                    Cube* cube = new Cube(p);
                    Cubes.push_back(cube);
                    CubesPositions.insert({ p, cube });
                }
            }
        }
    }

    FindAdjacentCubes(Cubes);

    int Solution = 0;
    for (size_t i = 0; i < Cubes.size(); ++i)
    {
        Solution += (6 - Cubes[i]->AdjacentCubes.size());
    }

    for (size_t i = 0; i < Cubes.size(); ++i)
    {
        delete Cubes[i];
    }

    return Solution;
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}