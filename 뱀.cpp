#include <iostream>
#include <vector>
#include <unordered_map>
#include <deque>

using namespace std;
using Position = vector<int>;

enum BoardElement
{
	EMPTY = 'E',
	SNAKE = 'S',
	APPLE = 'A',
};

enum Direction
{
	RIGHT = 'D',
	LEFT = 'L',
};

const Position G_RIGHT = { 0, 1 };
const Position G_LEFT = { 0, -1 };
const Position G_UP = { -1, 0 };
const Position G_DOWN = { 1, 0 };

vector<vector<BoardElement>> board;
vector<vector<Direction>> commands(10001);
deque<Position> snake = { { 0, 0 } };
Position look = G_RIGHT;

void init();
bool is_game_over(const Position& position);
void stretch(const Position& next);
void roll_up(const Position& tail);
void rotate(const Direction direction);

int main()
{
	init();

	int time = 1;

	while (true)
	{
		Position head = snake.front();
		Position tail = snake.back();
		Position next = { head[0] + look[0], head[1] + look[1] };

		if (is_game_over(next))
			break;

		if (board[next[0]][next[1]] == APPLE)
		{
			stretch(next);
		}
		if (board[next[0]][next[1]] == EMPTY)
		{
			stretch(next);
			roll_up(tail);
		}

		for (Direction direction : commands[time])
			rotate(direction);

		time++;
	}

	cout << time << endl;
	
	return 0;
}

void init()
{
	int board_size;
	cin >> board_size;
	board.resize(board_size, vector<BoardElement>(board_size, EMPTY));
	board[0][0] = SNAKE;

	int apple_count;
	cin >> apple_count;
	for (int i = 0; i < apple_count; i++)
	{
		int y, x;
		cin >> y >> x;
		board[y - 1][x - 1] = APPLE;
	}

	int command_count;
	cin >> command_count;
	for (int i = 0; i < command_count; i++)
	{
		int time;
		char direction;
		cin >> time >> direction;
		commands[time].push_back(static_cast<Direction>(direction));
	}
}

bool is_game_over(const Position& position)
{
	if (position[0] < 0 || position[1] < 0 || position[0] >= board.size() || position[1] >= board.size())
		return true;

	if (board[position[0]][position[1]] == SNAKE)
		return true;

	return false;
}

void stretch(const Position& next)
{
	snake.push_front({ next[0], next[1] });
	board[next[0]][next[1]] = SNAKE;
}

void roll_up(const Position& tail)
{
	snake.pop_back();
	board[tail[0]][tail[1]] = EMPTY;
}

void rotate(const Direction direction)
{
	Position direction_vector;

	const bool is_rotate_right = direction == RIGHT;
	const bool is_rotate_left = direction == LEFT;

	const bool is_look_right = look[0] == G_RIGHT[0] && look[1] == G_RIGHT[1];
	const bool is_look_left = look[0] == G_LEFT[0] && look[1] == G_LEFT[1];
	const bool is_look_up = look[0] == G_UP[0] && look[1] == G_UP[1];
	const bool is_look_down = look[0] == G_DOWN[0] && look[1] == G_DOWN[1];

	if (is_look_right)
	{
		if (is_rotate_right)
			direction_vector = G_DOWN;
		if (is_rotate_left)
			direction_vector = G_UP;
	}
	if (is_look_left)
	{
		if (is_rotate_right)
			direction_vector = G_UP;
		if (is_rotate_left)
			direction_vector = G_DOWN;
	}
	if (is_look_up)
	{
		if (is_rotate_right)
			direction_vector = G_RIGHT;
		if (is_rotate_left)
			direction_vector = G_LEFT;
	}
	if (is_look_down)
	{
		if (is_rotate_right)
			direction_vector = G_LEFT;
		if (is_rotate_left)
			direction_vector = G_RIGHT;
	}

	look = direction_vector;
}