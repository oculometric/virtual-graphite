#pragma once

enum OLDepthWrite
{
	NEVER,
	LESS,
	LESSEQUAL,
	EQUAL,
	GREATEREQUAL,
	GREATER,
	ALWAYS,
};

struct OLRenderConfig
{
	bool cull_backfaces = true;
	OLDepthWrite depth_mode = OLDepthWrite::LESS;
};

