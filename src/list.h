/*
  Copyright (c) 2003-2006 Thomas Graf <tgraf@suug.ch>

  Taken from the libnl project and customized to fit for latrace.
  Released undder following license notice.

  Copyright (C) 2008, 2009, 2010 Jiri Olsa <olsajiri@gmail.com>

  This file is part of the latrace.

  The latrace is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The latrace is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the latrace (file COPYING).  If not, see 
  <http://www.gnu.org/licenses/>.
*/


#ifndef LIST_H
#define LIST_H

struct lt_list_head
{
	struct lt_list_head *	next;
	struct lt_list_head *	prev;
};


static inline void __lt_list_add(struct lt_list_head *obj,
				 struct lt_list_head *prev,
				 struct lt_list_head *next)
{
	prev->next = obj;
	obj->prev = prev;
	next->prev = obj;
	obj->next = next;
}

static inline void lt_list_add_tail(struct lt_list_head *obj,
				    struct lt_list_head *head)
{
	__lt_list_add(obj, head->prev, head);
}

static inline void lt_list_add_head(struct lt_list_head *obj,
				    struct lt_list_head *head)
{
	__lt_list_add(obj, head, head->next);
}

static inline void lt_list_del(struct lt_list_head *obj)
{
	obj->next->prev = obj->prev;
	obj->prev->next = obj->next;
}

static inline int lt_list_empty(struct lt_list_head *head)
{
	return head->next == head;
}

#define lt_container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - ((size_t) &((type *)0)->member));})

#define lt_list_entry(ptr, type, member) \
	lt_container_of(ptr, type, member)

#define lt_list_at_tail(pos, head, member) \
	((pos)->member.next == (head))

#define lt_list_at_head(pos, head, member) \
	((pos)->member.prev == (head))

#define LT_LIST_HEAD(name) \
	struct lt_list_head name = { &(name), &(name) }

#define lt_list_first_entry(head, type, member)			\
	lt_list_entry((head)->next, type, member)

#define lt_list_for_each_entry(pos, head, member)				\
	for (pos = lt_list_entry((head)->next, typeof(*pos), member);	\
	     &(pos)->member != (head); 	\
	     (pos) = lt_list_entry((pos)->member.next, typeof(*(pos)), member))

#define lt_list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = lt_list_entry((head)->next, typeof(*pos), member),	\
		n = lt_list_entry(pos->member.next, typeof(*pos), member);	\
	     &(pos)->member != (head); 					\
	     pos = n, n = lt_list_entry(n->member.next, typeof(*n), member))

#define lt_init_list_head(head) \
	do { (head)->next = (head); (head)->prev = (head); } while (0)

#endif
