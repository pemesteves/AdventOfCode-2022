// Day13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <list>
#include <set>

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

vector<string> getPackets(string List)
{
    vector<string> Packets;

    string Packet;
    int NumParentheses = 0;

    for (size_t i = 1; i < List.size() - 1; ++i)
    {
        char c = List[i];

        if (c == ',' && NumParentheses == 0)
        {
            Packets.push_back(Packet);
            Packet = "";
        }
        else
        {
            if (c == '[')
            {
                ++NumParentheses;
            }
            else if (c == ']')
            {
                --NumParentheses;
            }

            Packet += c;
        }
    }

    if (Packet.size() > 0)
    {
        Packets.push_back(Packet);
    }

    return Packets;
}

int comparePackets(string Packet1, string Packet2)
{
    if (Packet1[0] != '[' && Packet2[0] != '[')
    {
        return stoi(Packet1) - stoi(Packet2);
    }

    if (Packet1[0] != '[')
    {
        Packet1 = '[' + Packet1 + ']';
    }

    if (Packet2[0] != '[')
    {
        Packet2 = '[' + Packet2 + ']';
    }

    vector<string> Packets1 = getPackets(Packet1);
    vector<string> Packets2 = getPackets(Packet2);

    size_t Index = 0;
    for (; Index < Packets1.size() && Index < Packets2.size(); ++Index)
    {
        int Ret = comparePackets(Packets1[Index], Packets2[Index]);

        if (Ret != 0)
        {
            return Ret;
        }
    }

    return Packets1.size() - Packets2.size();
}

int part1(vector<string> input)
{
    int solution = 0;
    for (size_t i = 0; i < input.size(); ++i)
    {
        string List1 = input[i];
        ++i;
        string List2 = input[i];
        ++i;

        if (comparePackets(List1, List2) <= 0)
        {
            solution += (i / 3 + 1);
        }
    }

    return solution;
}

int part2(vector<string> input)
{
    list<string> Packets = { input[0] };
    
    input.push_back("[[2]]");
    input.push_back("[[6]]");

    for (size_t i = 1; i < input.size(); ++i)
    {
        if (input[i].size() == 0)
        {
            continue;
        }

        string List = input[i];
        bool Inserted = false;
        for (auto it = Packets.begin(); it != Packets.end(); ++it)
        {
            if (comparePackets(List, *it) < 0)
            {
                Packets.insert(it, List);
                Inserted = true;
                break;
            }
        }

        if (!Inserted)
        {
            Packets.push_back(List);
        }
    }

    size_t Index = 1;
    int solution = 1;
    for (auto it = Packets.begin(); it != Packets.end(); ++it)
    {
        if (*it == "[[2]]" || *it == "[[6]]")
        {
            solution *= Index;
        }

        ++Index;
    }

    return solution;
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}