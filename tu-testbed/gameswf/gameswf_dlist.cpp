// gameswf_dlist.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A list of active characters.



#include "gameswf_dlist.h"
#include "gameswf_log.h"
#include "gameswf_render.h"
#include "gameswf.h"
#include <typeinfo>



namespace gameswf
{
	/*static*/ int	display_object_info::compare(const void* _a, const void* _b)
	{
		display_object_info*	a = (display_object_info*) _a;
		display_object_info*	b = (display_object_info*) _b;

		if (a->m_character->get_depth() < b->m_character->get_depth())
		{
			return -1;
		}
		else if (a->m_character->get_depth() == b->m_character->get_depth())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	
	int	display_list::find_display_index(int depth)
		// Find the index in the display list matching the given
		// depth.  Failing that, return the index of the first object
		// with a larger depth.
	{
		int	size = m_display_object_array.size();
		if (size == 0)
		{
			return 0;
		}
		
		// Binary search.
		int	jump = size >> 1;
		int	index = jump;
		for (;;)
		{
			jump >>= 1;
			if (jump < 1) jump = 1;
			
			if (depth > m_display_object_array[index].m_character->get_depth()) {
				if (index == size - 1)
				{
					index = size;
					break;
				}
				index += jump;
			}
			else if (depth < m_display_object_array[index].m_character->get_depth())
			{
				if (index == 0
					|| depth > m_display_object_array[index - 1].m_character->get_depth())
				{
					break;
				}
				index -= jump;
			}
			else
			{
				// match -- return this index.
				break;
			}
		}
		
		assert(index >= 0 && index <= size);
		
		return index;
	}
	

	int	display_list::get_display_index(int depth)
		// Like the above, but looks for an exact match, and returns -1 if failed.
	{
		int	index = find_display_index(depth);
		if (index >= m_display_object_array.size()
			|| get_display_object(index).m_character->get_depth() != depth)
		{
			// No object at that depth.
			return -1;
		}
		return index;
	}


	character*	display_list::get_character_at_depth(int depth)
	{
		int	index = get_display_index(depth);
		if (index != -1)
		{
			character*	ch = m_display_object_array[index].m_character;
			if (ch->get_depth() == depth)
			{
				return ch;
			}
		}

		return NULL;
	}


	character*	display_list::get_character_by_name(const tu_string& name)
	{
		// See if we have a match on the display list.
		for (int i = 0, n = get_character_count(); i < n; i++)
		{
			character*	ch = get_character(i);
			if (strcmp(ch->get_name(), name) == 0)
			{
				// Found it.
				return ch;
			}
		}
		return NULL;
	}

	
	void	display_list::add_display_object(
		character* ch, 
		Uint16 depth, 
		const cxform& color_xform, 
		const matrix& mat, 
		float ratio,
		Uint16 clip_depth)
	{
//		IF_VERBOSE_DEBUG(log_msg("dl::add(%d, '%s')\n", depth, ch->get_name()));//xxxxx

		assert(ch);
		
		// Eliminate an existing object if it's in the way.
		int	size = m_display_object_array.size();
		int index = find_display_index(depth);
		if (index >= 0 && index < size)
		{
			display_object_info & dobj = m_display_object_array[index];

			if (dobj.m_character->get_depth() == depth)
			{
				delete dobj.m_character;
				dobj.m_character = NULL;
				m_display_object_array.remove(index);
			}
		}

		ch->set_depth(depth);

		display_object_info	di;
		di.m_ref = true;
		di.m_character = ch;
		di.m_character->set_depth(depth);
		di.m_character->set_cxform(color_xform);
		di.m_character->set_matrix(mat);
		di.m_character->set_ratio(ratio);
		di.m_character->set_clip_depth(clip_depth);

		// Insert into the display list...
		index = find_display_index(depth);
		
		m_display_object_array.insert(index, di);

		// do the "onClipEvent (load)" event.
		ch->on_event(event_id::LOAD);
	}
	
	
	void	display_list::move_display_object(
		Uint16 depth,
		bool use_cxform,
		const cxform& color_xform,
		bool use_matrix,
		const matrix& mat,
		float ratio,
		Uint16 clip_depth)
	// Updates the transform properties of the object at
	// the specified depth.
	{
//		IF_VERBOSE_DEBUG(log_msg("dl::move(%d)\n", depth));//xxxxx

		int	size = m_display_object_array.size();
		if (size <= 0)
		{
			// error.
			log_error("error: move_display_object() -- no objects on display list\n");
			//			assert(0);
			return;
		}
		
		int	index = find_display_index(depth);
		if (index < 0 || index >= size)
		{
			// error.
			log_error("error: move_display_object() -- can't find object at depth %d\n", depth);
			//			assert(0);
			return;
		}
		
		display_object_info&	di = m_display_object_array[index];
		character*	ch = di.m_character;
		if (ch->get_depth() != depth)
		{
			// error
			log_error("error: move_display_object() -- no object at depth %d\n", depth);
			//			assert(0);
			return;
		}

		di.m_ref = true;

		if (ch->get_accept_anim_moves() == false)
		{
			// This character is rejecting anim moves.  This happens after it
			// has been manipulated by ActionScript.
			return;
		}

		if (use_cxform)
		{
			ch->set_cxform(color_xform);
		}
		if (use_matrix)
		{
			ch->set_matrix(mat);
		}
		ch->set_ratio(ratio);
		ch->set_clip_depth(clip_depth);
	}
	
	
	void	display_list::replace_display_object(
		character* ch,
		Uint16 depth,
		bool use_cxform,
		const cxform& color_xform,
		bool use_matrix,
		const matrix& mat,
		float ratio,
		Uint16 clip_depth)
	// Puts a new character at the specified depth, replacing any
	// existing character.  If use_cxform or use_matrix are false,
	// then keep those respective properties from the existing
	// character.
	{
//		IF_VERBOSE_DEBUG(log_msg("dl::replace(%d)\n", depth));//xxxxx

		int	size = m_display_object_array.size();
		int	index = find_display_index(depth);
		if (index < 0 || index >= size)
		{
			// Error.
//			IF_VERBOSE_DEBUG(log_msg("dl::replace_display_object() no obj at depth %d\n", depth));
			// Fallback -- add the object.
			add_display_object(ch, depth, color_xform, mat, ratio, clip_depth);
			return;
		}
		
		display_object_info&	di = m_display_object_array[index];
		if (di.m_character->get_depth() != depth)
		{
			// error
//			IF_VERBOSE_DEBUG(log_msg("warning: replace_display_object() -- no object at depth %d\n", depth));
			return;
		}
		
		character*	old_ch = di.m_character;

		// Put the new character in its place.
		assert(ch);
		ch->set_depth(depth);
		ch->restart();
		
		// Set the display properties.
		di.m_ref = true;
		di.m_character = ch;

		if (use_cxform)
		{
			ch->set_cxform(color_xform);
		}
		else
		{
			// Use the cxform from the old character.
			ch->set_cxform(old_ch->get_cxform());
		}

		if (use_matrix)
		{
			ch->set_matrix(mat);
		}
		else
		{
			// Use the matrix from the old character.
			ch->set_matrix(old_ch->get_matrix());
		}

		ch->set_ratio(ratio);
		ch->set_clip_depth(clip_depth);

		// Delete the old character.
		delete old_ch;
	}
	
	
	void	display_list::remove_display_object(Uint16 depth)
	// Removes the object at the specified depth.
	{
//		IF_VERBOSE_DEBUG(log_msg("dl::remove(%d)\n", depth));//xxxxx

		int	size = m_display_object_array.size();
		if (size <= 0)
		{
			// error.
			log_error("remove_display_object: no characters in display list\n");
			return;
		}
		
		int	index = find_display_index(depth);
		if (index < 0 || index >= size)
		{
			// error -- no character at the given depth.
			log_error("remove_display_object: no character at depth %d\n", depth);
			return;
		}
		
		// Removing the character at get_display_object(index).
		display_object_info&	di = m_display_object_array[index];
		
		// Remove reference only.
		di.m_ref = false;
	}
	
	
	void	display_list::clear()
	// clear the display list.
	{
		int i, n = m_display_object_array.size();
		for (i = 0; i < n; i++)
		{
			character*	ch = m_display_object_array[i].m_character;
			ch->on_event(event_id::UNLOAD);
			delete ch;
		}
		
		m_display_object_array.clear();
	}
	
	
	void	display_list::reset()
	// reset the references to the display list.
	{
		//printf("### reset the display list!\n");
		int i, n = m_display_object_array.size();
		for (i = 0; i < n; i++)
		{
			m_display_object_array[i].m_ref = false;
		}
	}
	
	
	void	display_list::update()
	// remove unreferenced objects.
	{
		//printf("### update the display list!\n");
		
		int r = 0;
		int i, n = m_display_object_array.size();
		for (i = n-1; i >= 0; i--)
		{
			display_object_info & dobj = m_display_object_array[i];
			
			if (dobj.m_ref == false)
			{
				delete dobj.m_character;
				dobj.m_character = NULL;

				m_display_object_array.remove(i);
				r++;
			}
		}
		
		//printf("### removed characters: %4d active characters: %4d\n", r, m_display_object_array.size());
	}
	
	
	void	display_list::advance(float delta_time)
	// advance referenced characters.
	{
		int i, n = m_display_object_array.size();
		for (i = 0; i < n; i++)
		{
			display_object_info & dobj = m_display_object_array[i];
			
			if (dobj.m_ref == true)
			{
				character*	ch = dobj.m_character;
				assert(ch);

				if (ch->get_visible() == false)
				{
					// Don't advance.
					continue;
				}

				dobj.m_character->advance(delta_time);
			}
		}
	}
	
	
	void	display_list::display()
	// Display the referenced characters. Lower depths
	// are obscured by higher depths.
	{
		bool masked = false;
		int highest_masked_layer = 0;
		
		//log_msg("number of objects to be drawn %i\n", m_display_object_array.size());
		
		for (int i = 0; i < m_display_object_array.size(); i++)
		{
			display_object_info&	dobj = m_display_object_array[i];

			character*	ch = dobj.m_character;
			assert(ch);

			if (ch->get_visible() == false)
			{
				// Don't display.
				continue;
			}

			if (ch->get_clip_depth() > 0)
			{
//				log_msg("depth %i, clip_depth %i\n", dobj.m_depth, dobj.m_clip_depth);
			}

			// check whether a previous mask should be disabled
			if (masked)
			{
				if (ch->get_depth() > highest_masked_layer)
				{
//					log_msg("disabled mask before drawing depth %i\n", ch->get_depth());
					masked = false;
					// turn off mask
					render::disable_mask();
				}
			}

			// check whether this object should become mask
			if (ch->get_clip_depth() > 0)
			{
				//log_msg("begin submit mask\n");
				render::begin_submit_mask();
			}
			
			ch->display();

			if (ch->get_clip_depth() > 0)
			{
//				log_msg("object drawn\n");
			}
			
			// if this object should have become a mask,
			// inform the renderer that it now has all
			// information about it
			if (ch->get_clip_depth() > 0)
			{
				//log_msg("end submit mask\n");
				render::end_submit_mask();
				highest_masked_layer = ch->get_clip_depth();
				masked = true;
			}
		}
		
		if (masked)
		{
			// If a mask masks the scene all the way up to the highest
			// layer, it will not be disabled at the end of drawing
			// the display list, so disable it manually.
			render::disable_mask();
		}
	}

}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
