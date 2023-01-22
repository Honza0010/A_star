#include "astar.h"
#include <cmath>
#include <fstream>
#include "Exception.h"
#include <iostream>

using namespace std;

bool Cell::operator<(const Cell& b)
{
    return this->f_cost < b.f_cost;
}


int heurestic(const Cell& a, const Cell& end)
{
    return 10 * (abs(a.x - end.x) + abs(a.y - end.y));
}



/*Astar::Astar(string input, int start_x, int start_y, int end_x, int end_y)
{
    read_file(input);
    start.x = start_x;
    start.y = start_y;
    end.x = end_x;
    end.y = end_y;
}*/

Astar::Astar(int rows, int columns)
{
    this->m = rows;
    this->n = columns;

    vector<int> help;           //��dek matice grid (bludi�t�)
    vector<Cell> cells_help;    //��dek matice grid_cells (pro vkl�d�n� ��dk� bun�k)
    for (int i = 0; i < this->m; i++)
    {
        for (int j = 0; j < this->n; j++)
        {
            help.push_back(0);

            Cell c;     //Pomocna bunka pro vkladani do grid_cells
            c.x = i;
            c.y = j;
            c.parentX = -1;
            c.parentY = -1;
            c.f_cost = 0;
            c.g_cost = 0;
            c.h_cost = 0;

            cells_help.push_back(c);
        }
        grid.push_back(help);
        grid_cells.push_back(cells_help);
        help.clear();
        cells_help.clear();
    }
}

void Astar::setStart(int x, int y)
{
    if((x < 0 || x >= m) || (y < 0 && y >= n))
        throw MyException("Zadal jsi spatne souradnice");
    start.x = x;
    start.y = y;
}

void Astar::setEnd(int x, int y)
{
    if((x < 0 || x >= m) || (y < 0 && y >= n))
        throw MyException("Zadal jsi spatne souradnice");
    end.x = x;
    end.y = y;
}

std::vector<Cell> Astar::getPath()
{
    return this->path;
}

void Astar::setGrid(int x, int y, int value)
{
    if((x < 0 || x >= m) || (y < 0 && y >= n))
        throw MyException("Zadal jsi spatne souradnice");

    if(value != 0 && value != 1)
        return;

    this->grid[x][y] = value;
}

std::vector<std::vector<bool> > Astar::getExplored()
{
    return this->explored;
}

bool Astar::is_start_end_valid()
{
    if ((start.x < 0 || start.x > m) || (start.y < 0 || start.y > n) || (end.x < 0 || end.x > m) || (end.y < 0 || end.y > n))
        return false;
    else if (grid[start.x][start.y] == 0 || grid[end.x][end.y] == 0)
        return false;
    else
        return true;
}

void Astar::read_file(string input)
{
    ifstream file;
    file.open(input);

    if (file.is_open())
    {
        file >> rows;
        file >> columns;
        vector<int> help;           //��dek matice grid (bludi�t�)
        vector<Cell> cells_help;    //��dek matice grid_cells (pro vkl�d�n� ��dk� bun�k)
        int help_;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                file >> help_;
                help.push_back(help_);

                Cell c;     //Pomocna bunka pro vkladani do grid_cells
                c.x = i;
                c.y = j;
                c.parentX = -1;
                c.parentY = -1;
                c.f_cost = 0;
                c.g_cost = 0;
                c.h_cost = 0;

                cells_help.push_back(c);
            }
            grid.push_back(help);
            grid_cells.push_back(cells_help);
            help.clear();
            cells_help.clear();
        }
    }
    else
    {
        throw MyException("Soubor se nepovedlo otevrit");
    }

}

void Astar::search()
{
    //vector<vector<bool>> explored;

    for (int i = 0; i < m; i++)
    {
        vector<bool> help;
        for (int j = 0; j < n; j++)
        {
            help.push_back(false);
        }
        explored.push_back(help);
        help.clear();
    }


    vector<vector<bool>> on_border;

    for (int i = 0; i < m; i++)
    {
        vector<bool> help;
        for (int j = 0; j < n; j++)
        {
            help.push_back(false);
        }
        on_border.push_back(help);
        help.clear();
    }

    has_solution = false;

    grid_cells[start.x][start.y].f_cost = 0;
    grid_cells[start.x][start.y].h_cost = 0;
    grid_cells[start.x][start.y].g_cost = 0;
    grid_cells[start.x][start.y].parentX = start.x;
    grid_cells[start.x][start.y].parentY = start.y;


    border.push(grid_cells[start.x][start.y]);
    on_border[start.x][start.y] = true;

    while (!border.empty())  //Dokud nen� hranice pr�zdn�
    {

        Cell help = border.top();
        while (!border.empty())
        {
            if (explored[help.x][help.y] == true)
            {
                border.pop();
                help = border.top();
            }    
            else
            {
                break;
            }
        }
        border.pop();
        on_border[help.x][help.y] = false;      //Help jsme vyjmuli z hranice

        explored[help.x][help.y] = true;        //Help bude prozkouman� po t�to iteraci
        if (is_end(help))                        //Kontrola, jestli help nen� c�l
        {
            has_solution = true;
            this->make_path();
            return;
        }

        for (int i = -1; i <= 1; i++)     //Tyto dva for cykly jsou pro prozkoum�n� v�ech soused� uzlu help
        {
            for (int j = -1; j <= 1; j++)
            {
                if ((help.x + i < 0 || help.x + i >= m) || (help.y + j < 0 || help.y + j >= n))
                {
                    continue;
                }
                if ((i == 0 && j == 0) || (grid[help.x + i][help.y + j] == 1))   //Pokud jsme v na�em prvku a nebo sousedn� prvek je p�ek��ka
                {
                    continue;
                }
                if (explored[help.x + i][help.y + j] == true)    //Pokud je ji� prvek na pozici i,j prozkouman�
                {
                    continue;
                }

                int just_g_cost;    //St�vaj�c� g_cost
 
                if (i == 0 || j == 0)    //Pokud je sousedn� prvek kolmo od na�eho prvku
                {
                    just_g_cost = grid_cells[help.x][help.y].g_cost + 10;
                }
                else        //Pokud je sousedn� prvek na diagon�le
                {
                    just_g_cost = grid_cells[help.x][help.y].g_cost + 14;
                }

                if (on_border[help.x + i][help.y + j] == false)        //Kontrola, zda je�t� nen� prvek na pozici i,j u� na hranici a nebo je st�vaj�c� g_cost lep��
                {
                    //border.push(grid_cells[i][j]);
                    grid_cells[help.x + i][help.y + j].parentX = help.x;
                    grid_cells[help.x + i][help.y + j].parentY = help.y;
                    grid_cells[help.x + i][help.y + j].g_cost = just_g_cost;
                    grid_cells[help.x + i][help.y + j].h_cost = heurestic(grid_cells[help.x + i][help.y + j], end);
                    grid_cells[help.x + i][help.y + j].f_cost = grid_cells[help.x + i][help.y + j].g_cost + grid_cells[help.x + i][help.y + j].h_cost;
                    border.push(grid_cells[help.x + i][help.y + j]);
                    on_border[help.x + i][help.y + j] = true;
                }
                else if (just_g_cost < grid_cells[help.x + i][help.y + j].g_cost)      //Kontrola, kter� g_cost je lep��
                {
                    grid_cells[help.x + i][help.y + j].parentX = help.x;
                    grid_cells[help.x + i][help.y + j].parentY = help.y;
                    grid_cells[help.x + i][help.y + j].g_cost = just_g_cost;
                    grid_cells[help.x + i][help.y + j].h_cost = heurestic(grid_cells[help.x + i][help.y + j], end);
                    grid_cells[help.x + i][help.y + j].f_cost = grid_cells[help.x + i][help.y + j].g_cost + grid_cells[help.x + i][help.y + j].h_cost;
                    border.push(grid_cells[help.x + i][help.y + j]);
                    on_border[help.x + i][help.y + j] = true;
                }
                
            }
        }
    }
    
    
}

bool Astar::is_end(const Cell& a)
{
    if (a.x == end.x && a.y == end.y)
    {
        return true;
    }
    return false;
}

void Astar::write_path()
{
    for (int i = 0; i < path.size(); i++)
    {
        cout << "(" << path[i].x << "," << path[i].y << ")" << " ";
    }
    cout << endl;
}

void Astar::make_path()
{
    if (has_solution == false)
    {
        return;
    }


    vector<Cell> path_help; //Vytvo�� cestu od konce do c�le

    int x = end.x;
    int y = end.y;
    while (x != start.x || y != start.y)
    {
        path_help.push_back(grid_cells[x][y]);
        int i = x;
        int j = y;
        x = grid_cells[i][j].parentX;
        y = grid_cells[i][j].parentY;
    }
    path_help.push_back(grid_cells[start.x][start.y]);

    for (int i = path_help.size()-1; i >= 0; i--)
    {
        path.push_back(path_help[i]);
    }


}


void Astar::write()
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}



