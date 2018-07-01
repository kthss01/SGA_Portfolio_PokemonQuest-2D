#pragma once

#define BIGNUM 5000

struct aStarTile {
	bool walkable;
	bool listOn;
	int x, y;
	int F, G, H;

	aStarTile* parent;
};

#include "GameObject\TileMap.h"

class AStar {
private:
	typedef struct tagAStarVERTEX {
		Vector2 position;
		DWORD color;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	} ASTARVERTEX, *LPASTARVERTEX;

	aStarTile _tile[TILE_ROW][TILE_COL];
	vector<aStarTile*> _openList;
	vector<aStarTile*> _closeList;

	int _startX, _startY;
	int _endX, _endY;

	int _lastIndex;

	int _Cx;
	int _Cy;
	int _Cg;

	aStarTile * _targetTile;

	TileMap* _map;
public:
	AStar();
	~AStar();

	void PathInit(int startX, int startY, int endX, int endY);
	void PathFind();

	void SetStart(int x, int y) { 
		_startX = x; 
		_startY = y;
	}

	void SetEnd(int x, int y) {
		_endX = x;
		_endY = y;
	}

	POINT GetPosition() { 
		if (_targetTile != NULL) {
			POINT pos = { _targetTile->x, _targetTile->y };
			return pos;
		}
		return POINT();
	}

	aStarTile* GetTargetTile() { return _targetTile; }

	void UpdateTargetTile() {
		_targetTile = _targetTile->parent;
	}

	void SetTileMap(TileMap* map) { _map = map; }

	void DrawPath(DWORD color = 0xff000000);
};