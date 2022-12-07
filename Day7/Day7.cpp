// Day7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef int uint;

class FilesystemObject
{
    string name;

protected:

    class FilesystemDirectory* parent;
    uint size;

public:

    FilesystemObject(string n, class FilesystemDirectory* p) : name(n), parent(p) {}

    string getName() const { return name; }
    int getSize() const { return size; }
    class FilesystemDirectory* getParent() const { return parent; }
};

class FilesystemFile : public FilesystemObject
{
public:
    FilesystemFile(string n, class FilesystemDirectory* p, uint s) : FilesystemObject(n, p) { size = s; }
};

class FilesystemDirectory : public FilesystemObject
{
    vector<class FilesystemFile*> files;
    vector<class FilesystemDirectory*> directories;

public:
    FilesystemDirectory(string n, class FilesystemDirectory* p) : FilesystemObject(n, p) {}

    void AddFile(FilesystemFile* file)
    {
        files.push_back(file);
    }

    void AddDirectory(FilesystemDirectory* directory)
    {
        directories.push_back(directory);
    }

    void setSize(int s) { size = s; }

    vector<class FilesystemFile*> getFiles() const { return files; }
    vector<class FilesystemDirectory*> getDirectories() const { return directories; }
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

uint DirectorySize(FilesystemDirectory* Directory)
{
    vector<class FilesystemDirectory*> directories = Directory->getDirectories();

    int size = 0;
    for (size_t i = 0; i < directories.size(); ++i)
    {
        size += DirectorySize(directories[i]);
    }

    vector<class FilesystemFile*> files = Directory->getFiles();
    for (size_t i = 0; i < files.size(); ++i)
    {
        size += files[i]->getSize();
    }

    Directory->setSize(size);

    return size;
}

FilesystemDirectory* BuildFilesystem(vector<string> input)
{
    FilesystemDirectory* const RootDirectory = new FilesystemDirectory("/", nullptr);
    FilesystemDirectory* CurrentDirectory = RootDirectory;

    string line, line_part;
    int space_index;    
    uint file_size;
    bool foundLS = false;
    for (size_t i = 0; i < input.size(); ++i)
    {
        line = input[i];

        if (line[0] == '$')
        {
            foundLS = false;
            line = line.substr(line.find(' ') + 1);
            space_index = line.find(' ');
            line_part = line.substr(0, space_index);
            if (line_part == "cd")
            {
                line_part = line.substr(space_index + 1);
                if (line_part == "/")
                {
                    CurrentDirectory = RootDirectory;
                }
                else if (line_part == "..")
                {
                    CurrentDirectory = CurrentDirectory->getParent();
                }
                else
                {
                    vector<class FilesystemDirectory*> directories = CurrentDirectory->getDirectories();

                    for (size_t j = 0; j < directories.size(); ++j)
                    {
                        if (directories[j]->getName() == line_part)
                        {
                            CurrentDirectory = directories[j];
                            break;
                        }
                    }
                }
            }
            else if (line_part == "ls")
            {
                foundLS = true;
                continue;
            }
            else
            {
                cout << "Unknown command: " << line_part << endl;
            }
        }
        else if (foundLS)
        {
            space_index = line.find(' ');
            line_part = line.substr(0, space_index);
            if (line_part == "dir")
            {
                line_part = line.substr(space_index + 1);

                FilesystemDirectory* NewDirectory = new FilesystemDirectory(line_part, CurrentDirectory);
                CurrentDirectory->AddDirectory(NewDirectory);
            }
            else
            {
                file_size = stoul(line_part);
                line_part = line.substr(space_index + 1);

                CurrentDirectory->AddFile(new FilesystemFile(line_part, CurrentDirectory, file_size));
            }
        }
    }

    DirectorySize(RootDirectory);

    return RootDirectory;
}

int GetSumSizes(FilesystemDirectory* const Directory)
{
    int size = Directory->getSize() <= 100000 ? Directory->getSize() : 0;

    vector<class FilesystemDirectory*> directories = Directory->getDirectories();

    for (size_t i = 0; i < directories.size(); ++i)
    {
        size += GetSumSizes(directories[i]);
    }
    return size;
}

int GetMinSizeToFreeUp(FilesystemDirectory* const Directory, const int SpaceNeeded)
{
    int size = Directory->getSize() >= SpaceNeeded ? Directory->getSize() : INT_MAX;
    vector<class FilesystemDirectory*> directories = Directory->getDirectories();
    for (size_t i = 0; i < directories.size(); ++i)
    {
        int s = GetMinSizeToFreeUp(directories[i], SpaceNeeded);
        if (s < size)
        {
            size = s;
        }
    }
    return size;
}

int part1(vector<string> input)
{
    FilesystemDirectory* const RootDirectory = BuildFilesystem(input);

    return GetSumSizes(RootDirectory);
}

int part2(vector<string> input)
{
    FilesystemDirectory* const RootDirectory = BuildFilesystem(input);

    return GetMinSizeToFreeUp(RootDirectory, 30000000 - (70000000 - RootDirectory->getSize()));
}

int main()
{
    vector<string> input = readFile();

    //int solution = part1(input);
    int solution = part2(input);
    cout << solution << endl;
}