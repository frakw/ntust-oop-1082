#pragma once
#include <vector>
class Map;
class Creature;
typedef struct _Coord_ Coord;

using namespace std;
class Grid {
public:
	Coord coord;
	Grid* father = nullptr;
	int F = 0;
	int G = 0;
	int H = 0;
	Grid(Coord in, Grid* the_father, Coord end_point);
	void calc_val(Grid* the_father, Coord end_point);

};
int exist_in(vector<Grid*>* data_ptr, Coord check);