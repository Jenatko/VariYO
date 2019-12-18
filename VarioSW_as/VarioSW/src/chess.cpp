/*
* chess.cpp
*
* Created: 5.5.2019 15:38:14
*  Author: Jena
*/

#include <GxEPD2/GxEPD2_BW.h>
//#include <GxGDEP015OC1/GxGDEP015OC1.h>

#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "button_control.h"

#include "chess.h"


ChessGameStruct ChessGame;


//ChessPiece ChessPieces[32];

void playChessGame(){
	initChessGame();
	drawChessBoard();
	chooseChessField(ChessGame.chosenField);
	selectChessField(ChessGame.selectedField);
	display.display();
	int pieceMoved = 0;
	
	while (1) {

		buttons.buttonUpdate();


		if (buttons.getFlag()) {

			while(buttons.getFlag()){
				switch (buttons.getButtonPressed()) {
					case (UP):
					ChessGame.selectedField -=8;
					ChessGame.selectedField = ChessGame.selectedField%64;
					
					break;

					case (DOWN):
					ChessGame.selectedField +=8;
					ChessGame.selectedField = ChessGame.selectedField%64;
					break;

					case (LEFT):
					ChessGame.selectedField -=1;
					ChessGame.selectedField = ChessGame.selectedField%64;
					break;

					case (RIGHT):
					ChessGame.selectedField +=1;
					ChessGame.selectedField = ChessGame.selectedField%64;
					break;
					
					case (PRESS):
					if(ChessGame.selectedField == ChessGame.chosenField){
						ChessGame.chosenField = -1;
					}
					else if(ChessGame.chosenField == -1){
						ChessGame.chosenField = ChessGame.selectedField;
					}
					else{
						movePiece(ChessGame.chosenField, ChessGame.selectedField);
						ChessGame.chosenField = -1;
						pieceMoved = 1;
					}
					break;
				}
			}
			drawChessBoard();
			chooseChessField(ChessGame.chosenField);
			selectChessField(ChessGame.selectedField);
			display.display(!pieceMoved);
			pieceMoved = 0;
			
		}
		if(ChessGame.kingisdead)
		break;
	}
	
}

void movePiece(int origin, int destination){
	int x_from = origin%8;
	int y_from = origin/8;
	
	int x_to = destination%8;
	int y_to = destination/8;
	

	
	int index_orig = -1;
	int index_dest = -1;
	int dest_empty=2;
	int origin_empty=2;
	
	for(int i = 0; i < 32; i++){
		if(ChessGame.ChessPieces[i].pos_x == x_from && ChessGame.ChessPieces[i].pos_y == y_from && ChessGame.ChessPieces[i].alive == 1){
			origin_empty = 0;
			index_orig = i;
			break;
		}
		else{
			origin_empty = 1;
		}
		
	}
	
	for(int i = 0; i < 32; i++){
		if(ChessGame.ChessPieces[i].pos_x == x_to && ChessGame.ChessPieces[i].pos_y == y_to && ChessGame.ChessPieces[i].alive == 1){
			dest_empty = 0;
			index_dest = i;
			break;
		}
		else{
			dest_empty=1;
		}
		
	}


	

	
	if(origin_empty == 0 && dest_empty){
		ChessGame.ChessPieces[index_orig].pos_x = x_to;
		ChessGame.ChessPieces[index_orig].pos_y = y_to;
		ChessGame.ChessPieces[index_orig].moved = 1;
	}
	else if(origin_empty == 0 && dest_empty == 0){
		if(ChessGame.ChessPieces[index_orig].color != ChessGame.ChessPieces[index_dest].color){
			ChessGame.ChessPieces[index_orig].pos_x = x_to;
			ChessGame.ChessPieces[index_orig].pos_y = y_to;
			ChessGame.ChessPieces[index_orig].moved = 1;
			ChessGame.ChessPieces[index_dest].alive	= 0;
			if(ChessGame.ChessPieces[index_dest].type == CHESS_KING)
			ChessGame.kingisdead = 1;
			//display.update();
		}
	}
	if(ChessGame.ChessPieces[index_orig].type == CHESS_PAWN)
	if(ChessGame.ChessPieces[index_orig].pos_y == 0 ||ChessGame.ChessPieces[index_orig].pos_y == 7)
	ChessGame.ChessPieces[index_orig].type = CHESS_QUEEN;
	
	
}

void initChessGame(){
	initChessPieces();
	ChessGame.chosenField = -1;
	ChessGame.selectedField = 0;
	ChessGame.kingisdead = 0;
	ChessGame.turn = CHESS_WHITE;
	
}

void initChessPieces(){
	
	ChessGame.ChessPieces[0].type = CHESS_PAWN;
	ChessGame.ChessPieces[1].type = CHESS_PAWN;
	ChessGame.ChessPieces[2].type = CHESS_PAWN;
	ChessGame.ChessPieces[3].type = CHESS_PAWN;
	ChessGame.ChessPieces[4].type = CHESS_PAWN;
	ChessGame.ChessPieces[5].type = CHESS_PAWN;
	ChessGame.ChessPieces[6].type = CHESS_PAWN;
	ChessGame.ChessPieces[7].type = CHESS_PAWN;
	
	ChessGame.ChessPieces[8].type = CHESS_ROOK;
	ChessGame.ChessPieces[8].pos_x = 0;
	ChessGame.ChessPieces[9].type = CHESS_ROOK;
	ChessGame.ChessPieces[9].pos_x = 7;
	
	ChessGame.ChessPieces[10].type = CHESS_KNIGHT;
	ChessGame.ChessPieces[10].pos_x = 1;
	ChessGame.ChessPieces[11].type = CHESS_KNIGHT;
	ChessGame.ChessPieces[11].pos_x = 6;
	
	ChessGame.ChessPieces[12].type = CHESS_BISHOP;
	ChessGame.ChessPieces[12].pos_x = 2;
	ChessGame.ChessPieces[13].type = CHESS_BISHOP;
	ChessGame.ChessPieces[13].pos_x = 5;
	
	ChessGame.ChessPieces[14].type = CHESS_QUEEN;
	ChessGame.ChessPieces[14].pos_x = 3;
	ChessGame.ChessPieces[15].type = CHESS_KING;
	ChessGame.ChessPieces[15].pos_x = 4;
	
	ChessGame.ChessPieces[16].type = CHESS_PAWN;
	ChessGame.ChessPieces[17].type = CHESS_PAWN;
	ChessGame.ChessPieces[18].type = CHESS_PAWN;
	ChessGame.ChessPieces[19].type = CHESS_PAWN;
	ChessGame.ChessPieces[20].type = CHESS_PAWN;
	ChessGame.ChessPieces[21].type = CHESS_PAWN;
	ChessGame.ChessPieces[22].type = CHESS_PAWN;
	ChessGame.ChessPieces[23].type = CHESS_PAWN;
	
	ChessGame.ChessPieces[24].type = CHESS_ROOK;
	ChessGame.ChessPieces[24].pos_x = 0;
	ChessGame.ChessPieces[25].type = CHESS_ROOK;
	ChessGame.ChessPieces[25].pos_x = 7;
	
	ChessGame.ChessPieces[26].type = CHESS_KNIGHT;
	ChessGame.ChessPieces[26].pos_x = 1;
	ChessGame.ChessPieces[27].type = CHESS_KNIGHT;
	ChessGame.ChessPieces[27].pos_x = 6;
	
	ChessGame.ChessPieces[28].type = CHESS_BISHOP;
	ChessGame.ChessPieces[28].pos_x = 2;
	ChessGame.ChessPieces[29].type = CHESS_BISHOP;
	ChessGame.ChessPieces[29].pos_x = 5;
	
	ChessGame.ChessPieces[30].type = CHESS_QUEEN;
	ChessGame.ChessPieces[30].pos_x = 3;
	ChessGame.ChessPieces[31].type = CHESS_KING;
	ChessGame.ChessPieces[31].pos_x = 4;
	
	
	
	for(int i = 0; i < 32; i++){
		ChessGame.ChessPieces[i].alive = 1;
		ChessGame.ChessPieces[i].moved = 0;
		if (i<16)
		ChessGame.ChessPieces[i].color = CHESS_WHITE;
		else
		ChessGame.ChessPieces[i].color = CHESS_BLACK;
		
		if(i<8){
			ChessGame.ChessPieces[i].pos_y = 6;
			ChessGame.ChessPieces[i].pos_x = i%8;
		}
		else if(i<16)
		ChessGame.ChessPieces[i].pos_y = 7;
		else if(i<24){
			ChessGame.ChessPieces[i].pos_y = 1;
			ChessGame.ChessPieces[i].pos_x = i%8;
		}
		else
		ChessGame.ChessPieces[i].pos_y = 0;
	}
	
}

void drawChessBoard(){
	display.fillScreen(GxEPD_WHITE);
	
	/*
	for(int i = 0; i < 8*CHESS_SIZE; i++){
	if(i%2 == 0){
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT, i + CHESSBOARD_OFFSET_TOP, 8*CHESS_SIZE, GxEPD_BLACK);
	}
	}
	
	*/
	
	for(int i = 0; i < 8*CHESS_SIZE; i++){
		for(int j = 0; j < 8*CHESS_SIZE; j++){
			if(i%2 ==0){
				if(j%2 == 0){
					if((i/CHESS_SIZE % 2) != 0){
						if((j/CHESS_SIZE % 2) == 0){
							display.drawPixel(CHESSBOARD_OFFSET_LEFT + i, j + CHESSBOARD_OFFSET_TOP, GxEPD_BLACK);
						}
					}
					if((i/CHESS_SIZE % 2) == 0){
						if((j/CHESS_SIZE % 2) != 0){
							display.drawPixel(CHESSBOARD_OFFSET_LEFT + i, j + CHESSBOARD_OFFSET_TOP, GxEPD_BLACK);
						}
					}
					
				}
			}
			
			if(i%2 ==1){
				if(j%2 == 1){
					if((i/CHESS_SIZE % 2) != 0){
						if((j/CHESS_SIZE % 2) == 0){
							display.drawPixel(CHESSBOARD_OFFSET_LEFT + i, j + CHESSBOARD_OFFSET_TOP, GxEPD_BLACK);
						}
					}
					if((i/CHESS_SIZE % 2) == 0){
						if((j/CHESS_SIZE % 2) != 0){
							display.drawPixel(CHESSBOARD_OFFSET_LEFT + i, j + CHESSBOARD_OFFSET_TOP, GxEPD_BLACK);
						}
					}
					
				}
			}
		}
	}
	/*
	for(int i = 0; i < 8; i++){
	for(int j = 0; j <8; j++){
	if(i%2 == 0){
	if(j%2 == 0){
	display.fillRect(j*CHESS_SIZE + CHESSBOARD_OFFSET_LEFT, i * CHESS_SIZE + CHESSBOARD_OFFSET_TOP, CHESS_SIZE, CHESS_SIZE, GxEPD_WHITE);
	}
	
	}
	if(i%2 == 1){
	if(j%2 == 1){
	display.fillRect(j*CHESS_SIZE + CHESSBOARD_OFFSET_LEFT, i * CHESS_SIZE + CHESSBOARD_OFFSET_TOP, CHESS_SIZE, CHESS_SIZE, GxEPD_WHITE);
	}
	
	}
	
	}
	
	}
	*/

	
	for(int i = 0; i < 32; i++){
		if(ChessGame.ChessPieces[i].alive == 1){
			if(ChessGame.ChessPieces[i].type == CHESS_PAWN)
			drawPawn(ChessGame.ChessPieces[i].pos_x, ChessGame.ChessPieces[i].pos_y, ChessGame.ChessPieces[i].color);
			if(ChessGame.ChessPieces[i].type == CHESS_ROOK)
			drawRook(ChessGame.ChessPieces[i].pos_x, ChessGame.ChessPieces[i].pos_y, ChessGame.ChessPieces[i].color);
			if(ChessGame.ChessPieces[i].type == CHESS_KNIGHT)
			drawKnight(ChessGame.ChessPieces[i].pos_x, ChessGame.ChessPieces[i].pos_y, ChessGame.ChessPieces[i].color);
			if(ChessGame.ChessPieces[i].type == CHESS_BISHOP)
			drawBishop(ChessGame.ChessPieces[i].pos_x, ChessGame.ChessPieces[i].pos_y, ChessGame.ChessPieces[i].color);
			if(ChessGame.ChessPieces[i].type == CHESS_QUEEN)
			drawQueen(ChessGame.ChessPieces[i].pos_x, ChessGame.ChessPieces[i].pos_y, ChessGame.ChessPieces[i].color);
			if(ChessGame.ChessPieces[i].type == CHESS_KING)
			drawKing(ChessGame.ChessPieces[i].pos_x, ChessGame.ChessPieces[i].pos_y, ChessGame.ChessPieces[i].color);
			
		}
		
	}
	//		display.fillRect(0, 0, GxEPD_WIDTH, 89, GxEPD_WHITE);
	
	
}

void drawPawn(int x, int y, int color){
	
	//display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8*CHESS_SIZE, GxEPD_WHITE);
	
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 4, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, 6, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, 8, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 8, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, 8, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, 8, GxEPD_WHITE);
	
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, 6, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 8, GxEPD_WHITE);
	
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 10, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, 10, GxEPD_WHITE);
	
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 12, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, 12, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 12, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 12, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, 12, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 12, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 21, 12, GxEPD_WHITE);
	
	if(color == 0){
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE +12 , y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 14, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 14, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 10,  GxEPD_BLACK);
	}
	else{
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, 2, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, 4, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8,6, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9,6, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,4, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11,2, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12,4, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13,6, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14,8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15,8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16,10, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17,10, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18,10, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19,10, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 10,  GxEPD_BLACK);
	}
}

void drawBishop(int x, int y, int color){

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 3, 3, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, 7, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 9, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, 11, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,15, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 17, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, 17, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 21, 15, GxEPD_WHITE);

	if(color == 0){
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 14, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);

		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 13,  GxEPD_BLACK);
		
		display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, 2, 9, GxEPD_BLACK);
		display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 6, 2, GxEPD_BLACK);
		
		
	}
	else{
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5,5, GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6,7, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, 9,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8,11, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, 11,GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, 13,GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11,13, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 15,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 15,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14,15, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15,15, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16,15, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17,15, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18,13, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19,13, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 13,  GxEPD_BLACK);
		
		display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, 2, 9, GxEPD_WHITE);
		display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 6, 2, GxEPD_WHITE);
	}
	
	
}

void drawQueen(int x, int y, int color){
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 3, 3, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, 3, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 5, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, 5, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, 5, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 21, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, 21, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,21, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 2, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, 19, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 2, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 19, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, 17, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 21, 13, GxEPD_WHITE);

	if(color == 0){
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 2, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 20, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);

		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);

		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 14, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 14, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 11,  GxEPD_BLACK);
		

		
		
	}
	else{
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6,3, GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7,3, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 3,GxEPD_BLACK);
		
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9,5, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 2, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 20, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,5, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,3, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,3, GxEPD_BLACK);
		

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, 5,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11,4, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11,4, GxEPD_BLACK);
		
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12,5, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 4,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12,4, GxEPD_BLACK);
		

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 15,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, 13,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 13,GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16,13, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 11,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 11,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, 11,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 11,  GxEPD_BLACK);
	}
	
	
}

void drawKing(int x, int y, int color){
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 2, 4, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 3, 4, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, 8, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 8, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, 8, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, 8, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 4, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9,7, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, 11, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, 13, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 15, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, 17, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 21, 13, GxEPD_WHITE);

	if(color == 0){
		
		display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 3, 2, 7, GxEPD_BLACK);
		display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 6, 2, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,5, GxEPD_BLACK);
		
		
		
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 14, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);

		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 11,  GxEPD_BLACK);
		

		
		
	}
	else{
		display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 3, 2, 7, GxEPD_BLACK);
		display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 6, 2, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,5, GxEPD_BLACK);
		
		
		
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, 9,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12,11, GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 13,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14,13, GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 15,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, 15,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 15,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 15,GxEPD_BLACK);

		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19,13, GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 11,  GxEPD_BLACK);
	}
}

void drawRook(int x, int y, int color){
	display.fillRect(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 2,y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, 19, 9, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, 11, GxEPD_WHITE);
	
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 11, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 21, 15, GxEPD_WHITE);
	
	if(color == 0){
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 4, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 5, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 4, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 4,GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 4,GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 19, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 11,GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 11,GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 13,  GxEPD_BLACK);
	}
	else{
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 4, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 5, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 4, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6,4, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, 5,GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6,4, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7,4, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7,5, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7,4, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8,17, GxEPD_BLACK);
		
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9,17, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, 17,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11,17, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 11,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 9,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, 9,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 9,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16,9, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 11,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 13,GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19,13, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 13,  GxEPD_BLACK);
	}
}

void drawKnight(int x, int y, int color){
	
	
	
	

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 3, 6, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, 6, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, 8, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, 9, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, 11, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 12, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9,14, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, 16, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, 16, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 16, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 3, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 16, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, 16, GxEPD_WHITE);

	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 15, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, 14, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 14, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 14, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 13, GxEPD_WHITE);
	display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 21, 12, GxEPD_WHITE);

	if(color == 0){
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 13, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 14, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 14, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 15, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, GxEPD_BLACK);

		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 18, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 16, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, GxEPD_BLACK);
		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 17, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 10,  GxEPD_BLACK);
		

		
		
	}
	else{
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 12, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 4, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 5,4, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 6, 6,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, 7,GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 7, GxEPD_WHITE);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, 9,GxEPD_BLACK);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_WHITE);
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 8, GxEPD_WHITE);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 9, 10,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 10,12, GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 11, 14,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 4, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 12, 14,GxEPD_BLACK);


		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 5, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13, 4,GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 11, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 13,7, GxEPD_BLACK);

		
		display.drawPixel(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 6, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 10, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 14,8, GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 9, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 15, 10,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 16,11, GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 17, 12,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 18, 11,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 7, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 19, 11,GxEPD_BLACK);

		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + 20, 10,  GxEPD_BLACK);
	}
}

void chooseChessField(int8_t field){
	
	if(field > -1 && field <64){
		int x = field%8;
		int y = field/8;;
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, CHESS_SIZE, GxEPD_BLACK);
			display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+1, CHESS_SIZE, GxEPD_BLACK);
			display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE, CHESS_SIZE, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-1, CHESS_SIZE, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, CHESS_SIZE, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+ CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, CHESS_SIZE, GxEPD_BLACK);
			display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, CHESS_SIZE, GxEPD_BLACK);
			display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+ CHESS_SIZE-1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, CHESS_SIZE, GxEPD_BLACK);
	}
}

void selectChessField(int8_t field){
	if(field > -1 && field <64){
		int x = field%8;
		int y = field/8;
		
		
		//		if (x%2+y%2 !=1)
		//		{
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+1, 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+2, 8, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-1, 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-2, 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-3, 8, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+ CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+1, 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+ CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+2, 8, GxEPD_BLACK);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-1, 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-2, 8, GxEPD_BLACK);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-3, 8, GxEPD_BLACK);
		
		
		
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+2, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_BLACK);
		
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+2, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_BLACK);
		
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-1 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-2 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-3 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_BLACK);
		
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-1 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-2 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_BLACK);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-3 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_BLACK);
		/*	}
		
		else{
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_WHITE);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+1, 8, GxEPD_WHITE);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+2, 8, GxEPD_WHITE);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-1, 8, GxEPD_WHITE);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-2, 8, GxEPD_WHITE);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-3, 8, GxEPD_WHITE);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_WHITE);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+ CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+1, 8, GxEPD_WHITE);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+ CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP+2, 8, GxEPD_WHITE);
		
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-1, 8, GxEPD_WHITE);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-2, 8, GxEPD_WHITE);
		display.drawFastHLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE + CHESS_SIZE - 8, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE-3, 8, GxEPD_WHITE);
		
		
		
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_WHITE);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_WHITE);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+2, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_WHITE);
		
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_WHITE);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+1, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_WHITE);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE+2, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_WHITE);
		
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-1 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_WHITE);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-2 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_WHITE);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-3 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP, 8, GxEPD_WHITE);
		
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-1 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_WHITE);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-2 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_WHITE);
		display.drawFastVLine(CHESSBOARD_OFFSET_LEFT + x*CHESS_SIZE-3 + CHESS_SIZE, y*CHESS_SIZE + CHESSBOARD_OFFSET_TOP + CHESS_SIZE - 8, 8, GxEPD_WHITE);
		}
		*/
		

	}
	
}
