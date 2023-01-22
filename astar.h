#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <string>

struct Cell
{
    int x, y;
    int parentX, parentY;
    int g_cost;      //Skutečná vzdálenost od startu
    int h_cost;      //Heurestické náklady
    int f_cost;      //f = g + h

    bool operator<(const Cell& b);
};

struct Compare_Cells
{
    bool operator()(const Cell& a, const Cell& b)
    {
        return a.f_cost > b.f_cost;
    }
};

int heurestic(const Cell& a, const Cell& end);

class Astar
{
private:
    std::vector<std::vector<int>> grid;         //Bludiště
    std::vector<std::vector<Cell>> grid_cells;      //Matice jednotlivých buněk bludiště
    int n, m;           //Rozměry
    std::priority_queue<Cell, std::vector<Cell>, Compare_Cells> border;   //Prioritní fronta pro hranici
    std::vector<Cell> path;             //Výsledná cesta
    Cell start;
    Cell end;
    bool has_solution;

    int rows;
    int columns;

    std::vector<std::vector<bool>> explored;

    void make_path();

public:
    //Astar(std::string input, int start_x, int start_y, int end_x, int end_y);
    Astar(int rows, int columns);

    void setStart(int x, int y);
    void setEnd(int x, int y);
    std::vector<Cell> getPath();
    bool getHasSolution(){return this->has_solution;}
    void setGrid(int x, int y, int value);
    std::vector<std::vector<bool>> getExplored();

    bool is_start_end_valid();
    void read_file(std::string input);


    void search();
    //void make_path();

    bool is_end(const Cell& a);

    void write_path();

    void write();

};

#endif // ASTAR_H
