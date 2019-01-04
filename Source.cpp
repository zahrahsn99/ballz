#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>           // SDL library
#include <cmath>                   // abs()
#include <ctime>                   // rand()
#include <linux/netfilter/xt_string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;


typedef struct {
	SDL_Rect Brick;
	int arrX, arrY;
} BrickObject;

vector<BrickObject> Bricks;

int board[9][6] = { 0 };


SDL_Window*     window;                 // holds window properties
SDL_Renderer*   renderer;               // holds rendering surface properties

SDL_Color dark_font = { 67, 68, 69 };     // dark grey
SDL_Color light_font = { 240, 249, 255 }; // light grey
SDL_Color whiteColor = { 255, 255, 255 };

bool resetBall = true;
  									    // Screen resolution
int SCREEN_WIDTH = 480;
int SCREEN_HEIGHT = 508;

const float BALL_INIT_SPEED = 4.0f;

const int BRICK_WIDTH = 74;
const int BRICK_HEIGHT = 40;

// Ball dimensions
const int BALL_WIDTH = 10;
const int BALL_HEIGHT = 10;

// Ball position
int x_ball = SCREEN_WIDTH / 2;
int y_ball = SCREEN_HEIGHT / 2;

int LEVEL = 0;
int PLEVEL = 0;

bool start = false;

int angle = 0;
bool gameover = false;

int basespeed = 4;


bool checkCollisionT(SDL_Rect ball)
{
	int topBall, bottomBall, posBall;
	int rightBrick, leftBrick, topBrick, bottomBrick;

	topBall = ball.y;
	bottomBall = ball.y + ball.h;
	posBall = ball.x;

	for (int i = 0; i < Bricks.size(); i++)
	{
		leftBrick = Bricks.at(i).Brick.x;
		rightBrick = Bricks.at(i).Brick.x + Bricks.at(i).Brick.w;
		topBrick = Bricks.at(i).Brick.y;
		bottomBrick = Bricks.at(i).Brick.y + Bricks.at(i).Brick.h;

		if (topBall <= bottomBrick && bottomBall > bottomBrick && posBall <= rightBrick && posBall >= leftBrick)
		{
			board[Bricks.at(i).arrX][Bricks.at(i).arrY]--;
			return true;
		}
	}

	//If none of the sides from A are outside B
	return false;
}

bool checkCollisionB(SDL_Rect ball)
{
	int topBall, bottomBall, posBall;
	int rightBrick, leftBrick, topBrick, bottomBrick;

	topBall = ball.y;
	bottomBall = ball.y + ball.h;
	posBall = ball.x;

	for (int i = 0; i < Bricks.size(); i++)
	{
		leftBrick = Bricks.at(i).Brick.x;
		rightBrick = Bricks.at(i).Brick.x + Bricks.at(i).Brick.w;
		topBrick = Bricks.at(i).Brick.y;
		bottomBrick = Bricks.at(i).Brick.y + Bricks.at(i).Brick.h;

		if (bottomBall >= topBrick && topBall < topBrick && posBall <= rightBrick && posBall >= leftBrick)
		{
			board[Bricks.at(i).arrX][Bricks.at(i).arrY]--;
			return true;
		}
	}

	//If none of the sides from A are outside B
	return false;
}

bool checkCollisionR(SDL_Rect ball)
{
	//The sides of the rectangles
	int leftBall, rightBall, posBall;
	int rightBrick, leftBrick, topBrick, bottomBrick;

	//Calculate the sides of rect A
	leftBall = ball.x;
	rightBall = ball.x + ball.w;
	posBall = ball.y;

	
	for (int i = 0; i < Bricks.size(); i++)
	{
		//Calculate the sides of Bricks
		leftBrick = Bricks.at(i).Brick.x;
		rightBrick = Bricks.at(i).Brick.x + Bricks.at(i).Brick.w;
		topBrick = Bricks.at(i).Brick.y;
		bottomBrick = Bricks.at(i).Brick.y + Bricks.at(i).Brick.h;

		if (rightBall >= leftBrick && leftBall < leftBrick && posBall > topBrick && posBall <bottomBrick)
		{
			board[Bricks.at(i).arrX][Bricks.at(i).arrY]--;
			return true;
		}
	}

	//If none of the sides from A are outside B
	return false;
}

bool checkCollisionL(SDL_Rect ball)
{
	//The sides of the rectangles
	int leftBall, rightBall, posBall;
	int rightBrick, leftBrick, topBrick, bottomBrick;

	//Calculate the sides of rect A
	leftBall = ball.x;
	rightBall = ball.x + ball.w;
	posBall = ball.y;


	for (int i = 0; i < Bricks.size(); i++)
	{
		//Calculate the sides of Bricks
		leftBrick = Bricks.at(i).Brick.x;
		rightBrick = Bricks.at(i).Brick.x + Bricks.at(i).Brick.w;
		topBrick = Bricks.at(i).Brick.y;
		bottomBrick = Bricks.at(i).Brick.y + Bricks.at(i).Brick.h;

		if (leftBall <= rightBrick && rightBall > rightBrick && posBall > topBrick && posBall <bottomBrick)
		{
			board[Bricks.at(i).arrX][Bricks.at(i).arrY]--;
			return true;
		}
	}

	//If none of the sides from A are outside B
	return false;
}

#pragma region Ltexture
//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
//Scene textures
LTexture gDotTexture;
#pragma endregion

#pragma region dot
class Dot
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 10;

	//Initializes the variables
	Dot();

	//Moves the dot and checks collision
	void move();

	//Shows the dot on the screen
	void render();

	void reset();

	void setspeed(int xVel , int yVel);
	void setspeed(float angle);

private:
	//The X and Y offsets of the dot
	double mPosX, mPosY;

	//The velocity of the dot
	double mVelX, mVelY;

	//Dot's collision box
	SDL_Rect mCollider;
};


Dot::Dot()
{
	//Initialize the offsets
	mPosX = (SCREEN_WIDTH - 20) / 2;
	mPosY = SCREEN_HEIGHT - 20;

	//Set collision box dimension
	mCollider.w = DOT_WIDTH+6;
	mCollider.h = DOT_HEIGHT+6;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

void Dot::reset()
{
	//Initialize the offsets
	mPosX = (SCREEN_WIDTH - 20) / 2;
	mPosY = SCREEN_HEIGHT - 20;

	//Initialize the velocity
	setspeed(angle);
}

void Dot::setspeed(int xVel,int yVel)
{
	mVelX = xVel;
	mVelY = yVel;
}

void Dot::setspeed(float angle)
{
	mVelX = sin(angle/20) * basespeed;
	mVelY = -1* cos(angle/20) * basespeed;
}

void Dot::move()
{
	//Move the dot left or right
	mPosX += mVelX;
	mCollider.x = mPosX;

	if (mPosY + DOT_HEIGHT > SCREEN_HEIGHT)
	{
		resetBall = true;
		start = false;
	}


	//If the dot collided or went too far to the left or right
	if ((mPosX < 0) || checkCollisionL(mCollider))
	{
		//Move back
		mVelX = abs(mVelX);
		mCollider.x += mVelX + 1;
		
	}

	//If the dot collided or went too far to the left or right
	if ((mPosX + DOT_WIDTH > SCREEN_WIDTH) || checkCollisionR(mCollider))
	{
		//Move back
		mVelX = -1 * abs(mVelX);
		mCollider.x += mVelX - 1;
	}

	//Move the dot up or down
	mPosY += mVelY;
	mCollider.y = mPosY;

	//If the dot collided or went too far up or down
	if ((mPosY < 0)  || checkCollisionT(mCollider))
	{
		//Move back
		mVelY = abs(mVelY);
		mCollider.y += mVelY + 1;
	}

	//If the dot collided or went too far up or down
	if (checkCollisionB(mCollider))
	{
		//Move back
		mVelY = -1 * abs(mVelY);
		mCollider.y += mVelY - 1;
	}
	
}

void Dot::render()
{
	//Show the dot
	gDotTexture.render(mPosX, mPosY);
}

#pragma endregion

SDL_Rect brick1;

// Launch ball
bool launch_ball = false;


// Ball movement
int ball_dx = 0;        // movement in pixels over the x-axis for the next frame (speed on the x-axis)
int ball_dy = 0;        // movement in pixels over the y-axis for the next frame (speed on the y-axis)

int speed = 8;          // ball speed = √(dx²+dy²)
int hit_count = 0;      // counts the number of hits of the ball with the right paddle
						// after three hits, speed increases by one 


bool bounce = false;    // true when next frame renders ball after collision impact (ball has bounced)

						// Match score
int score1 = 0;
int score2 = 0;

Dot dot;
void update()
{
	if (start)
	{
		dot.move();
	}
}

SDL_Texture *renderText(const string &msg, const string &fontPath, SDL_Color color, int fontSize, SDL_Renderer *ren) {
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
	string t = TTF_GetError();
	if (font == NULL) {
		return NULL;
	}
	
	SDL_Surface *surface = TTF_RenderText_Blended(font, msg.c_str(), color);
	if (surface == NULL) {
		TTF_CloseFont(font);
		return NULL;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface);


	SDL_FreeSurface(surface);
	TTF_CloseFont(font);

	return tex;
}

void renderTexture(SDL_Texture *tex , SDL_Renderer *ren, int x, int y, int w = -1, int h = -1) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;

	// If no width and height are specified, use the texture's actual width and height
	if (w == -1 || h == -1)
		SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(ren, tex, NULL, &dest);
}


void draw_angle(SDL_Renderer *ren)
{
	for (int i = 0; i < 6; i++)
	{
		SDL_Rect arrowhead = { (SCREEN_WIDTH / 2) + (sin(angle / 20.0) * i*20) , SCREEN_HEIGHT - (cos(angle / 20.0)*i*20) - 10,3,3 };
		SDL_RenderFillRect(renderer, &arrowhead);
	}
}

char num[256];
void draw_bricks(SDL_Renderer *ren)
{
	Bricks.clear();
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (board[i][j] > 0)
			{
				int brx = j *(BRICK_WIDTH + 4) + 8, bry = i *(BRICK_HEIGHT + 4) + 8;
				BrickObject brick = { brx,bry,BRICK_WIDTH,BRICK_HEIGHT };
				brick.arrX = i;
				brick.arrY = j;
				Bricks.push_back(brick);
				SDL_RenderFillRect(renderer, &brick.Brick);
				sprintf(num,"%d",board[i][j]);
				//string thisCell = num;
				SDL_Texture *p1score = renderText(num, "fonts/sample.ttf", whiteColor, 15, renderer);
				renderTexture(p1score, renderer, brx + BRICK_WIDTH / 2 - 7, bry + BRICK_HEIGHT / 2 - 7);
				SDL_DestroyTexture(p1score);
			}
			
		}
	}
}

void resetboard()
{
	for (int i = 0; i <9; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			board[i][j] = 0;
		}
	}
}
void update_board()
{	
	for (int i = 0; i < 6; i++){
		if (board[8][i])
			gameover = true;
	}
	for (int i = 8; i > 0; i--)
	{
		for (int j = 0; j < 6; j++)
		{
			board[i][j] = board[i - 1][j];
		}
	}
	for (int i = 0; i < 6; i++) {
		board[0][i] = 0;
	}
	int rand1 = rand() % 6;
	board[1][rand1] = LEVEL;
	int rand2;
	do
	{
		rand2 = rand() % 6;
		board[1][rand2] = LEVEL;
	} while (rand1 == rand2);
}

unsigned int timer = 0;
// Render objects on screen
void render() {
	timer++;

	// Clear screen (background color)
	SDL_SetRenderDrawColor(renderer, 227, 231, 240, 255);        // dark grey
	SDL_RenderClear(renderer);

	// Paddle color
	SDL_SetRenderDrawColor(renderer, 212, 120, 102, 255);

	if (gameover)
	{
		SDL_Texture *p1score =  renderText("Game Over", "fonts/sample.ttf", dark_font, 45, renderer);
		renderTexture(p1score, renderer, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 30);
		SDL_DestroyTexture(p1score);
	}
	else {

		if (LEVEL != PLEVEL)
		{
			update_board();
		}
		PLEVEL = LEVEL;
		
		if (!start)
		{
			draw_angle(renderer);
			dot.reset();
		}
		if (resetBall)
		{
			LEVEL++;
			SDL_Texture *p1score = renderText("LEVEL" , "fonts/sample.ttf", dark_font, 100, renderer);
	     	renderTexture(p1score, renderer, SCREEN_WIDTH , SCREEN_HEIGHT );
		    SDL_DestroyTexture(p1score);
			
			resetBall = false;
		}

		draw_bricks(renderer);
		dot.render();
	}

	// Swap buffers
	SDL_RenderPresent(renderer);
	
}

void cleanUp() {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// Shuts down SDL
	SDL_Quit();
}
bool quit = false;
void gameLoop() {
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(& e)) {
			if (e.type == SDL_QUIT)  quit = true;
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					if (gameover)
					{
						angle = 0;
						LEVEL = 1;
						PLEVEL = 0;
						gameover = false;
						Bricks.clear();
						resetboard();
					}
					else {
						start = true;
					}
					break;
				case SDL_SCANCODE_RIGHT:
					if( angle < 24)
						angle++;
					break;
				case SDL_SCANCODE_LEFT:
					if (angle > -24)
						angle--;
					break;
				}
			}
		}
		update();
		render();
	}

	cleanUp();
}
bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load press texture
	if (!gDotTexture.loadFromFile("dot.bmp"))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	return success;
}

void initialize() {

	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create window in the middle of the screen
	window = SDL_CreateWindow("Bricks",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	TTF_Init();

	loadMedia();
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	initialize();
	gameLoop();
	return 0;
}