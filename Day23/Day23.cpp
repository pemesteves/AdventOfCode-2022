// Day23.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

/////////////////////
// DATA STRUCTURES //
/////////////////////

struct Position
{
    int Row;
    int Col;

    Position(int r, int c)
        : Row(r)
        , Col(c)
    {}

    bool operator==(const Position& p) const
    {
        return Row == p.Row && Col == p.Col;
    }

    int GetDistance(const Position& p) const
    {
        return abs(Row - p.Row) + abs(Col - p.Col);
    }
};

struct Elf
{
    Position Pos;

    mutable Position* Target;

    Elf(Position p)
        : Pos(p)
        , Target(nullptr)
    {}

    bool operator==(const Elf e) const
    {
        return Pos == e.Pos;
    }
};

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

    template<>
    struct hash<Elf>
    {
        std::size_t operator()(const Elf& e) const
        {
            return hash<Position>()(e.Pos);
        }
    };
}

long long part1(vector<string> input)
{
    unordered_set<Elf> Elves;
    unordered_set<Elf> NewElves;

    for (size_t Row = 0; Row < input.size(); ++Row)
    {
        for (size_t Col = 0; Col < input[Row].size(); ++Col)
        {
            if (input[Row][Col] == '#')
            {
                Elves.insert(Elf(Position(Row, Col)));
            }
        }
    }

    for (size_t Round = 0; Round < 10; ++Round)
    {
        for (auto it = Elves.begin(); it != Elves.end(); ++it)
        {
            Position Pos = it->Pos;

            auto Nit = Elves.find(Elf(Position(Pos.Row - 1, Pos.Col)));
            auto NEit = Elves.find(Elf(Position(Pos.Row - 1, Pos.Col + 1)));
            auto Eit = Elves.find(Elf(Position(Pos.Row, Pos.Col + 1)));
            auto SEit = Elves.find(Elf(Position(Pos.Row + 1, Pos.Col + 1)));
            auto Sit = Elves.find(Elf(Position(Pos.Row + 1, Pos.Col)));
            auto SWit = Elves.find(Elf(Position(Pos.Row + 1, Pos.Col - 1)));
            auto Wit = Elves.find(Elf(Position(Pos.Row, Pos.Col - 1)));
            auto NWit = Elves.find(Elf(Position(Pos.Row - 1, Pos.Col - 1)));

            if (Nit == Elves.end() && NEit == Elves.end() && Eit == Elves.end() && SEit == Elves.end()
                && Sit == Elves.end() && SWit == Elves.end() && Wit == Elves.end() && NWit == Elves.end())
            {
                continue;
            }

            switch (Round % 4)
            {
                case 0:
                {
                    if (Nit == Elves.end() && NEit == Elves.end() && NWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row - 1, Pos.Col);
                    }
                    else if (Sit == Elves.end() && SEit == Elves.end() && SWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row + 1, Pos.Col);
                    }
                    else if (Wit == Elves.end() && SWit == Elves.end() && NWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row, Pos.Col - 1);
                    }
                    else if (Eit == Elves.end() && SEit == Elves.end() && NEit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row, Pos.Col + 1);
                    }
                    break;
                }
                case 1:
                {
                    if (Sit == Elves.end() && SEit == Elves.end() && SWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row + 1, Pos.Col);
                    }
                    else if (Wit == Elves.end() && SWit == Elves.end() && NWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row, Pos.Col - 1);
                    }
                    else if (Eit == Elves.end() && SEit == Elves.end() && NEit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row, Pos.Col + 1);
                    }
                    else if (Nit == Elves.end() && NEit == Elves.end() && NWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row - 1, Pos.Col);
                    }
                    break;
                }
                case 2:
                {
                    if (Wit == Elves.end() && SWit == Elves.end() && NWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row, Pos.Col - 1);
                    }
                    else if (Eit == Elves.end() && SEit == Elves.end() && NEit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row, Pos.Col + 1);
                    }
                    else if (Nit == Elves.end() && NEit == Elves.end() && NWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row - 1, Pos.Col);
                    }
                    else if (Sit == Elves.end() && SEit == Elves.end() && SWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row + 1, Pos.Col);
                    }
                    break;
                }
                case 3:
                {
                    if (Eit == Elves.end() && SEit == Elves.end() && NEit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row, Pos.Col + 1);
                    }
                    else if (Nit == Elves.end() && NEit == Elves.end() && NWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row - 1, Pos.Col);
                    }
                    else if (Sit == Elves.end() && SEit == Elves.end() && SWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row + 1, Pos.Col);
                    }
                    else if (Wit == Elves.end() && SWit == Elves.end() && NWit == Elves.end())
                    {
                        it->Target = new Position(Pos.Row, Pos.Col - 1);
                    }
                    break;
                }
                default:
                {
                    cout << "Invalid value" << endl;
                    break;
                }
            }
        }

        NewElves.clear();
        for (auto it = Elves.begin(); it != Elves.end(); ++it)
        {
            if (it->Target == nullptr)
            {
                NewElves.insert(*it);
                continue;
            }

            auto it1 = it;
            ++it1;
            bool CanMove = true;
            for (; it1 != Elves.end(); ++it1)
            {
                if (it1->Target != nullptr && *it->Target == *it1->Target)
                {
                    CanMove = false;
                    it1->Target = nullptr;
                }
            }

            if (CanMove)
            {
                NewElves.insert(Elf(Position(*it->Target)));
            }
            else
            {
                it->Target = nullptr;
                NewElves.insert(*it);
            }
        }

        Elves = NewElves;
    }

    int MinRow = INT_MAX, MaxRow = INT_MIN;
    int MinCol = INT_MAX, MaxCol = INT_MIN;
    for (auto it = Elves.begin(); it != Elves.end(); ++it)
    {
        Position Pos = it->Pos;
    
        if (Pos.Row < MinRow) MinRow = Pos.Row;
        if (Pos.Row > MaxRow) MaxRow = Pos.Row;
        if (Pos.Col < MinCol) MinCol = Pos.Col;
        if (Pos.Col > MaxCol) MaxCol = Pos.Col;
    }

    return (abs(MaxRow - MinRow) + 1) * (abs(MaxCol - MinCol) + 1) - Elves.size();
}

long long part2(vector<string> input)
{
    unordered_set<Elf> Elves;
    unordered_set<Elf> NewElves;

    for (size_t Row = 0; Row < input.size(); ++Row)
    {
        for (size_t Col = 0; Col < input[Row].size(); ++Col)
        {
            if (input[Row][Col] == '#')
            {
                Elves.insert(Elf(Position(Row, Col)));
            }
        }
    }

    long Round = 0;
    while (true)
    {
        bool MovedAnyElf = false;
        for (auto it = Elves.begin(); it != Elves.end(); ++it)
        {
            Position Pos = it->Pos;

            auto Nit = Elves.find(Elf(Position(Pos.Row - 1, Pos.Col)));
            auto NEit = Elves.find(Elf(Position(Pos.Row - 1, Pos.Col + 1)));
            auto Eit = Elves.find(Elf(Position(Pos.Row, Pos.Col + 1)));
            auto SEit = Elves.find(Elf(Position(Pos.Row + 1, Pos.Col + 1)));
            auto Sit = Elves.find(Elf(Position(Pos.Row + 1, Pos.Col)));
            auto SWit = Elves.find(Elf(Position(Pos.Row + 1, Pos.Col - 1)));
            auto Wit = Elves.find(Elf(Position(Pos.Row, Pos.Col - 1)));
            auto NWit = Elves.find(Elf(Position(Pos.Row - 1, Pos.Col - 1)));

            if (Nit == Elves.end() && NEit == Elves.end() && Eit == Elves.end() && SEit == Elves.end()
                && Sit == Elves.end() && SWit == Elves.end() && Wit == Elves.end() && NWit == Elves.end())
            {
                continue;
            }

            switch (Round % 4)
            {
            case 0:
            {
                if (Nit == Elves.end() && NEit == Elves.end() && NWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row - 1, Pos.Col);
                }
                else if (Sit == Elves.end() && SEit == Elves.end() && SWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row + 1, Pos.Col);
                }
                else if (Wit == Elves.end() && SWit == Elves.end() && NWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row, Pos.Col - 1);
                }
                else if (Eit == Elves.end() && SEit == Elves.end() && NEit == Elves.end())
                {
                    it->Target = new Position(Pos.Row, Pos.Col + 1);
                }
                break;
            }
            case 1:
            {
                if (Sit == Elves.end() && SEit == Elves.end() && SWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row + 1, Pos.Col);
                }
                else if (Wit == Elves.end() && SWit == Elves.end() && NWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row, Pos.Col - 1);
                }
                else if (Eit == Elves.end() && SEit == Elves.end() && NEit == Elves.end())
                {
                    it->Target = new Position(Pos.Row, Pos.Col + 1);
                }
                else if (Nit == Elves.end() && NEit == Elves.end() && NWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row - 1, Pos.Col);
                }
                break;
            }
            case 2:
            {
                if (Wit == Elves.end() && SWit == Elves.end() && NWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row, Pos.Col - 1);
                }
                else if (Eit == Elves.end() && SEit == Elves.end() && NEit == Elves.end())
                {
                    it->Target = new Position(Pos.Row, Pos.Col + 1);
                }
                else if (Nit == Elves.end() && NEit == Elves.end() && NWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row - 1, Pos.Col);
                }
                else if (Sit == Elves.end() && SEit == Elves.end() && SWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row + 1, Pos.Col);
                }
                break;
            }
            case 3:
            {
                if (Eit == Elves.end() && SEit == Elves.end() && NEit == Elves.end())
                {
                    it->Target = new Position(Pos.Row, Pos.Col + 1);
                }
                else if (Nit == Elves.end() && NEit == Elves.end() && NWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row - 1, Pos.Col);
                }
                else if (Sit == Elves.end() && SEit == Elves.end() && SWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row + 1, Pos.Col);
                }
                else if (Wit == Elves.end() && SWit == Elves.end() && NWit == Elves.end())
                {
                    it->Target = new Position(Pos.Row, Pos.Col - 1);
                }
                break;
            }
            default:
            {
                cout << "Invalid value" << endl;
                break;
            }
            }
        }

        NewElves.clear();
        for (auto it = Elves.begin(); it != Elves.end(); ++it)
        {
            if (it->Target == nullptr)
            {
                NewElves.insert(*it);
                continue;
            }

            auto it1 = it;
            ++it1;
            bool CanMove = true;
            for (; it1 != Elves.end(); ++it1)
            {
                if (it1->Target != nullptr && *it->Target == *it1->Target)
                {
                    CanMove = false;
                    it1->Target = nullptr;
                }
            }

            if (CanMove)
            {
                MovedAnyElf = true;
                NewElves.insert(Elf(Position(*it->Target)));
            }
            else
            {
                it->Target = nullptr;
                NewElves.insert(*it);
            }
        }

        Elves = NewElves;
        ++Round;

        if (!MovedAnyElf)
        {
            break;
        }
    }

    return Round;
}

int main()
{
    vector<string> input = readFile();

    //long long solution = part1(input);
    long long solution = part2(input);
    cout << solution << endl;

    return 0;
}