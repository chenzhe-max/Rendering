//debug视图
//当我们不去定义WITH_DEBUG_INFO的时候，宏自动变成空，被宏包裹起来的东西也就不存在了
#pragma once

#ifdef WITH_DEBUG_INFO 
#define DEBUG_LINE(...) __VA_ARGS__ ;
#else
#define DEBUG_LINE(...)
#endif