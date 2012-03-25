#ifndef _LIST_MAP_H
#define _LIST_MAP_H

#include <stdint.h>

#include "List.h"
#include "log.h"

#ifdef DEBUG
#define LIST_MAP_DEFAULT_HASH_SIZE		29
#else
#define LIST_MAP_DEFAULT_HASH_SIZE		1000003
#endif

template<typename ListMapData>
class ListMap
{
public:

	//friend	class iterator;
	class iterator
	{
	private:
		//ListMap<ListMapData>& S;
		list_head* it;		
		list_head* end_pos;		
	public:
		friend class ListMap;
		iterator(list_head* in_it)
		{
			it = in_it;
		};

		iterator(ListMap<ListMapData>& is)
			: it(NULL),end_pos(NULL)
		{
		};

		iterator()
			: it(NULL),end_pos(NULL)
		{
		};

		iterator(iterator& other)	
		{
			this->it = other.it;
			end_pos = other.end_pos;
		};


		iterator& operator =(iterator& other)	
		{
			this->it = other.it;
			end_pos = other.end_pos;
			return *this;
		};

		iterator& operator ++()	
		{
			if(this->it != end_pos)
			{			
				this->it = this->it->next;
			}
			return *this;
		};
		
		bool operator !=(iterator& other)	
		{
			return (this->it != other.it);
		};

		bool operator ==(iterator& other)	
		{
			return (this->it == other.it);
		};
	};

private:	
	typedef	struct _Node
	{
		list_head hash_index;
		list_head list_index;
		uint64_t key_var;	//�����Node��key
		uint64_t order_var;	//�����Node����ʱ���ݵĲ���ֵ
		ListMapData* data;		
	}Node;

	private:
		list_head* bucket;		
		uint32_t hash_size;
		list_head listhead;

		uint64_t _size;

		iterator it_begin;
		iterator it_end;

		static inline uint64_t hashfn(uint64_t key, uint32_t hash_size)
		{
	//			key ^= (key >> 56);
	//			key ^= (key >> 48);
			key ^= (key >> 32);
	//			key ^= (key >> 24);
			key ^= (key >> 16);
			key ^= (key >> 8);
			return (key % hash_size);
		};	

/******************************************************************************
 * ListMap.add_to_hash - �ڲ����������������ڲ������ṹ
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 10dec2009, Davidfan written
 * --------------------
 ******************************************************************************/
	Node* make_node(uint64_t key, ListMapData *p, uint64_t order_var)
	{
		ListMapData* old_data = get(key);
		if(old_data != NULL || p == NULL)
		{
			//Map��֧��key�ظ�, Ҳ��֧�ֿսṹ
			return NULL;
		}		

		//��ȡ�����ṹ
		Node* new_node = new Node();
		if(new_node == NULL) 
		{
			return NULL;
		}
		
		new_node->data = p;
		new_node->key_var = key;
		new_node->order_var = order_var;
		return new_node;
	};
	
public:
	ListMap(uint32_t in_hash_size = 0)
	{
		if(in_hash_size > 0)
		{
			hash_size = in_hash_size;
		}
		else
		{
			hash_size = LIST_MAP_DEFAULT_HASH_SIZE;
		}

		//���ﲻ���bucket==NULL,���ΪNULL,�Ǿ�ֱ�ӱ�����
		bucket = new list_head[hash_size];
		for(int i = 0; i < (int)hash_size; i++)
		{
			 INIT_LIST_HEAD(&bucket[i]);
		}
		INIT_LIST_HEAD(&listhead);
		
		it_begin.it = listhead.next;
		it_begin.end_pos = &listhead;		
		it_end.it = &listhead;
		it_end.end_pos = &listhead;		
		_size = 0;	
	};
	
	~ListMap()
	{
		for ( int i=0; i<(int)hash_size; i++ ) 
		{
			list_head *pos, *n;
			list_head *head = &bucket[i];
		
			list_for_each_safe(pos, n, head) 
			{
				Node *node = list_entry(pos, Node, hash_index);
				if(node != NULL)
				{
					erase(node->key_var); 	
				}	
			}
		}
	};
	
/******************************************************************************
 * ListMap.add_head - ���뷽�������뵽list_mapͷ��
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 10dec2009, Davidfan written
 * --------------------
 ******************************************************************************/
	ListMapData* add_head(uint64_t key, ListMapData *p)
	{
		Node* new_node = make_node(key, p, 0);	
		if(new_node == NULL)
		{
			return NULL;
		}

		int i = hashfn(key, hash_size);
	 	list_add_head(&new_node->hash_index, &bucket[i]);	
	 	list_add_head(&new_node->list_index, &listhead);	
		_size++;
		return p;
	};

/******************************************************************************
 * ListMap.add_head - ���뷽����ֱ�Ӳ��뵽list_mapβ��
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 10dec2009, Davidfan written
 * --------------------
 ******************************************************************************/
	ListMapData* add_tail(uint64_t key, ListMapData *p)
	{
		Node* new_node = make_node(key, p, 0);	
		if(new_node == NULL)
		{
			return NULL;
		}

		int i = hashfn(key, hash_size);
	 	list_add_head(&new_node->hash_index, &bucket[i]);	
	 	list_add_tail(&new_node->list_index, &listhead);	
		_size++;
		return p;
	};	

	/******************************************************************************
	 * ListMap.move_2_head - ��key��ָ�����ƶ����ײ�
	 * DESCRIPTION: - 
	 *	  N/A 
	 * Input: 
	 * Output: 
	 * Returns: key��ָ��Ԫ�ش��ڣ��򷵻ظ�Ԫ�����ã����򷵻�NULL
	 * 
	 * modification history
	 * --------------------
	 * 01a, 17dec2009, Davidfan written
	 * --------------------
	 ******************************************************************************/
	ListMapData* move_2_head(uint64_t key)
	{
		ListMapData* pdata = erase(key);
		if(pdata == NULL) return NULL;
	
		return add_head(key, pdata);
	};	
	
	/******************************************************************************
	 * ListMap.move_2_tail - ��key��ָ�����ƶ���β��
	 * DESCRIPTION: - 
	 *	  N/A 
	 * Input: 
	 * Output: 
	 * Returns: key��ָ��Ԫ�ش��ڣ��򷵻ظ�Ԫ�����ã����򷵻�NULL
	 * 
	 * modification history
	 * --------------------
	 * 01a, 17dec2009, Davidfan written
	 * --------------------
	 ******************************************************************************/
	ListMapData* move_2_tail(uint64_t key)
	{
		ListMapData* pdata = erase(key);
		if(pdata == NULL) return NULL;
	
		return add_tail(key, pdata);
	};	
	

	ListMapData* erase(uint64_t key)
	{
		list_head *pos;
		list_head *head = &bucket[hashfn(key, hash_size)];
		Node* node = NULL;
		ListMapData* data = NULL;

		list_for_each(pos, head) 
		{
			Node* tmp_node = list_entry(pos, Node, hash_index);
		
			if (tmp_node->key_var == key)
			{
				node = tmp_node;
				break;
			}
		}

		if(NULL != node)
		{
			list_del(&node->hash_index);		
			list_del(&node->list_index);
			_size--;
			data = node->data;
			delete node;
		}
		return data;
	};

	/******************************************************************************
	 * ListMap.erase - ͨ��������ɾ�����
	 * DESCRIPTION: - 
	 *	  ע��::::���������û��ָ����Ч��㣬�򷵻�NULL�������Ӳ��仯�����������ָ����һ��Ԫ�� 
	 * Input: 
	 * Output: 
	 * Returns: ����ɾ��ǰ������ָ��Ľ�������(�ϲ�Ӧ����Ҫ�����ͷŸ�������ָ���ʵ��)��
	 * 
	 * modification history
	 * --------------------
	 * 01a, 17dec2009, Davidfan written
	 * --------------------
	 ******************************************************************************/
	ListMapData* erase(iterator& it)
	{
		if(it == end() || it.it == NULL)
		{
			return NULL;
		}
		Node* node = list_entry(it.it, Node, list_index);
		++it;
		ListMapData* pdata = erase(node->key_var);
		return pdata;	
	}

	ListMapData* get(uint64_t key)
	{
		list_head *pos;
		list_head *head = &bucket[hashfn(key, hash_size)];
		
		//int loop=0;	  
		list_for_each(pos, head) 
		{
			Node* new_node = list_entry(pos, Node, hash_index);
		
			if (new_node->key_var == key) 
				return new_node->data;
		}
		return NULL;
	};

	ListMapData* get_by_it(iterator& it)
	{
		Node* new_node = list_entry(it.it, Node, list_index);
		
		return new_node->data;
	};

	iterator& begin()	
	{
		it_begin.it = listhead.next;
		return it_begin;
	};

	iterator& end()	
	{
//		return iterator(&listhead);
//			it_end.it = &listhead;
		return it_end;
	};

//		list_head* head()	{return &listhead;};
	
	int size(){return _size;};
};


#endif  /* _LIST_MAP_H */
