#pragma once

#include "Config.h"
#include "HttpApi.h"
#include "StartupParameters.h"

namespace ldr {

struct Contex
{
	Config				Cfg;
	HttpApi				Api;
	StartupParameters	StartupParams;	

};

}