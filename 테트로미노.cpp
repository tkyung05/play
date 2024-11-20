#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

const vector<vector<int>> I = { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } };
const vector<vector<int>> O = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } };
const vector<vector<int>> T = { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 1 } };
const vector<vector<int>> J = { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 2 } };
const vector<vector<int>> L = { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 0 } };
const vector<vector<int>> S = { { 0, 0 }, { 0, 1 }, { -1, 1 }, { -1, 2 } };
const vector<vector<int>> Z = { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } };

vector<vector<int>> paper;
int height, width;

void input();
vector<vector<vector<int>>> rotate(const vector<vector<int>>& tetromino);
int put(const vector<int>& position, const vector<vector<int>>& tetromino);

int main()
{
	input();
	
	int max_num = 0;
	vector<vector<vector<int>>> rotated_tetrominos;

	for (vector<vector<int>> tetromino : { I, O, T, J, L, S, Z })
	{
		for (vector<vector<int>> rotated_tetromino : rotate(tetromino))
			rotated_tetrominos.push_back(rotated_tetromino);
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (const vector<vector<int>>& tetromino : rotated_tetrominos)
			{
				int num = put({ y, x }, tetromino);
				max_num = max(max_num, num);
			}
		}
	}

	printf("%d", max_num);
	return 0;
}

void input()
{
	scanf_s("%d %d", &height, &width);
	paper.resize(height, vector<int>(width));
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++) 
			scanf_s("%d", &paper[y][x]);
	}
}

vector<vector<vector<int>>> rotate(const vector<vector<int>>& tetromino)
{
	vector<vector<int>> rotated_90_tetromino;
	vector<vector<int>> rotated_180_tetromino;
	vector<vector<int>> rotated_270_tetromino;

	for (const vector<int>& block : tetromino)
		rotated_90_tetromino.push_back({ block[1] * -1, block[0] * -1 });
	for (const vector<int>& block : tetromino)
		rotated_180_tetromino.push_back({ block[0] * -1, block[1] * -1 });
	for (const vector<int>& block : tetromino)
		rotated_270_tetromino.push_back({ block[1], block[0] });

	return { tetromino, rotated_90_tetromino, rotated_180_tetromino, rotated_270_tetromino };
}

int put(const vector<int>& position, const vector<vector<int>>& tetromino)
{
	int num = 0;

	for (const vector<int>& block : tetromino)
	{
		vector<int> put_position = { position[0] + block[0], position[1] + block[1] };
		
		if (put_position[0] < 0 || put_position[0] >= height || put_position[1] < 0 || put_position[1] >= width)
			return 0;

		num += paper[put_position[0]][put_position[1]];
	}

	return num;
}
