#ifndef CFIELD_H
#define CFIELD_H

#include <inttypes.h>
#include <vector>
#include <QString>
class QIODevice;

namespace ActorRobot {

enum WallDir {
	LEFT_WALL = 1,
	RIGHT_WALL = 2,
	DOWN_WALL =  4,
	UP_WALL = 8
};

static const int MIN_TEMP = -273;

struct CFieldItem
{
	CFieldItem();
	void setWalls(int wallByte);
	int getWalls() const;
	bool isEmpty(int wMask = 0) const;

	bool isColored;
	bool mark;
	QChar upChar, downChar;
	float radiation;
	float temperature;

	bool upWall;
	bool downWall;
	bool rightWall;
	bool leftWall;
};

class ConsoleField
{
public:
	ConsoleField(uint32_t rows, uint32_t cols);
	void reset(uint32_t rows, uint32_t cols);

	const CFieldItem *getItem(uint32_t row, uint32_t col) const;
	CFieldItem *getItem(uint32_t row, uint32_t col);
	CFieldItem *getCurItem();
	static int createWallMask(uint32_t x, uint32_t y, uint32_t mx, uint32_t my);

	bool goLeft();
	bool goRight();
	bool goUp();
	bool goDown();

	bool isUpWall() const;
	bool isDownWall() const;
	bool isLeftWall() const;
	bool isRightWall() const;

	uint32_t  robotCol() const { return roboCol; }
	uint32_t  robotRow() const { return roboRow; }
	uint32_t  Cols() const { return roboCols; }
	uint32_t  Rows() const { return roboRows; }

	int saveToFile(const QString &filename) const;
	int saveToDataStream(QIODevice &stream) const;
	int loadFromFile(const QString &filename);
	int loadFromDataStream(QIODevice *stream);

private:
	std::vector< std::vector<CFieldItem> > items;
	uint32_t roboRow, roboCol;
	uint32_t roboRows, roboCols;
};

} // ActorRobot namespace
#endif // CFIELD_H
