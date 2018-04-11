#pragma once

#include "Maths.h"

struct UniformBufferFormatNull
{

};

struct UniformBufferFormatViewInfo
{
	glm::mat4 
		projection,
		view,
		viewProjection;
};