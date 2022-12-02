// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

enum Choice
{
    Choice_NONE = 0,
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3
};

enum Outcome
{
    Outcome_NONE = 0,
    LOSE = 1,
    DRAW = 2,
    WIN = 3
};

Choice getChoice(char choice)
{
    switch (choice)
    {
    case 'A':
    case 'X':
        return ROCK;
    case 'B':
    case 'Y':
        return PAPER;
    case 'C':
    case 'Z':
        return SCISSORS;
    default:
        break;
    }
    return Choice_NONE;
}

Outcome getOutcome(char choice)
{
    switch (choice)
    {
    case 'X':
        return LOSE;
    case 'Y':
        return DRAW;
    case 'Z':
        return WIN;
    default:
        break;
    }
    return Outcome_NONE;
}

int getPart1Points(char opponent, char mine)
{
    int points = 0;

    Choice myChoice = getChoice(mine);
    Choice opponentChoice = getChoice(opponent);

    points += myChoice;

    if (myChoice == opponentChoice)
    {
        points += 3;
    }
    else if (
        (myChoice == ROCK && opponentChoice == SCISSORS) ||
        (myChoice == SCISSORS && opponentChoice == PAPER) ||
        (myChoice == PAPER && opponentChoice == ROCK))
    {
        points += 6;
    }
    return points;
}

int getPart2Points(char opponent, char wantedOutcome)
{
    int points = 0;

    Choice opponentChoice = getChoice(opponent);
    Outcome outcome = getOutcome(wantedOutcome);

    switch (outcome)
    {
        case DRAW:
            points += 3;
            break;
        case WIN:
            points += 6;
            break;
        default:
            break;
    }

    switch (opponentChoice)
    {
    case PAPER:
    {
        switch (outcome)
        {
        case DRAW:
            points += PAPER;
            break;
        case WIN:
            points += SCISSORS;
            break;
        case LOSE:
            points += ROCK;
            break;
        default:
            break;
        }
        break;
    }
    case SCISSORS:
    {
        switch (outcome)
        {
        case DRAW:
            points += SCISSORS;
            break;
        case WIN:
            points += ROCK;
            break;
        case LOSE:
            points += PAPER;
            break;
        default:
            break;
        }
        break;
    }
    case ROCK:
    {
        switch (outcome)
        {
        case DRAW:
            points += ROCK;
            break;
        case WIN:
            points += PAPER;
            break;
        case LOSE:
            points += SCISSORS;
            break;
        default:
            break;
        }
        break;
    }
    }

    return points;
}


int main()
{
    ifstream f;
    f.open("input.txt");

    
    int solution = 0;
    string line;

    char opponentShape, myShape;
    while (!f.eof())
    {
        getline(f, line);

        istringstream iss(line);
        iss >> opponentShape >> myShape;

        //solution += getPart1Points(opponentShape, myShape);
        solution += getPart2Points(opponentShape, myShape);
    }

    f.close();

    cout << solution << endl;

    return 0;
}