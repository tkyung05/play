#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

void move(const vector<vector<int>>& points, const int robotNumber, const vector<int>& route);

const int HEIGHT = 101;
const int WIDTH = 101;

vector<vector<unordered_map<int, int>>> center(HEIGHT, vector<unordered_map<int, int>>(WIDTH));

int solution(vector<vector<int>> points, vector<vector<int>> routes)
{
    int warningCount = 0;

    for (int robotNumber = 0; robotNumber < routes.size(); robotNumber++)
    {
        move(points, robotNumber, routes[robotNumber]);
    }

    for (int y = 1; y < HEIGHT; y++)
    {
        for (int x = 1; x < WIDTH; x++)
        {
            for (auto& visitedTime : center[y][x])
            {
                if (visitedTime.second > 1)
                    warningCount++;
            }
        }
    }

    return warningCount;
}

void move(const vector<vector<int>>& points, const int robotNumber, const vector<int>& route)
{
    int time = 0;
    vector<int> position = { points[route[0] - 1][0], points[route[0] - 1][1] };
    center[position[0]][position[1]][time]++;

    for (int point : route)
    {
        vector<int> destination = points[point - 1];
        int directionY = position[0] < destination[0] ? 1 : -1;
        int directionX = position[1] < destination[1] ? 1 : -1;

        while (position[0] != destination[0])
        {
            time++;
            position[0] += directionY;
            center[position[0]][position[1]][time]++;
        }

        while (position[1] != destination[1])
        {
            time++;
            position[1] += directionX;
            center[position[0]][position[1]][time]++;
        }
    }
}