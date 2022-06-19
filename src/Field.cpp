//#include "Field.h"
#include "LINK.h"
/*Realization of Field*/

Field::Field(){
	for (int i = 0; i < sizeOfBoard; i++)
		for (int j = 0; j < sizeOfBoard; j++)
			matrixOfCells[i][j] = nullptr;
};

FigColor Field::getFieldColor(oxygine::Point pos) { return matrixOfCells[pos.x][pos.y] ? matrixOfCells[pos.x][pos.y]->getColor() : FigColor::NONE; };

void Field::showDeleteAnim(oxygine::spActor actor)
{
	//oxygine::spStage stage = oxygine::getStage();
	//actor->oxygine::Actor::setVisible(false);
	//stage->addChild(actor);
	//cout << "showDeleteAnim" << endl;

};

void Field::deleteChess(oxygine::Point p){
	showDeleteAnim(matrixOfCells[p.x][p.y]);
	//cout << "Delete Chess" << endl;
	//(matrixOfCells[p.x][p.y])->oxygine::Actor::addTween(oxygine::createTween(oxygine::Actor::TweenAlpha(0), 20));
	//addTween(createTween(Actor::TweenAlpha(0), dur, 1, true));
	(matrixOfCells[p.x][p.y])->detach();	
	matrixOfCells[p.x][p.y] = nullptr;
};