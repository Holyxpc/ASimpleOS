#ifndef __LIST_H__
#define __LIST_H__


struct list_head{
		struct list_head *next;
		struct list_head *prev;
};


#define offsetof(type, member)	\
			((size_t)&((type *)0) -> member)

#define container_of(ptr, type, member)({	\
	const typeof(((type*)0)->member) *__mptr = (ptr);\
	(type *)((char *)__mptr - offsetof(type, member));})

#define list_entry(ptr, type, member)	\
			container_of(ptr, type, member)

#define prefetch(x)	__builtin_prefetch(x)

#define list_for_each(pos, head) \
			for(pos = (head)->next;\
					prefetch(pos->next), pos != (head);\
				pos = pos->next)

#define list_for_each_prev(pos, head)\
			for(pos = (head)->prev;\
					prefetch(pos->prev), pos != (head);\
				pos = pos->prev)
#define list_for_each_safe(pos, n, head)\
			for(pos = (head)->next, n = pos->next;\
					pos != (head);\
				pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member)	\
			for(pos = list_entry((head)->next, typeof(*pos), member);\
				prefetch(pos->member.next), &pos->member != (head);\
				pos = list_entry(pos->member.next, typeof(*pos), member))

static inline void INIT_LIST_HEAD(struct list_head *list){
		list->next = list;
		list->prev = list;
}

static inline void __list_add(struct list_head *inserted,
								struct list_head *prev,
								struct list_head *next){
		next->prev = inserted;
		inserted->next = next;
		inserted->prev = prev;
		prev->next = inserted;
}

static inline void list_add_tail(struct list_head *inserted, struct list_head *head){
		__list_add(inserted, head->prev, head);
}

#endif

