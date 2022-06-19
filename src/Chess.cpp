#include "LINK.h"
//#include "Chess.h"

extern Field mainField;
FigColor turn = FigColor::WHITE; // true - white, false - black


/*some helpful func-s*/


void Figure::onTweenDone(oxygine::Event *event)
{
	oxygine::logs::messageln("tween done"); 
	cout << "tween done" << endl;
}

void changeTurn()
{
	if(turn == FigColor::WHITE)
		turn = FigColor::BLACK;
	else turn = FigColor::WHITE;
}

/* convertation from point to board coord */
oxygine::Vector2 GraphicCell(oxygine::Point p)
{
	oxygine::Vector2 vec (offset+p.x*cellSize, offset+p.y*cellSize);
	return vec;
}

/* align on cells of board */
oxygine::Vector2 aligningPosition( const oxygine::Vector2 & curPosition , oxygine::Vector2 size ){
	auto centerPos = curPosition + size / 2;
	oxygine::Vector2 alignPosition(centerPos.x-(int(centerPos.x-offset) % cellSize), centerPos.y-(int(centerPos.y-offset) % cellSize));
	return alignPosition;
}

/* convertation from board coord to point */
oxygine::Point PointCell(oxygine::Vector2 v, oxygine::Vector2 size) //size - размер объекта в пискелях
{
	oxygine::Point cellPos(
		int(v.x-offset) / cellSize,
		int(v.y-offset) / cellSize
	);
	return cellPos;
}

FigColor invertColorField(FigColor color)
{
	if(color == FigColor::BLACK)
		return FigColor::WHITE;
	else if (color == FigColor::WHITE) return FigColor::BLACK;

}

/*vector<oxygine::Point> checkOfFigures(oxygine::Point pos, Chess typeOfFigure, FigColor color)
{
	vector<oxygine::Point> positions;
	size_t i, j;
	//mainField.matrixOfCells[i][j].setPos(pos);

	return positions;
} */

/* Realization of Figure class */

Figure::Figure(oxygine::Point p, FigColor c, Chess t)
{
	//cout << "Figure" << endl;
	pos = p;
    colorOfFigure = c;

	std::string nameType;

	switch(t){
		case Chess::pawn: 
            nameType = "pawn";	
            break;
        case Chess::rook: 
            nameType = "rook";	
            break;
		case Chess::knight: 
            nameType = "knight";	
            break;
		case Chess::bishop: 
            nameType = "bishop";	
            break;
		case Chess::queen: 
            nameType = "queen";	
            break;
		case Chess::king: 
            nameType = "king";	
            break;
	}

	if(colorOfFigure == FigColor::BLACK)
		nameType += "-black";
    else if(colorOfFigure == FigColor::WHITE) 
		nameType += "-white";

	setResAnim(gameResources.getResAnim(nameType));
	
	oxygine::Point newPos(p.x, p.y);
	setPos(newPos);

	pos = newPos;

};

Figure& Figure::setPos(oxygine::Point p){
	setPosition(GraphicCell(p));
	mainField.setPlace(pos, p);
	return *this;
}

void Figure::showPossibleWays(){

	vector<oxygine::Point> possibleWays = getPossibleWays(getPos());
	
	//std::cout << "begin" << std::endl;
	
	for(auto i : possibleWays){
		next_steps.push_back( new Sprite() );
		next_steps.back()->setResAnim(gameResources.getResAnim("step"));
		next_steps.back()->setPosition( GraphicCell(i) );
		oxygine::getStage()->addChild(next_steps.back());
	}
	//std::cout << "end" << std::endl;
};

void Figure::hidePossibleWays()
{
	
	size_t size = next_steps.size();

	for(int i = 0; i != size; i++)
    {
		oxygine::getStage()->removeChild(next_steps.back());
		next_steps.pop_back();
	}
	
};

oxygine::Point Figure::getPos(){ return PointCell(getPosition(), getSize()); };


void Figure::move(const oxygine::Vector2& pos)
{
	//cout << "move" << endl;
	oxygine::Vector2 localPos = stage2local(pos);
	oxygine::Vector2 delta = localPos - curLocation;
	oxygine::Transform tr = getTransform();
	tr.x = 0;
	tr.y = 0;
	oxygine::Vector2 p = tr.transform(delta);
	setPosition(getPosition() + p);
}

void Figure::onEvent(oxygine::Event* ev)
{
	oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(ev);
	//bool firstClick = true;
	if (te->type == oxygine::TouchEvent::TOUCH_DOWN){
		//if(firstClick){
			//cout << "ooooooooooooooo" << endl;
			curLocation = te->localPosition;

			//firstClick = false;

		
			//setFlag(true);
			if(getColor() == turn)
				showPossibleWays();	
	
		//_stage->removeEventListeners(this);

    	//oxygine::spActor _content;
		//oxygine::spActor _easeGraph;

        //_easeGraph->detach();
        //_easeGraph->setPriority(-1);

        //_content->addChild(_easeGraph);

		//attachTo(_content);


        //addTween(createTween(Actor::TweenRotation(getRotation() + (float)MATH_PI * 2), 50, 1));
		
		//attachTo(oxygine::getStage()); 
		//addTween(Sprite::TweenRotation(2*3,14), 2000, 5);

		//oxygine::spTween tween = addTween(Actor::TweenPosition(GraphicCell(pos) + oxygine::Vector2(200, 200)), 2000);
		//tween->setDoneCallback(CLOSURE(this, &Figure::onTweenDone));

		_stage->addEventListener(oxygine::TouchEvent::TOUCH_UP, CLOSURE(this, &Figure::onEvent));
		//addEventListener(oxygine::TouchEvent::TOUCH_DOWN, CLOSURE(this, &Figure::Turn));

		
		/*}else if (te->type == oxygine::TouchEvent::MOVE){
		cout << "sssssssssssssss" << endl;
		

		_stage->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, CLOSURE(this, &Figure::Turn));
		hidePossibleWays();
		}
		_stage->removeEventListeners(this);*/

	}
	if(te->type == oxygine::TouchEvent::TOUCH_UP){

		
			//firstClick = true;

			//cout << "aaaaaaaaaaaaaaa" << endl;

			hidePossibleWays();
			//setFlag(false);
			
			//координаты Левого-верхниего взятого мышкой объекта
			oxygine::Vector2 whereMousUp = getPosition();
			//cout << whereMousUp.x << whereMousUp.y << endl;

			//выровненная позиция whereMousUp на доске по центрам клеточек
			oxygine::Vector2 alignPos = aligningPosition(whereMousUp, getSize());

			auto newCellPos = PointCell(alignPos, getSize());

			bool isNewCellFree = mainField.isFreePlace(newCellPos);

			bool newCellPos_is_possibleStep = false;
			std::vector<oxygine::Point> possibleSteps = getPossibleWays(pos);
			for (auto possibleStep: possibleSteps)
				newCellPos_is_possibleStep = newCellPos_is_possibleStep || (possibleStep == newCellPos);

			if(newCellPos_is_possibleStep && getColor() == turn)
			{
				if(isNewCellFree) 
				{
					setPos(newCellPos);

					pos = newCellPos;
					//oxygine::Actor::addTween(oxygine::createTween(oxygine::Actor::TweenPosition(GraphicCell(pos) + alignPos), 2000));

					//int duration = 500;//ms
					//oxygine::spTween tween = this->addTween(Actor::TweenPosition(GraphicCell(pos) + alignPos), duration);
					//tween->setDoneCallback(CLOSURE(this, &Figure::onTweenDone));
					

					changeTurn();

				}else if(mainField.getFieldColor(newCellPos)==invertColorField(colorOfFigure))
				{
					mainField.deleteChess(newCellPos);

					//oxygine::sleep(400);

					setPos(newCellPos);
					
					pos = newCellPos;

					//this->oxygine::Actor::addTween(oxygine::createTween(oxygine::Actor::TweenPosition(GraphicCell(pos) + alignPos), 2000));
					
					changeTurn();
				}else{ //стоит своя фигура

					setPos(pos);
				};

			}else{
				setPos(pos);
			};
			
			_stage->removeEventListeners(this);

		}
	

};




/*

oxygine::spTween Chess::swap(Figure& a, Figure& b)
{
    oxygine::spTween tween;
    tween = a.jewel->swap(getPos(b.pos));
    tween = b.jewel->swap(getPos(a.pos));
    std::swap(a.jewel, b.jewel);


    tween->setDataObject(new swapData(&a, &b));
    return tween;
}

void Figure::swapped(oxygine::Event* event)
{   
    //std::cout << "swapped" << std::endl;
    oxygine::TweenEvent* te = oxygine::safeCast<oxygine::TweenEvent*>(event);
    oxygine::spObject data = te->tween->getDataObject();
    swapData* sw = safeCast<swapData*>(data.get());
    space& a = *sw->a;
    space& b = *sw->b;
    std::vector<space*> spaces;
    findMatches(spaces, a);
    findMatches(spaces, b);
    if (spaces.empty())
    {
        a.jewel->swap(getPos(b.pos));
        b.jewel->swap(getPos(a.pos));

        std::swap(a.jewel, b.jewel);
    }
}

void Figure::touched(oxygine::Event* event)
{
    //std::cout << "touched" << std::endl;
    oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);
	
    oxygine::Vector2 pos = te->localPosition;

    oxygine::Point spacePos;
    spacePos.x = (int)(pos.x / JewelSize.x);
    spacePos.y = (int)(pos.y / JewelSize.y);

    //oxygine::space* sp = getSpace(spacePos);


    if (selected) 
    {
        //_selected->jewel->unselect();
        //if (sp)
        //{
		//координаты Левого-верхниего взятого мышкой объекта
		oxygine::Vector2 whereMousUp = getPosition();

		//выровненная позиция whereMousUp на доске по центрам клеточек
		oxygine::Vector2 alignPos = aligningPosition(whereMousUp, getSize());

		auto newCellPos = PointCell(alignPos, getSize());

		bool isNewCellFree = mainField.isFreePlace( newCellPos );
		bool isNewPosInFieldByCornerOne = (8*cellSize+getSize().x/2 > whereMousUp.x-offset) && (8*cellSize+getSize().y/2 > whereMousUp.y-offset);
		bool isNewPosInFieldByCornerTwo = (whereMousUp.x-offset > 0-getSize().x/2 ) && (whereMousUp.y-offset > 0-getSize().y/2 );

		bool newCellPos_is_possibleStep = false;
		std::vector<oxygine::Point> possibleSteps = getPossibleWays( pos );
		for (oxygine::Point possibleStep : possibleSteps)
			newCellPos_is_possibleStep = newCellPos_is_possibleStep || (possibleStep == newCellPos);	


		_addTween(createTween(Actor::TweenPosition(this->getPosition() + alignPos), 2000, 1, true));



		if( newCellPos_is_possibleStep && isNewPosInFieldByCornerOne && isNewPosInFieldByCornerTwo )
		{
			//Point dir = _selected->pos - sp->pos;
			//if (dir.x == 0 && abs(dir.y) == 1 || dir.y == 0 && abs(dir.x) == 1)
			if( isNewCellFree ) //если ячейка пустая
			{
				// std::cout << "свободно" << std::endl;

				//setPos( newCellPos );

				// PrevCellPos = getPositionByCell();
				//pos = newCellPos;

				oxygine::spTween tween = swap(this, *sp);
				tween->setDoneCallback(CLOSURE(this, &Figure::swapped));
				
			}else if( mainField.getFieldColor(newCellPos)==invertColorField(colorOfFigure) ) //если занята фигурой противника
			//если на этом месте (newCellPos) фигура другого цвета
			{
				// std::cout << "занято другим цветом! Скушать!" << std::endl;

				mainField.deleteChess( newCellPos );

				setPos( newCellPos );
				
				// PrevCellPos = getPositionByCell();
				pos = newCellPos;

			}else{ //если занята сюзной фигурой
				// std::cout << "занято своим цветом" << std::endl;

				setPos(pos);
			};
			
			_stage->removeEventListeners(this);
		}else{ //выход за пределы поля
			// std::cout << "Игрок хочет поставить фигуру за пределы поля" << std::endl;
			// std::cout << "или Эта фигура так не ходит" << std::endl;

			setPos(pos);
		};
        //}

        selected = false;

    }else //первый touch (выбор фигуры)
    {
		curLocation = te->localPosition;
		showPossibleWays();
        selected = true;
    }
}



*/




void Figure::update(const oxygine::UpdateState& us)
{
	//cout << "update" << endl;
	oxygine::pointer_index pind = getPressed();
	if (!pind)
		return;
	oxygine::PointerState* state = oxygine::Input::instance.getTouchByIndex(pind);
	move(_stage->parent2local(state->getPosition()));
}

void Figure::onAdded2Stage()
{	
	//cout << "onAdded2Stage" << endl;
	addEventListener(oxygine::TouchEvent::TOUCH_DOWN, CLOSURE(this, &Figure::onEvent));
}

void Figure::onRemovedFromStage()
{
	_stage->removeEventListeners(this);
}





/* Realization of figures` classes */

vector<oxygine::Point> Pawn::getPossibleWays(oxygine::Point pos)
{
	vector<oxygine::Point> steps;
	/* need check for eat figure*/

	if (getColor() == FigColor::BLACK){
		if(!mainField.matrixOfCells[pos.x][pos.y + 1])
			steps.push_back(oxygine::Point(pos.x, pos.y + 1));
		if (pos.y == 1 && !mainField.matrixOfCells[pos.x][pos.y + 2] && !mainField.matrixOfCells[pos.x][pos.y + 1])
			steps.push_back(oxygine::Point(pos.x, pos.y + 2));

	if(pos.x != 7)
		if(mainField.matrixOfCells[pos.x + 1][pos.y + 1])
			if(mainField.matrixOfCells[pos.x + 1][pos.y + 1]->getColor() == invertColorField(this->getColor()))
				steps.push_back(oxygine::Point(pos.x + 1, pos.y + 1));
	if(pos.x != 0)
		if(mainField.matrixOfCells[pos.x - 1][pos.y + 1])
			if(mainField.matrixOfCells[pos.x - 1][pos.y + 1]->getColor() == invertColorField(this->getColor()))
				steps.push_back(oxygine::Point(pos.x - 1, pos.y + 1));




		/*size_t j = pos.y + 1;
		size_t i = pos.x;
		if(mainField.matrixOfCells[i - 1][j])
			if ((mainField.matrixOfCells[i - 1][j]->getPos().y == pos.y - 1  || mainField.matrixOfCells[i - 1][j]->getPos().y == pos.y + 1) 
			&& mainField.matrixOfCells[i - 1][j]->getColor() == invertColorField(this->getColor()))
				steps.push_back(oxygine::Point(mainField.matrixOfCells[i - 1][j]->getPos().x, mainField.matrixOfCells[i - 1][j]->getPos().y));
		
		if(mainField.matrixOfCells[i + 1][j])
			if ((mainField.matrixOfCells[i + 1][j]->getPos().y == pos.y - 1  || mainField.matrixOfCells[i + 1][j]->getPos().y == pos.y + 1) 
			&& mainField.matrixOfCells[i + 1][j]->getColor() == invertColorField(this->getColor()))
				steps.push_back(oxygine::Point(mainField.matrixOfCells[i + 1][j]->getPos().x, mainField.matrixOfCells[i + 1][j]->getPos().y));*/

	}
	else if (getColor() == FigColor::WHITE){
		if(!mainField.matrixOfCells[pos.x][pos.y - 1])
			steps.push_back(oxygine::Point(pos.x, pos.y - 1));
		if (pos.y == 6 && !mainField.matrixOfCells[pos.x][pos.y - 2] && !mainField.matrixOfCells[pos.x][pos.y - 1])
			steps.push_back(oxygine::Point(pos.x, pos.y - 2));

		if(pos.x != 7)
			if(mainField.matrixOfCells[pos.x + 1][pos.y - 1])
				if(mainField.matrixOfCells[pos.x+1][pos.y - 1]->getColor() == invertColorField(this->getColor()))
					steps.push_back(oxygine::Point(pos.x + 1, pos.y - 1));
		if(pos.x != 0)
			if(mainField.matrixOfCells[pos.x - 1][pos.y - 1])
				if(mainField.matrixOfCells[pos.x - 1][pos.y - 1]->getColor() == invertColorField(this->getColor()))	
					steps.push_back(oxygine::Point(pos.x - 1, pos.y - 1));

		/*size_t j = pos.y - 1;
		size_t i = pos.x;
		if(mainField.matrixOfCells[i - 1][j])
			if ((mainField.matrixOfCells[i - 1][j]->getPos().y == pos.y - 1  || mainField.matrixOfCells[i - 1][j]->getPos().y == pos.y + 1) 
			&& mainField.matrixOfCells[i - 1][j]->getColor() == invertColorField(this->getColor()))
				steps.push_back(oxygine::Point(mainField.matrixOfCells[i - 1][j]->getPos().x, mainField.matrixOfCells[i - 1][j]->getPos().y));
		
		if(mainField.matrixOfCells[i + 1][j])
			if ((mainField.matrixOfCells[i + 1][j]->getPos().y == pos.y - 1  || mainField.matrixOfCells[i + 1][j]->getPos().y == pos.y + 1) 
			&& mainField.matrixOfCells[i + 1][j]->getColor() == invertColorField(this->getColor()))
				steps.push_back(oxygine::Point(mainField.matrixOfCells[i + 1][j]->getPos().x, mainField.matrixOfCells[i + 1][j]->getPos().y)); */
	}


	return steps;
}

vector<oxygine::Point> Bishop::getPossibleWays(oxygine::Point pos)
{
	std::vector<oxygine::Point> steps;
	std::vector<oxygine::Point> helpSteps;


	for(int i = 1; i < sizeOfBoard; i++){
		helpSteps.push_back(oxygine::Point(pos.x + i, pos.y + i));
		helpSteps.push_back(oxygine::Point(pos.x + i, pos.y - i));
		helpSteps.push_back(oxygine::Point(pos.x - i, pos.y + i));
		helpSteps.push_back(oxygine::Point(pos.x - i, pos.y - i));
	}


/*	while((!mainField.matrixOfCells[pos.x + i][pos.y + i] || mainField.matrixOfCells[pos.x + i][pos.y + i]->getColor() == invertColorField(this->getColor())) && i < sizeOfBoard){
		helpSteps.push_back(oxygine::Point(pos.x + i, pos.y + i));
		i++;}
	i = 1;
	while((!mainField.matrixOfCells[pos.x + i][pos.y - i] || mainField.matrixOfCells[pos.x + i][pos.y - i]->getColor() == invertColorField(this->getColor())) && i < sizeOfBoard){
		helpSteps.push_back(oxygine::Point(pos.x + i, pos.y - i));
		i++;}
	i = 1;
	while((!mainField.matrixOfCells[pos.x - i][pos.y + i] || mainField.matrixOfCells[pos.x - i][pos.y + i]->getColor() == invertColorField(this->getColor())) && i < sizeOfBoard){
		helpSteps.push_back(oxygine::Point(pos.x - i, pos.y + i));
		i++;}
	i = 1;
	while((!mainField.matrixOfCells[pos.x - i][pos.y - i] || mainField.matrixOfCells[pos.x - i][pos.y - i]->getColor() == invertColorField(this->getColor())) && i < sizeOfBoard){
		helpSteps.push_back(oxygine::Point(pos.x - i, pos.y - i));
		i++;}*/


	for(auto newPoint: helpSteps) 
	if(newPoint.x >= 0 && newPoint.x < 8 && newPoint.y >= 0 && newPoint.y < 8 
	&& (!mainField.matrixOfCells[newPoint.x][newPoint.y] 
	|| mainField.matrixOfCells[newPoint.x][newPoint.y]->getColor() == invertColorField(this->getColor())))
		steps.push_back(newPoint);

	return steps;	
}

vector<oxygine::Point> Knight::getPossibleWays(oxygine::Point pos)
{
	std::vector<oxygine::Point> steps;
	std::vector<oxygine::Point> helpSteps;


	helpSteps.push_back(oxygine::Point(pos.x+1, pos.y+2));
	helpSteps.push_back(oxygine::Point(pos.x-1, pos.y+2));
	helpSteps.push_back(oxygine::Point(pos.x+1, pos.y-2));
	helpSteps.push_back(oxygine::Point(pos.x-1, pos.y-2));

	helpSteps.push_back(oxygine::Point(pos.x+2, pos.y+1));
	helpSteps.push_back(oxygine::Point(pos.x-2, pos.y+1));
	helpSteps.push_back(oxygine::Point(pos.x+2, pos.y-1));
	helpSteps.push_back(oxygine::Point(pos.x-2, pos.y-1));

	for(auto newPoint: helpSteps) 
		if(newPoint.x >= 0 && newPoint.x < 8 && newPoint.y >= 0 && newPoint.y < 8 
		&& (!mainField.matrixOfCells[newPoint.x][newPoint.y]
		|| mainField.matrixOfCells[newPoint.x][newPoint.y]->getColor() == invertColorField(this->getColor())))
			steps.push_back(newPoint);

	return steps;
	
}

vector<oxygine::Point> Rook::getPossibleWays(oxygine::Point pos)
{
	std::vector<oxygine::Point> steps;


	for(int i = 0; i < sizeOfBoard; i++){
		if(pos != oxygine::Point(pos.x, i) && (!mainField.matrixOfCells[pos.x][i] 
		|| mainField.matrixOfCells[pos.x][i]->getColor() == invertColorField(this->getColor())))
			steps.push_back(oxygine::Point(pos.x, i));
		if(pos != oxygine::Point(i, pos.y) && (!mainField.matrixOfCells[i][pos.y]
		|| mainField.matrixOfCells[i][pos.y]->getColor() == invertColorField(this->getColor())))

			steps.push_back(oxygine::Point(i, pos.y));
	}

	return steps;
	
}

vector<oxygine::Point> King::getPossibleWays(oxygine::Point pos)
{
	std::vector<oxygine::Point> steps;

	
	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
			if(i == 0 && j == 0)
				continue;
			if(pos.x + i >= 0 && pos.x + i < 8 && pos.y + j >= 0 && pos.y + j < 8 && (!mainField.matrixOfCells[pos.x + i][pos.y + j]
			|| mainField.matrixOfCells[pos.x + i][pos.y + j]->getColor() == invertColorField(this->getColor())))

			steps.push_back(oxygine::Point(pos.x + i, pos.y + j));
		}
	}

	return steps;
	
}

vector<oxygine::Point> Queen::getPossibleWays(oxygine::Point pos)
{
	std::vector<oxygine::Point> steps = Rook::getPossibleWays(pos);
	std::vector<oxygine::Point> secondPart = Bishop::getPossibleWays(pos);

	steps.insert(steps.end(), secondPart.begin(), secondPart.end());

	return steps;
	
}

/*Cell::Cell(oxygine::Point p, FigColor c)
{
	//pos = p;
	//colorOfFigure = c;

	std::string nameType = "unvisible";

	setResAnim(gameResources.getResAnim(nameType));
} */

