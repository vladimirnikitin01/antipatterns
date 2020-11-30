//
// Created by dmitry on 5/14/20.
//

#ifndef ANTIPATTERNS_ASTAR_H
#define ANTIPATTERNS_ASTAR_H

#include "Constants/GamePlayConstants.h"
#include <algorithm>
#include <cmath>
#include <set>
#include <vector>

class AStar {
public:
  struct Point {
    int x = 0;
    int y = 0;
    int f = 0;
    int g = 0;
    int h = 0; //из формул A*

    Point(int x_, int y_);

    Point() {}

    bool operator<(const Point &b) const;
  };

  struct cmp {
    bool operator()(Point a, Point b);
  };

  class Graph {
  private:
    const std::vector<std::vector<int>>
        adj; //поле/матрица если мы не можем пройти по клетке, то ее значение -1
    std::vector<std::vector<bool>>
        open_list_bool; //проверка в открытом списке или нет
    std::vector<std::vector<bool>>
        close_list; // проверка в закрытом списке по индексу точки
    int count_in_open_list = 0;
    int width = 0;
    int length = 0;

  public:
    Graph() = default;

    explicit Graph(const std::vector<std::vector<int>> &map);

    std::pair<int, int> Size();

    int GetWidth();

    int GetLength();

    int GetH(const Point &end, const Point &now);

    int GetG(const Point &current, const Point &now);

    void MyErase(const Point &now, std::set<Point, cmp> &open_list);

    void ChangeGHF(const Point &current, Point &now,
                   std::set<Point, cmp> &open_list,
                   std::vector<std::vector<std::pair<int, int>>> &parents);

    bool IsInsideCloseList(const Point &now);

    bool IsInsideOpenList(const Point &now);

    void AssignGHF(const Point &end, Point &current, Point &now,
                   std::set<Point, cmp> &open_list,
                   std::vector<std::vector<std::pair<int, int>>> &parents,
                   bool &status);

    void AStar(int x1, int y1, int x2, int y2,
               std::vector<std::vector<std::pair<int, int>>> &parents);

    bool IsLineSolid(float x1, float y1, float x2, float y2);
  };

private:
  Graph graph_;
  const int grid_size_;

public:
  explicit AStar(const std::vector<std::vector<int>> &map, int grid_size);

  std::vector<std::pair<int, int>> AlgorithmAStar(std::pair<int, int> start,
                                                  std::pair<int, int> end);

  std::pair<int, int> GetPoint(std::pair<int, int> start,
                               std::pair<int, int> end);

  std::vector<std::pair<int, int>> GetRoad(std::pair<int, int> start,
                                           std::pair<int, int> end);

  bool IsLineSolid(float start_x, float start_y, float end_x, float end_y);

  int GetGridSize() const;
};

#endif // ANTIPATTERNS_ASTAR_H
