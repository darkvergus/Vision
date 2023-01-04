#include <VisionRendering/Utils/Defines.h>

#include "VisionGame/Core/Application.h"

FORCE_DEDICATED_GPU

#ifdef _DEBUG
int main()
#else
#undef APIENTRY
#include "Windows.h"
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#endif
{
	VisionGame::Core::Application app;
	app.Run();

	return EXIT_SUCCESS;
}