#pragma once
#include "View.h"
class ImageView :
    public View
{
    public:
 //   ImageView(const std::string& imagePath, const std::string& name = "ImageView")
 //       : View(name), imagePath(imagePath) {}
 //   void draw() override {
 //       // Code to draw the image using the imagePath
 //       // This is a placeholder for actual image rendering logic
 //       std::cout << "Drawing image from: " << imagePath << std::endl;

	//}

    	Texture2D texture;
	ImageView(const char* filepath, int x, int y, int w, int h);
	void draw() override;
public:

	ImageView() = default;
};

