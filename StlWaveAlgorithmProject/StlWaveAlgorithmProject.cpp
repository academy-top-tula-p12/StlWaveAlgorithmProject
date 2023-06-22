#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include "Console.h";

using namespace std;

enum CellType
{
    Start = 0,
    Finish = -2,
    Wall = -1,
    Space = -3
};

struct Cell
{
    int row;
    int column;
    friend bool operator==(const Cell& cell1, const Cell& cell2)
    {
        return (cell1.row == cell2.row && cell1.column == cell2.column);
    }
};

using Map = vector<vector<int>>;
using Front = array<vector<Cell>, 2>;
using Way = vector<Cell>;

const vector<Cell> Offset{ {-1, 0}, { 0, 1 }, { 1, 0 }, { 0, -1 } };

Map MapFromFile(string fileName, Cell& start, Cell& finish);
void MapPrint(Map map);
void MapConsolePrint(Map map, Way way);

int main()
{
    Cell start;
    Cell finish;
    
    Map map = MapFromFile("labirint.txt", start, finish);
    MapPrint(map);

    bool isFinish{ true };
    Front fronts;
    bool frontCurrent{ false };
    int frontNumber{ 1 };

    fronts[frontCurrent].push_back(start);
    bool isBreak{ false };

    while (true)
    {
        fronts[!frontCurrent].clear();
        //for (int c = 0; c < fronts[frontCurrent].size(); c++)
        for(auto cellFront : fronts[frontCurrent])
        {
            int crow = cellFront.row;
            int ccolumn = cellFront.column;
            for (auto cellOff : Offset)
            {
                int orow = crow + cellOff.row;
                int ocolumn = ccolumn + cellOff.column;
                if (orow < 0 || orow >= map.size() || ocolumn < 0 || ocolumn >= map[0].size())
                    continue;
                if (map[orow][ocolumn] == CellType::Finish ||
                    map[orow][ocolumn] == CellType::Space)
                {
                    fronts[!frontCurrent].push_back(Cell{ orow, ocolumn });
                    if (map[orow][ocolumn] == CellType::Finish)
                    {
                        map[orow][ocolumn] = frontNumber;
                        isBreak = true;
                        break;
                    }
                    map[orow][ocolumn] = frontNumber;
                }
            }
            if (isBreak) break;
        }
        if (isBreak) break;
        if (fronts[!frontCurrent].size() == 0)
        {
            isFinish = false;
            break;
        }
        frontCurrent = !frontCurrent;
        frontNumber++;
    }

    cout << "\n\n";
    MapPrint(map);
    cout << "\n" << frontNumber << "\n";
    
    if (!isFinish)
    {
        cout << "\n no finish\n";
        return 0;
    }
        
    Way way;
    way.push_back(finish);
    
    while (true)
    {
        int wrow = way[way.size() - 1].row;
        int wcolumn = way[way.size() - 1].column;
        for (auto cellOff : Offset)
        {
            int orow = wrow + cellOff.row;
            int ocolumn = wcolumn + cellOff.column;
            if (map[orow][ocolumn] == frontNumber - 1)
            {
                way.push_back({ orow, ocolumn });
                break;
            }
        }
        if (frontNumber - 1 == 0)
            break;
        frontNumber--;
    }
    reverse(way.begin(), way.end());

    for_each(way.begin(), way.end(), [](Cell cell) 
        {
            cout << "[" << cell.row << "," << cell.column << "] -> ";
        });

}

Map MapFromFile(string fileName, Cell& start, Cell& finish)
{
    Map map;

    fstream streamMap;
    streamMap.open(fileName, ios::in);

    int bufferSize{ 1024 };
    char* buffer = new char[bufferSize] {};

    int row{};
    while (!streamMap.eof())
    {
        map.push_back(vector<int>());
        streamMap.getline(buffer, bufferSize);

        for (int column = 0; column < strlen(buffer); column++)
        {
            CellType cellType{ CellType::Space };

            switch (buffer[column])
            {
            case '#': cellType = CellType::Wall; break;
            case 's': 
            case 'S': cellType = CellType::Start;
                start = Cell{ row, column };
                break;
            case 'f':
            case 'F': cellType = CellType::Finish;
                finish = Cell{ row, column };
                break;
            case ' ': cellType = CellType::Space; break;
            }
            map[row].push_back(cellType);
        }
        row++;
    }
    streamMap.close();

    return map;
}

void MapPrint(Map map)
{
    for (int r = 0; r < map.size(); r++)
    {
        for (int c = 0; c < map[0].size(); c++)
        {
            if (map[r][c] == CellType::Wall) 
                cout << "##";
            else if (map[r][c] == CellType::Space) 
                cout << "  ";
            else
                cout << setw(2) << map[r][c];
        }
            
        cout << "\n";
    }
}

void MapConsolePrint(Map map, Way way)
{

}