// Day22.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
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

enum Direction
{
    RIGHT = 0,
    DOWN  = 1,
    LEFT  = 2,
    UP    = 3
};

enum Rotation
{
    None = 0,
    R    = 1,
    L    = 2
};

struct InstructionType
{
    bool bIsNumber;
    Rotation Rotate;
    int Value;

    InstructionType()
        : bIsNumber(false)
        , Rotate(Rotation::None)
        , Value(-1)
    {}

    void SetNumber(int v)
    {
        bIsNumber = true;
        Value = v;
    }

    void SetRotation(Rotation r)
    {
        bIsNumber = false;
        Rotate = r;
    }
};

struct Position
{
    int Row;
    int Col;

    Position(int r, int c)
        : Row(r)
        , Col(c)
    {}

    bool operator==(const Position& InPos) const
    {
        return Row == InPos.Row && Col == InPos.Col;
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
}

struct Person
{
    Position Pos;
    Direction Dir;

    Person(int r, int c, Direction d)
        : Pos(r, c)
        , Dir(d)
    {}

    void Rotate(Rotation InRotation)
    {
        switch (Dir)
        {
        case RIGHT:
            Dir = InRotation == Rotation::R ? Direction::DOWN : Direction::UP;
            break;
        case DOWN:
            Dir = InRotation == Rotation::R ? Direction::LEFT : Direction::RIGHT;
            break;
        case LEFT:
            Dir = InRotation == Rotation::R ? Direction::UP : Direction::DOWN;
            break;
        case UP:
            Dir = InRotation == Rotation::R ? Direction::RIGHT : Direction::LEFT;
            break;
        default:
            break;
        }
    }

    bool FollowDirection(const vector<string>& Map)
    {
        int MapLength = Map.size(), RowLength = Map[0].size();

        switch (Dir)
        {
            case  Direction::RIGHT:
            {
                int NextColIndex = Pos.Col;
                do
                {
                    NextColIndex = (NextColIndex + 1) % RowLength;
                } while (Map[Pos.Row][NextColIndex] == ' ');

                if (Map[Pos.Row][NextColIndex] == '#')
                {
                    return false;
                }

                Pos.Col = NextColIndex;

                break;
            }
            case Direction::DOWN:
            {
                int NextRowIndex = Pos.Row;
                do
                {
                    NextRowIndex = (NextRowIndex + 1) % MapLength;
                } while (Map[NextRowIndex][Pos.Col] == ' ');

                if (Map[NextRowIndex][Pos.Col] == '#')
                {
                    return false;
                }
                
                Pos.Row = NextRowIndex;

                break;
            }
            case Direction::LEFT:
            {
                int NextColIndex = Pos.Col;
                do
                {
                    NextColIndex = (NextColIndex - 1) % RowLength;
                    if (NextColIndex < 0)
                    {
                        NextColIndex = RowLength + NextColIndex;
                    }
                } while (Map[Pos.Row][NextColIndex] == ' ');

                if (Map[Pos.Row][NextColIndex] == '#')
                {
                    return false;
                }

                Pos.Col = NextColIndex;
               
                break;
            }
            case Direction::UP:
            {
                int NextRowIndex = Pos.Row;
                do
                {
                    NextRowIndex = (NextRowIndex - 1) % MapLength;
                    if (NextRowIndex < 0)
                    {
                        NextRowIndex = MapLength + NextRowIndex;
                    }
                } while (Map[NextRowIndex][Pos.Col] == ' ');

                if (Map[NextRowIndex][Pos.Col] == '#')
                {
                    return false;
                }

                Pos.Row = NextRowIndex;
                
                break;
            }
            default:
                cout << "Invalid direction!" << endl;
                return false;
        }

        return true;
    }

    bool FollowDirectionInCube(
        const vector<string>& Map,
        const unordered_map<Position, pair<Position, Direction>>& NextRightIndices,
        const unordered_map<Position, pair<Position, Direction>>& NextDownIndices,
        const unordered_map<Position, pair<Position, Direction>>& NextLeftIndices,
        const unordered_map<Position, pair<Position, Direction>>& NextUpIndices
    )
    {
        int MapLength = Map.size(), RowLength = Map[0].size();

        switch (Dir)
        {
        case  Direction::RIGHT:
        {
            auto it = NextRightIndices.find(Position(Pos.Row, Pos.Col));
            if (it != NextRightIndices.end())
            {
                if (Map[it->second.first.Row][it->second.first.Col] == '#')
                {
                    return false;
                }

                Pos.Row = it->second.first.Row;
                Pos.Col = it->second.first.Col;
                Dir = it->second.second;
            }
            else
            {
                if (Map[Pos.Row][Pos.Col + 1] == '#')
                {
                    return false;
                }

                Pos.Col = Pos.Col + 1;
            }

            break;
        }
        case Direction::DOWN:
        {
            auto it = NextDownIndices.find(Position(Pos.Row, Pos.Col));
            if (it != NextDownIndices.end())
            {
                if (Map[it->second.first.Row][it->second.first.Col] == '#')
                {
                    return false;
                }

                Pos.Row = it->second.first.Row;
                Pos.Col = it->second.first.Col;
                Dir = it->second.second;
            }
            else
            {
                if (Map[Pos.Row + 1][Pos.Col] == '#')
                {
                    return false;
                }

                Pos.Row = Pos.Row + 1;
            }

            break;
        }
        case Direction::LEFT:
        {
            auto it = NextLeftIndices.find(Position(Pos.Row, Pos.Col));
            if (it != NextLeftIndices.end())
            {
                if (Map[it->second.first.Row][it->second.first.Col] == '#')
                {
                    return false;
                }

                Pos.Row = it->second.first.Row;
                Pos.Col = it->second.first.Col;
                Dir = it->second.second;
            }
            else
            {
                if (Map[Pos.Row][Pos.Col - 1] == '#')
                {
                    return false;
                }

                Pos.Col = Pos.Col - 1;
            }

            break;
        }
        case Direction::UP:
        {
            auto it = NextUpIndices.find(Position(Pos.Row, Pos.Col));
            if (it != NextUpIndices.end())
            {
                if (Map[it->second.first.Row][it->second.first.Col] == '#')
                {
                    return false;
                }

                Pos.Row = it->second.first.Row;
                Pos.Col = it->second.first.Col;
                Dir = it->second.second;
            }
            else
            {
                if (Map[Pos.Row - 1][Pos.Col] == '#')
                {
                    return false;
                }

                Pos.Row = Pos.Row - 1;
            }

            break;
        }
        default:
            cout << "Invalid direction!" << endl;
            return false;
        }

        return true;
    }
};

Person* BuildMap(const vector<string>& input, vector<string>& OutMap)
{
    int MaxRowLength = 0;
    for (size_t i = 0; i < input.size() - 2; ++i)
    {
        if (input[i].size() > MaxRowLength)
        {
            MaxRowLength = (int)input[i].size();
        }
    }

    Person* OutPerson = nullptr;
    OutMap = vector<string>(input.size() - 2, string(MaxRowLength, ' '));
    for (size_t i = 0; i < input.size() - 2; ++i)
    {
        for (size_t j = 0; j < input[i].size(); ++j)
        {
            if (OutPerson == nullptr && input[i][j] == '.')
            {
                OutPerson = new Person(i, j, Direction::RIGHT);
            }

            OutMap[i][j] = input[i][j];
        }
    }

    return OutPerson;
}

vector<InstructionType> GetInstructions(string InstructionsString)
{
    vector<InstructionType> Instructions;
    ostringstream oss;

    for (size_t i = 0; i < InstructionsString.size(); ++i)
    {
        if (InstructionsString[i] == 'R' || InstructionsString[i] == 'L')
        {
            if (oss.str().size() > 0)
            {
                InstructionType NumberInst;
                NumberInst.SetNumber(stoi(oss.str()));
                Instructions.push_back(NumberInst);
                oss.str("");
            }

            InstructionType RotInst;
            RotInst.SetRotation(InstructionsString[i] == 'R' ? Rotation::R : Rotation::L);
            Instructions.push_back(RotInst);
        }
        else
        {
            oss << InstructionsString[i];
        }
    }

    if (oss.str().size() > 0)
    {
        InstructionType NumberInst;
        NumberInst.SetNumber(stoi(oss.str()));
        Instructions.push_back(NumberInst);
    }

    return Instructions;
}

long long part1(vector<string> input)
{
    vector<string> Map;
    Person* person = BuildMap(input, Map);

    vector<InstructionType> Instructions = GetInstructions(input[input.size() - 1]);

    for (size_t i = 0; i < Instructions.size(); ++i)
    {
        const InstructionType& Instruction = Instructions[i];
    
        if (Instruction.bIsNumber)
        {
            for (int v = 0; v < Instruction.Value; ++v)
            {
                if (!person->FollowDirection(Map))
                {
                    break;
                }
            }
        }
        else
        {
            person->Rotate(Instruction.Rotate);
        }
    }

    return 1000 * (person->Pos.Row + 1) + 4 * (person->Pos.Col + 1) + person->Dir;
}

long long part2(vector<string> input, bool test = false)
{
    vector<string> Map;
    Person* person = BuildMap(input, Map);

    vector<InstructionType> Instructions = GetInstructions(input[input.size() - 1]);

    // Hardcoded to avoid creating a folding algorithm
    unordered_map<Position, pair<Position, Direction>> NextRightIndices, NextDownIndices, NextLeftIndices, NextUpIndices;
    if (test)
    {
        NextRightIndices.insert({
            {Position(0, 11) , {Position(11, 15), Direction::LEFT}},
            {Position(1, 11) , {Position(10, 15), Direction::LEFT}},
            {Position(2, 11) , {Position(9, 15) , Direction::LEFT}},
            {Position(3, 11) , {Position(8, 15) , Direction::LEFT}},
            {Position(4, 11) , {Position(8, 15) , Direction::DOWN}},
            {Position(5, 11) , {Position(8, 14) , Direction::DOWN}},
            {Position(6, 11) , {Position(8, 13) , Direction::DOWN}},
            {Position(7, 11) , {Position(8, 12) , Direction::DOWN}},
            {Position(8, 15) , {Position(3, 11) , Direction::LEFT}},
            {Position(9, 15) , {Position(2, 11) , Direction::LEFT}},
            {Position(10, 15), {Position(1, 11) , Direction::LEFT}},
            {Position(11, 15), {Position(0, 11) , Direction::LEFT}}
        });

        NextDownIndices.insert({
            {Position(7, 0)  , {Position(11, 11), Direction::UP}},
            {Position(7, 1)  , {Position(11, 10), Direction::UP}},
            {Position(7, 2)  , {Position(11, 9) , Direction::UP}},
            {Position(7, 3)  , {Position(11, 8) , Direction::UP}},
            {Position(7, 4)  , {Position(11, 8) , Direction::RIGHT}},
            {Position(7, 5)  , {Position(10, 8) , Direction::RIGHT}},
            {Position(7, 6)  , {Position(9, 8)  , Direction::RIGHT}},
            {Position(7, 7)  , {Position(8, 8)  , Direction::RIGHT}},
            {Position(11, 8) , {Position(7, 3)  , Direction::UP}},
            {Position(11, 9) , {Position(7, 2)  , Direction::UP}},
            {Position(11, 10), {Position(7, 1)  , Direction::UP}},
            {Position(11, 11), {Position(7, 0)  , Direction::UP}},
            {Position(11, 12), {Position(7, 0)  , Direction::RIGHT}},
            {Position(11, 13), {Position(6, 0)  , Direction::RIGHT}},
            {Position(11, 14), {Position(5, 0)  , Direction::RIGHT}},
            {Position(11, 15), {Position(4, 0)  , Direction::RIGHT}}
        });

        NextLeftIndices.insert({
            {Position(0, 8) , {Position(4, 4)  , Direction::DOWN}},
            {Position(1, 8) , {Position(4, 5)  , Direction::DOWN}},
            {Position(2, 8) , {Position(4, 6)  , Direction::DOWN}},
            {Position(3, 8) , {Position(4, 7)  , Direction::DOWN}},
            {Position(4, 0) , {Position(11, 15), Direction::UP}},
            {Position(5, 0) , {Position(11, 14), Direction::UP}},
            {Position(6, 0) , {Position(11, 13), Direction::UP}},
            {Position(7, 0) , {Position(11, 12), Direction::UP}},
            {Position(8, 8) , {Position(7, 7)  , Direction::UP}},
            {Position(9, 8) , {Position(7, 6)  , Direction::UP}},
            {Position(10, 8), {Position(7, 5)  , Direction::UP}},
            {Position(11, 8), {Position(7, 4)  , Direction::UP}}
        });

        NextUpIndices.insert({
            {Position(0, 8) , {Position(4, 3) ,  Direction::DOWN}},
            {Position(0, 9) , {Position(4, 2) ,  Direction::DOWN}},
            {Position(0, 10), {Position(4, 1) ,  Direction::DOWN}},
            {Position(0, 11), {Position(4, 0) ,  Direction::DOWN}},
            {Position(4, 3) , {Position(0, 8) ,  Direction::DOWN}},
            {Position(4, 2) , {Position(0, 9) ,  Direction::DOWN}},
            {Position(4, 1) , {Position(0, 10),  Direction::DOWN}},
            {Position(4, 0) , {Position(0, 11),  Direction::DOWN}},
            {Position(4, 4) , {Position(0, 8) ,  Direction::RIGHT}},
            {Position(4, 5) , {Position(1, 8) ,  Direction::RIGHT}},
            {Position(4, 6) , {Position(2, 8) ,  Direction::RIGHT}},
            {Position(4, 7) , {Position(3, 8) ,  Direction::RIGHT}},
            {Position(8, 12), {Position(7, 11),  Direction::LEFT}},
            {Position(8, 13), {Position(6, 11),  Direction::LEFT}},
            {Position(8, 14), {Position(5, 11),  Direction::LEFT}},
            {Position(8, 15), {Position(4, 11),  Direction::LEFT}}
        });
    }
    else
    {
        for (size_t i = 0; i < 50; ++i)
        {
            NextUpIndices.insert({ Position(0, 50 + i), {Position(150 + i, 0), Direction::RIGHT} });
            NextLeftIndices.insert({ Position(150 + i, 0), {Position(0, 50 + i), Direction::DOWN} });

            NextUpIndices.insert({ Position(0, 100 + i), {Position(199, 0 + i), Direction::UP} });
            NextDownIndices.insert({ Position(199, 0 + i), {Position(0, 100 + i), Direction::DOWN} });

            NextRightIndices.insert({ Position(0 + i, 149), {Position(149 - i, 99), Direction::LEFT} });
            NextRightIndices.insert({ Position(149 - i, 99), {Position(0 + i, 149), Direction::LEFT} });

            NextDownIndices.insert({ Position(49, 100 + i), {Position(50 + i, 99), Direction::LEFT} });
            NextRightIndices.insert({ Position(50 + i, 99), {Position(49, 100 + i), Direction::UP} });

            NextDownIndices.insert({ Position(149, 50 + i), {Position(150 + i, 49), Direction::LEFT} });
            NextRightIndices.insert({ Position(150 + i, 49), {Position(149, 50 + i), Direction::UP} });

            NextLeftIndices.insert({ Position(50 + i, 50), {Position(100, 0 + i), Direction::DOWN} });
            NextUpIndices.insert({ Position(100, 0 + i), {Position(50+ i, 50), Direction::RIGHT} });

            NextLeftIndices.insert({ Position(0 + i, 50), {Position(149 - i, 0), Direction::RIGHT} });
            NextLeftIndices.insert({ Position(149 - i, 0), {Position(0 + i, 50), Direction::RIGHT} });
        }
    }

    for (size_t i = 0; i < Instructions.size(); ++i)
    {
        const InstructionType& Instruction = Instructions[i];

        if (Instruction.bIsNumber)
        {
            for (int v = 0; v < Instruction.Value; ++v)
            {
                if (!person->FollowDirectionInCube(Map, NextRightIndices, NextDownIndices, NextLeftIndices, NextUpIndices))
                {
                    break;
                }
            }
        }
        else
        {
            person->Rotate(Instruction.Rotate);
        }
    }

    return 1000 * (person->Pos.Row + 1) + 4 * (person->Pos.Col + 1) + person->Dir;
}

int main()
{
    vector<string> input = readFile();

    //long long solution = part1(input);
    long long solution = part2(input);
    cout << solution << endl;

    return 0;
}