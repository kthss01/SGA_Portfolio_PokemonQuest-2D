#include "stdafx.h"
#include "AStar.h"

AStar::AStar()
{
}

AStar::~AStar()
{
}

void AStar::PathInit(int startX, int startY, int endX, int endY)
{
	this->SetStart(startX, startY);
	this->SetEnd(endX, endY);

	tagTile(*tiles)[TILE_COL] = _map->GetTiles();

	for (int i = 0; i < TILE_ROW; i++) {
		for (int j = 0; j < TILE_COL; j++) {
			_tile[i][j].listOn = false;
			_tile[i][j].parent = NULL;
			_tile[i][j].F = BIGNUM;
			_tile[i][j].G = 0;
			_tile[i][j].H = 0;
			_tile[i][j].x = i;
			_tile[i][j].y = j;

			if (tiles[i][j].block)
				_tile[i][j].walkable = false;
			else
				_tile[i][j].walkable = true;
		}
	}

	_openList.clear();
	_closeList.clear();

	_tile[_endX][_endY].listOn = true;
	_Cx = _tile[_endX][_endY].x;
	_Cy = _tile[_endX][_endY].y;
	_Cg = _tile[_endX][_endY].G;

	_closeList.push_back(&_tile[_endX][_endY]);

	_lastIndex = 0;
	_targetTile = NULL;
}

void AStar::PathFind()
{
	// left, right, up, down, leftup, rightdown, leftdown, rightup
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, 1, 1, -1 };
	bool check[8];

	while (true) {
		for (int i = 0; i < 8; i++) check[i] = false;

		// AddOpenList()
		_Cx = _closeList[_lastIndex]->x;
		_Cy = _closeList[_lastIndex]->y;
		_Cg = _closeList[_lastIndex]->G;

		for (int i = 0; i < 8; i++) {
			int x = _Cx + dx[i];
			int y = _Cy + dy[i];
			if (0 <= x && x < TILE_ROW && 0 <= y && y < TILE_COL) {
				if (_tile[x][y].walkable) {
					check[i] = true;
					if (i < 4) {
						if (!_tile[x][y].listOn) {
							_tile[x][y].listOn = true;
							_tile[x][y].G = _Cg + 10;
							_tile[x][y].parent =
								_closeList[_lastIndex];
							_openList.push_back(&_tile[x][y]);
						}
						else {
							if (_Cg + 10 < _tile[x][y].G) {
								_tile[x][y].G = _Cg + 10;
								_tile[x][y].parent =
									_closeList[_lastIndex];
							}
						}
					}
					else {
						// leftup
						if ((i == 4 && check[0] && check[2]) ||
							// rightdown
							(i == 5 && check[1] && check[3]) ||
							// leftdown	
							(i == 6 && check[0] && check[3]) ||
							// rightup
							(i == 7 && check[1] && check[2])) {

							if (!_tile[x][y].listOn) {
								_tile[x][y].listOn = true;
								_tile[x][y].G = _Cg + 14;
								_tile[x][y].parent =
									_closeList[_lastIndex];
								_openList.push_back(&_tile[x][y]);
							}
							else {
								if (_Cg + 14 < _tile[x][y].G) {
									_tile[x][y].G = _Cg + 14;
									_tile[x][y].parent =
										_closeList[_lastIndex];
								}
							}
						}
					}
				}
			}
		}

		// no way
		if (_openList.size() == 0) {
			_targetTile = NULL;
			break;
		}

		// CalCulate H, F
		for (int i = 0; i < _openList.size(); i++) {
			int vertical = (_startX - _openList[i]->x) * 10;
			int horizontal = (_startY - _openList[i]->y) * 10;

			// 방향이 반대로 넘어가는 경우
			if (vertical < 0) vertical *= -1;
			if (horizontal < 0) horizontal *= -1;

			_openList[i]->H = vertical + horizontal;

			_openList[i]->F = _openList[i]->G + _openList[i]->H;
		}

		// AddCloseList()
		int index = 0;
		int min = BIGNUM;

		for (int i = 0; i < _openList.size(); i++) {
			if (_openList[i]->F < min) {
				min = _openList[i]->F;
				index = i;
			}
		}

		_closeList.push_back(_openList[index]);
		_openList.erase(_openList.begin() + index);

		_lastIndex++;

		// Check Arrive()
		// found path
		if (_closeList[_lastIndex]->x == _startX &&
			_closeList[_lastIndex]->y == _startY) {
			_targetTile = _closeList[_lastIndex]->parent;
			break;
		}
	}
}

void AStar::DrawPath(DWORD color)
{
	vector< pair<int, int> > path;
	aStarTile* temp = _targetTile;
	while (temp != NULL) {
		path.push_back(make_pair(temp->x, temp->y));
		temp = temp->parent;
	}

	LPASTARVERTEX vertices = new ASTARVERTEX[path.size()];

	for (int i = 0; i < path.size(); i++) {
		vertices[i].position = _map->GetTileCenterPos(
			path[i].first, path[i].second);
		vertices[i].color = color;
	}

	if (path.size() != 0) {
		D2D::GetDevice()->SetFVF(ASTARVERTEX::FVF);
		D2D::GetDevice()->DrawPrimitiveUP(
			D3DPT_LINESTRIP, path.size()-1, vertices, sizeof(ASTARVERTEX));
	}
	
	delete[] vertices;
}
