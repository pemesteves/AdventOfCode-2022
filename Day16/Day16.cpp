// Day16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
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

struct Valve
{
    string Name;
    int FlowRate;
    vector<Valve*> Valves;

    Valve(string n, int f)
        : Name(n)
        , FlowRate(f)
        , Valves()
    {}

    bool operator==(const Valve& v) const
    {
        return Name == v.Name;
    }
};

struct Path
{
    static int MaxPressureReleased;

    string CurrentValveName;
    int CurrentPressureReleased;
    int OpenValvesPressure;
    unordered_set<string> OpenValves;

    Path(string v, int p, int o, unordered_set<string> ov)
        : CurrentValveName(v)
        , CurrentPressureReleased(p)
        , OpenValvesPressure(o)
        , OpenValves(ov)
    {
        if (CurrentPressureReleased > MaxPressureReleased) MaxPressureReleased = CurrentPressureReleased;
    }

    void OpenValve(Valve* v)
    {
        OpenValvesPressure += v->FlowRate;
        OpenValves.insert(v->Name);
    }

    bool operator==(const Path& p) const
    {
        return CurrentValveName == p.CurrentValveName
            && CurrentPressureReleased == p.CurrentPressureReleased
            && OpenValvesPressure == p.OpenValvesPressure;
    }
};

int Path::MaxPressureReleased = 0;

namespace std
{
    template <>
    struct hash<Path>
    {
        std::size_t operator()(const Path& p) const
        {
            return (hash<string>()(p.CurrentValveName)
                ^ (hash<int>()(p.CurrentPressureReleased) << 1)
                ^ (hash<int>()(p.OpenValvesPressure) << 2));
        }
    };
};

unordered_map<string, Valve*> ParseValves(const vector<string>& input)
{
    unordered_map<string, Valve*> Valves;

    vector<string> ValvesNames;
    for (size_t i = 0; i < input.size(); ++i)
    {
        string line = input[i].substr(input[i].find(' ') + 1);

        string Name = line.substr(0, line.find(' '));
        line = line.substr(line.find('=') + 1);
        int FlowRate = stoi(line.substr(0, line.find(';')));

        Valves.insert({ Name, new Valve(Name, FlowRate) });
        ValvesNames.push_back(Name);
    }

    // i is also the index of the valve
    for (size_t i = 0; i < input.size(); ++i)
    {
        Valve* v = Valves.find(ValvesNames[i])->second;

        string line = input[i].substr(input[i].find("to") + 3);
        line = line.substr(line.find(' ') + 1);
        int CommaIndex = 0;
        do
        {
            CommaIndex = line.find(',');

            string ValveName = line.substr(0, CommaIndex);

            v->Valves.push_back(Valves.find(ValveName)->second);

            line = line.substr(CommaIndex + 2);
        } while (CommaIndex != string::npos);
    }

    return Valves;
}

//////////////
// SOLUTION //
//////////////

int part1(vector<string> input)
{
    unordered_map<string, Valve*> Valves = ParseValves(input);

    unordered_set<Path> VisitedPaths;
    unordered_set<Path> Paths = { Path("AA", 0, 0, {}) };

    for (int Minute = 1; Minute <= 30; ++Minute)
    {
        unordered_set<Path> NewPaths;

        for (auto it = Paths.begin(); it != Paths.end(); ++it)
        {
            Path p = *it;

            // Already visited this path
            if (VisitedPaths.find(p) != VisitedPaths.end())
            {
                continue;
            }

            VisitedPaths.insert(p);

            p.CurrentPressureReleased += p.OpenValvesPressure;

            Valve* v = Valves.find(p.CurrentValveName)->second;

            if (p.OpenValves.find(p.CurrentValveName) == p.OpenValves.end())
            {
                Path NewPath(p.CurrentValveName, p.CurrentPressureReleased, p.OpenValvesPressure, p.OpenValves);
                NewPath.OpenValve(v);

                NewPaths.insert(NewPath);
            }

            for (size_t ValveIndex = 0; ValveIndex < v->Valves.size(); ++ValveIndex)
            {
                Valve* NextValve = v->Valves[ValveIndex];

                NewPaths.insert(Path(NextValve->Name, p.CurrentPressureReleased, p.OpenValvesPressure, p.OpenValves));
            }
        }

        Paths = NewPaths;
    }

    int Solution = 0;
    for (auto it = Paths.begin(); it != Paths.end(); ++it)
    {
        if (it->CurrentPressureReleased > Solution)
        {
            Solution = it->CurrentPressureReleased;
        }
    }

    return Solution;
}

int part2(vector<string> input)
{
    unordered_map<string, Valve*> Valves = ParseValves(input);

    unordered_set<Path> MyVisitedPaths, ElephantVisitedPaths;
    unordered_set<Path> MyPaths = { Path("AA", 0, 0, {}) }, ElephantPaths = { Path("AA", 0, 0, {}) };

    for (int Minute = 1; Minute <= 26; ++Minute)
    {
        unordered_set<Path> MyNewPaths, ElephantNewPaths;

        for (auto it = MyPaths.begin(); it != MyPaths.end(); ++it)
        {
            Path p = *it;

            // Already visited this path
            if (MyVisitedPaths.find(p) != MyVisitedPaths.end())
            {
                continue;
            }

            MyVisitedPaths.insert(p);

            p.CurrentPressureReleased += p.OpenValvesPressure;

            Valve* v = Valves.find(p.CurrentValveName)->second;

            if (p.OpenValves.find(p.CurrentValveName) == p.OpenValves.end())
            {
                Path NewPath(p.CurrentValveName, p.CurrentPressureReleased, p.OpenValvesPressure, p.OpenValves);
                NewPath.OpenValve(v);

                MyNewPaths.insert(NewPath);
            }

            for (size_t ValveIndex = 0; ValveIndex < v->Valves.size(); ++ValveIndex)
            {
                Valve* NextValve = v->Valves[ValveIndex];

                MyNewPaths.insert(Path(NextValve->Name, p.CurrentPressureReleased, p.OpenValvesPressure, p.OpenValves));
            }
        }

        for (auto it = ElephantPaths.begin(); it != ElephantPaths.end(); ++it)
        {
            Path p = *it;

            // Elephant already visited this path
            if (ElephantVisitedPaths.find(p) != ElephantVisitedPaths.end())
            {
                continue;
            }

            ElephantVisitedPaths.insert(p);

            p.CurrentPressureReleased += p.OpenValvesPressure;

            Valve* v = Valves.find(p.CurrentValveName)->second;

            if (p.OpenValves.find(p.CurrentValveName) == p.OpenValves.end())
            {
                Path NewPath(p.CurrentValveName, p.CurrentPressureReleased, p.OpenValvesPressure, p.OpenValves);
                NewPath.OpenValve(v);

                ElephantNewPaths.insert(NewPath);
            }

            for (size_t ValveIndex = 0; ValveIndex < v->Valves.size(); ++ValveIndex)
            {
                Valve* NextValve = v->Valves[ValveIndex];

                ElephantNewPaths.insert(Path(NextValve->Name, p.CurrentPressureReleased, p.OpenValvesPressure, p.OpenValves));
            }
        }

        MyPaths = MyNewPaths;
        ElephantPaths = ElephantNewPaths;
    }

    int Solution = 0;
    for (auto it = MyPaths.begin(); it != MyPaths.end(); ++it)
    {
        const Path& MyPath = *it;
        const unordered_set<string>& MyOpenValves = MyPath.OpenValves;

        if (MyPath.CurrentPressureReleased + Path::MaxPressureReleased <= Solution)
        {
            continue;
        }

        for (auto it1 = ElephantPaths.begin(); it1 != ElephantPaths.end(); ++it1)
        {
            const Path& ElephantPath = *it1;
            const unordered_set<string>& ElephantValves = ElephantPath.OpenValves;

            if (MyPath.CurrentPressureReleased + ElephantPath.CurrentPressureReleased <= Solution)
            {
                continue;
            }

            bool IsValid = true;
            for (auto vIt = MyOpenValves.begin(); vIt != MyOpenValves.end(); ++vIt)
            {
                if (ElephantValves.find(*vIt) != ElephantValves.end())
                {
                    IsValid = false;
                    break;
                }
            }

            if (IsValid)
            {
                Solution = MyPath.CurrentPressureReleased + ElephantPath.CurrentPressureReleased;
            }
        }
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