// Day17.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

struct Rock
{
    vector<string> Shape;

    vector<pair<long, long>> RightIndices;
    vector<pair<long, long>> LeftIndices;
    vector<pair<long, long>> UpIndices;
    vector<pair<long, long>> DownIndices;

    Rock(vector<string> s)
        : Shape(s)
    {
        vector<long> Down(s[0].size(), 0), Up(s[0].size(), s.size() - 1);

        for (size_t i = 0; i < s.size(); ++i)
        {
            long LeftIndex = s[i].size() - 1;
            long RightIndex = 0;
            for (size_t j = 0; j < s[i].size(); ++j)
            {
                if (s[i][j] != '#')
                {
                    continue;
                }

                if (Up[j] > s.size() - i - 1)
                {
                    Up[j] = s.size() - i - 1;
                }

                if (Down[j] < s.size() - i - 1)
                {
                    Down[j] = s.size() - i - 1;
                }

                if (j > RightIndex)
                {
                    RightIndex = j;
                }

                if (j < LeftIndex)
                {
                    LeftIndex = j;
                }
            }

            LeftIndices.push_back(make_pair(s.size() - i - 1, LeftIndex));
            RightIndices.push_back(make_pair(s.size() - i - 1, RightIndex));
        }

        for (size_t i = 0; i < Down.size(); ++i)
        {
            UpIndices.push_back(make_pair(Down[i], i));
            DownIndices.push_back(make_pair(Up[i], i));
        }
    }

    Rock(vector<string> s, vector<pair<long, long>> rInd, vector<pair<long, long>> lInd,
        vector<pair<long, long>> uInd, vector<pair<long, long>> dInd)
        : Shape(s)
        , LeftIndices(rInd)
        , RightIndices(lInd)
        , UpIndices(uInd)
        , DownIndices(dInd)
    {}
};

void printChamber(const vector<string>& InChamber)
{
    system("cls");
    for (long i = InChamber.size() - 1; i >= 0; --i)
    {
        for (size_t j = 0; j < InChamber[i].size(); ++j)
        {
            cout << InChamber[i][j];
        }
        cout << endl;
    }
    system("pause");
}

void printRockInChamber(vector<string>& InOutChamber, const vector<string>& InRockShape, const long X, const long Y, const char C = '#')
{
    for (size_t i = 0; i < InRockShape.size(); ++i)
    {
        for (size_t j = 0; j < InRockShape[i].size(); ++j)
        {
            if (InRockShape[i][j] != '.')
            {
                InOutChamber[Y + InRockShape.size() - i - 1][X + j] = C;
            }
        }
    }
}

long part1(vector<string> input)
{
    long NumRocks = 2022;

    vector<Rock> RockTypes;
    RockTypes.push_back(Rock({ "####" }));
    RockTypes.push_back(Rock({ ".#.", "###", ".#." }));
    RockTypes.push_back(Rock({ "..#", "..#", "###" }));
    RockTypes.push_back(Rock({ "#", "#", "#", "#" }));
    RockTypes.push_back(Rock({ "##", "##" }));

    string JetPattern = input[0];

    long X, Y, LastY = 0, JetIndex = 0;

    vector<string> Chamber(10, string(7, '.'));

    for (size_t RockIndex = 0; RockIndex < NumRocks; ++RockIndex)
    {
        const Rock& r = RockTypes[RockIndex % RockTypes.size()];
        const vector<string>& RockShape = r.Shape;

        X = 2;
        Y = LastY + 3;

        if (Y + RockShape.size() >= Chamber.size())
        {
            for (size_t i = 0; i < 3; ++i)
            {
                Chamber.push_back(string(7, '.'));
            }
        }

        //vector<string> TestChamber = Chamber;
        //printRockInChamber(TestChamber, RockShape, X, Y, '@');
        //printChamber(TestChamber);

        bool CanGoDown = true;
        do
        {
            char Jet = JetPattern[JetIndex % JetPattern.size()];
            ++JetIndex;

            // Jet Influence
            if (Jet == '>')
            {
                const vector<pair<long, long>>& RightIndices = r.RightIndices;

                bool CanGoRight = true;
                for (size_t Index = 0; Index < RightIndices.size(); ++Index)
                {
                    const pair<long, long>& RightIndex = RightIndices[Index];
                    if (X + RightIndex.second + 1 > Chamber[0].size() || Chamber[Y + RightIndex.first][X + RightIndex.second + 1] != '.')
                    {
                        CanGoRight = false;
                        break;
                    }
                }

                if (CanGoRight)
                {
                    ++X;
                }
            }
            else
            {
                const vector<pair<long, long>>& LeftIndices = r.LeftIndices;

                bool CanGoLeft = true;
                for (size_t Index = 0; Index < LeftIndices.size(); ++Index)
                {
                    const pair<long, long>& LeftIndex = LeftIndices[Index];
                    if (X + LeftIndex.second - 1 < 0 || Chamber[Y + LeftIndex.first][X + LeftIndex.second - 1] != '.')
                    {
                        CanGoLeft = false;
                        break;
                    }
                }

                if (CanGoLeft)
                {
                    --X;
                }
            }

            // TestChamber = Chamber;
            // printRockInChamber(TestChamber, RockShape, X, Y);
            // printChamber(TestChamber);


            // Down Influence
            const vector<pair<long, long>>& DownIndices = r.DownIndices;

            for (size_t Index = 0; Index < DownIndices.size(); ++Index)
            {
                const pair<long, long>& DownIndex = DownIndices[Index];
                if (Y + DownIndex.first - 1 < 0 || Chamber[Y + DownIndex.first - 1][X + DownIndex.second] != '.')
                {
                    CanGoDown = false;
                    break;
                }
            }

            if (CanGoDown)
            {
                --Y;
            }

            //TestChamber = Chamber;
            //printRockInChamber(TestChamber, RockShape, X, Y, '@');
            //printChamber(TestChamber);

        } while (CanGoDown);

        printRockInChamber(Chamber, RockShape, X, Y);

        if (Y + RockShape.size() > LastY)
        {
            LastY = Y + RockShape.size();
        }
    }

    return LastY;
}

long long part2(vector<string> input)
{
    long long NumRocks = 1000000000000;
    long long SampleSize = 10000;

    vector<Rock> RockTypes;
    RockTypes.push_back(Rock({ "####" }));
    RockTypes.push_back(Rock({ ".#.", "###", ".#." }));
    RockTypes.push_back(Rock({ "..#", "..#", "###" }));
    RockTypes.push_back(Rock({ "#", "#", "#", "#" }));
    RockTypes.push_back(Rock({ "##", "##" }));

    string JetPattern = input[0];

    long long X, Y, LastY = 0, JetIndex = 0;

    vector<string> Chamber(10, string(7, '.'));

    vector<long long> HeightDifferences;

    for (size_t RockIndex = 0; RockIndex < SampleSize; ++RockIndex)
    {
        const Rock& r = RockTypes[RockIndex % RockTypes.size()];
        const vector<string>& RockShape = r.Shape;

        X = 2;
        Y = LastY + 3;

        if (Y + RockShape.size() >= Chamber.size())
        {
            for (size_t i = 0; i < 3; ++i)
            {
                Chamber.push_back(string(7, '.'));
            }
        }

        bool CanGoDown = true;
        do
        {
            char Jet = JetPattern[JetIndex % JetPattern.size()];
            ++JetIndex;

            // Jet Influence
            if (Jet == '>')
            {
                const vector<pair<long, long>>& RightIndices = r.RightIndices;

                bool CanGoRight = true;
                for (size_t Index = 0; Index < RightIndices.size(); ++Index)
                {
                    const pair<long, long>& RightIndex = RightIndices[Index];
                    if (X + RightIndex.second + 1 > Chamber[0].size() || Chamber[Y + RightIndex.first][X + RightIndex.second + 1] != '.')
                    {
                        CanGoRight = false;
                        break;
                    }
                }

                if (CanGoRight)
                {
                    ++X;
                }
            }
            else
            {
                const vector<pair<long, long>>& LeftIndices = r.LeftIndices;

                bool CanGoLeft = true;
                for (size_t Index = 0; Index < LeftIndices.size(); ++Index)
                {
                    const pair<long, long>& LeftIndex = LeftIndices[Index];
                    if (X + LeftIndex.second - 1 < 0 || Chamber[Y + LeftIndex.first][X + LeftIndex.second - 1] != '.')
                    {
                        CanGoLeft = false;
                        break;
                    }
                }

                if (CanGoLeft)
                {
                    --X;
                }
            }

            // Down Influence
            const vector<pair<long, long>>& DownIndices = r.DownIndices;

            for (size_t Index = 0; Index < DownIndices.size(); ++Index)
            {
                const pair<long, long>& DownIndex = DownIndices[Index];
                if (Y + DownIndex.first - 1 < 0 || Chamber[Y + DownIndex.first - 1][X + DownIndex.second] != '.')
                {
                    CanGoDown = false;
                    break;
                }
            }

            if (CanGoDown)
            {
                --Y;
            }
        } while (CanGoDown);

        printRockInChamber(Chamber, RockShape, X, Y);
        HeightDifferences.push_back(Y + RockShape.size() - LastY);

        if (Y + RockShape.size() > LastY)
        {
            LastY = Y + RockShape.size();
        }
    }

    for (size_t i = 0; i < HeightDifferences.size() / 2; ++i)
    {
        for (size_t j = i + 1; j < HeightDifferences.size() / 2; ++j)
        {
            if (HeightDifferences[i] != HeightDifferences[j])
            {
                continue;
            }

            bool AreEqual = true;
            for (size_t k = i, l = j; k < HeightDifferences.size() && l < HeightDifferences.size(); ++k, ++l)
            {
                if (HeightDifferences[k] != HeightDifferences[l])
                {
                    AreEqual = false;
                    break;
                }
            }

            if (AreEqual)
            {
                long Preamble = 0, Repetition = 0, RepetitionPart = 0;
                for (size_t k = 0; k < i; ++k)
                {
                    if (HeightDifferences[k] <= 0) continue;

                    Preamble += HeightDifferences[k];
                }

                for (size_t k = i; k < j; ++k)
                {
                    if (HeightDifferences[k] <= 0) continue;

                    Repetition += HeightDifferences[k];

                    if (k < i + ((NumRocks - i) % (j - i)))
                    {
                        RepetitionPart += HeightDifferences[k];
                    }
                }

                return Preamble + Repetition * ((NumRocks - i) / (j - i)) + RepetitionPart;
            }
        }
    }

    return 0;
}

long main()
{
    vector<string> input = readFile();

    //long solution = part1(input);
    long long solution = part2(input);
    cout << solution << endl;
}