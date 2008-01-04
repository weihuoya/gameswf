// gameswf_canvas.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Drawing API implementation


#ifndef GAMESWF_CANVAS_H
#define GAMESWF_CANVAS_H

#include "gameswf_shape.h"

namespace gameswf
{

	struct canvas : public shape_character_def
	{
		float m_current_x;
		float m_current_y;
		int m_current_fill;
		int m_current_line;
		int m_current_path;

		canvas();
		~canvas();
		virtual canvas* cast_to_canvas() { return this; }

		void begin_fill(const rgba& color);
		void end_fill();
		void close_path();

		void move_to(float x, float y);
		void line_to(float x, float y);
		void curve_to(float cx, float cy, float ax, float ay);

		void add_path(bool new_path);
		void set_line_style(Uint16 width, const rgba& color);
	};


}	// end namespace gameswf

#endif // GAMESWF_CANVAS_H

