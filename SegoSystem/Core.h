
#ifndef CORE_H
#define CORE_H


#include <memory>
#include <assert.h>
#include <string>
#include <Log/Log.h>

#ifdef WINDOWS
#define Sego_API
#else
#define Sego_API

#endif

//ASSERT

#define SG_ASSERT(x,...) {if(!(x)) { SG_ERROR("Assertion Failed: {0}",__VA_ARGS__);}}
#define SG_CORE_ASSERT(x, ...) {if(!(x)) {SG_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__);}}


#endif
