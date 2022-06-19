#pragma once

#include "oxygine-framework.h"

#include <array>
#include <vector>
#include <string>
#include <fstream> //for saving (coming soon)

//using namespace oxygine;
using namespace std;

extern oxygine::Resources gameResources;

oxygine::Vector2 moveFigure (const oxygine::Vector2 position, oxygine::Vector2 size);


DECLARE_SMART(Figure, spFigure);

const int offset = 25;
const int cellSize = 90;


/*Enums*/
enum class FigColor
{
    BLACK = 0,
    WHITE = 1,
    NONE
};

enum class Chess 
{
    pawn = 0, 
    rook,
    knight,  
    bishop, 
    queen, 
    king = 5,
    unvis
};

class Figure : public oxygine::Sprite
{
    private:
        FigColor colorOfFigure;

        oxygine::Point pos = oxygine::Point(-1, -1);

        vector<oxygine::spSprite> next_steps; //Sprite - набор изображений (с)CSS

        bool selected = false; 

        //oxygine::spSprite sprite;

    public:
        oxygine::Vector2 curLocation;

        /*Con-e(-s)*/
        Figure(oxygine::Point p, FigColor c, Chess type); 

        /*Setter(-s)-Getter(-s)*/
        Figure& setFlag(bool f){ selected = f; return *this; }
        Figure& setPos(oxygine::Point p);

        bool getFlag(){ return selected; }
        oxygine::Point getPos();
        FigColor getColor(){ return colorOfFigure; }

        /*Methods*/
        virtual vector<oxygine::Point> getPossibleWays(oxygine::Point pos) = 0;
        void showPossibleWays();
        void hidePossibleWays();

        void swapped(oxygine::Event* event){};
        void touched(oxygine::Event* event){};
        
        void move(const oxygine::Vector2& pos);
        void onEvent(oxygine::Event* ev);

        void update(const oxygine::UpdateState& us);

        void onAdded2Stage();

        void onRemovedFromStage();

        void onTweenDone(oxygine::Event *event);

        /*De-e*/
        virtual ~Figure(){};

};


class Pawn: public Figure
{
    private:
        bool firstStep; //first step or not
    public:
        /*Con-e(-s)*/
        Pawn(oxygine::Point p, FigColor c): 
            Figure(p, c, Chess::pawn) {};

        /*Methods*/
        virtual vector<oxygine::Point> getPossibleWays(oxygine::Point pos) override;

};

class Bishop: virtual public Figure     //слон
{    
    public:
        /*Con-e(-s)*/
        Bishop(oxygine::Point p, FigColor c): 
            Figure(p, c, Chess::bishop) {};
        /*Methods*/
        virtual vector<oxygine::Point> getPossibleWays(oxygine::Point pos) override;

};

class Knight: public Figure
{
    public:
        /*Con-e(-s)*/
        Knight(oxygine::Point p, FigColor c): 
            Figure(p, c, Chess::knight) {};
        /*Methods*/
        virtual vector<oxygine::Point> getPossibleWays(oxygine::Point pos) override;

};

class Rook: virtual public Figure       //ладья
{      
    public:
        /*Con-e(-s)*/
        Rook(oxygine::Point p, FigColor c): 
            Figure(p, c, Chess::rook) {};
        /*Methods*/
        virtual vector<oxygine::Point> getPossibleWays(oxygine::Point pos) override;

};

class King: public Figure
{
    public:
        /*Con-e(-s)*/
        King(oxygine::Point p, FigColor c): 
            Figure(p, c, Chess::king) {};
        /*Methods*/
        virtual vector<oxygine::Point> getPossibleWays(oxygine::Point pos) override;

};

class Queen: public Rook, Bishop
{
    public:
        /*Con-e(-s)*/
        Queen(oxygine::Point p, FigColor c): 
            Figure(p, c, Chess::queen),
            Rook(p, c),
            Bishop(p, c) {};
        /*Methods*/
        virtual vector<oxygine::Point> getPossibleWays(oxygine::Point pos) override;

};

