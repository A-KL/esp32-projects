#pragma once

class TFTCanvas :
	public Canvas<Color>
{
public:
	TFTCanvas(TFT_eSPI* tft) : _tft(tft), _background(0, 0, 0)
	{}

	void Init(const Color& color);

	void DrawPoint(int x, int y, const Color& color);

	void DrawLine(int x0, int y0, int x1, int y1, const Color& color);

	void DrawFilledRect(int x0, int y0, int w, int h, const Color& color);

	void DrawRect(int x0, int y0, int w, int h, const Color& color);

	void DrawText(int x, int y, int w, int h, const char* text, const Color& color);

	void DrawImage(int x, int y, int w, int h, const unsigned short* data);

	void LoadFont(const uint8_t array[]);
	
	void SetFont(int index, unsigned char size);

	inline int Height() const
	{
		return TFT_HEIGHT;
	}

	inline int Width() const
	{
		return TFT_WIDTH;
	}

	void Clear(const Color& color);

	void Clear();

	void Update();

	virtual ~TFTCanvas();

private:
	TFT_eSPI* _tft;

	const Color _background;
};

