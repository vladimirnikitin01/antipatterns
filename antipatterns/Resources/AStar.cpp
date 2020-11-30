//
// Created by dmitry on 5/14/20.
//

#include "AStar.h"

#include <iostream>
#include <utility>
AStar::Point::Point(int x_, int y_) {
  x = x_;
  y = y_;
}
bool AStar::Point::operator<(const AStar::Point &b) const {
  return (f < b.f || (f == b.f && x < b.x) ||
          (f == b.f && x == b.x && y < b.y));
}

bool AStar::cmp::operator()(AStar::Point a, AStar::Point b) { return (a < b); }

AStar::Graph::Graph(const std::vector<std::vector<int>> &map) : adj(map) {
  width = map.size();
  length = map.at(0).size();
}
std::pair<int, int> AStar::Graph::Size() { return {width, length}; }

int AStar::Graph::GetWidth() { return (width); }

int AStar::Graph::GetLength() { return (length); }

int AStar::Graph::GetH(const AStar::Point &end, const AStar::Point &now) {
  int h_x = abs(end.x - now.x) + abs(end.y - now.y);
  return (h_x);
}
int AStar::Graph::GetG(const AStar::Point &current, const AStar::Point &now) {
  return (current.g + adj[now.x][now.y]);
}
void AStar::Graph::MyErase(const AStar::Point &now,
                           std::set<Point, cmp> &open_list) {
  for (auto i = open_list.begin(); i != open_list.end(); ++i) {
    if (i->x == now.x && i->y == now.y) {
      open_list.erase(i);
      break;
    }
  }
}
void AStar::Graph::ChangeGHF(
    const AStar::Point &current, AStar::Point &now,
    std::set<Point, cmp> &open_list,
    std::vector<std::vector<std::pair<int, int>>> &parents) {
  int new_g = current.g + adj[now.x][now.y];
  if (new_g < now.g) {
    MyErase(now, open_list);
    now.g = new_g;
    now.f = now.h + now.g;
    parents[now.x][now.y] = {current.x, current.y};
    open_list.insert(now);
  }
}
bool AStar::Graph::IsInsideCloseList(const AStar::Point &now) {
  return close_list[now.x][now.y];
}
bool AStar::Graph::IsInsideOpenList(const AStar::Point &now) {
  return open_list_bool[now.x][now.y];
}
void AStar::Graph::AssignGHF(
    const AStar::Point &end, AStar::Point &current, AStar::Point &now,
    std::set<Point, cmp> &open_list,
    std::vector<std::vector<std::pair<int, int>>> &parents, bool &status) {
  if (IsInsideOpenList(now)) {
    ChangeGHF(current, now, open_list, parents);
  } else {
    now.g = GetG(current, now);
    now.h = GetH(end, now);
    now.f = now.g + now.h;
    parents[now.x][now.y] = {current.x, current.y};
    open_list.insert(now);
    ++count_in_open_list;
    open_list_bool[now.x][now.y] = true;
    if (now.x == end.x && now.y == end.y) {
      status = true;
    }
  }
}
void AStar::Graph::AStar(
    int x1, int y1, int x2, int y2,
    std::vector<std::vector<std::pair<int, int>>> &parents) {
  int w = adj.size();
  int l = adj[0].size();
  close_list.resize(w);
  for (size_t i = 0; i < w; ++i) {
    close_list[i].assign(l, false);
  }
  open_list_bool.resize(w);
  for (size_t i = 0; i < w; ++i) {
    open_list_bool[i].assign(l, false);
  }

  std::set<Point, cmp> open_list;
  Point start(x1, y1);
  start.f = 0;
  Point end(x2, y2);
  open_list.insert(start);
  ++count_in_open_list;
  bool status = false; //дошли ли до конечной
  while (count_in_open_list > 0) {
    Point current = *open_list.begin();
    open_list.erase(*open_list.begin());
    --count_in_open_list;
    close_list[current.x][current.y] = true;
    open_list_bool[current.x][current.y] = false;
    int i = current.x;
    int j = current.y;
    //теперь рассматриваем всех соседей
    if (i - 1 >= 0 && adj[i - 1][j] != room_const::kSolidTileValue) { // left
      Point now(i - 1, j);
      if (!IsInsideCloseList(now)) {
        AssignGHF(end, current, now, open_list, parents, status);
      }
      if (status) {
        break;
      }
    }
    if (i + 1 < width &&
        adj[i + 1][j] != room_const::kSolidTileValue) { // right
      Point now(i + 1, j);
      if (!IsInsideCloseList(now)) {
        AssignGHF(end, current, now, open_list, parents, status);
      }
      if (status) {
        break;
      }
    }
    if (j - 1 >= 0 && adj[i][j - 1] != room_const::kSolidTileValue) { // up
      Point now(i, j - 1);
      if (!IsInsideCloseList(now)) {
        AssignGHF(end, current, now, open_list, parents, status);
      }
      if (status) {
        break;
      }
    }
    if (j + 1 < length &&
        adj[i][j + 1] != room_const::kSolidTileValue) { // down
      Point now(i, j + 1);
      if (!IsInsideCloseList(now)) {
        AssignGHF(end, current, now, open_list, parents, status);
      }
      if (status) {
        break;
      }
    }
  }
}
bool AStar::Graph::IsLineSolid(float x1, float y1, float x2, float y2) {
  float abs_dist = sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
  if (abs_dist < move_const::kSmallValue) {
    return false;
  }
  float cos_a = (x2 - x1) / abs_dist, sin_a = (y2 - y1) / abs_dist;
  int x1_i{}, x2_i;
  while (sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) >
         2 * move_const::kLineStepValue / 3) {
    x1 += move_const::kLineStepValue * cos_a;
    y1 += move_const::kLineStepValue * sin_a;
    x1_i = static_cast<int>(std::floor(x1));
    x2_i = static_cast<int>(std::floor(y1));
    if (adj.at(x1_i).at(x2_i) == room_const::kSolidTileValue) {
      return true;
    }
  }
  return false;
}

AStar::AStar(const std::vector<std::vector<int>> &map, int grid_size)
    : graph_(map), grid_size_(grid_size) {}

std::vector<std::pair<int, int>>
AStar::AlgorithmAStar(std::pair<int, int> start, std::pair<int, int> end) {
  std::vector<std::vector<std::pair<int, int>>> parent;
  int w = graph_.GetWidth();
  parent.resize(w);
  for (size_t i = 0; i < w; ++i) {
    parent[i].assign(graph_.GetLength(), {-1, -1});
  }

  graph_.AStar(start.first, start.second, end.first, end.second, parent);
  std::vector<std::pair<int, int>> road;
  int x = end.first;
  int y = end.second;
  road.emplace_back(x, y);
  while (true) {
    if (x == start.first && y == start.second) {
      break;
    }
    int x1 = parent[x][y].first;
    int y1 = parent[x][y].second;
    if (x1 == room_const::kSolidTileValue ||
        y1 == room_const::kSolidTileValue) {
      road.clear();
      road.emplace_back(start);
      return road;
    }
    x = x1;
    y = y1;
    road.emplace_back(x, y);
  }
  std::reverse(road.begin(), road.end());
  return (road);
}
std::pair<int, int> AStar::GetPoint(std::pair<int, int> start,
                                    std::pair<int, int> end) {
  auto road = AlgorithmAStar(start, end);
  // start == end
  if (road.size() == 1) {
    return {road[0].first, road[0].second};
  }
  return {road[1].first, road[1].second};
}
std::vector<std::pair<int, int>> AStar::GetRoad(std::pair<int, int> start,
                                                std::pair<int, int> end) {
  return (AlgorithmAStar(start, end));
}
bool AStar::IsLineSolid(float start_x, float start_y, float end_x,
                        float end_y) {
  return graph_.IsLineSolid(start_x / static_cast<float>(grid_size_),
                            start_y / static_cast<float>(grid_size_),
                            end_x / static_cast<float>(grid_size_),
                            end_y / static_cast<float>(grid_size_));
}
int AStar::GetGridSize() const { return grid_size_; }
