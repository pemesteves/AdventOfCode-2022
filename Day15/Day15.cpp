// Day15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <list>

using namespace std;

struct Position
{
    static long long MIN_X, MAX_X;

    long long X;
    long long Y;

    Position(long long x, long long y)
        : X(x)
        , Y(y)
    {
        if (x < MIN_X) MIN_X = x;
        if (x > MAX_X) MAX_X = x;
    }
};

long long Position::MIN_X = INT_MAX;
long long Position::MAX_X = INT_MIN;

struct Beacon : Position
{
    Beacon(long long x, long long y)
        : Position(x, y)
    {}
};

struct Sensor : Position
{
private:
    long long Distance;
    
public:
    static long long MAX_DISTANCE;
    
    Beacon* Beacon;

    Sensor(long long x, long long y)
        : Position(x, y),
        Beacon(nullptr)
    {}

    void SetDistance(long long d)
    {
        Distance = d;
        if (d > MAX_DISTANCE) MAX_DISTANCE = d;
    }

    long long GetDistance() const
    {
        return Distance;
    }
};

long long Sensor::MAX_DISTANCE = INT_MIN;

struct Bounds
{
    long long MinX;
    long long MaxX;

    Bounds(long long InMinX, long long InMaxX)
        : MinX(InMinX)
        , MaxX(InMaxX)
    {}
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

long long manhattanDistance(Position p1, Position p2)
{
    return abs(p1.X - p2.X) + abs(p1.Y - p2.Y);
}

void findSensors(const vector<string>& InInput, vector<Sensor*>& OutSensors)
{
    string line;
    for (size_t i = 0; i < InInput.size(); ++i)
    {
        line = InInput[i].substr(InInput[i].find('=') + 1);
        long long X = stoll(line.substr(0, line.find(' ')));
        line = line.substr(line.find('=') + 1);
        long long Y = stoll(line.substr(0, line.find(' ')));

        Sensor* s = new Sensor(X, Y);

        line = line.substr(line.find('=') + 1);
        X = stoll(line.substr(0, line.find(' ')));
        line = line.substr(line.find('=') + 1);
        Y = stoll(line);

        s->Beacon = new Beacon(X, Y);
        s->SetDistance(manhattanDistance(*s, *s->Beacon));
        OutSensors.push_back(s);
    }
}

void destroySensors(const vector<Sensor*>& InSensors)
{
    for (size_t i = 0; i < InSensors.size(); ++i)
    {
        delete InSensors[i];
    }
}

long long findPositionsWhereBeaconCannotBePresent(const vector<Sensor*>& InSensors, const long long RowY)
{
    string row(abs(Position::MIN_X) + abs(Position::MAX_X) + 2 * Sensor::MAX_DISTANCE, '.');

    long long ZeroOffset = Position::MIN_X - Sensor::MAX_DISTANCE;

    for (size_t i = 0; i < InSensors.size(); ++i)
    {
        Sensor* s = InSensors[i];

        long long RowX = s->X - ZeroOffset;

        if (s->Y == RowY) row[RowX] = 'S';
        if (s->Beacon->Y == RowY) row[s->Beacon->X - ZeroOffset] = 'B';

        if (manhattanDistance(*s, Position(s->X, RowY)) > s->GetDistance())
        {
            continue;
        }

        if (row[RowX] == '.')
        {
            row[RowX] = '#';
        }

        for (long long x = RowX - 1; x >= 0; --x)
        {
            if (manhattanDistance(*s, Position(x + ZeroOffset, RowY)) > s->GetDistance())
            {
                break;
            }

            if (row[x] == '.') row[x] = '#';
        }

        for (long long x = RowX + 1; x < row.size(); ++x)
        {
            if (manhattanDistance(*s, Position(x + ZeroOffset, RowY)) > s->GetDistance())
            {
                break;
            }

            if (row[x] == '.') row[x] = '#';
        }
    }

    long long result = 0;
    for (long long x = 0; x < row.size(); ++x)
    {
        if (row[x] == '#') ++result;
    }
    return result;
}

long long part1(vector<string> input)
{
    vector<Sensor*> Sensors;
    findSensors(input, Sensors);

    long long result = findPositionsWhereBeaconCannotBePresent(Sensors, 2000000);

    destroySensors(Sensors);

    return result;
}

long long part2(vector<string> input)
{
    vector<Sensor*> Sensors;
    findSensors(input, Sensors);

    long MaxRowSize = 4000000;

    for (long long y = 0; y <= MaxRowSize; ++y)
    {
        vector<Bounds> RowBounds;
        RowBounds.push_back(Bounds(0, MaxRowSize));

        for (size_t i = 0; i < Sensors.size(); ++i)
        {
            Sensor* s = Sensors[i];

            long long MaxDistance = s->GetDistance() - abs(y - s->Y);

            if (MaxDistance < 0)
            {
                // Sensor doesn't affect this row
                continue;
            }

            long long Max = s->X + MaxDistance, Min = s->X - MaxDistance;

            for (size_t Index = 0; Index < RowBounds.size(); ++Index)
            {
                const Bounds b = RowBounds[Index];

                if (Min <= b.MinX && Max >= b.MaxX)
                {
                    RowBounds.erase(RowBounds.begin() + Index);
                    --Index;
                }
                else if (Min > b.MinX && Max < b.MaxX)  
                {
                    RowBounds.erase(RowBounds.begin() + Index);
                    RowBounds.insert(RowBounds.begin() + Index, Bounds(b.MinX, Min - 1));
                    RowBounds.insert(RowBounds.begin() + Index + 1, Bounds(Max + 1, b.MaxX));
                    ++Index;
                }
                else if (Min > b.MinX && Min <= b.MaxX)
                {
                    RowBounds.erase(RowBounds.begin() + Index);
                    RowBounds.insert(RowBounds.begin() + Index, Bounds(b.MinX, Min - 1));
                }
                else if (Max >= b.MinX && Max < b.MaxX)
                {
                    RowBounds.erase(RowBounds.begin() + Index);
                    RowBounds.insert(RowBounds.begin() + Index, Bounds(Max + 1, b.MaxX));
                }
            }
        }

        if (RowBounds.size() > 0)
        {
            const Bounds& b = RowBounds[0];

            if (b.MinX == b.MaxX)
            {
                return b.MinX * 4000000 + y;
            }
        }
    }

    destroySensors(Sensors);

    return 0;
}

int main()
{
    vector<string> input = readFile();

    //long long solution = part1(input);
    long long solution = part2(input);
    cout << solution << endl;
}