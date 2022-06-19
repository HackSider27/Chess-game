#include "oxygine-framework.h"

#include "LINK.h"

using namespace oxygine;

Resources gameResources;
Field mainField;


class MyStage: public Actor
{
public:

	spSprite bg; // Фон

	MyStage(){
		bg = new Sprite;
		bg->setResAnim(gameResources.getResAnim("board"));
		addChild(bg);

		FigColor figColor;
		int mainRow;
		int rowForPawn;

		// расстановка фигур по борде и прикрепление их к Stage
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 2; j++)
			{
				if(j==1){
					figColor = FigColor::BLACK;
					mainRow = 0;
					rowForPawn = 1;
				}else{
					figColor = FigColor::WHITE;
					mainRow = 7;
					rowForPawn = 6;
				}

				if (i==0 || i==7)
					mainField.matrixOfCells[i][mainRow] = new Rook(Point(i, mainRow), figColor);
				if (i==1 || i==6)
					mainField.matrixOfCells[i][mainRow] = new Knight(Point(i, mainRow), figColor);
				if (i==2 || i==5)	
					mainField.matrixOfCells[i][mainRow] = new Bishop(Point(i, mainRow), figColor);
				if (i==3)
					mainField.matrixOfCells[i][mainRow] = new Queen(Point(i, mainRow), figColor);
				if (i==4)
					mainField.matrixOfCells[i][mainRow] = new King(Point(i, mainRow), figColor);
			
				addChild(mainField.matrixOfCells[i][mainRow]);
				
				mainField.matrixOfCells[i][rowForPawn] = new Pawn(Point(i, rowForPawn), figColor);
				addChild(mainField.matrixOfCells[i][rowForPawn]);
			}
			/*for(int j = 2; j < 6; j++){
				mainField.matrixOfCells[i][j] = new Cell(Point(i, j), FigColor::NONE);
				addChild(mainField.matrixOfCells[i][j]);
			}*/
		}

	};

};

DECLARE_SMART(MyStage,spMyStage)

void myStage_preinit() {}

//called from main.cpp
void myStage_init()
{
	//load xml file with resources definition
	gameResources.loadXML("res.xml");

	spMyStage stage = new MyStage;

	//add stage to Stage(oxygine) as child
	getStage()->addChild(stage);

}

//called each frame from main.cpp
void myStage_update() {}

//called each frame from main.cpp
void myStage_destroy()
{
	//free previously loaded resources
	gameResources.free();
}
