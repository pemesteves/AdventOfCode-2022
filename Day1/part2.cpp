#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void swap(int &i1, int &i2)
{
    int i_temp = i1;
    i1 = i2;
    i2 = i_temp;
}

void updateStack(int &elf1, int &elf2, int &elf3, int new_value)
{
    if (new_value > elf3)
    {
        elf3 = new_value;
    }   

    if (elf2 < elf3)
    {
        swap(elf2, elf3);
    }

    if (elf1 < elf2)
    {
        swap(elf1, elf2);
    }

}

int main()
{
    ifstream f;
    f.open("input.txt");

    string line;
    int currentCalories = 0;
    int elf1 = 0, elf2 = 0, elf3 = 0;
    while(!f.eof())
    {
        getline(f, line, '\n');
        
        if (line.size() == 0)
        {
            updateStack(elf1, elf2, elf3, currentCalories);

            currentCalories = 0;
            continue;
        }

        currentCalories += stoi(line);
    }

    f.close();
    
    updateStack(elf1, elf2, elf3, currentCalories);

    cout << (elf1 + elf2 + elf3) << endl;
    
    return 0;
}