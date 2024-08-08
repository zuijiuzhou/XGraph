#include "Arrays.h"

#define ArrayImpl Int8Array
#include "ArrayImpl.h"
#undef ArrayImpl
#define ArrayImpl Int16Array
#include "ArrayImpl.h"
#undef ArrayImpl
#define ArrayImpl Int32Array
#include "ArrayImpl.h"
#undef ArrayImpl

#define ArrayImpl UInt8Array
#include "ArrayImpl.h"
#undef ArrayImpl
#define ArrayImpl UInt16Array
#include "ArrayImpl.h"
#undef ArrayImpl
#define ArrayImpl UInt32Array
#include "ArrayImpl.h"
#undef ArrayImpl

#define ArrayImpl FloatArray
#include "ArrayImpl.h"
#undef ArrayImpl

#define ArrayImpl Vec2fArray
#include "ArrayImpl.h"
#undef ArrayImpl
#define ArrayImpl Vec3fArray
#include "ArrayImpl.h"
#undef ArrayImpl
#define ArrayImpl Vec4fArray
#include "ArrayImpl.h"
#undef ArrayImpl