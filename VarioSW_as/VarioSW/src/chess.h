/*
 * chess.h
 *
 * Created: 5.5.2019 15:38:29
 *  Author: Jena
 */ 


#ifndef CHESS_H_
#define CHESS_H_

#define CHESSBOARD_OFFSET_LEFT 0
#define CHESSBOARD_OFFSET_TOP 0
#define CHESS_SIZE 23

#define CHESS_PAWN 1
#define CHESS_ROOK 2
#define CHESS_KNIGHT 3
#define CHESS_BISHOP 4
#define CHESS_KING 5
#define CHESS_QUEEN 6

#define CHESS_BLACK 1
#define CHESS_WHITE 0

typedef struct ChessPiece{
	uint8_t pos_x;
	uint8_t pos_y;
	uint8_t alive;
	uint8_t type;
	uint8_t color;
	uint8_t moved;
} ChessPiece;

typedef struct ChessGameStruct{
	ChessPiece ChessPieces[32];
	uint8_t turn;
	uint8_t selectedField;
	int8_t chosenField;
	int8_t kingisdead;
} ChessGameStruct;



void playChessGame();
void initChessGame();
void movePiece(int origin, int destination);
void chooseChessField(int8_t field);
void selectChessField(int8_t field);
void initChessPieces();
void drawChessBoard();
void drawPawn(int x, int y, int color);
void drawBishop(int x, int y, int color);
void drawQueen(int x, int y, int color);
void drawKing(int x, int y, int color);
void drawRook(int x, int y, int color);
void drawKnight(int x, int y, int color);





#endif /* CHESS_H_ */