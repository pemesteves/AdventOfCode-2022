// Day19.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

struct Robot
{
    int OreCost;
    int ClayCost;
    int ObsidianCost;

    Robot(int ore, int clay, int obsidian)
        : OreCost(ore)
        , ClayCost(clay)
        , ObsidianCost(obsidian)
    {}

    virtual bool CanCreateNew(int OreQuantity, int ClayQuantity, int ObsidianQuantity) const = 0;

    bool operator==(const Robot& r) const
    {
        return OreCost == r.OreCost
            && ClayCost == r.ClayCost
            && ObsidianCost == r.ObsidianCost;
    }
};

struct OreRobot : Robot
{
    OreRobot(int ore)
        : Robot(ore, 0, 0)
    {}

    virtual bool CanCreateNew(int OreQuantity, int ClayQuantity, int ObsidianQuantity) const override
    {
        return OreQuantity >= OreCost;
    }
};

struct ClayRobot : Robot
{
    ClayRobot(int ore)
        : Robot(ore, 0, 0)
    {}

    virtual bool CanCreateNew(int OreQuantity, int ClayQuantity, int ObsidianQuantity) const override
    {
        return OreQuantity >= OreCost;
    }
};

struct ObsidianRobot : Robot
{
    ObsidianRobot(int ore, int clay)
        : Robot(ore, clay, 0)
    {}

    virtual bool CanCreateNew(int OreQuantity, int ClayQuantity, int ObsidianQuantity) const override
    {
        return OreQuantity >= OreCost && ClayQuantity >= ClayCost;
    }
};

struct GeodeRobot : Robot
{
    GeodeRobot(int ore, int obsidian)
        : Robot(ore, 0, obsidian)
    {}

    virtual bool CanCreateNew(int OreQuantity, int ClayQuantity, int ObsidianQuantity) const override
    {
        return OreQuantity >= OreCost && ObsidianQuantity >= ObsidianCost;
    }
};

struct Blueprint
{
    OreRobot OreRbt;
    ClayRobot ClayRbt;
    ObsidianRobot ObsidianRbt;
    GeodeRobot GeodeRbt;

    int NumOreRobots;
    int NumClayRobots;
    int NumObsidianRobots;
    int NumGeodeRobots;

    int OreQuantity;
    int ClayQuantity;
    int ObsidianQuantity;
    int GeodeQuantity;

    Blueprint(OreRobot InOreRobot, ClayRobot InClayRobot, ObsidianRobot InObsidianRobot, GeodeRobot InGeodeRobot)
        : OreRbt(InOreRobot)
        , ClayRbt(InClayRobot)
        , ObsidianRbt(InObsidianRobot)
        , GeodeRbt(InGeodeRobot)
        , NumOreRobots(1)
        , NumClayRobots(0)
        , NumObsidianRobots(0)
        , NumGeodeRobots(0)
        , OreQuantity(0)
        , ClayQuantity(0)
        , ObsidianQuantity(0)
        , GeodeQuantity(0)
    {}

    void Produce()
    {
        OreQuantity += NumOreRobots;
        ClayQuantity += NumClayRobots;
        ObsidianQuantity += NumObsidianRobots;
        GeodeQuantity += NumGeodeRobots;
    }

    bool CanCreateOreRobot() const
    {
        return OreRbt.CanCreateNew(OreQuantity, ClayQuantity, ObsidianQuantity);
    }

    bool CanCreateClayRobot() const
    {
        return ClayRbt.CanCreateNew(OreQuantity, ClayQuantity, ObsidianQuantity);
    }
    
    bool CanCreateObsidianRobot() const
    {
        return ObsidianRbt.CanCreateNew(OreQuantity, ClayQuantity, ObsidianQuantity);
    }

    bool CanCreateGeodeRobot() const
    {
        return GeodeRbt.CanCreateNew(OreQuantity, ClayQuantity, ObsidianQuantity);
    }

    void CreateOreRobot()
    {
        ++NumOreRobots;
        OreQuantity -= OreRbt.OreCost;
    }

    void CreateClayRobot()
    {
        ++NumClayRobots;
        OreQuantity -= ClayRbt.OreCost;
    }

    void CreateObsidianRobot()
    {
        ++NumObsidianRobots;
        OreQuantity -= ObsidianRbt.OreCost;
        ClayQuantity -= ObsidianRbt.ClayCost;
    }

    void CreateGeodeRobot()
    {
        ++NumGeodeRobots;
        OreQuantity -= GeodeRbt.OreCost;
        ObsidianQuantity -= GeodeRbt.ObsidianCost;
    }

    bool operator==(const Blueprint& b) const
    {
        return OreRbt == b.OreRbt && ClayRbt == b.ClayRbt && ObsidianRbt == b.ObsidianRbt && GeodeRbt == b.GeodeRbt
            && NumOreRobots == b.NumOreRobots && NumClayRobots == b.NumClayRobots && NumObsidianRobots == b.NumObsidianRobots && NumGeodeRobots == b.NumGeodeRobots
            && OreQuantity == b.OreQuantity && ClayQuantity == b.ClayQuantity && ObsidianQuantity == b.ObsidianQuantity && GeodeQuantity == b.GeodeQuantity;
    }
};

namespace std
{
    template <>
    struct hash<Robot>
    {
        std::size_t operator()(const Robot& r) const
        {
            return (hash<int>()(r.OreCost)
                 ^ (hash<int>()(r.ClayCost) << 1)
                 ^ (hash<int>()(r.ObsidianCost) << 2));
        }
    };

    template <>
    struct hash<Blueprint>
    {
        std::size_t operator()(const Blueprint& b) const
        {
            return (hash<Robot>()(b.OreRbt)
                 ^ (hash<Robot>()(b.ClayRbt) << 1)
                 ^ (hash<Robot>()(b.ObsidianRbt) << 2)
                 ^ (hash<Robot>()(b.GeodeRbt) << 3)
                 ^ (hash<int>()(b.NumOreRobots) << 4)
                 ^ (hash<int>()(b.NumClayRobots) << 5)
                 ^ (hash<int>()(b.NumObsidianRobots) << 6)
                 ^ (hash<int>()(b.NumGeodeRobots) << 7)
                 ^ (hash<int>()(b.OreQuantity) << 8)
                 ^ (hash<int>()(b.ClayQuantity) << 9)
                 ^ (hash<int>()(b.ObsidianQuantity) << 10)
                 ^ (hash<int>()(b.GeodeQuantity) << 11));
        }
    };
};

//////////////
// SOLUTION //
//////////////

vector<Blueprint> ParseBlueprints(const vector<string>& input)
{
    vector<Blueprint> Blueprints;

    for (size_t Index = 0; Index < input.size(); ++Index)
    {
        string Line = input[Index];

        // First part is just the name of the blueprint
        Line = Line.substr(Line.find(':') + 2);

        // ORE ROBOT
        Line = Line.substr(Line.find("costs") + 6);

        OreRobot OreRbt(stoi(Line.substr(0, Line.find(' '))));

        // CLAY ROBOT
        Line = Line.substr(Line.find("costs") + 6);

        ClayRobot ClayRbt(stoi(Line.substr(0, Line.find(' '))));

        // OBSIDIAN ROBOT
        Line = Line.substr(Line.find("costs") + 6);

        int Ore = stoi(Line.substr(0, Line.find(' ')));
        Line = Line.substr(Line.find("and") + 4);

        ObsidianRobot ObsidianRbt(Ore, stoi(Line.substr(0, Line.find(' '))));

        // GEODE ROBOT
        Line = Line.substr(Line.find("costs") + 6);
        Ore = stoi(Line.substr(0, Line.find(' ')));

        Line = Line.substr(Line.find("and") + 4);

        GeodeRobot GeodeRbt(Ore, stoi(Line.substr(0, Line.find(' '))));

        Blueprints.push_back(Blueprint(OreRbt, ClayRbt, ObsidianRbt, GeodeRbt));
    }

    return Blueprints;
}

int part1(const vector<string>& input)
{
    vector<Blueprint> Blueprints = ParseBlueprints(input);

    int Solution = 0;

    for (size_t BlueprintIndex = 0; BlueprintIndex < Blueprints.size(); ++BlueprintIndex)
    {
        // Start with the main blueprint
        unordered_set<Blueprint> CurrentBlueprints = { Blueprints[BlueprintIndex] };
        // Temporary variable for assigning new blueprints;
        unordered_set<Blueprint> TempBlueprints;

        int BlueprintMaxGeode = 0;
        for (int Minute = 1; Minute <= 24; ++Minute)
        {
            for (auto it = CurrentBlueprints.begin(); it != CurrentBlueprints.end(); ++it)
            {
                Blueprint b = *it;

                bool CanCreateGeodeRobot = b.CanCreateGeodeRobot();
                bool CanCreateObsidianRobot = b.CanCreateObsidianRobot();
                bool CanCreateClayRobot = b.CanCreateClayRobot();
                bool CanCreateOreRobot = b.CanCreateOreRobot();

                b.Produce();

                if (b.GeodeQuantity + (b.NumGeodeRobots + (24 - Minute) / (float)2) * (24 - Minute) < BlueprintMaxGeode)
                {
                    continue;
                }

                if (b.GeodeQuantity > BlueprintMaxGeode)
                {
                    BlueprintMaxGeode = b.GeodeQuantity;
                }

                // The best is to create a geode robot when possible
                if (CanCreateGeodeRobot)
                {
                    Blueprint GeodeBlueprint = b;
                    GeodeBlueprint.CreateGeodeRobot();
                    TempBlueprints.insert(GeodeBlueprint);

                    continue;
                }

                TempBlueprints.insert(b);

                if (CanCreateOreRobot)
                {
                    Blueprint OreBlueprint = b;
                    OreBlueprint.CreateOreRobot();
                    TempBlueprints.insert(OreBlueprint);
                }

                if (CanCreateClayRobot)
                {
                    Blueprint ClayBlueprint = b;
                    ClayBlueprint.CreateClayRobot();
                    TempBlueprints.insert(ClayBlueprint);
                }

                if (CanCreateObsidianRobot)
                {
                    Blueprint ObsidianBlueprint = b;
                    ObsidianBlueprint.CreateObsidianRobot();
                    TempBlueprints.insert(ObsidianBlueprint);
                }
            }

            CurrentBlueprints = TempBlueprints;
        }

        Solution += (BlueprintMaxGeode * (BlueprintIndex + 1));
    }

    return Solution;
}

long long part2(const vector<string>& input)
{
    vector<Blueprint> Blueprints = ParseBlueprints(input);

    long long Solution = 1;

    for (size_t BlueprintIndex = 0; BlueprintIndex < 3 && BlueprintIndex < Blueprints.size(); ++BlueprintIndex)
    {
        // Start with the main blueprint
        unordered_set<Blueprint> CurrentBlueprints = { Blueprints[BlueprintIndex] };
        // Temporary variable for assigning new blueprints;
        unordered_set<Blueprint> TempBlueprints;

        int BlueprintMaxGeode = 0;
        for (int Minute = 1; Minute <= 32; ++Minute)
        {
            for (auto it = CurrentBlueprints.begin(); it != CurrentBlueprints.end(); ++it)
            {
                Blueprint b = *it;

                bool CanCreateGeodeRobot = b.CanCreateGeodeRobot();
                bool CanCreateObsidianRobot = b.CanCreateObsidianRobot();
                bool CanCreateClayRobot = b.CanCreateClayRobot();
                bool CanCreateOreRobot = b.CanCreateOreRobot();

                b.Produce();

                if (b.GeodeQuantity + (b.NumGeodeRobots + (32 - Minute) / (float)2) * (32 - Minute) < BlueprintMaxGeode)
                {
                    continue;
                }

                if (b.GeodeQuantity > BlueprintMaxGeode)
                {
                    BlueprintMaxGeode = b.GeodeQuantity;
                }

                // The best is to create a geode robot when possible
                if (CanCreateGeodeRobot)
                {
                    Blueprint GeodeBlueprint = b;
                    GeodeBlueprint.CreateGeodeRobot();
                    TempBlueprints.insert(GeodeBlueprint);

                    continue;
                }

                TempBlueprints.insert(b);

                if (CanCreateOreRobot)
                {
                    Blueprint OreBlueprint = b;
                    OreBlueprint.CreateOreRobot();
                    TempBlueprints.insert(OreBlueprint);
                }

                if (CanCreateClayRobot)
                {
                    Blueprint ClayBlueprint = b;
                    ClayBlueprint.CreateClayRobot();
                    TempBlueprints.insert(ClayBlueprint);
                }

                if (CanCreateObsidianRobot)
                {
                    Blueprint ObsidianBlueprint = b;
                    ObsidianBlueprint.CreateObsidianRobot();
                    TempBlueprints.insert(ObsidianBlueprint);
                }
            }

            CurrentBlueprints = TempBlueprints;
        }

        Solution *= BlueprintMaxGeode;
    }

    return Solution;
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    long long solution = part2(input);
    cout << solution << endl;
}