#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <vector>
#include <map>
#include <math.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <iostream>

#define PI            3.145
#define DISTANCE      75
#define V             2
#define PATH_RADIUS   0.1
#define MAX_BALLS     20
#define MAX_TIME      30

using namespace std;
// --------MainVariables-------
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Mix_Chunk* click = NULL;
Mix_Music* music1 = NULL;
Mix_Chunk* shot = NULL;
Mix_Chunk* gameOver = NULL;
bool running = true;
bool sound = true;
bool music = true;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
string Username = "";
int Map = -1;
int Mode = -1;
int Score = 0;
bool endGame = false;
int colorCounter=0;
// --------MenusVariables-------
bool login_signup_menu = true;
bool login_menu = false;
bool signup_menu = false;
bool setting_menu = false;
bool pause_menu = false;
bool leader_board = false;
bool maps_menu = false;
bool mode_menu = false;
bool start_menu = false;
bool end_menu = false;
bool help = false;
bool game = false;
// --------TextureManager-------
map <string,SDL_Texture*> Textures;
bool addTexture(string fileName,string name);
void destroyTexture(string name);
void setBackground(string backgroundName);
void showTexture(string textureName,float x,float y,float angle);
void showTextureInSize(string textureName,float x,float y,float angle,int w,int h);
// --------TextManager-------
map <string,SDL_Texture*> Texts;
bool addTxt(string txt,string txtName,string fontFile,SDL_Color fontColor,int fontSize);
void destroyTxt(string txtName);
void showText(string name,double xPos,double yPos);
void setButton(string name,double xPos,double yPos,int backR1,int backR2,int r,int g,int b,int a);
void setInput(string name,double xPos,double yPos,int wInput,int hInput,int r,int g,int b,int a);
// --------FileManager-------
bool isPassRight(string username,string password);
void addUsername(string username,string password);
void createRank();
void showRank();
bool updateScore(string username,int newScore);
vector <string> users;
vector <long long int> scores;
// --------LoginSignupVariables-------
int loginColor[4] = {0,50,0,230};
int signupColor[4] = {0,50,0,200};
int selectedLogin_signupColor[4] = {0,100,0,250};
int defaultLogin_signupColor[4] = {0,50,0,200};
int selectLogin_signup = 0;
bool login_signupIsSelected = false;
// --------LoginVariables-------
string loginUsernameInput = " ";
string loginPassInput = " ";
int loginInput = 0;
// --------SignupVariables-------
string signupUsernameInput = " ";
string signupPassInput = " ";
int signupInput = 0;
// --------StartVariables-------
int startColor[3] = {25,25,50};
int settingColor[3] = {25,25,50};
int leaderColor[3] = {25,25,50};
int defaultStartColor[3] = {25,25,50};
int selectedStartColor[3] = {255,255,255};
int selectStart = 1000;
// --------SettingVariables-------
int selectSetting = 1001;
int selectSettingColor[4] = {25,25,50,255};
int defaultSettingColor[4] = {85,85,85,255};
int helpColor[4] = {85,85,85,255};
int soundColor[4] = {85,85,85,255};
int musicColor[4] = {85,85,85,255};
int exitColor[4] = {85,85,85,255};
// --------MapsVariables-------
int mapSelector = 1;
int map1Border[3] = {25,25,50};
int map2Border[3] = {25,25,50};
int map3Border[3] = {25,25,50};
int mapBorderDefault[3] = {25,25,50};
int mapBorderSelected[3] = {0,0,0};
// --------ModesVariables-------
int modeSelector = 1;
int mode1Color[3] = {25,25,50};
int mode2Color[3] = {25,25,50};
int modeSelected[3] = {255,255,255};
int modeDefault[3] = {25,25,50};
// --------EndVariables-------
int endExitColor[3] = {25,25,50};
int endRestartColor[3] = {25,25,50};
int endDefault[3] = {25,25,50};
int endSelected[3] = {255,255,255};
int endSelector = 1;
// --------ResumeVariables-------
int resumeColor[3] = {85,85,85};
int pauseDefault[3] = {85,85,85};
int pauseSelected[3] = {25,25,50};
int pauseSelector = 1001;
// --------Game-------
// --------Game::Cannon
double cannonX = SCREEN_WIDTH/2,cannonY = SCREEN_HEIGHT/2;
double cannonTeta = 0;
double mouseX=0,mouseY=0;
// --------Game::Shots
int shotCounter = 0;
string colors[4] = {"red","green","yellow","blue"};
struct Shot
{
    double shotX,shotY,shotTeta,shootedTeta;
    bool isShooted;
    string shotColor,shotName;
    int shotId;
    Shot(double teta) : shotTeta(teta) , shotX(cannonX+cos(teta)*DISTANCE) , shotY(cannonY-sin(teta)*DISTANCE) , shotId(shotCounter),isShooted(false) {}

    void createShot()
    {
        int c = (rand()+2)%4;
        shotColor = colors[c];
        shotName = shotColor+"-shot: "+to_string(shotId);
        addTexture("assets/images/balls/"+shotColor+"-ball.png",shotName);
        showTexture(shotName,shotX,shotY,shotTeta);
    }

    void renderShot()
    {
        if(isShooted) showTexture(shotName,shotX,shotY,90-shootedTeta*180/PI);
        else showTexture(shotName,shotX,shotY,cannonTeta);
    }

    void updateShot()
    {
        if(isShooted)
        {
            shotX += V*cos(shootedTeta);
            shotY -= V*sin(shootedTeta);
            if(!(0<=shotX<=SCREEN_WIDTH || 0<=shotY<=SCREEN_HEIGHT)) destroyShot();
        }
        else
        {
            shotTeta = PI/2 - cannonTeta*PI/180;
            shootedTeta = shotTeta;
            shotX = cannonX + cos(shotTeta) * DISTANCE;
            shotY = cannonY - sin(shotTeta) * DISTANCE;
        }
    }

    void destroyShot()
    {
        destroyTexture(shotName);
    }
};

struct Shots
{
    vector <Shot> shots;

    void addShot()
    {
        Shot s(cannonTeta);
        s.createShot();
        shots.push_back(s);
        shotCounter++;
    }

    void renderShots()
    {
        for(int i=0;i<shots.size();i++) shots.at(i).renderShot();
    }

    void updateShots()
    {
        for(int i=0;i<shots.size();i++) shots.at(i).updateShot();
    }
} shots;
// --------Game::Paths
struct Point
{
    double x,y;
    void set_to(double set_to_x,double set_to_y) {x=set_to_x;y=set_to_y;}
};

struct Path
{
    Point* points;
    int sizePath;
    double Xmin,Xmax,Ymin,Ymax;
    double startX,startY,teta,dTeta,d;
    Path(int s,double startX,double startY,double teta,double dTeta) : sizePath(s) , Xmin(0) , Xmax(0) , Ymin(0) , Ymax(0) , d(30000) ,
            startX(startX) , startY(startY) , teta(teta) , dTeta(dTeta) {};

    void createPath()
    {
        points = new Point[sizePath];
        points[0].set_to(startX,startY);

        double x,y;
        for(int i=1;i<sizePath;i++)
        {
            x = points[i-1].x + PATH_RADIUS*cos(teta);
            y = points[i-1].y + PATH_RADIUS*sin(teta);

            points[i].set_to(x,y);
            checkPoints(points[i]);
            teta+=dTeta;
            dTeta+=PI/d;
        }

        fitPath();
    }

    void checkPoints(Point p)
    {
        if(p.x>Xmax) Xmax = p.x;
        if(p.x<Xmin) Xmin = p.x;
        if(p.y>Ymax) Ymax = p.y;
        if(p.y<Ymin) Ymin = p.y;
    }

    void fitPath()
    {
        for(int i=0;i<sizePath;i++)
        {
            points[i].x = (points[i].x - Xmin) * (SCREEN_WIDTH-100)/(Xmax-Xmin)+50;
            points[i].y = (points[i].y - Ymin) * (SCREEN_HEIGHT-100)/(Ymax-Ymin)+50;
        }
    }

};

Path path1(250,1,1,91,0.00031513);
Path path2(300,1,1,142,0.000280078);
Path path3(300,1,1,10,PI/180);

Path paths[3] = {path1,path2,path3};
double firstPointsX[3] = {1238.4,1030.27,457.883};
double firstPointsY[3] = {732.083,752.572,754.694};
// --------Game::balls
int c=-1;
int ballCounter=0;
struct Ball
{
    double ballX,ballY,ballTeta;
    int index,id,counter;
    Path path;
    string ballColor,ballName;
    bool isGoing;

    Ball(Path p) : index(0) , path(p) , ballTeta(0) , id(ballCounter) , ballX(p.points[0].x) , ballY(p.points[0].y) , isGoing(true) {}

    void createBall()
    {
        if(colorCounter==0)
        {
            colorCounter = rand() % 3+1;
            int x = rand()%4;
            while(c==x) x = rand() % 4;
            c=x;
        }
        if(colorCounter>0)
        {
            ballColor = colors[c];
            colorCounter--;
        }
        ballName = ballColor + "-ball: "+to_string(id);
        addTexture("assets/images/balls/"+ballColor+"-ball.png",ballName);
    }

    void renderBall()
    {
        showTexture(ballName,ballX,ballY,ballTeta);
    }

    void updateBall()
    {
        if(isGoing)
        {
        if(index==path.sizePath-1) {destroyBall();endGame = true;Mix_PlayChannel(-1,gameOver,0);SDL_Delay(1000);}
        else if(counter>=30) {index++;counter=0;}
        counter++;
        ballTeta-=0.5;
        ballX = path.points[index].x;
        ballY = path.points[index].y;
        }
    }

    void destroyBall()
    {
        destroyTexture(ballName);
    }
};

struct Balls
{
    vector <Ball> balls;

    void addBall(Path path)
    {
        Ball b(path);
        b.createBall();
        balls.push_back(b);
        ballCounter++;
    }

    void renderBalls()
    {
        for(int i=0;i<balls.size();i++) balls.at(i).renderBall();
    }

    void updateBalls()
    {
        for(int i=1;i<balls.size();i++)
            balls.at(i).isGoing = balls.at(i-1).isGoing;
         for(int i=0;i<balls.size();i++) balls.at(i).updateBall();
    }


    void clearBalls()
    {
        for(int i=0;i<balls.size();i++) balls.at(i).destroyBall();
    }
} balls;

int strartGameSecond;

// --------MainFunctions-------
bool init();
void render();
void handlEvents();
void update();
void close();

void musicManager() ;
// ----------------------------------------------------------------------------------- MainFunction
int main(int argc,char** argv)
{
    if(!init()) return -1;

    while(running)
    {
        render();
        handlEvents();
        update();
        SDL_RenderPresent(renderer);
    }

    close();

    return 0;
}
// -----------------------------------------------------------------------------------
bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)<0) return false;

    window = SDL_CreateWindow("Jungle Marble Blast",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if(window==NULL) return false;
    renderer = SDL_CreateRenderer(window,-1,0);
    if(renderer==NULL) return false;

    int IMG_FLAGS = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(IMG_FLAGS)&IMG_FLAGS)) return false;

    if(!addTexture("assets/images/login_signup_background.jpg","login_signup_background")) return false;
    if(!addTexture("assets/images/start_menu_background.jpg","start_menu_background")) return false;
    if(!addTexture("assets/images/grass.jpg","grass")) return false;
    if(!addTexture("assets/images/maps/map1.png","map1")) return false;
    if(!addTexture("assets/images/maps/map2.png","map2")) return false;
    if(!addTexture("assets/images/maps/map3.png","map3")) return false;
    if(!addTexture("assets/images/cannon.png","cannon")) return false;

    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0) return false;
    click = Mix_LoadWAV("assets/audios/click.wav");
    music1 = Mix_LoadMUS("assets/audios/music1.mp3");
    shot = Mix_LoadWAV("assets/audios/shot.wav");
    gameOver = Mix_LoadWAV("assets/audios/gameOver.wav");
    if(click==NULL || music1==NULL || shot==NULL || gameOver==NULL) return false;
    Mix_PlayMusic(music1,-1);

    if(TTF_Init()!=0) return false;

    SDL_Color login_signupColor = {0,0,0};
    if(!addTxt("Login","login","assets/fonts/FORTE.ttf",login_signupColor,85)) return false;
    if(!addTxt("Register","signup","assets/fonts/FORTE.ttf",login_signupColor,85)) return false;
    if(!addTxt("Username","username","assets/fonts/FORTE.ttf",{0,0,0},100)) return false;
    if(!addTxt("Password","password","assets/fonts/FORTE.ttf",{0,0,0},100)) return false;
    if(!addTxt("Help","help","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;
    if(!addTxt("Sound : ON","sound-off","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;
    if(!addTxt("Sound : OFF","sound-on","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;
    if(!addTxt("Music : ON","music-off","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;
    if(!addTxt("Music : OFF","music-on","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;
    if(!addTxt("Exit","exit","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;
    if(!addTxt("Start","start","assets/fonts/FORTE.ttf",{255,255,255},50)) return false;
    if(!addTxt("Setting","setting","assets/fonts/FORTE.ttf",{255,255,255},50)) return false;
    if(!addTxt("Leader Board","leaderboard","assets/fonts/FORTE.ttf",{255,255,255},50)) return false;
    if(!addTxt("Start","startSelected","assets/fonts/FORTE.ttf",{25,25,50},50)) return false;
    if(!addTxt("Setting","settingSelected","assets/fonts/FORTE.ttf",{25,25,50},50)) return false;
    if(!addTxt("Leader Board","leaderboardSelected","assets/fonts/FORTE.ttf",{25,25,50},50)) return false;
    if(!addTxt("JUNGLE MARBLE","logo-1","assets/fonts/kopers.ttf",{50,0,100},85)) return false;
    if(!addTxt("BLAST","logo-2","assets/fonts/kopers.ttf",{50,0,100},85)) return false;
    if(!addTxt("Clear Balls","modeClear","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;
    if(!addTxt("Time","modeTime","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;
    if(!addTxt("Clear Balls","modeClearSelected","assets/fonts/FORTE.ttf",{25,25,50},85)) return false;
    if(!addTxt("Time","modeTimeSelected","assets/fonts/FORTE.ttf",{25,25,50},85)) return false;
    if(!addTxt("Your Score: ","yourScore","assets/fonts/FORTE.ttf",{255,255,255},50)) return false;
    if(!addTxt("Your Username: ","yourUsername","assets/fonts/FORTE.ttf",{255,255,255},50)) return false;
    if(!addTxt("Exit","endExit","assets/fonts/FORTE.ttf",{25,25,50},50)) return false;
    if(!addTxt("Exit","endExitSelected","assets/fonts/FORTE.ttf",{255,255,255},50)) return false;
    if(!addTxt("Restart","restart","assets/fonts/FORTE.ttf",{255,255,255},50)) return false;
    if(!addTxt("Restart","restartSelected","assets/fonts/FORTE.ttf",{25,25,50},50)) return false;
    if(!addTxt("Resume","resume","assets/fonts/FORTE.ttf",{255,255,255},85)) return false;

    for(int i=0;i<3;i++) paths[i].createPath();

    return true;
}

void render()
{
    musicManager();
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,185,185,185,255);
    // ------------------------------------------------------------------------------------------------ LoginSignup Menu
    if(login_signup_menu)
    {
        setBackground("login_signup_background");
        setButton("login",SCREEN_WIDTH/2,SCREEN_HEIGHT/3,200,100,loginColor[0],loginColor[1],loginColor[2],loginColor[3]);
        setButton("signup",SCREEN_WIDTH/2,2*SCREEN_HEIGHT/3,200,100,signupColor[0],signupColor[1],signupColor[2],signupColor[3]);
    }
    // ------------------------------------------------------------------------------------------------ Login Menu
    else if(login_menu)
    {
        SDL_Color inputColor = {0,0,0};
        setBackground("login_signup_background");
        showText("username",SCREEN_WIDTH/2,SCREEN_HEIGHT/4+50);
        if(loginUsernameInput.size()==0) loginUsernameInput = " ";
            addTxt(loginUsernameInput,"loginUsernameInput"+loginUsernameInput,"assets/fonts/Konnect-Bold.otf",inputColor,50);
            setInput("loginUsernameInput"+loginUsernameInput,SCREEN_WIDTH/2,3*SCREEN_HEIGHT/8+50,4*SCREEN_WIDTH/5,100,85,85,85,255);
            destroyTxt("loginUsernameInput"+loginUsernameInput);
        showText("password",SCREEN_WIDTH/2,5*SCREEN_HEIGHT/8);
        if(loginPassInput.size()==0) loginPassInput = " ";
            addTxt(loginPassInput,"loginPassInput"+loginPassInput,"assets/fonts/Konnect-Bold.otf",inputColor,50);
            setInput("loginPassInput"+loginPassInput,SCREEN_WIDTH/2,3*SCREEN_HEIGHT/4,4*SCREEN_WIDTH/5,100,85,85,85,255);
            destroyTxt("loginPassInput"+loginPassInput);
    }
    // ------------------------------------------------------------------------------------------------ Signup Menu
    else if(signup_menu)
    {
        SDL_Color inputColor = {0,0,0};
        setBackground("login_signup_background");
        showText("username",SCREEN_WIDTH/2,SCREEN_HEIGHT/4+50);
        if(signupUsernameInput.size()==0) signupUsernameInput = " ";
            addTxt(signupUsernameInput,"signupUsernameInput"+signupUsernameInput,"assets/fonts/Konnect-Bold.otf",inputColor,50);
            setInput("signupUsernameInput"+signupUsernameInput,SCREEN_WIDTH/2,3*SCREEN_HEIGHT/8+50,4*SCREEN_WIDTH/5,100,85,85,85,255);
            destroyTxt("signupUsernameInput"+signupUsernameInput);
        showText("password",SCREEN_WIDTH/2,5*SCREEN_HEIGHT/8);
        if(signupPassInput.size()==0) signupPassInput = " ";
        addTxt(signupPassInput,"signupPassInput"+signupPassInput,"assets/fonts/Konnect-Bold.otf",inputColor,50);
            setInput("signupPassInput"+signupPassInput,SCREEN_WIDTH/2,3*SCREEN_HEIGHT/4,4*SCREEN_WIDTH/5,100,85,85,85,255);
            destroyTxt("signupPassInput"+signupPassInput);
    }
    // ------------------------------------------------------------------------------------------------ Setting Menu
    else if(setting_menu)
    {
        setBackground("start_menu_background");
        setInput("help",SCREEN_WIDTH/2,SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,helpColor[0],helpColor[1],helpColor[2],helpColor[3]);
        if(music) setInput("music-on",SCREEN_WIDTH/2,2*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,musicColor[0],musicColor[1],musicColor[2],musicColor[3]);
        else setInput("music-off",SCREEN_WIDTH/2,2*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,musicColor[0],musicColor[1],musicColor[2],musicColor[3]);
        if(sound) setInput("sound-on",SCREEN_WIDTH/2,3*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,soundColor[0],soundColor[1],soundColor[2],soundColor[3]);
        else setInput("sound-off",SCREEN_WIDTH/2,3*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,soundColor[0],soundColor[1],soundColor[2],soundColor[3]);
        setInput("exit",SCREEN_WIDTH/2,SCREEN_HEIGHT-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,exitColor[0],exitColor[1],exitColor[2],exitColor[3]);
    }
    // ------------------------------------------------------------------------------------------------ Start Menu
    else if(start_menu)
    {
        setBackground("start_menu_background");
        showText("logo-1",SCREEN_WIDTH/2,SCREEN_WIDTH/8-20);
        showText("logo-2",SCREEN_WIDTH/2,SCREEN_WIDTH/8+80);
        if(selectStart%3==1) setButton("startSelected",SCREEN_WIDTH/2,4*SCREEN_HEIGHT/8,150,70,startColor[0],startColor[1],startColor[2],255);
        else setButton("start",SCREEN_WIDTH/2,4*SCREEN_HEIGHT/8,150,70,startColor[0],startColor[1],startColor[2],255);
        if(selectStart%3==2) setButton("settingSelected",SCREEN_WIDTH/2,5*SCREEN_HEIGHT/8+50,150,70,settingColor[0],settingColor[1],settingColor[2],255);
        else setButton("setting",SCREEN_WIDTH/2,5*SCREEN_HEIGHT/8+50,150,70,settingColor[0],settingColor[1],settingColor[2],255);
        if(selectStart%3==0) setButton("leaderboardSelected",SCREEN_WIDTH/2,6*SCREEN_HEIGHT/8+100,150,70,leaderColor[0],leaderColor[1],leaderColor[2],255);
        else setButton("leaderboard",SCREEN_WIDTH/2,6*SCREEN_HEIGHT/8+100,150,70,leaderColor[0],leaderColor[1],leaderColor[2],255);
    }
    // ------------------------------------------------------------------------------------------------ Leader Menu
    else if(leader_board)
    {
        setBackground("start_menu_background");
        showRank();
    }
    // ------------------------------------------------------------------------------------------------ Help
    else if(help)
    {
        setBackground("start_menu_background");

    }
    // ------------------------------------------------------------------------------------------------ Maps Menu
    else if(maps_menu)
    {
        setBackground("start_menu_background");

        SDL_Rect rect1 = {110-20,100-20,500+40,300+40};
        SDL_SetRenderDrawColor(renderer,map1Border[0],map1Border[1],map1Border[2],255);
        SDL_RenderFillRect(renderer,&rect1);
        showTextureInSize("map1",110,100,0,500,300);

        SDL_Rect rect2 = {110+500+60-20,100-20,500+40,300+40};
        SDL_SetRenderDrawColor(renderer,map2Border[0],map2Border[1],map2Border[2],255);
        SDL_RenderFillRect(renderer,&rect2);
        showTextureInSize("map2",110+500+60,100,0,500,300);

        SDL_Rect rect3 = {110+280-20,100+300+50-20,500+40,300+40};
        SDL_SetRenderDrawColor(renderer,map3Border[0],map3Border[1],map3Border[2],255);
        SDL_RenderFillRect(renderer,&rect3);
        showTextureInSize("map3",110+280,100+300+50,0,500,300);
    }
    // ------------------------------------------------------------------------------------------------ Mode Menu
    else if(mode_menu)
    {
        setBackground("start_menu_background");
        if(modeSelector==1) setButton("modeClearSelected",SCREEN_WIDTH/4,SCREEN_HEIGHT/3,250,200,mode1Color[0],mode1Color[1],mode1Color[2],255);
        else setButton("modeClear",SCREEN_WIDTH/4,SCREEN_HEIGHT/3,250,200,mode1Color[0],mode1Color[1],mode1Color[2],255);
        if(modeSelector==2) setButton("modeTimeSelected",3*SCREEN_WIDTH/4,SCREEN_HEIGHT/3,250,200,mode2Color[0],mode2Color[1],mode2Color[2],255);
        else setButton("modeTime",3*SCREEN_WIDTH/4,SCREEN_HEIGHT/3,250,200,mode2Color[0],mode2Color[1],mode2Color[2],255);
    }
    // ------------------------------------------------------------------------------------------------ End Menu
    else if(end_menu)
    {
        setBackground("start_menu_background");
        addTxt(Username,Username,"assets/fonts/Konnect-Bold.otf",{100,120,200},100);
        addTxt(to_string(Score),"Score","assets/fonts/Konnect-Bold.otf",{100,120,200},100);
        showText("yourUsername",SCREEN_WIDTH/4,SCREEN_HEIGHT/5);
        showText(Username,3*SCREEN_WIDTH/4,SCREEN_HEIGHT/5);
        showText("yourScore",SCREEN_WIDTH/4,2*SCREEN_HEIGHT/5);
        showText("Score",3*SCREEN_WIDTH/4,2*SCREEN_HEIGHT/5);
        destroyTxt(Username);
        destroyTxt("Score");

        if(endSelector==1) setButton("endExitSelected",3*SCREEN_WIDTH/4,4*SCREEN_HEIGHT/5,100,50,endExitColor[0],endExitColor[1],endExitColor[2],255);
        else setButton("endExit",3*SCREEN_WIDTH/4,4*SCREEN_HEIGHT/5,100,50,endExitColor[0],endExitColor[1],endExitColor[2],255);
        if(endSelector==2) setButton("restart",SCREEN_WIDTH/4,4*SCREEN_HEIGHT/5,100,50,endRestartColor[0],endRestartColor[1],endRestartColor[2],255);
        else setButton("restartSelected",SCREEN_WIDTH/4,4*SCREEN_HEIGHT/5,100,50,endRestartColor[0],endRestartColor[1],endRestartColor[2],255);
    }
    // ------------------------------------------------------------------------------------------------ Pause Menu
    else if(pause_menu)
    {
        setBackground("login_signup_background");
        setInput("resume",SCREEN_WIDTH/2,SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,resumeColor[0],resumeColor[1],resumeColor[2],255);
        if(sound) setInput("sound-off",SCREEN_WIDTH/2,2*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,soundColor[0],soundColor[1],soundColor[2],255);
        else setInput("sound-on",SCREEN_WIDTH/2,2*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,soundColor[0],soundColor[1],soundColor[2],255);
        if(music) setInput("music-off",SCREEN_WIDTH/2,3*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,musicColor[0],musicColor[1],musicColor[2],255);
        else setInput("music-on",SCREEN_WIDTH/2,3*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,musicColor[0],musicColor[1],musicColor[2],255);
        setInput("exit",SCREEN_WIDTH/2,4*SCREEN_HEIGHT/4-100,SCREEN_WIDTH/2,SCREEN_HEIGHT/6,exitColor[0],exitColor[1],exitColor[2],255);
    }
    // ------------------------------------------------------------------------------------------------ Game
    else if(game)
    {
        setBackground("grass");
        showTexture("cannon",cannonX,cannonY,cannonTeta);

       shots.renderShots();
       double x=firstPointsX[Map-1],y=firstPointsY[Map-1];
       if(Mode==1)
       {
            if(ballCounter<80)
            {
                if(ballCounter==0)
                {
                    balls.addBall(paths[Map-1]);
                }
                else
                {
                    if((balls.balls.back().ballX-x)*(balls.balls.back().ballX-x)+(balls.balls.back().ballY-y)*(balls.balls.back().ballY-y)>55*55)
                        balls.addBall(paths[Map-1]);
                        x = balls.balls.back().ballX;
                        y = balls.balls.back().ballY;
                }
            }
       }
       else
       {
            if(SDL_GetTicks()/1000-strartGameSecond<=MAX_TIME)
            {
                if(ballCounter==0)
                {
                    balls.addBall(paths[Map-1]);
                }
                else
                {
                    if((balls.balls.back().ballX-x)*(balls.balls.back().ballX-x)+(balls.balls.back().ballY-y)*(balls.balls.back().ballY-y)>55*55)
                        balls.addBall(paths[Map-1]);
                        x = balls.balls.back().ballX;
                        y = balls.balls.back().ballY;
                }
            }

            addTxt("Time: "+to_string(MAX_TIME-((SDL_GetTicks()/1000)-strartGameSecond)),"time","assets/fonts/FORTE.ttf",{255,255,255},50);
            showText("time",100,50);
            destroyTxt("time");
       }
       balls.renderBalls();
    }
}

void handlEvents()
{
    SDL_Event e;
    if(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                running = false;
            break;
        }
        // ------------------------------------------------------------------------------------------------ LoginSignup Menu
        if(login_signup_menu)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym==SDLK_UP || e.key.keysym.sym==SDLK_DOWN)
                        {selectLogin_signup++;
                        if(sound) Mix_PlayChannel(-1,click,0);}
                    else if(e.key.keysym.sym==SDLK_SPACE)
                        if(selectLogin_signup!=0)
                        {
                            login_signup_menu = false;
                            if(selectLogin_signup%2==1) login_menu = true;
                            else if(selectLogin_signup%2==0) signup_menu = true;
                        }
                break;

            }
        }
        // ------------------------------------------------------------------------------------------------ Login Menu
        else if(login_menu)
        {
            switch(e.type)
            {
                case SDL_TEXTINPUT:
                    if(loginInput==0)
                    {
                        if(loginUsernameInput==" ") loginUsernameInput="";
                        loginUsernameInput+=e.text.text;
                    }
                    else if(loginInput==1)
                    {
                        if(loginPassInput==" ") loginPassInput="";
                        loginPassInput+=e.text.text;
                    }
                break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_BACKSPACE:
                            if(loginInput==0 && loginUsernameInput.size()>0) loginUsernameInput.pop_back();
                            else if(loginInput==1 && loginPassInput.size()>0) loginPassInput.pop_back();
                        break;
                        case SDLK_UP:
                            if(loginInput==1) loginInput=0; break;
                        case SDLK_DOWN:
                            if(loginInput==0) loginInput=1; break;
                        case SDLK_RETURN:
                            if(loginUsernameInput!=" "&&loginPassInput!=" ")
                            {
                                Username = loginUsernameInput;
                                if(isPassRight(loginUsernameInput,loginPassInput))
                                {
                                    login_menu = false;
                                    start_menu = true;
                                }
                            }
                    }
            }
        }
        // ------------------------------------------------------------------------------------------------ Signup Menu
        else if(signup_menu)
        {
            switch(e.type)
            {
                case SDL_TEXTINPUT:
                if(signupInput==0)
                    {
                        if(signupUsernameInput==" ") signupUsernameInput="";
                        signupUsernameInput+=e.text.text;
                    }
                else if(signupInput==1)
                    {
                        if(signupPassInput==" ") signupPassInput="";
                        signupPassInput+=e.text.text;
                    }
                break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_BACKSPACE:
                            if(signupInput==0 && signupUsernameInput.size()>0) signupUsernameInput.pop_back();
                            else if(signupInput==1 && signupPassInput.size()>0) signupPassInput.pop_back();
                        break;
                        case SDLK_UP:
                            if(signupInput==1) signupInput=0; break;
                        case SDLK_DOWN:
                            if(signupInput==0) signupInput=1; break;
                        case SDLK_RETURN:
                            if(signupUsernameInput!=" "&&signupPassInput!=" ")
                            {
                                Username = signupUsernameInput;
                                signup_menu = false;
                                start_menu = true;
                                addUsername(signupUsernameInput,signupPassInput);
                            }
                        break;
                    }
             }

        }
        // ------------------------------------------------------------------------------------------------ Setting Menu
        else if(setting_menu)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym==SDLK_UP) {selectSetting--;if(sound) Mix_PlayChannel(-1,click,0);}
                    else if(e.key.keysym.sym==SDLK_DOWN) {selectSetting++;if(sound) Mix_PlayChannel(-1,click,0);}
                    else if(e.key.keysym.sym==SDLK_SPACE)
                    {
                        setting_menu = false;
                        if(selectSetting%4==0) running=false;
                        else if(selectSetting%4==1) help = true;
                        else if(selectSetting%4==3) {sound = !sound;start_menu=true;}
                        else if(selectSetting%4==2) {music = !music;start_menu=true;}
                    }
                break;
            }
        }
        // ------------------------------------------------------------------------------------------------ Start Menu
        else if(start_menu)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym==SDLK_UP) {selectStart--;if(sound) Mix_PlayChannel(-1,click,0);}
                    else if(e.key.keysym.sym==SDLK_DOWN) {selectStart++;if(sound) Mix_PlayChannel(-1,click,0);}
                    else if(e.key.keysym.sym==SDLK_SPACE)
                    {
                        start_menu=false;
                        if(selectStart%3==1) maps_menu = true;
                        else if(selectStart%3==2) setting_menu = true;
                        else
                        {
                            createRank();
                            leader_board = true;
                        }
                    }
                break;
            }
        }
        // ------------------------------------------------------------------------------------------------ Leader Menu
        else if(leader_board)
        {
            if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_SPACE)
            {
                leader_board=false;
                start_menu = true;
                users.clear();
                scores.clear();
            }
        }
        // ------------------------------------------------------------------------------------------------ Help
        else if(help)
        {
            if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_SPACE)
            {
                help=false;
                start_menu = true;
            }
        }
        // ------------------------------------------------------------------------------------------------ Maps Menu
        else if(maps_menu)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym==SDLK_RIGHT||e.key.keysym.sym==SDLK_LEFT)
                        {
                            if(mapSelector==1) mapSelector=2;
                            else if(mapSelector==2) mapSelector=1;
                            if(sound) Mix_PlayChannel(-1,click,0);
                        }
                    else if(e.key.keysym.sym==SDLK_UP||e.key.keysym.sym==SDLK_DOWN)
                        {
                            if(mapSelector==1||mapSelector==2) mapSelector=3;
                            else mapSelector = 1;
                            if(sound) Mix_PlayChannel(-1,click,0);
                        }
                    else if(e.key.keysym.sym==SDLK_SPACE)
                        {
                            Map = mapSelector;
                            if(Map==3)
                            {
                                cannonX+=100;
                                cannonY-=30;
                            }
                            shots.addShot();
                            maps_menu=false;
                            mode_menu=true;
                        }
                    break;
            }
        }
        // ------------------------------------------------------------------------------------------------ Modes Menu
        else if(mode_menu)
        {
            if(e.type==SDL_KEYDOWN)
            {
                if(e.key.keysym.sym==SDLK_RIGHT||e.key.keysym.sym==SDLK_LEFT)
                {
                    if(modeSelector==1) modeSelector=2;
                    else modeSelector=1;
                    if(sound) Mix_PlayChannel(-1,click,0);
                }
                else if(e.key.keysym.sym==SDLK_SPACE)
                {
                    Mode = modeSelector;
                    strartGameSecond = SDL_GetTicks()/1000;
                    mode_menu = false;
                    game = true;
                }
            }
        }
        // ------------------------------------------------------------------------------------------------ End Menu
        else if(end_menu)
        {
            if(e.type==SDL_KEYDOWN)
            {
                if(e.key.keysym.sym==SDLK_RIGHT||e.key.keysym.sym==SDLK_LEFT)
                {
                    if(endSelector==1) endSelector=2;
                    else endSelector=1;
                    if(sound) Mix_PlayChannel(-1,click,0);
                }
                else if(e.key.keysym.sym==SDLK_SPACE)
                {
                    Score=0;
                    if(endSelector==1)
                    {
                        end_menu = false;
                        start_menu = true;
                        ballCounter=0;
                    }
                    else running = false;
                }
            }
        }
        // ------------------------------------------------------------------------------------------------ Pause Menu
        if(pause_menu)
        {
            if(e.type==SDL_KEYDOWN)
            {
                if(e.key.keysym.sym==SDLK_UP) {pauseSelector--;if(sound) Mix_PlayChannel(-1,click,0);}
                else if(e.key.keysym.sym==SDLK_DOWN) {pauseSelector++;if(sound) Mix_PlayChannel(-1,click,0);}
                else if(e.key.keysym.sym==SDLK_SPACE)
                {
                    pause_menu = false;
                    if(pauseSelector%4==1) game = true;
                    else if(pauseSelector%4==2) {sound = !sound;pause_menu=true;}
                    else if(pauseSelector%4==3) {music = !music;pause_menu=true;}
                    else
                    {
                        start_menu = true;
                        balls.clearBalls();
                        Balls balls;
                    }
                }
            }
        }
        // ------------------------------------------------------------------------------------------------ Game
        else if(game)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym==SDLK_ESCAPE)
                    {
                        game=false;
                        pause_menu = true;
                    }
                    else if(e.key.keysym.sym==SDLK_UP)
                    {
                        balls.addBall(paths[Map-1]);
                    }
                break;
                case SDL_MOUSEMOTION:
                    mouseX = e.motion.x;
                    mouseY = e.motion.y;
                    cannonTeta = atan((cannonY-mouseY)/(mouseX-cannonX));
                    if(mouseX<cannonX) cannonTeta += PI;
                    cannonTeta = 90 - cannonTeta*(180/PI);
                break;
                case SDL_MOUSEBUTTONDOWN:
                    shots.shots.back().isShooted=true;
                    shots.addShot();
                    if(sound) Mix_PlayChannel(-1,shot,0);
                break;
            }
        }
        // ------------------------------------------------------------------------------------------------ END HANDLEVENTS
    }
}


void update()
{
    // ------------------------------------------------------------------------------------------------ LoginSignup Menu
    if(login_signup_menu)
    {
        if(selectLogin_signup%2==1 && selectLogin_signup!=0)
        {
            for(int i=0;i<4;i++) loginColor[i] = selectedLogin_signupColor[i];
            for(int i=0;i<4;i++) signupColor[i] = defaultLogin_signupColor[i];
        }
        else if(selectLogin_signup%2==0 && selectLogin_signup!=0)
        {
            for(int i=0;i<4;i++) loginColor[i] = defaultLogin_signupColor[i];
            for(int i=0;i<4;i++) signupColor[i] = selectedLogin_signupColor[i];
        }
    }
    // ------------------------------------------------------------------------------------------------ Setting Menu
    else if(setting_menu)
    {
        switch(abs(selectSetting)%4)
        {
            case 1:
            {
                for(int i=0;i<4;i++)
                {
                    helpColor[i] = selectSettingColor[i];
                    soundColor[i] = defaultSettingColor[i];
                    musicColor[i] = defaultSettingColor[i];
                    exitColor[i] = defaultSettingColor[i];
                }
            }
            break;
            case 3:
            {
                for(int i=0;i<4;i++)
                {
                    helpColor[i] = defaultSettingColor[i];
                    soundColor[i] = selectSettingColor[i];
                    musicColor[i] = defaultSettingColor[i];
                    exitColor[i] = defaultSettingColor[i];
                }
            }
            break;
            case 2:
            {
                for(int i=0;i<4;i++)
                {
                    helpColor[i] = defaultSettingColor[i];
                    soundColor[i] = defaultSettingColor[i];
                    musicColor[i] = selectSettingColor[i];
                    exitColor[i] = defaultSettingColor[i];
                }
            }
            break;
            case 0:
            {
                for(int i=0;i<4;i++)
                {
                    helpColor[i] = defaultSettingColor[i];
                    soundColor[i] = defaultSettingColor[i];
                    musicColor[i] = defaultSettingColor[i];
                    exitColor[i] = selectSettingColor[i];
                }
            }
            break;
        }
    }
    // ------------------------------------------------------------------------------------------------ Start Menu
    else if(start_menu)
    {
        switch(selectStart%3)
        {
            case 1:
                for(int i=0;i<3;i++)
                {
                    startColor[i] = selectedStartColor[i];
                    settingColor[i] = defaultStartColor[i];
                    leaderColor[i] = defaultStartColor[i];
                }
            break;
            case 2:
                for(int i=0;i<3;i++)
                {
                    startColor[i] = defaultStartColor[i];
                    settingColor[i] = selectedStartColor[i];
                    leaderColor[i] = defaultStartColor[i];
                }
            break;
            case 0:
                for(int i=0;i<3;i++)
                {
                    startColor[i] = defaultStartColor[i];
                    settingColor[i] = defaultStartColor[i];
                    leaderColor[i] = selectedStartColor[i];
                }
            break;
        }
    }
    // ------------------------------------------------------------------------------------------------ Maps Menu
    else if(maps_menu)
    {
        switch(mapSelector)
        {
            case 1:
                for(int i=0;i<3;i++)
                {
                    map1Border[i] = mapBorderSelected[i];
                    map2Border[i] = mapBorderDefault[i];
                    map3Border[i] = mapBorderDefault[i];
                }
            break;
            case 2:
                for(int i=0;i<3;i++)
                {
                    map1Border[i] = mapBorderDefault[i];
                    map2Border[i] = mapBorderSelected[i];
                    map3Border[i] = mapBorderDefault[i];
                }
            break;
            case 3:
                for(int i=0;i<3;i++)
                {
                    map1Border[i] = mapBorderDefault[i];
                    map2Border[i] = mapBorderDefault[i];
                    map3Border[i] = mapBorderSelected[i];
                }
            break;
        }
    }
    // ------------------------------------------------------------------------------------------------ Modes Menu
    if(mode_menu)
    {
       if(modeSelector==1)
            {
                for(int i=0;i<3;i++)
                {
                    mode1Color[i] = modeSelected[i];
                    mode2Color[i] = modeDefault[i];
                }
            }
            else if(modeSelector==2)
            {
                for(int i=0;i<3;i++)
                {
                    mode2Color[i] = modeSelected[i];
                    mode1Color[i] = modeDefault[i];
                }
            }
    }

    // ------------------------------------------------------------------------------------------------ End Menu
    else if(end_menu)
    {
        if(endSelector==1)
        {
            for(int i=0;i<3;i++)
            {
                endExitColor[i] = endDefault[i];
                endRestartColor[i] = endSelected[i];
            }
        }
        else
        {
            for(int i=0;i<3;i++)
            {
                endExitColor[i] = endSelected[i];
                endRestartColor[i] = endDefault[i];
            }
        }
    }
    // ------------------------------------------------------------------------------------------------ Pause Menu
    else if(pause_menu)
    {
        if(pauseSelector%4==1)
        {
            for(int i=0;i<3;i++)
            {
                resumeColor[i] = pauseSelected[i];
                musicColor[i] = pauseDefault[i];
                soundColor[i] = pauseDefault[i];
                exitColor[i] = pauseDefault[i];
            }
        }
        else if(pauseSelector%4==2)
        {
            for(int i=0;i<3;i++)
            {
                resumeColor[i] = pauseDefault[i];
                musicColor[i] = pauseDefault[i];
                soundColor[i] = pauseSelected[i];
                exitColor[i] = pauseDefault[i];
            }
        }
        if(pauseSelector%4==3)
        {
            for(int i=0;i<3;i++)
            {
                resumeColor[i] = pauseDefault[i];
                musicColor[i] = pauseSelected[i];
                soundColor[i] = pauseDefault[i];
                exitColor[i] = pauseDefault[i];
            }
        }
        if(pauseSelector%4==0)
        {
            for(int i=0;i<3;i++)
            {
                resumeColor[i] = pauseDefault[i];
                musicColor[i] = pauseDefault[i];
                soundColor[i] = pauseDefault[i];
                exitColor[i] = pauseSelected[i];
            }
        }
    }
        // ------------------------------------------------------------------------------------------------ Game
    else if(game)
    {
        bool s = false;
        int erases[10],e=0;
        shots.updateShots();
        balls.updateBalls();
        for(int i=0;i<shots.shots.size();i++)
        {
            for(int j=0;j<balls.balls.size();j++)
            {
                if(!s && shots.shots.at(i).shotColor==balls.balls.at(j).ballColor &&(balls.balls.at(j).ballX-shots.shots.at(i).shotX)*(balls.balls.at(j).ballX-shots.shots.at(i).shotX) +
                    (balls.balls.at(j).ballY-shots.shots.at(i).shotY)*(balls.balls.at(j).ballY-shots.shots.at(i).shotY) <= (50)*(50))
                {
                    shots.shots.at(i).destroyShot();
                    shots.shots.erase(shots.shots.begin()+i);
                    balls.balls.at(j).destroyBall();
                    erases[e++] = j;
                    Score+=10;
                    bool x=true;
                    if(j-4>=0 && j+4<=balls.balls.size())
                    {
                    for(int p=j-1;p>j-4;p--)
                    {
                        if(x&&balls.balls.at(j).ballColor==balls.balls.at(p).ballColor)
                            { balls.balls.at(p).destroyBall();Score+=10;erases[e++]=p;}
                        else break;
                    }

                    for(int p=j+1;p<j+4;p++)
                    {

                        if(balls.balls.at(j).ballColor==balls.balls.at(p).ballColor)
                            { balls.balls.at(p).destroyBall();Score+=10;erases[e++]=p;}
                        else break;
                    }
                    }
                    else if(j-4<0 && j+4<=balls.balls.size())
                    {
                    for(int p=j-1;p>=0;p--)
                    {
                        if(x&&balls.balls.at(j).ballColor==balls.balls.at(p).ballColor)
                            { balls.balls.at(p).destroyBall();Score+=10;erases[e++]=p;}
                        else break;
                    }

                    for(int p=j+1;p<j+4;p++)
                    {

                        if(balls.balls.at(j).ballColor==balls.balls.at(p).ballColor)
                            { balls.balls.at(p).destroyBall();Score+=10;erases[e++]=p;}
                        else break;
                    }
                    }
                    else if(j-4>=0 && j+4>balls.balls.size())
                    {
                    for(int p=j-1;p>j-4;p--)
                    {
                        if(x&&balls.balls.at(j).ballColor==balls.balls.at(p).ballColor)
                            { balls.balls.at(p).destroyBall();Score+=10;erases[e++]=p;}
                        else break;
                    }

                    for(int p=j+1;p<balls.balls.size();p++)
                    {

                        if(balls.balls.at(j).ballColor==balls.balls.at(p).ballColor)
                            { balls.balls.at(p).destroyBall();Score+=10;erases[e++]=p;}
                        else break;
                    }
                    }
                    else
                    {
                        for(int p=j-1;p>=0;p--)
                    {
                        if(x&&balls.balls.at(j).ballColor==balls.balls.at(p).ballColor)
                            { balls.balls.at(p).destroyBall();Score+=10;erases[e++]=p;}
                        else break;
                    }

                    for(int p=j+1;p<balls.balls.size();p++)
                    {

                        if(balls.balls.at(j).ballColor==balls.balls.at(p).ballColor)
                            { balls.balls.at(p).destroyBall();Score+=10;erases[e++]=p;}
                        else break;
                    }
                    }

                    for(int i=0;i<e;i++) balls.balls.erase(balls.balls.begin()+erases[i]);

                    s=true;
                }

            }
        }
        if(endGame)
        {
            game=false;
            end_menu=true;
            endGame=false;
            balls.clearBalls();
            balls.balls.clear();
            shots.shots.clear();
            ballCounter=0;
            if(Mode==2)
            {
                int leftSecond =30-((SDL_GetTicks()/1000)-strartGameSecond);
                Score/=10;
                Score+=leftSecond*10;
            }
            updateScore(Username,Score);

        }
    }
}

void close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Texts.clear();
    Textures.clear();

    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

// -------------------------------------------------------------------------------------------------------------------- TexturesManager

bool addTexture(string fileName,string name)
{
    SDL_Surface* surf = IMG_Load(fileName.c_str());
    if(surf==NULL) return false;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surf);
    if(texture==NULL) return false;

    SDL_FreeSurface(surf);

    Textures[name.c_str()] = texture;

    return true;
}

void destroyTexture(string name)
{
    SDL_DestroyTexture(Textures[name]);
    Textures[name] = NULL;
}

void setBackground(string backgroundName)
{
    SDL_Rect rect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderCopy(renderer,Textures[backgroundName],NULL,&rect);
}

void showTexture(string textureName,float x,float y,float angle)
{
    int w,h;
    SDL_QueryTexture(Textures[textureName],NULL,NULL,&w,&h);
    SDL_Rect rect = {x-w/2,y-h/2,w,h};
    SDL_RenderCopyEx(renderer,Textures[textureName],NULL,&rect,angle,NULL,SDL_FLIP_NONE);
}

void showTextureInSize(string textureName,float x,float y,float angle,int w,int h)
{
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderCopyEx(renderer,Textures[textureName],NULL,&rect,angle,NULL,SDL_FLIP_NONE);
}

// -------------------------------------------------------------------------------------------------------------------- TextManager

bool addTxt(string txt,string txtName,string fontFile,SDL_Color fontColor,int fontSize)
{
    TTF_Font* font = TTF_OpenFont(fontFile.c_str(),fontSize);
    if(font==NULL) return false;

    SDL_Surface* fontSurf = TTF_RenderText_Blended(font,txt.c_str(),fontColor);
    if(fontSurf==NULL) return false;

    SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer,fontSurf);
    if(fontTexture==NULL) return false;

    SDL_FreeSurface(fontSurf);
    TTF_CloseFont(font);

    Texts[txtName] = fontTexture;

    return true;
}

void destroyTxt(string txtName)
{
    SDL_DestroyTexture(Texts[txtName]);
    Texts[txtName] = NULL;
}

void showText(string name,double xPos,double yPos)
{
    int w,h;
    SDL_QueryTexture(Texts[name],NULL,NULL,&w,&h);
    int x = xPos-w/2,y = yPos-h/2;
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderCopy(renderer,Texts[name],NULL,&rect);
}

void setButton(string name,double xPos,double yPos,int backR1,int backR2,int r,int g,int b,int a)
{
    int w,h;
    SDL_QueryTexture(Texts[name],NULL,NULL,&w,&h);
    int x = xPos-w/2,y = yPos-h/2;
    filledEllipseRGBA(renderer,x+w/2,y+h/2,backR1,backR2,r,g,b,a);
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderCopy(renderer,Texts[name],NULL,&rect);
}

void setInput(string name,double xPos,double yPos,int wInput,int hInput,int r,int g,int b,int a)
{
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_Rect back_ = {xPos-wInput/2,yPos-hInput/2,wInput,hInput};
    SDL_SetRenderDrawColor(renderer,r,g,b,a);
    SDL_RenderFillRect(renderer,&back_);
    int w,h;
    SDL_QueryTexture(Texts[name],NULL,NULL,&w,&h);
    int x = xPos-w/2,y = yPos-h/2;
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderCopy(renderer,Texts[name],NULL,&rect);
}
// -------------------------------------------------------------------------------------------------------------------- FileManager
bool isPassRight(string username,string password)
{
    ifstream file;
    file.open("assets/database/username_password.txt");
    bool isFound = false,isRight = false;
    string temp;
    while(getline(file,temp))
    {
        for(int i=0;i<username.size();i++)
        {
            if(temp[i]==username[i]) isFound=true;
            else
            {
                isFound = false;
                break;
            }
        }

        if(isFound && temp[username.size()]==':')
        {
            if(temp.size()-username.size()-1==password.size())
            {
                for(int i=username.size()+1;i<temp.size();i++)
                {
                    if(temp[i]==password[i-username.size()-1])
                        isRight = true;
                    else
                    {
                        isRight = false;
                        break;
                    }
                }
            }
        }
        if(isRight)
            {
                file.close();
                return true;
            }
    }
    file.close();
    return false;
}

void addUsername(string username,string password)
{
    ofstream file;
    file.open("assets/database/username_password.txt",ios_base::app);
    file<<username<<":"<<password<<"\n";
    file.close();
    ofstream f;
    f.open("assets/database/username_score.txt",ios::app);
    f<<username<<":0 \n";
    f.close();
}

void createRank()
{
    ifstream readFile;
    fstream writeFile;
    readFile.open("assets/database/username_score.txt");
    writeFile.open("assets/database/ranking.txt");
    string temp;
    while(getline(readFile,temp))
    {
        int i=0;
        string user="",s="";
        while(temp[i]!=':')
        {
            user+=temp[i];
            i++;
        }
        for(int j=i+1;j<temp.size();j++) s+=temp[j];
        long long int score = stoi(s);
        users.push_back(user);
        scores.push_back(score);
    }
    for(int i=0;i<users.size();i++)
    {
        for(int j=0;j<i;j++)
        {
            if(scores.at(i)>scores.at(j))
            {
                long long int n = scores.at(j);
                scores.at(j) = scores.at(i);
                scores.at(i) = n;
                string s = users.at(j);
                users.at(j) = users.at(i);
                users.at(i) = s;
            }
        }
    }

    readFile.close();
    writeFile.close();
}

void clearFile(string path)
{
    ofstream file;
    file.open(path.c_str(),ios::trunc);
    file.close();
}

bool updateScore(string username,int newScore)
{
  ofstream temp;
  temp.open("assets/database/temp.txt",ios::app);
  ifstream file;
  file.open("assets/database/username_score.txt",ios::in);
  string line = "";
  while(getline(file,line))
  {
    bool isFound = false;
    for(int i=0;i<username.size();i++)
    {
        if(username[i]==line[i]) isFound=true;
        else {isFound=false;break;}
    }

    if(!isFound) temp<<line<<endl;
    else
    {
        string preScore="";
        for(int i=username.size()+1;i<line.size();i++) preScore+=line[i];
        if(stoi(preScore)<newScore) temp<<username<<":"<<to_string(newScore)<<endl;
        else temp<<line<<endl;
    }
  }
  file.close();
  temp.close();
  clearFile("assets/database/username_score.txt");
  ofstream fil;
  fil.open("assets/database/username_score.txt",ios::app);
  ifstream tem;
  tem.open("assets/database/temp.txt",ios::in);
  while(getline(tem,line))
    fil<<line<<endl;
  fil.close();
  tem.close();
  clearFile("assets/database/temp.txt");
}


void showRank()
{
    double s = (SCREEN_HEIGHT/(users.size()+1));
    if(s>100) s=100;
    for(int i=0;i<users.size();i++)
    {
        addTxt(users.at(i),"user"+to_string(i),"assets/fonts/Konnect-Bold.otf",{155,200,55},s);
        addTxt(to_string(scores.at(i)),"score"+to_string(i),"assets/fonts/Konnect-Bold.otf",{185,250,185},s);
    }

    for(int i=0;i<users.size();i++)
    {
        showText("user"+to_string(i),SCREEN_WIDTH/4,s+i*(s+20));
        showText("score"+to_string(i),3*SCREEN_WIDTH/4,s+i*(s+20));
        thickLineRGBA(renderer,SCREEN_WIDTH/4,s+i*(s+20)+s/2,3*SCREEN_WIDTH/4,s+i*(s+20)+s/2,10,50,50,50,255);
    }

    for(int i=0;i<users.size();i++)
    {
        destroyTxt("user"+to_string(i));
        destroyTxt("score"+to_string(i));
    }
}

void musicManager()
{
    if(music==false && Mix_PlayingMusic()==1)
        Mix_PauseMusic();
    else if(music==true && Mix_PausedMusic()==1)
        Mix_ResumeMusic();
    else if(music==true && Mix_PlayingMusic()==0)
        Mix_PlayMusic(music1,-1);
}
