#include "Maze.h"

#include <ctime>
#include <cstdio>
#include <vector>
using namespace std;

typedef vector<int> vi;

// Union-Find Disjoint Sets Library written in OOP manner, using both path compression and union by rank heuristics
class UnionFind {                                              // OOP style
private:
	vi p, rank, setSize;                       // remember: vi is vector<int>
	int numSets;
public:
	UnionFind(int N) {
		setSize.assign(N, 1); numSets = N; rank.assign(N, 0);
		p.assign(N, 0); for (int i = 0; i < N; i++) p[i] = i;
	}
	int findSet(int i) { return (p[i] == i) ? i : (p[i] = findSet(p[i])); }
	bool isSameSet(int i, int j) { return findSet(i) == findSet(j); }
	void unionSet(int i, int j) {
		if (!isSameSet(i, j)) {
			numSets--;
			int x = findSet(i), y = findSet(j);
			// rank is used to keep the tree short
			if (rank[x] > rank[y]) { p[y] = x; setSize[x] += setSize[y]; }
			else {
				p[x] = y; setSize[y] += setSize[x];
				if (rank[x] == rank[y]) rank[y]++;
			}
		}
	}
	int numDisjointSets() { return numSets; }
	int sizeOfSet(int i) { return setSize[findSet(i)]; }
};

Maze::Maze(int n, int start, int end)
{
	UnionFind ufd(n*n);

	this->map = new bool*[n*n];
	for (int i = 0; i < n*n; i++) {
		this->map[i] = new bool[4];
		for (int j = 0; j < 4; j++)
			this->map[i][j] = true;
	}

	srand(time(NULL));

	while (!ufd.isSameSet(start, end)) {
		int cell_1 = rand() % (n*n);
		int random_wall = 1 + rand() % 2;

		if ((cell_1%n == n - 1) && (cell_1 / n == (n - 1))) {
			random_wall = -1;
		}
		else if (cell_1%n == n - 1) { //last column in the grid
			 random_wall = 2;  //we cannot destroy wall 1 because it's the grid container
		}

		else if (cell_1 / n == (n - 1)) {  //the upper row of the grid
			 random_wall = 1; //we cannot destroy wall 2 because it's the grid container
		}

		int cell_2 = cell_1 + 1;  //next cell to the right
		if (random_wall == 2)
			cell_2 = cell_1 + n; //next cell to the top

		if (random_wall != -1 && !ufd.isSameSet(cell_1, cell_2)) {
			ufd.unionSet(cell_1, cell_2);
			this->map[cell_1][random_wall] = false;
			this->map[cell_2][(random_wall + 2) % 4] = false;
		}
	}

}

bool**  Maze::getMap() {
	return this->map;

}