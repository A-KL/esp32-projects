#pragma once

struct UIRect
{
	int x, y;
	int w, h;

	bool Diff(const UIRect& other, UIRect& result) const
	{
		const auto x_end = x + w;
		const auto y_end = y + h;

		const auto other_x_end = other.x + other.w;
		const auto other_y_end = other.y + other.h;

		if (other_x_end < x || x_end < other.x || y > other_y_end || y_end < other.y) 
		{
			return false;
		}

		const auto left = x > other.x ? x : other.x;
		const auto top = y > other.y ? y : other.y;

		const auto right = x_end < other_x_end ? x_end : other_x_end;
		const auto bottom = y_end < other_y_end ? y_end : other_y_end;
		
		result.x = left;
		result.y = top;
		result.w = right - left;
		result.h = bottom - top;

		return true;
	}
};