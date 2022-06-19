#pragma once

#ifndef FIELD_H_
#define FIELD_H_

#include "LINK.h"


const int sizeOfBoard = 8;


class Field
{
	private:
		//Figure* selected;

	public:
		std::array<std::array<spFigure, sizeOfBoard>, sizeOfBoard> matrixOfCells; //first - x, second - y

		/*Con-e(-s)*/
		Field();

		/*Setter(-s)-Getter(-s)*/
		Field& setPlace(oxygine::Point PrevPos, oxygine::Point newPos){ 
			matrixOfCells[PrevPos.x][PrevPos.y].swap(matrixOfCells[newPos.x][newPos.y]); 
			return *this; 
		};

		FigColor getFieldColor(oxygine::Point pos);


		/*Methods*/
		void deleteChess(oxygine::Point p);

		void showDeleteAnim(oxygine::spActor actor);

		bool isFreePlace(oxygine::Point p) { return getFieldColor(p) == FigColor::NONE ? true : false; };
	
};



#endif