#pragma once
#include "View.h"
class Label :
    public View
{
public:

	
	const char* text = "Label";
	int fontSize = 20;

	void update() override;
	void draw() override;
};

