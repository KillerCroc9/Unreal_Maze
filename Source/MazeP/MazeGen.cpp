// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeGen.h"
#include "CoreMinimal.h"
#include <iostream>
#include <vector>
#include <set>
#include <tuple>

// Sets default values
using namespace std;
struct DSU
{
    vector<int> parent, sze;
    void build(int n)
    {
        parent.resize(n, 0), sze.resize(n, -1);
        for (int k = 0; k < n; k++)
            parent[k] = k, sze[k] = 1;
    }
    int find(int v)
    {
        if (parent[v] == v)
            return v;
        else
            return parent[v] = find(parent[v]);
    }
    void unite(int a, int b)
    {
        a = find(a), b = find(b);
        if (a != b)
        {
            if (sze[a] < sze[b])
                swap(a, b);
            parent[b] = a, sze[a] += sze[b];
        }
    }
};
class Maze
{
public:
    vector<vector<char>> maze;
    int start;
    int end;
    int n;
    void buildmaze();
    int AssignWeight();
    Maze(int t);

private:
    set<tuple<int, int, int>> edges; // {weight,cell1,cell2} edge between cell1 and cell2
    set<pair<int, int>> mst; // {cell1,cell2} edge between cell1 and cell2
    DSU dsu;
};
Maze::Maze(int t) //Generating maze with no cells connected yet
{
    n = t;
    maze.resize(2 * n + 1, vector<char>(2 * n + 1, '-'));
    dsu.build(n * n + 1);
    for (int i = 1; i <= 2 * n + 1; i += 2)
    {
        for (int j = 1; j <= 2 * n + 1; j += 2)
        {
            if (i != 2 * n + 1 && j != 2 * n + 1)
                maze[i][j] = ' ';
            if (j != 2 * n + 1)
                maze[i - 1][j] = '-';
            maze[i - 1][j - 1] = '+';
            if (i != 2 * n + 1)
                maze[i][j - 1] = '|';
        }
    }
}

void Maze::buildmaze()
{
    for (int i = 1; i <= n; i++) // cells are numbered from 1 to n*n
    {
        int f = (i - 1) * n;
        for (int j = f + 1; j <= f + n; j++)
        {
            if (j < f + n)
            {
                int getWeight = AssignWeight();
                edges.insert({ getWeight, j, j + 1 });  // Edge between current cell and right cell
            }
            if (j + n <= n * n)
            {
                int getWeight = AssignWeight();
                edges.insert({ getWeight, j, j + n });  // Edge between current cell and bottom cell
            }
        }
    }
    
    for (auto [w, u, v] : edges)
    {
        if (dsu.find(u) != dsu.find(v)) // unite if u and v dont belong to same component
        {
            dsu.unite(u, v);
            mst.insert({ u, v });
        }
    }
    for (auto [u, v] : mst) // Removing edges that are present in MST to generate the maze
    {
        int i = (u - 1) / n, j = (u - 1) % n;
        if (v == u + 1) maze[2 * i + 1][2 * j + 2] = ' ';
        else maze[2 * i + 2][2 * j + 1] = ' ';
    }

    start = rand() % n;
    end = rand() % n;
    maze[0][2 * start + 1] = ' ';
    maze[2 * n][2 * (end)+1] = ' ';
}

int Maze::AssignWeight() // Generates Random Weight
{
    return rand() % 1000;
}

MazeGen::MazeGen()
{
    
}



void dijkstra(Maze& m)
{
    int n = 2 * m.n + 1;
    pair<int, int> start = { 0, 2 * m.start + 1 }, end = { 2 * m.n, 2 * m.end + 1 };
    vector<vector<int>> dis(n, vector<int>(n, INT_MAX)); // stores min distance from start to the current cell
    vector<vector<pair<int, int>>> par(n, vector<pair<int, int>>(n)); // storing the parent cell of each cell , used for backtracking the solution
    set<pair<int, pair<int, int>>> s; // stores {min distance to reach the cell , cell}
    s.insert({ 0, start });
    while (!s.empty())
    {
        pair<int, pair<int, int>> a = *s.begin();
        s.erase(s.begin());
        int dist = a.first, x = a.second.first, y = a.second.second;
        if (x < n - 1 && m.maze[x + 1][y] == ' ' && dis[x + 1][y] > dist + 1) // checking if distance to cell can be minimised
        {
            if (s.find({ dis[x + 1][y], {x + 1, y} }) != s.end())
                s.erase(s.find({ dis[x + 1][y], {x + 1, y} }));
            s.insert({ dist + 1, {x + 1, y} });
            dis[x + 1][y] = dist + 1; // updating the min distance as min distance of parent + 1
            par[x + 1][y] = { x, y };
        }
        if (x > 0 && m.maze[x - 1][y] == ' ' && dis[x - 1][y] > dist + 1) // checking if distance to cell can be minimised
        {
            if (s.find({ dis[x - 1][y], {x - 1, y} }) != s.end())
                s.erase(s.find({ dis[x - 1][y], {x - 1, y} }));
            s.insert({ dist + 1, {x - 1, y} });
            dis[x - 1][y] = dist + 1; // updating the min distance as min distance of parent + 1
            par[x - 1][y] = { x, y };
        }
        if (y > 0 && m.maze[x][y - 1] == ' ' && dis[x][y - 1] > dist + 1) // checking if distance to cell can be minimised
        {
            if (s.find({ dis[x][y - 1], {x, y - 1} }) != s.end())
                s.erase(s.find({ dis[x][y - 1], {x, y - 1} }));
            s.insert({ dist + 1, {x, y - 1} });
            dis[x][y - 1] = dist + 1; // updating the min distance as min distance of parent + 1
            par[x][y - 1] = { x, y };
        }
        if (y < n - 1 && m.maze[x][y + 1] == ' ' && dis[x][y + 1] > dist + 1) // checking if distance to cell can be minimised
        {
            if (s.find({ dis[x][y + 1], {x, y + 1} }) != s.end())
                s.erase(s.find({ dis[x][y + 1], {x, y + 1} }));
            s.insert({ dist + 1, {x, y + 1} });
            dis[x][y + 1] = dist + 1; // updating the min distance as min distance of parent + 1
            par[x][y + 1] = { x, y };
        }
    }
    if (dis[end.first][end.second] > 1e9) // Solution will always exist since MST always forms a connected component including all cells
    {
    }
    else
    {
        pair<int, int> x = end;
        while (x != start) // Backtracking the Maze Solution
        {
            int a = x.first, b = x.second;
            m.maze[a][b] = 'X';
            x = par[a][b];
            if (x == start)
                break;
        }
        m.maze[x.first][x.second] = 'X';
    }
}



TArray<TArray<char>> MazeGen::MazeGrid()
{
    TArray<TArray<char>> UnrealMaze;

    Maze maze(14);
    maze.buildmaze();
    dijkstra(maze);
  
    for (int i = 0; i < maze.maze.size(); i++)
    {
		TArray<char> Row;
        for (int j = 0; j < maze.maze.size(); j++)
        {
			Row.Add(maze.maze[i][j]);
		}
		UnrealMaze.Add(Row);
	}

    return UnrealMaze;
}




