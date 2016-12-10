#pragma once
class Maze
{
public:
	bool** map;

public:
	Maze(int n, int start, int end);
	bool** getMap();
};

