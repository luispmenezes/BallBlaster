#ifndef _gameLogic_h
#define _gameLogic_h

void initGameLogic( void );
void resetGameLogic(void);
void gameLoop( void );
void checkMatches(int i);
int* getContig(int i);
void updateCoord(int idx, float x,float y, float z);
void onClick(int x, int y);
int isValid(int a, int b);
void setModelColor(int idx, Color c);
Color getRandomColor( void );
void addAnimIN(int idx);
void addAnimOUT(int idx);
void animate( void );
int getScore( void );
float getTime2Move( void );

#endif