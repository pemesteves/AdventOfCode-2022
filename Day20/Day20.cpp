// Day20.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

vector<long long> readFile()
{
    ifstream f;
    f.open("input.txt");

    vector<long long> input;

    string line;

    while (!f.eof())
    {
        getline(f, line);

        input.push_back(stoi(line));
    }

    f.close();

    return input;
}

struct ListNode
{
    long long Value;

    ListNode* Next;
    ListNode* Previous;

    ListNode(long long v)
        : Value(v)
        , Next(nullptr)
        , Previous(nullptr)
    {}
};

long long part1(vector<long long> input)
{
    vector<ListNode*> InputNodes;

    ListNode* FirstNode = new ListNode(input[0]);
    InputNodes.push_back(FirstNode);

    ListNode* ZeroNode = nullptr;
    ListNode* PreviousNode = FirstNode;
    for (size_t i = 1; i < input.size(); ++i)
    {
        ListNode* CurrentNode = new ListNode(input[i]);
        PreviousNode->Next = CurrentNode;
        CurrentNode->Previous = PreviousNode;
        PreviousNode = CurrentNode;

        InputNodes.push_back(CurrentNode);

        if (input[i] == 0)
        {
            ZeroNode = CurrentNode;
        }
    }

    FirstNode->Previous = PreviousNode;
    PreviousNode->Next = FirstNode;

    for (size_t i = 0; i < InputNodes.size(); ++i)
    {
        ListNode* CurrentNode = InputNodes[i];

        long long ModuloValue = CurrentNode->Value % ((long long)InputNodes.size() - 1);

        if (ModuloValue == 0)
        {
            continue;
        }

        if (ModuloValue > 0)
        {
            for (long long v = 0; v < ModuloValue; ++v)
            {
                ListNode* PreviousNode = CurrentNode->Previous;
                ListNode* NextNode = CurrentNode->Next;

                PreviousNode->Next = NextNode;
                NextNode->Previous = PreviousNode;

                CurrentNode->Next = NextNode->Next;
                NextNode->Next = CurrentNode;

                CurrentNode->Previous = NextNode;
                CurrentNode->Next->Previous = CurrentNode;
            }
        }
        else
        {
            for (long long v = 0; v > ModuloValue; --v)
            {
                ListNode* PreviousNode = CurrentNode->Previous;
                ListNode* NextNode = CurrentNode->Next;

                PreviousNode->Next = NextNode;
                NextNode->Previous = PreviousNode;

                CurrentNode->Previous = PreviousNode->Previous;
                PreviousNode->Previous = CurrentNode;

                CurrentNode->Next = PreviousNode;
                CurrentNode->Previous->Next = CurrentNode;
            }
        }
    }

    long long Solution = 0;
    ListNode* CurrentNode = ZeroNode;
    for (int i = 1; i <= 3000; ++i)
    {
        CurrentNode = CurrentNode->Next;

        if (i == 1000 || i == 2000 || i == 3000)
        {
            Solution += CurrentNode->Value;
        }
    }

    return Solution;
}

long long part2(vector<long long> input)
{
    vector<ListNode*> InputNodes;

    ListNode* FirstNode = new ListNode(input[0] * 811589153);
    InputNodes.push_back(FirstNode);

    ListNode* ZeroNode = nullptr;
    ListNode* PreviousNode = FirstNode;
    for (size_t i = 1; i < input.size(); ++i)
    {
        ListNode* CurrentNode = new ListNode(input[i] * 811589153);
        PreviousNode->Next = CurrentNode;
        CurrentNode->Previous = PreviousNode;
        PreviousNode = CurrentNode;

        InputNodes.push_back(CurrentNode);

        if (input[i] == 0)
        {
            ZeroNode = CurrentNode;
        }
    }

    FirstNode->Previous = PreviousNode;
    PreviousNode->Next = FirstNode;

    for (int j = 0; j < 10; ++j)
    {
        for (size_t i = 0; i < InputNodes.size(); ++i)
        {
            ListNode* CurrentNode = InputNodes[i];

            long long ModuloValue = CurrentNode->Value % ((long long) InputNodes.size() - 1);

            if (ModuloValue == 0)
            {
                continue;
            }

            if (ModuloValue > 0)
            {
                for (long long v = 0; v < ModuloValue; ++v)
                {
                    ListNode* PreviousNode = CurrentNode->Previous;
                    ListNode* NextNode = CurrentNode->Next;

                    PreviousNode->Next = NextNode;
                    NextNode->Previous = PreviousNode;

                    CurrentNode->Next = NextNode->Next;
                    NextNode->Next = CurrentNode;

                    CurrentNode->Previous = NextNode;
                    CurrentNode->Next->Previous = CurrentNode;
                }
            }
            else
            {
                for (long long v = 0; v > ModuloValue; --v)
                {
                    ListNode* PreviousNode = CurrentNode->Previous;
                    ListNode* NextNode = CurrentNode->Next;

                    PreviousNode->Next = NextNode;
                    NextNode->Previous = PreviousNode;

                    CurrentNode->Previous = PreviousNode->Previous;
                    PreviousNode->Previous = CurrentNode;

                    CurrentNode->Next = PreviousNode;
                    CurrentNode->Previous->Next = CurrentNode;
                }
            }
        }
    }

    long long Solution = 0;
    ListNode* CurrentNode = ZeroNode;
    for (int i = 1; i <= 3000; ++i)
    {
        CurrentNode = CurrentNode->Next;

        if (i == 1000 || i == 2000 || i == 3000)
        {
            Solution += CurrentNode->Value;
        }
    }

    return Solution;
}

int main()
{
    vector<long long> input = readFile();

    //long long solution = part1(input);
    long long solution = part2(input);
    cout << solution << endl;

    return 0;
}