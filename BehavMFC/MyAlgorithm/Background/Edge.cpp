#include "Edge.h"

Edge::Edge()
{
}

Edge::~Edge()
{
}

int Edge::funMain(string ch, Mat min, Mat mout)
{
	if (ch == "no")
	{
		return 0;
	}
	else if (ch == "rob")
	{

	}
	else if (ch == "pre")
	{

	}
	else if (ch == "sob")
	{

	}
	else if (ch == "lap")
	{
	}
	else if (ch == "can")
	{
		//gray
		if (min.channels() != 1)
			cvtColor(min, min, CV_BGR2GRAY);
		//blur
		blur(min, min, Size(3, 3));
		//canny
		Canny(min, mout, 50, 70);
	}
	else
	{
		return 0;
	}
	return 1;
}
