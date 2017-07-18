// compute.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ncl/gl/GlfwApp.h>
#include "ComputeScene.h"
using namespace std;
using namespace ncl;
using namespace gl;

static ConsoleAppender CONSOLE_APPENDER;
vector<Appender*> Logger::appenders = { &CONSOLE_APPENDER };
const unsigned int Logger::level = LEVEL_DEBUG;

int main()
{
	Scene* scene = new ComputeScene;
	start(scene);
    return 0;
}

