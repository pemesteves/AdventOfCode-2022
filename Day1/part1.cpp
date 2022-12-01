#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    ifstream f;
    f.open("input.txt");

    string line;
    int mostCalories = 0, currentCalories = 0;
    while(!f.eof())
    {
        getline(f, line, '\n');
        
        if (line.size() == 0)
        {
            if (currentCalories > mostCalories)
            {
                mostCalories = currentCalories;
            }

            currentCalories = 0;
            continue;
        }

        currentCalories += stoi(line);
    }

    f.close();
    
    if (currentCalories > mostCalories)
    {
        mostCalories = currentCalories;
    }

    cout << mostCalories << endl;
    
    return 0;
}