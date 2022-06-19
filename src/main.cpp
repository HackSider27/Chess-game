#include "core/oxygine.h"
#include "Stage.h"
#include "DebugActor.h"
#include "MyStage.h"

using namespace oxygine;

// This function is called each frame
int mainloop()
{
	// Update engine-internal components
	// If input events are available, they are passed to Stage::instance.handleEvent
	// If the function returns true, it means that the user requested the application to terminate
	bool done = core::update();

	// It gets passed to our myStage game implementation
	myStage_update();

	// Update our stage
	// Update all actors. Actor::update will also be called for all its children
	oxygine::getStage()->update();

	if (core::beginRendering())
	{
		Color clearColor(32, 155, 32, 255); //цвет фона
		Rect viewport(Point(0, 0), core::getDisplaySize());

		// Render all actors inside the stage. Actor::render will also be called for all its children
		oxygine::getStage()->render(clearColor, viewport);

		core::swapDisplayBuffers();
	}

	return done ? 1 : 0;
}

// Application entry point
void run()
{
	ObjectBase::__startTracingLeaks();

	// Initialize Oxygine's internal stuff
	core::init_desc desc;
	desc.title = "Chess";

	desc.w = 736;
	desc.h = 736; //Размер фоновой картинки

	myStage_preinit();
	core::init(&desc);


	// Create the stage. Stage is a root node for all updateable and drawable objects
	oxygine::Stage::instance = new Stage(true);
	Point size = core::getDisplaySize();
	oxygine::getStage()->setSize(size);

	// DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff
	// DebugActor::show();

	// Initializes our myStage game. See myStage.cpp
	myStage_init();

	// This is the main game loop.
	while (1)
	{
		int done = mainloop();
		if (done)
			break;
	}
	/*
	 If we get here, the user has requested the Application to terminate.
	 We dump and log all our created objects that have not been freed yet
	*/
	ObjectBase::dumpCreatedObjects();

	/*
	Let's clean up everything right now and call ObjectBase::dumpObjects() again.
	We need to free all allocated resources and delete all created actors.
	All actors/sprites are smart-pointer objects and don't need to be removed by hand.
	But now we want to delete it by hand.
	*/

	// See myStage.cpp for the shutdown function implementation
	myStage_destroy();


	//renderer.cleanup();

	// Releases all internal components and the stage
	core::release();

	// The dump list should be empty by now,
	// we want to make sure that there aren't any memory leaks, so we call it again.
	ObjectBase::dumpCreatedObjects();

	ObjectBase::__stopTracingLeaks();
}

#include "SDL_main.h"
#include "SDL.h"

extern "C"
{

	int main(int argc, char* argv[])
	{

		run();

		return 0;
	}
};
