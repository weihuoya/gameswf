// gameswf_impl.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation code for the gameswf SWF player library.


#ifndef GAMESWF_MOVIE_H
#define GAMESWF_MOVIE_H

#include "gameswf.h"
#include "gameswf_action.h"
#include "gameswf_types.h"
#include "gameswf_log.h"
#include <assert.h>
#include "base/container.h"
#include "base/utility.h"
#include "base/smart_ptr.h"
#include <stdarg.h>

namespace gameswf
{
	struct movie_root;
	struct swf_event;

	struct movie : public movie_interface
	{
		virtual void set_extern_movie(movie_interface* m) { }
		virtual movie_interface*	get_extern_movie() { return NULL; }

		virtual movie_definition*	get_movie_definition() { return NULL; }
		virtual movie_root*		get_root() { return NULL; }
		virtual movie_interface*	get_root_interface() { return NULL; }
		virtual movie*			get_root_movie() { return NULL; }

		virtual float			get_pixel_scale() const { return 1.0f; }
		virtual character*		get_character(int id) { return NULL; }

		virtual matrix			get_world_matrix() const { return matrix::identity; }
		virtual cxform			get_world_cxform() const { return cxform::identity; }

		//
		// display-list management.
		//

		virtual execute_tag*	find_previous_replace_or_add_tag(int current_frame, int depth, int id)
		{
			return NULL;
		}

		virtual character*	add_display_object(
			Uint16 character_id,
			const char*		 name,
			const array<swf_event*>& event_handlers,
			Uint16			 depth,
			bool			 replace_if_depth_is_occupied,
			const cxform&		 color_transform,
			const matrix&		 mat,
			float			 ratio,
			Uint16			clip_depth)
		{
			return NULL;
		}

		virtual void	move_display_object(
			Uint16		depth,
			bool		use_cxform,
			const cxform&	color_transform,
			bool		use_matrix,
			const matrix&	mat,
			float		ratio,
			Uint16		clip_depth)
		{
		}

		virtual void	replace_display_object(
			Uint16		character_id,
			const char*	name,
			Uint16		depth,
			bool		use_cxform,
			const cxform&	color_transform,
			bool		use_matrix,
			const matrix&	mat,
			float		ratio,
			Uint16		clip_depth)
		{
		}

		virtual void	replace_display_object(
			character*	ch,
			const char*	name,
			Uint16		depth,
			bool		use_cxform,
			const cxform&	color_transform,
			bool		use_matrix,
			const matrix&	mat,
			float		ratio,
			Uint16		clip_depth)
		{
		}

		virtual void	remove_display_object(Uint16 depth, int id)	{}

		virtual void	set_background_color(const rgba& color) {}
		virtual void	set_background_alpha(float alpha) {}
		virtual float	get_background_alpha() const { return 1.0f; }
		virtual void	set_display_viewport(int x0, int y0, int width, int height) {}

		virtual void	add_action_buffer(action_buffer* a) { assert(0); }
		virtual void	do_actions(const array<action_buffer*>& action_list) { assert(0); }

		virtual void	goto_frame(int target_frame_number) { assert(0); }
		virtual bool	goto_labeled_frame(const char* label) { assert(0); return false; }

		virtual void	set_play_state(play_state s) {}
		virtual play_state	get_play_state() const { assert(0); return STOP; }

		virtual void	notify_mouse_state(int x, int y, int buttons)
			// The host app uses this to tell the movie where the
			// user's mouse pointer is.
		{
		}

		virtual void	get_mouse_state(int* x, int* y, int* buttons)
			// Use this to retrieve the last state of the mouse, as set via
			// notify_mouse_state().
		{
			assert(0);
		}

		struct drag_state
		{
			movie*	m_character;
			bool	m_lock_center;
			bool	m_bound;
			float	m_bound_x0;
			float	m_bound_y0;
			float	m_bound_x1;
			float	m_bound_y1;

			drag_state()
				:
			m_character(0), m_lock_center(0), m_bound(0),
				m_bound_x0(0), m_bound_y0(0), m_bound_x1(1), m_bound_y1(1)
			{
			}
		};
		virtual void	get_drag_state(drag_state* st) { assert(0); *st = drag_state(); }
		virtual void	set_drag_state(const drag_state& st) { assert(0); }
		virtual void	stop_drag() { assert(0); }


		// External
		virtual void	set_variable(const char* path_to_var, const char* new_value)
		{
			assert(0);
		}

		// External
		virtual void	set_variable(const char* path_to_var, const wchar_t* new_value)
		{
			assert(0);
		}

		// External
		virtual const char*	get_variable(const char* path_to_var) const
		{
			assert(0);
			return "";
		}

		virtual void * get_userdata() { assert(0); return NULL; }
		virtual void set_userdata(void *) { assert(0); }

		// External
		virtual bool	has_looped() const { return true; }


		//
		// Mouse/Button interface.
		//

		virtual movie* get_topmost_mouse_entity(float x, float y) { return NULL; }
		virtual bool	get_track_as_menu() const { return false; }
		virtual void	on_button_event(event_id id) { on_event(id); }


		//
		// ActionScript.
		//


		virtual movie*	get_relative_target(const tu_string& name)
		{
			assert(0);	
			return NULL;
		}

		// ActionScript event handler.	Returns true if a handler was called.
//		virtual bool	on_event(const event_id& id) { return false; }

		int    add_interval_timer(void *timer)
		{
			log_msg("FIXME: %s: unimplemented\n", __FUNCTION__);
			return -1;	// ???
		}

		void	clear_interval_timer(int x)
		{
			log_msg("FIXME: %s: unimplemented\n", __FUNCTION__);
		}

		virtual void	do_something(void *timer)
		{
			log_msg("FIXME: %s: unimplemented\n", __FUNCTION__);
		}

		// Special event handler; sprites also execute their frame1 actions on this event.
//		virtual void	on_event_load() { on_event(event_id::LOAD); }

#if 0
		// tulrich: @@ is there a good reason these are in the
		// vtable?  I.e. can the caller just call
		// on_event(event_id::SOCK_DATA) instead of
		// on_event_xmlsocket_ondata()?
		virtual void	on_event_xmlsocket_ondata() { on_event(event_id::SOCK_DATA); }
		virtual void	on_event_xmlsocket_onxml() { on_event(event_id::SOCK_XML); }
		virtual void	on_event_interval_timer() { on_event(event_id::TIMER); }
		virtual void	on_event_load_progress() { on_event(event_id::LOAD_PROGRESS); }
#endif
		// as_object_interface stuff
		virtual void	set_member(const tu_stringi& name, const as_value& val) { assert(0); }
		virtual bool	get_member(const tu_stringi& name, as_value* val) { assert(0); return false; }

		virtual void	call_frame_actions(const as_value& frame_spec) { assert(0); }

		virtual movie*	to_movie() { return this; }

		virtual character*	clone_display_object(const tu_string& newname, Uint16 depth, as_object* init_object)
		{
			assert(0);
			return NULL; 
		}

		virtual void	remove_display_object(const tu_string& name) { assert(0); }

		// Forward vararg call to version taking va_list.
		virtual const char*	call_method(const char* method_name, const char* method_arg_fmt, ...)
		{
			va_list	args;
			va_start(args, method_arg_fmt);
			const char*	result = call_method_args(method_name, method_arg_fmt, args);
			va_end(args);

			return result;
		}

		virtual const char*	call_method_args(const char* method_name, const char* method_arg_fmt, va_list args)
			// Override this if you implement call_method.
		{
			assert(0);
			return NULL;
		}

		virtual void	execute_frame_tags(int frame, bool state_only = false) {}

		// External.
		virtual void	attach_display_callback(const char* path_to_object, void (*callback)(void*), void* user_ptr)
		{
			assert(0);
		}

		virtual void	set_display_callback(void (*callback)(void*), void* user_ptr)
			// Override me to provide this functionality.
		{
		}

		virtual bool can_handle_mouse_event() { return false; }

	};


	// Information about how to display a character.
	struct display_info
	{
		movie*	m_parent;
		int	m_depth;
		cxform	m_color_transform;
		matrix	m_matrix;
		float	m_ratio;
		Uint16	m_clip_depth;

		display_info()
			:
		m_parent(NULL),
			m_depth(0),
			m_ratio(0.0f),
			m_clip_depth(0)
		{
		}

		void	concatenate(const display_info& di)
			// Concatenate the transforms from di into our
			// transforms.
		{
			m_depth = di.m_depth;
			m_color_transform.concatenate(di.m_color_transform);
			m_matrix.concatenate(di.m_matrix);
			m_ratio = di.m_ratio;
			m_clip_depth = di.m_clip_depth;
		}
	};

	// character is a live, stateful instance of a character_def.
	// It represents a single active element in a movie.
	struct character : public movie
	{
		int		m_id;
		movie*		m_parent;
		tu_string	m_name;
		int		m_depth;
		cxform		m_color_transform;
		matrix		m_matrix;
		float		m_ratio;
		Uint16		m_clip_depth;
		bool		m_visible;
		hash<event_id, as_value>	m_event_handlers;
		void		(*m_display_callback)(void*);
		void*		m_display_callback_user_ptr;

		character(movie* parent, int id)
			:
		m_id(id),
			m_parent(parent),
			m_depth(-1),
			m_ratio(0.0f),
			m_clip_depth(0),
			m_visible(true),
			m_display_callback(NULL),
			m_display_callback_user_ptr(NULL)

		{
			assert((parent == NULL && m_id == -1)
				|| (parent != NULL && m_id >= 0));
		}

		// Accessors for basic display info.
		int	get_id() const { return m_id; }
		movie*	get_parent() const { return m_parent; }
		void set_parent(movie* parent) { m_parent = parent; }  // for extern movie
		int	get_depth() const { return m_depth; }
		void	set_depth(int d) { m_depth = d; }
		const matrix&	get_matrix() const { return m_matrix; }
		void	set_matrix(const matrix& m)
		{
			assert(m.is_valid());
			m_matrix = m;
		}
		const cxform&	get_cxform() const { return m_color_transform; }
		void	set_cxform(const cxform& cx) { m_color_transform = cx; }
		void	concatenate_cxform(const cxform& cx) { m_color_transform.concatenate(cx); }
		void	concatenate_matrix(const matrix& m) { m_matrix.concatenate(m); }
		float	get_ratio() const { return m_ratio; }
		void	set_ratio(float f) { m_ratio = f; }
		Uint16	get_clip_depth() const { return m_clip_depth; }
		void	set_clip_depth(Uint16 d) { m_clip_depth = d; }

		void	set_name(const char* name) { m_name = name; }
		const tu_string&	get_name() const { return m_name; }

		// For edit_text support (Flash 5).  More correct way
		// is to do "text_character.text = whatever", via
		// set_member().
		virtual const char*	get_text_name() const { return ""; }
		virtual void	set_text_value(const char* new_text) { assert(0); }

		virtual matrix	get_world_matrix() const
			// Get our concatenated matrix (all our ancestor transforms, times our matrix).	 Maps
			// from our local space into "world" space (i.e. root movie space).
		{
			matrix	m;
			if (m_parent)
			{
				m = m_parent->get_world_matrix();
			}
			m.concatenate(get_matrix());

			return m;
		}

		virtual cxform	get_world_cxform() const
			// Get our concatenated color transform (all our ancestor transforms,
			// times our cxform).  Maps from our local space into normal color space.
		{
			cxform	m;
			if (m_parent)
			{
				m = m_parent->get_world_cxform();
			}
			m.concatenate(get_cxform());

			return m;
		}

		// Event handler accessors.
		const hash<event_id, as_value>* get_event_handlers() const 
		{ 
			return &m_event_handlers; 
		} 

		bool	get_event_handler(event_id id, as_value* result)
		{
			return m_event_handlers.get(id, result);
		}
		void	set_event_handler(event_id id, const as_value& method)
		{
			m_event_handlers.set(id, method);
		}

		// Movie interfaces.  By default do nothing.  sprite_instance and some others override these.
		virtual void	display() {}
		virtual float	get_height() { return 0; }
		virtual float	get_width() { return 0; }

		virtual movie*	get_root_movie() { return m_parent->get_root_movie(); }
		virtual int	get_current_frame() const { assert(0); return 0; }
		virtual bool	has_looped() const { assert(0); return false; }
		virtual void	restart() { /*assert(0);*/ }
		virtual void	advance(float delta_time) {}	// for buttons and sprites
		virtual void	goto_frame(int target_frame) {}
		virtual bool	get_accept_anim_moves() const { return true; }

		virtual void	get_drag_state(drag_state* st) { assert(m_parent); m_parent->get_drag_state(st); }

		virtual void	set_visible(bool visible) { m_visible = visible; }
		virtual bool	get_visible() const { return m_visible; }

		virtual void	set_display_callback(void (*callback)(void*), void* user_ptr)
		{
			m_display_callback = callback;
			m_display_callback_user_ptr = user_ptr;
		}

		virtual void	do_display_callback()
		{
			if (m_display_callback)
			{
				(*m_display_callback)(m_display_callback_user_ptr);
			}
		}

		virtual void	get_mouse_state(int* x, int* y, int* buttons) { get_parent()->get_mouse_state(x, y, buttons); }

		// Utility.
		void	do_mouse_drag();

		virtual bool has_keypress_event()
		{
			return false;
		}

	};

}

#endif
