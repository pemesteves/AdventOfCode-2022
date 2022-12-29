// Day24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
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

/////////////////////
// DATA STRUCTURES //
/////////////////////

struct Position
{
    static Position MAX_POS;
    static Position MIN_POS;

    int Row;
    int Col;

    Position()
        : Position(0, 0)
    {}

    Position(int r, int c)
        : Row(r)
        , Col(c)
    {}

    bool operator==(const Position& p) const
    {
        return Row == p.Row && Col == p.Col;
    }

    bool IsWithinBounds(const Position& p1, const Position& p2) const
    {
        if (*this == p1 || *this == p2) return true;

        return Row >= MIN_POS.Row && Row <= MAX_POS.Row
            && Col >= MIN_POS.Col && Col <= MAX_POS.Col;
    }
};

Position Position::MAX_POS = Position(0, 0);
Position Position::MIN_POS = Position(0, 0);

enum Direction
{
    UP = 0,
    DOWN = 1,
    RIGHT = 2,
    LEFT = 3
};

struct Blizzard
{
    Position Pos;
    Direction Dir;

    Blizzard(int row, int col, Direction d)
        : Pos(row, col)
        , Dir(d)
    {}

    void UpdateBlizzard()
    {
        switch (Dir)
        {
            case Direction::UP:
                --Pos.Row;
                if (Pos.Row < Position::MIN_POS.Row) Pos.Row = Position::MAX_POS.Row;
                break;
            case Direction::DOWN:
                ++Pos.Row;
                if (Pos.Row > Position::MAX_POS.Row) Pos.Row = Position::MIN_POS.Row;
                break;
            case Direction::LEFT:
                --Pos.Col;
                if (Pos.Col < Position::MIN_POS.Col) Pos.Col = Position::MAX_POS.Col;
                break;
            case Direction::RIGHT:
                ++Pos.Col;
                if (Pos.Col > Position::MAX_POS.Col) Pos.Col = Position::MIN_POS.Col;
                break;
            default:
                cout << "Invalid direction!" << endl;
                break;
        }
    }
};

////////////////////
// HASH FUNCTIONS //
////////////////////

namespace std
{
    template <>
    struct hash<Position>
    {
        std::size_t operator()(const Position& p) const
        {
            return (hash<int>()(p.Row) ^ (hash<int>()(p.Col) << 1));
        }
    };
}

//////////////
// SOLUTION //
//////////////

vector<Blizzard> ParseBlizzards(const vector<string>& input)
{
    vector<Blizzard> Blizzards;
    for (size_t i = 0; i < input.size(); ++i)
    {
        for (size_t j = 0; j < input[i].size(); ++j)
        {
            if (input[i][j] == '^')
            {
                Blizzards.push_back(Blizzard(i, j, Direction::UP));
            }
            else if (input[i][j] == 'v')
            {
                Blizzards.push_back(Blizzard(i, j, Direction::DOWN));
            }
            else if (input[i][j] == '>')
            {
                Blizzards.push_back(Blizzard(i, j, Direction::RIGHT));
            }
            else if (input[i][j] == '<')
            {
                Blizzards.push_back(Blizzard(i, j, Direction::LEFT));
            }
        }
    }
    return Blizzards;
}

Position ParsePosition(const vector<string>& input, int Row)
{
    for (size_t i = 0; i < input[Row].size(); ++i)
    {
        if (input[Row][i] == '.')
        {
            return Position(Row, i);
        }
    }
    return Position();
}

Position ParseStartPosition(const vector<string>& input)
{
    return ParsePosition(input, 0);
}

Position ParseTargetPosition(const vector<string>& input)
{
    return ParsePosition(input, input.size() - 1);
}

int findPath(const Position& StartPosition, const Position& TargetPosition, vector<Blizzard>& Blizzards, bool IsTargetDown = true)
{
    unordered_set<Position> ExpeditionPositions = { StartPosition };

    unordered_set<Position> BlizzardPositions;
    int NumIterations = 0;
    while (true)
    {
        BlizzardPositions.clear();
        for (size_t i = 0; i < Blizzards.size(); ++i)
        {
            Blizzards[i].UpdateBlizzard();
            BlizzardPositions.insert(Blizzards[i].Pos);
        }

        unordered_set<Position> NewPositions;
        for (auto it = ExpeditionPositions.begin(); it != ExpeditionPositions.end(); ++it)
        {
            Position Pos = *it;

            Position UpPos = Position(Pos.Row - 1, Pos.Col);
            Position DownPos = Position(Pos.Row + 1, Pos.Col);
            Position LeftPos = Position(Pos.Row, Pos.Col - 1);
            Position RightPos = Position(Pos.Row, Pos.Col + 1);

            if (IsTargetDown && DownPos == TargetPosition)
            {
                return NumIterations + 1;
            }
            else if (!IsTargetDown && UpPos == TargetPosition)
            {
                return NumIterations + 1;
            }

            // CanStay
            if (Pos.IsWithinBounds(StartPosition, TargetPosition) && BlizzardPositions.find(Pos) == BlizzardPositions.end()) NewPositions.insert(Pos);

            // CanGoUp
            if (UpPos.IsWithinBounds(StartPosition, TargetPosition) && BlizzardPositions.find(UpPos) == BlizzardPositions.end()) NewPositions.insert(UpPos);

            // CanGoDown
            if (DownPos.IsWithinBounds(StartPosition, TargetPosition) && BlizzardPositions.find(DownPos) == BlizzardPositions.end()) NewPositions.insert(DownPos);

            // CanGoLeft
            if (LeftPos.IsWithinBounds(StartPosition, TargetPosition) && BlizzardPositions.find(LeftPos) == BlizzardPositions.end()) NewPositions.insert(LeftPos);

            // CanGoRight
            if (RightPos.IsWithinBounds(StartPosition, TargetPosition) && BlizzardPositions.find(RightPos) == BlizzardPositions.end()) NewPositions.insert(RightPos);
        }

        ExpeditionPositions = NewPositions;
        ++NumIterations;
    }

    return 0;
}

int part1(vector<string> input)
{
    Position::MAX_POS = Position(input.size() - 2, input[0].size() - 2);
    Position::MIN_POS = Position(1, 1);

    vector<Blizzard> Blizzards = ParseBlizzards(input);

    Position StartPosition = ParseStartPosition(input);
    Position TargetPosition = ParseTargetPosition(input);

    return findPath(StartPosition, TargetPosition, Blizzards);
}

int part2(vector<string> input)
{
    Position::MAX_POS = Position(input.size() - 2, input[0].size() - 2);
    Position::MIN_POS = Position(1, 1);

    vector<Blizzard> Blizzards = ParseBlizzards(input);

    Position StartPosition = ParseStartPosition(input);
    Position TargetPosition = ParseTargetPosition(input);

    return findPath(StartPosition, TargetPosition, Blizzards)
         + findPath(TargetPosition, StartPosition, Blizzards, false)
         + findPath(StartPosition, TargetPosition, Blizzards);
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;

    return 0;
}