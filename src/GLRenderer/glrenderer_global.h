#pragma once

#ifdef GLRENDERER_LIB
#    define GLRENDERER_API __declspec(dllexport)
#else
#    define GLRENDERER_API __declspec(dllimport)
#endif