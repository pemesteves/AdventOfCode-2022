// Day25.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

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

unordered_map<char, long long> SnafuToDecimal = {
    { '2',  2 },
    { '1',  1 },
    { '0',  0 },
    { '-', -1 },
    { '=', -2 }
};

unordered_map<char, long long> DecimalToSnafu = {
    {  2, '2' },
    {  1, '1' },
    {  0, '0' },
    { -1, '-' },
    { -2, '=' }
};

struct SNAFU
{
    static unordered_map<int, long long> Pow5;

    string Value;

    SNAFU(string v)
        : Value(v)
    {
        if (v.size() > Pow5.size())
        {
            for (size_t i = Pow5.size(); i < v.size(); ++i)
            {
                Pow5.insert({ i, pow(5, i) });
            }
        }
    }

    long long ConvertToDecimal()
    {
        long long Decimal = 0;
        long long Length = Value.size();
        for (size_t i = 0; i < Length; ++i)
        {
            Decimal += (Pow5[Length - i - 1] * SnafuToDecimal[Value[i]]);
        }

        return Decimal;
    }
};

unordered_map<int, long long> SNAFU::Pow5 = {};

string ConvertDecimalToSNAFU(long long Decimal)
{
    string Result;
    long long Divisor = 1, MaxExponent = 0;
    double TempDecimal = Decimal;
    for (; TempDecimal > 0; ++MaxExponent, Divisor *= 5)
    {
        TempDecimal = round(TempDecimal / 5);
    }

    Divisor /= 5;
    for (long long j = 0; j < MaxExponent; ++j, Divisor /= 5)
    {
        long long SnafuValue = round(Decimal / (double)Divisor);
        Decimal -= (SnafuValue * Divisor);
        Result += DecimalToSnafu[SnafuValue];
    }

    return Result;
}

string part1(vector<string> input)
{
    long long Result = 0;
    for (size_t i = 0; i < input.size(); ++i)
    {
        Result += SNAFU(input[i]).ConvertToDecimal();
    }

    return ConvertDecimalToSNAFU(Result);
}

int main()
{
    vector<string> input = readFile();

    string solution = part1(input);
    cout << solution << endl;

    return 0;
}