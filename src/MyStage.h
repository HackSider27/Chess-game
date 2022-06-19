#pragma once

#ifndef MYSTAGE_H_
#define MYSTAGE_H_

void myStage_preinit();

//called from main.cpp
void myStage_init();

//called each frame from main.cpp
void myStage_update();

//called each frame from main.cpp
void myStage_destroy();

#endif