// gameswf.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Player implementation

//TODO

#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_player.h"
#include "base/tu_timer.h"

namespace gameswf
{
	player* create_player()
	{
		return NULL;
	}

	player::player() :
		mouse_x(0),
		mouse_y(0),
		mouse_buttons(0),
		start_ticks(tu_timer::get_ticks()),
		last_ticks(tu_timer::get_ticks()),
		frame_counter(0),
		last_logged_fps(tu_timer::get_ticks()),
		do_render(true)
	{
	}

	player::~player()
	{
	}

	void player::run()
	{
/*		for (;;)
		{
			Uint32	ticks;
			if (do_render)
			{
				ticks = tu_timer::get_ticks();
			}
			else
			{
				// Simulate time.
				ticks = last_ticks + (Uint32) (1000.0f / movie_fps);
			}
			int	delta_ticks = ticks - last_ticks;
			float	delta_t = delta_ticks / 1000.f;
			last_ticks = ticks;

			// Check auto timeout counter.
			if (exit_timeout > 0
				&& ticks - start_ticks > (Uint32) (exit_timeout * 1000))
			{
				// Auto exit now.
				break;
			}

			bool ret = true;
			if (do_render)
			{
				SDL_Event	event;
				// Handle input.
				while (ret)
				{
					if (SDL_PollEvent(&event) == 0)
					{
						break;
					}

					//printf("EVENT Type is %d\n", event.type);
					switch (event.type)
					{
					case SDL_VIDEORESIZE:
						//todo
						//					s_scale = (float) event.resize.w / (float) width;
						//					width = event.resize.w;
						//					height = event.resize.h;
						//					if (SDL_SetVideoMode(event.resize.w, event.resize.h, 0, SDL_OPENGL | SDL_RESIZABLE) == 0)
						//					{
						//						fprintf(stderr, "SDL_SetVideoMode() failed.");
						//						exit(1);
						//					}
						//					glOrtho(-OVERSIZE, OVERSIZE, OVERSIZE, -OVERSIZE, -1, 1);
						break;

					case SDL_USEREVENT:
						//printf("SDL_USER_EVENT at %s, code %d%d\n", __FUNCTION__, __LINE__, event.user.code);
						ret = false;
						break;
					case SDL_KEYDOWN:
						{
							SDLKey	key = event.key.keysym.sym;
							bool	ctrl = (event.key.keysym.mod & KMOD_CTRL) != 0;

							if (key == SDLK_ESCAPE
								|| (ctrl && key == SDLK_q)
								|| (ctrl && key == SDLK_w))
							{
								goto done;
							}
							else if (ctrl && key == SDLK_p)
							{
								// Toggle paused state.
								if (m->get_play_state() == gameswf::movie_interface::STOP)
								{
									m->set_play_state(gameswf::movie_interface::PLAY);
								}
								else
								{
									m->set_play_state(gameswf::movie_interface::STOP);
								}
							}
							else if (ctrl && key == SDLK_i)
							{
								// Init library, for detection of memory leaks (for testing purposes)
								// Load the actual movie.
								md = gameswf::create_movie(infile);
								if (md == NULL)
								{
									fprintf(stderr, "error: can't create a movie from '%s'\n", infile);
									exit(1);
								}
								m = md->create_instance();
								if (m == NULL)
								{
									fprintf(stderr, "error: can't create movie instance\n");
									exit(1);
								}
								gameswf::set_current_root(m.get_ptr());
							}
							else if (ctrl && (key == SDLK_LEFTBRACKET || key == SDLK_KP_MINUS))
							{
								m->goto_frame(m->get_current_frame()-1);
							}
							else if (ctrl && (key == SDLK_RIGHTBRACKET || key == SDLK_KP_PLUS))
							{
								m->goto_frame(m->get_current_frame()+1);
							}
							else if (ctrl && key == SDLK_a)
							{
								// Toggle antialiasing.
								s_antialiased = !s_antialiased;
								//gameswf::set_antialiased(s_antialiased);
							}
							else if (ctrl && key == SDLK_t)
							{
								// test text replacement / variable setting:
								m->set_variable("test.text", "set_edit_text was here...\nanother line of text for you to see in the text box\nSome UTF-8: ñö£ç°ÄÀÔ¿");
							}
							else if (ctrl && key == SDLK_g)
							{
								// test get_variable.
								message_log("testing get_variable: '");
								message_log(m->get_variable("test.text"));
								message_log("'\n");
							}
							else if (ctrl && key == SDLK_m)
							{
								// Test call_method.
								const char* result = m->call_method(
									"test_call",
									"%d, %f, %s, %ls",
									200,
									1.0f,
									"Test string",
									L"Test long string");

								if (result)
								{
									message_log("call_method: result = ");
									message_log(result);
									message_log("\n");
								}
								else
								{
									message_log("call_method: null result\n");
								}
							}
							else if (ctrl && key == SDLK_b)
							{
								// toggle background color.
								s_background = !s_background;
							}
							//							else if (ctrl && key == SDLK_f)	//xxxxxx
							//							{
							//								extern bool gameswf_debug_show_paths;
							//								gameswf_debug_show_paths = !gameswf_debug_show_paths;
							//							}
							else if (ctrl && key == SDLK_EQUALS)
							{
								float	f = gameswf::get_curve_max_pixel_error();
								f *= 1.1f;
								gameswf::set_curve_max_pixel_error(f);
								printf("curve error tolerance = %f\n", f);
							}
							else if (ctrl && key == SDLK_MINUS)
							{
								float	f = gameswf::get_curve_max_pixel_error();
								f *= 0.9f;
								gameswf::set_curve_max_pixel_error(f);
								printf("curve error tolerance = %f\n", f);
							} else if (ctrl && key == SDLK_F2) {
								// Toggle wireframe.
								static bool wireframe_mode = false;
								wireframe_mode = !wireframe_mode;
								if (wireframe_mode) {
									glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
								} else {
									glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
								}
								// TODO: clean up this interafce and re-enable.
								// 					} else if (ctrl && key == SDLK_d) {
								// 						// Flip a special debug flag.
								// 						gameswf_tesselate_dump_shape = true;
							}

							gameswf::key::code c = translate_key(key);
							if (c != gameswf::key::INVALID)
							{
								gameswf::get_current_root()->notify_key_event(c, true);
							}

							break;
						}

					case SDL_KEYUP:
						{
							SDLKey	key = event.key.keysym.sym;

							gameswf::key::code c = translate_key(key);
							if (c != gameswf::key::INVALID)
							{
								gameswf::get_current_root()->notify_key_event(c, false);
							}

							break;
						}

					case SDL_MOUSEMOTION:
						mouse_x = (int) (event.motion.x / s_scale);
						mouse_y = (int) (event.motion.y / s_scale);
						break;

					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						{
							int	mask = 1 << (event.button.button - 1);
							if (event.button.state == SDL_PRESSED)
							{
								mouse_buttons |= mask;
							}
							else
							{
								mouse_buttons &= ~mask;
							}
							break;
						}

					case SDL_QUIT:
						goto done;
						break;

					default:
						break;
					}
				}
			}

			m = gameswf::get_current_root();
			m->set_display_viewport(0, 0, width, height);
			m->set_background_alpha(s_background ? 1.0f : 0.05f);

			m->notify_mouse_state(mouse_x, mouse_y, mouse_buttons);

			Uint32 t_advance = SDL_GetTicks();
			m->advance(delta_t * speed_scale);
			t_advance = SDL_GetTicks() - t_advance;

			if (do_render)
			{
				glDisable(GL_DEPTH_TEST);	// Disable depth testing.
				glDrawBuffer(GL_BACK);
			}

			Uint32 t_display = SDL_GetTicks();
			m->display();
			t_display = SDL_GetTicks() - t_display;

			//printf("advance time: %d, display time %d\n", t_advance, t_display);

			frame_counter++;

			if (do_render)
			{
				SDL_GL_SwapBuffers();
				//glPopAttrib ();

				if (s_measure_performance == false)
				{
					// Don't hog the CPU.
					SDL_Delay(delay);
				}
				else
				{
					// Log the frame rate every second or so.
					if (last_ticks - last_logged_fps > 1000)
					{
						float	delta = (last_ticks - last_logged_fps) / 1000.f;

						if (delta > 0)
						{
							printf("fps = %3.1f\n", frame_counter / delta);
						}
						else
						{
							printf("fps = *inf*\n");
						}

						last_logged_fps = last_ticks;
						frame_counter = 0;
					}
				}
			}
		}*/
	}

}

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:

