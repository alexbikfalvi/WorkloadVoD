#pragma once
#define _CRTDBG_MAP_ALLOC
#define _CRT_RAND_S

#include <crtdbg.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <map>
#include <list>

using namespace std;

#define EPSILON 1E-10

#define DOUBLE_EQ(x,y) (((y - EPSILON) < x) && (x < (y + EPSILON)))
#define DOUBLE_LT(x,y) (x < (y + EPSILON))
#define DOUBLE_GT(x,y) (x > (y - EPSILON))

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
