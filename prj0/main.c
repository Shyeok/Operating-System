#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
#define INPUT_MAX 200
#define STRUCT_MAX 10
struct list_infos // Add struct for save list information
{
	char list_name[20];
	struct list *list_pointer;
};
struct hash_infos // Add struct for save hash information
{
	char hash_name[20];
	struct hash *hash_pointer;
};
struct bitmap_infos // Add struct for save bitmap information
{
	char bm_name[20];
	struct bitmap *bm_pointer;
};
struct list_data // Add struct for list data
{
	struct list_elem e_list;
	int data;
};
struct hash_data // Add struct for hash data
{
	struct hash_elem e_hash;
	int data;
};
int find_list_struct(struct list_infos *l_a, int l_num, char *st_nam) // Add function for find input list
{
	int i;
	int find_flag = -1;
	for(i = 0; i < l_num; i++)
	{
		if(strcmp((l_a[i]).list_name,st_nam) == 0)
		{
			find_flag = i;
			break;
		}
	}
	return find_flag;
}
int find_hash_struct(struct hash_infos *h_a, int h_num, char *st_nam) // Add function for find input hash
{
	int i;
	int find_flag = -1;
	for(i = 0; i < h_num; i++)
	{
		if(strcmp((h_a[i]).hash_name,st_nam) == 0)
		{
			find_flag = i;
			break;
		}
	}
	return find_flag;
}
int find_bitmap_struct(struct bitmap_infos *b_a, int b_num, char *st_nam) // Add function for find input bitmap
{
	int i;
	int find_flag = -1;
	for(i = 0; i < b_num; i++)
	{
		if(strcmp((b_a[i]).bm_name,st_nam) == 0)
		{
			find_flag = i;
			break;
		}
	}
	return find_flag;
}
bool less_function_l(const struct list_elem *list_elem_1, const struct list_elem *list_elem_2, void *aux) // less function for list
{
	if(list_entry(list_elem_1,struct list_data,e_list)->data < list_entry(list_elem_2,struct list_data,e_list)->data)return true;
	else
	{
		return false;
	}
}
bool less_function_h(const struct hash_elem *hash_elem_1, const struct hash_elem *hash_elem_2, void *aux) // less function for hash
{
	if(hash_entry(hash_elem_1,struct hash_data,e_hash)->data < hash_entry(hash_elem_2,struct hash_data,e_hash)->data)return true;
	else
	{
		return false;
	}
}
unsigned hash_hash_function(const struct hash_elem *t_hash_elem, void *aux) // return hash data key
{
	return hash_int(hash_entry(t_hash_elem,struct hash_data,e_hash)->data);
}
void hash_action_func_squ(struct hash_elem *e, void *aux) // put square value
{
	int i;
	int val, val_da;
	val = hash_entry(e,struct hash_data,e_hash)->data;
	val_da = 1;
	for(i = 0; i < 2;i++)
	{
		val_da *= val;
	}
	hash_entry(e,struct hash_data,e_hash)->data = val_da;
}
void hash_action_func_tri(struct hash_elem *e, void *aux) // put triple value
{
	int i;
	int val, val_da;
	val = hash_entry(e,struct hash_data,e_hash)->data;
	val_da = 1;
	for(i = 0; i < 3; i++)
	{
		val_da *= val;
	}
	hash_entry(e,struct hash_data,e_hash)->data = val_da;
}
void hash_action_func_des(struct hash_elem *e, void *aux) // destruction hash_elem
{
	list_remove(&(e->list_elem));
}
struct list_elem* find_list_elem(struct list *t_l, int idx) // get want list_elem address
{
	struct list_elem *t_l_e;
	t_l_e = NULL;
	for(t_l_e=list_begin(t_l);t_l_e!=list_end(t_l);t_l_e=list_next(t_l_e))
	{
		if(idx == 0)break;
		idx = idx - 1;
	}
	return t_l_e;
}
int main()
{
	int i, j, delete_flag = 0, find_flag = 0;
	int list_num = 0, hash_num = 0, bm_num = 0;
	char inp_str[INPUT_MAX];
	char inp_comd[INPUT_MAX], struct_typ[INPUT_MAX];
	char struct_nam[INPUT_MAX];
	struct list *n_list, *t_list, *hash_bucket;
	struct hash *n_hash, *t_hash;
	struct bitmap *n_bm, *t_bm;
	struct list_infos list_arry[STRUCT_MAX];
	struct hash_infos hash_arry[STRUCT_MAX];
	struct bitmap_infos bm_arry[STRUCT_MAX];
	struct list_elem *t_list_elem, *t_list_elem_1;
	struct hash_elem *t_hash_elem, *t_hash_elem_1;
	struct list_data *t_list_data;
	struct hash_data *t_hash_data;
	while(1)
	{
		fgets(inp_str,sizeof(inp_str),stdin);
		sscanf(inp_str,"%s",inp_comd);
		if(strcmp(inp_comd,"quit") == 0)break; // check input quit
		if(strcmp(inp_comd,"create") == 0) // create part
		{
			sscanf(inp_str,"%*s%s%s",struct_typ,struct_nam);
			if(strcmp(struct_typ,"list") == 0)
			{
				n_list = (struct list*)malloc(sizeof(struct list));
				list_init(n_list);
				list_arry[list_num].list_pointer = n_list;
				strcpy(list_arry[list_num].list_name,struct_nam);
				list_num += 1;
			}
			else if(strcmp(struct_typ,"hashtable") == 0)
			{
				n_hash = (struct hash*)malloc(sizeof(struct hash));
				hash_init(n_hash,hash_hash_function,less_function_h,NULL);
				hash_arry[hash_num].hash_pointer = n_hash;
				strcpy(hash_arry[hash_num].hash_name,struct_nam);
				hash_num += 1;
			}
			else if(strcmp(struct_typ,"bitmap") == 0)
			{
				int bitmap_s;
				sscanf(inp_str,"%*s%*s%*s%d",&bitmap_s);
				n_bm = bitmap_create(bitmap_s);
				bm_arry[bm_num].bm_pointer = n_bm;
				strcpy(bm_arry[bm_num].bm_name,struct_nam);
				bm_num += 1;
			}
		}
		else if(strcmp(inp_comd,"delete") == 0) // delete part
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				while(1) // delete list element
				{
					if(list_empty(list_arry[find_flag].list_pointer) == true)break;
					t_list_elem = list_pop_back(list_arry[find_flag].list_pointer);
					free(t_list_elem);
				}
				free(list_arry[find_flag].list_pointer); // delete list
				for(i = find_flag; i < list_num - 1; i++)
				{
					list_arry[i] = list_arry[i + 1];
				}
				list_num -=1;
			}
			else if(find_flag == -1)
			{
				find_flag = find_hash_struct(hash_arry,hash_num,struct_nam);
				if(find_flag != -1)
				{
					hash_clear(hash_arry[find_flag].hash_pointer,hash_action_func_des); // delete hash element
					free(hash_arry[find_flag].hash_pointer); // delete hash
					for(i = find_flag; i < hash_num -1; i++)
					{
						hash_arry[i] = hash_arry[i + 1];
					}
					hash_num -=1;
				}
			}
			else if(find_flag == -1)
			{
				find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
				if(find_flag != -1)
				{
					bitmap_destroy(bm_arry[find_flag].bm_pointer); // destory bitmap
					for(i = find_flag; i < bm_num - 1; i++)
					{
						bm_arry[i] = bm_arry[i + 1];
					}
					bm_num -= 1;
				}
			}
		}
		else if(strcmp(inp_comd,"dumpdata") == 0) // dumpdata instruction
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1) // list part
			{
				t_list = list_arry[find_flag].list_pointer;
				for(t_list_elem = list_begin(t_list); t_list_elem !=list_end(t_list); t_list_elem = list_next(t_list_elem))
				{
					t_list_data = list_entry(t_list_elem,struct list_data,e_list);
					printf("%d ",t_list_data->data);
				}
				if(!(list_empty(t_list)))printf("\n");
			}
			if(find_flag == -1) // hash part
			{
				struct list_elem *t_hash_e;
				struct hash_iterator i;
				find_flag = find_hash_struct(hash_arry,hash_num,struct_nam);
				if(find_flag != -1)
				{
					t_hash = hash_arry[find_flag].hash_pointer;
					hash_first(&i,t_hash);
					while(hash_next(&i))
					{
						t_hash_data = hash_entry(hash_cur(&i),struct hash_data,e_hash);
						printf("%d ",t_hash_data->data);
					}
					if(t_hash->elem_cnt != 0)printf("\n");
				}
			}
			if(find_flag == -1) // bitmap part
			{
				find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
				if(find_flag != -1)
				{
					t_bm = bm_arry[find_flag].bm_pointer;
					for(i = 0; i < bitmap_size(t_bm); i++)
					{
						if(bitmap_test(t_bm,i) == true)printf("1");
						else if(bitmap_test(t_bm,i) == false)printf("0");
					}
					if(bitmap_size(t_bm) != 0)printf("\n");
				}
			}
		}
		else if(strcmp(inp_comd,"list_insert") == 0) // list part
		{
			int insert_idx, insert_data;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d%d",struct_nam,&insert_idx,&insert_data);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				t_list_elem = find_list_elem(t_list,insert_idx);
				if(t_list_elem != NULL)
				{
					t_list_data = (struct list_data*)malloc(sizeof(struct list_data));
					t_list_data->data = insert_data;
					t_list_elem_1 = &(t_list_data->e_list);
					list_insert(t_list_elem,t_list_elem_1);
				}
			}
		}
		else if(strcmp(inp_comd,"list_push_back") == 0)
		{
			int push_ba_da;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&push_ba_da);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list_data = (struct list_data*)malloc(sizeof(struct list_data));
				t_list_data->data = push_ba_da;
				t_list_elem = &(t_list_data->e_list);
				list_push_back(list_arry[find_flag].list_pointer,t_list_elem);
			}
		}
		else if(strcmp(inp_comd,"list_push_front") == 0)
		{
			int push_fr_da;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&push_fr_da);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list_data = (struct list_data*)malloc(sizeof(struct list_data));
				t_list_data->data = push_fr_da;
				t_list_elem = &(t_list_data->e_list);
				list_push_front(list_arry[find_flag].list_pointer,t_list_elem);
			}
		}
		else if(strcmp(inp_comd,"list_remove") == 0)
		{
			int remove_idx;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&remove_idx);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				for(t_list_elem = list_begin(t_list); t_list_elem != list_end(t_list); t_list_elem = list_next(t_list_elem))
				{
					if(remove_idx == 0)break;
					remove_idx -= 1;
				}
				if(remove_idx == 0)list_remove(t_list_elem);
			}
		}
		else if(strcmp(inp_comd,"list_pop_back") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				list_pop_back(t_list);
			}
		}
		else if(strcmp(inp_comd,"list_pop_front") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				list_pop_front(t_list);
			}
		}
		else if(strcmp(inp_comd,"list_back") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				t_list_elem = list_back(t_list);
				t_list_data = list_entry(t_list_elem,struct list_data,e_list);
				printf("%d\n",t_list_data->data);
			}
		}
		else if(strcmp(inp_comd,"list_front") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				t_list_elem = list_front(t_list);
				t_list_data = list_entry(t_list_elem,struct list_data,e_list);
				printf("%d\n",t_list_data->data);
			}
		}
		else if(strcmp(inp_comd,"list_insert_ordered") == 0)
		{
			int insert_or_da;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&insert_or_da);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				t_list_data = (struct list_data *)malloc(sizeof(struct list_data));
				t_list_data->data = insert_or_da;
				t_list_elem = &(t_list_data->e_list);
				list_insert_ordered(t_list,t_list_elem,less_function_l,NULL);
			}
		}
		else if(strcmp(inp_comd,"list_empty") == 0)
		{
			bool list_emp;
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				list_emp = list_empty(list_arry[find_flag].list_pointer);
				if(list_emp == true)printf("true\n");
				else 
				{
					printf("false\n");
				}
			}
		}
		else if(strcmp(inp_comd,"list_size") == 0)
		{
			size_t list_sz;
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				list_sz = list_size(list_arry[find_flag].list_pointer);
				printf("%d\n",list_sz);
			}
		}
		else if(strcmp(inp_comd,"list_max") == 0)
		{
			struct list_elem *list_max_v;
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				list_max_v = list_max(list_arry[find_flag].list_pointer,less_function_l,NULL);
				printf("%d\n",list_entry(list_max_v,struct list_data,e_list)->data);
			}
		}
		else if(strcmp(inp_comd,"list_min") == 0)
		{
			struct list_elem *list_min_v;
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				list_min_v = list_min(list_arry[find_flag].list_pointer,less_function_l,NULL);
				printf("%d\n",list_entry(list_min_v,struct list_data,e_list)->data);
			}
		}
		else if(strcmp(inp_comd,"list_reverse") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				list_reverse(list_arry[find_flag].list_pointer);
			}
		}
		else if(strcmp(inp_comd,"list_sort") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				list_sort(list_arry[find_flag].list_pointer,less_function_l,NULL);
			}
		}
		else if(strcmp(inp_comd,"list_splice") == 0)
		{
			int b_idx, st_idx, en_idx;
			int find_flag_1 = 0;
			char struct_nam_1[INPUT_MAX];
			struct list_elem *t_list_elem_1, *t_list_elem_2;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d%s%d%d",struct_nam,&b_idx,struct_nam_1,&st_idx,&en_idx);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			find_flag_1 = find_list_struct(list_arry,list_num,struct_nam_1);
			if(find_flag != -1 && find_flag_1 != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				t_list_elem = find_list_elem(t_list,b_idx);
				t_list = list_arry[find_flag_1].list_pointer;
				t_list_elem_1 = find_list_elem(t_list,st_idx);
				t_list = list_arry[find_flag_1].list_pointer;
				t_list_elem_2 = find_list_elem(t_list,en_idx);
				list_splice(t_list_elem,t_list_elem_1,t_list_elem_2);
			}
		}
		else if(strcmp(inp_comd,"list_unique") == 0)
		{
			char struct_nam_1[INPUT_MAX];
			int len, inp_type = 0, find_flag_1 = -1;
			struct list *t_list_1;
			len = strlen(inp_str);
			for(i = 0; i < len - 1; i++)
			{
				if(inp_str[i] == ' ' && inp_str[i + 1] != ' ')inp_type+=1;
			}
			sscanf(inp_str,"%*s%s",struct_nam);
			if(inp_type == 2)
			{
				sscanf(inp_str,"%*s%*s%s",struct_nam_1);
				find_flag_1 = find_list_struct(list_arry,list_num,struct_nam_1);
			}
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				t_list = list_arry[find_flag].list_pointer;
				if(inp_type == 1)
				{
					list_unique(t_list,NULL,less_function_l,NULL);
				}
				else if(find_flag_1 != -1 && inp_type == 2)
				{
					t_list_1 = list_arry[find_flag_1].list_pointer;
					list_unique(t_list,t_list_1,less_function_l,NULL);
				}
			}
		}
		else if(strcmp(inp_comd,"list_swap") == 0)
		{
			int swa_a, swa_b, swa_t;
			struct list_elem *t_list_elem_1;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d%d",struct_nam,&swa_a,&swa_b);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				if(swa_a > swa_b) // because my swap function assume that swa_a is always smaller than swb_b
				{
					swa_t = swa_a;
					swa_a = swa_b;
					swa_b = swa_t;
				}
				t_list = list_arry[find_flag].list_pointer;
				t_list_elem = find_list_elem(t_list,swa_a);
				t_list = list_arry[find_flag].list_pointer;
				t_list_elem_1 = find_list_elem(t_list,swa_b);
				list_swap(t_list_elem,t_list_elem_1);
			}
		}
		else if(strcmp(inp_comd,"list_shuffle") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s",struct_nam);
			find_flag = find_list_struct(list_arry,list_num,struct_nam);
			if(find_flag != -1)
			{
				list_shuffle(list_arry[find_flag].list_pointer);
			}
		}
		else if(strcmp(inp_comd,"hash_insert") == 0 || strcmp(inp_comd,"hash_replace") == 0) // hash part
		{
			int insert_v;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&insert_v);
			find_flag = find_hash_struct(hash_arry,hash_num,struct_nam);
			if(find_flag != -1)
			{
				t_hash_data = (struct hash_data*)malloc(sizeof(struct hash_data));
				t_hash_data->data = insert_v;
				t_hash_elem = &(t_hash_data->e_hash);
				if(strcmp(inp_comd,"hash_insert") == 0)hash_insert(hash_arry[find_flag].hash_pointer,t_hash_elem);
				else hash_replace(hash_arry[find_flag].hash_pointer,t_hash_elem);
			}
		}
		else if(strcmp(inp_comd,"hash_apply") == 0)
		{
			char od_str[INPUT_MAX];
			find_flag = -1;
			sscanf(inp_str,"%*s%s%s",struct_nam,od_str);
			find_flag = find_hash_struct(hash_arry,hash_num,struct_nam);
			if(find_flag != -1)
			{
				t_hash = hash_arry[find_flag].hash_pointer;
				if(strcmp(od_str,"square") == 0)hash_apply(t_hash,hash_action_func_squ);
				else if(strcmp(od_str,"triple") == 0)hash_apply(t_hash,hash_action_func_tri);
			}
		}
		else if(strcmp(inp_comd,"hash_delete") == 0 || strcmp(inp_comd,"hash_find") == 0)
		{
			int inp_val;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&inp_val);
			find_flag = find_hash_struct(hash_arry,hash_num,struct_nam);
			if(find_flag != -1)
			{
				t_hash = hash_arry[find_flag].hash_pointer;
				t_hash_elem = NULL;
				for(i = 0; i < t_hash->bucket_cnt; i++)
				{
					hash_bucket = &(t_hash->buckets[i]);
					for(t_list_elem = list_begin(hash_bucket); t_list_elem != list_end(hash_bucket); t_list_elem = list_next(t_list_elem))
					{
						if(hash_entry(list_entry(t_list_elem,struct hash_elem,list_elem),struct hash_data,e_hash)->data == inp_val)
						{
							t_hash_elem = list_entry(t_list_elem,struct hash_elem,list_elem);
							break;
						}
					}
					if(t_hash_elem != NULL)break;
				}
				if(t_hash_elem != NULL)
				{
					if(strcmp(inp_comd,"hash_delete") == 0)hash_delete(t_hash,t_hash_elem);
					else if(strcmp(inp_comd,"hash_find") == 0)
					{
						t_hash_elem_1 = hash_find(t_hash,t_hash_elem);
						if(t_hash_elem_1 != NULL)printf("%d\n",hash_entry(t_hash_elem_1,struct hash_data,e_hash)->data);
					}
				}
			}
		}
		else if(strcmp(inp_comd,"hash_empty") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_hash_struct(hash_arry,hash_num,struct_nam);
			if(find_flag != -1)
			{
				t_hash = hash_arry[find_flag].hash_pointer;
				if(hash_empty(t_hash))printf("true\n");
				else
				{
					printf("false\n");
				}
			}
		}
		else if(strcmp(inp_comd,"hash_size") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_hash_struct(hash_arry,hash_num,struct_nam);
			if(find_flag != -1)
			{
				t_hash = hash_arry[find_flag].hash_pointer;
				printf("%d\n",hash_size(t_hash));
			}
		}
		else if(strcmp(inp_comd,"hash_clear") == 0)
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_hash_struct(hash_arry,hash_num,struct_nam);
			if(find_flag != -1)
			{
				t_hash= hash_arry[find_flag].hash_pointer;
				hash_clear(t_hash,hash_action_func_des);
			}
		}
		else if(strcmp(inp_comd,"bitmap_size") == 0 || strcmp(inp_comd,"bitmap_dump") == 0) // bitmap part
		{
			find_flag = -1;
			sscanf(inp_str,"%*s%s",struct_nam);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				if(strcmp(inp_comd,"bitmap_size") == 0)printf("%d\n",bitmap_size(t_bm));
				else if(strcmp(inp_comd,"bitmap_dump") == 0)bitmap_dump(t_bm);
			}
		}
		else if(strcmp(inp_comd,"bitmap_set") == 0)
		{
			int set_idx;
			char inp_val_str[10];
			bool inp_bm;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d%s",struct_nam,&set_idx,inp_val_str);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				if(strcmp(inp_val_str,"true") == 0)inp_bm = true;
				else if(strcmp(inp_val_str,"false") == 0)inp_bm = false;
				bitmap_set(bm_arry[find_flag].bm_pointer,set_idx,inp_bm);
			}
		}
		else if(strcmp(inp_comd,"bitmap_mark") == 0 || strcmp(inp_comd,"bitmap_reset") == 0 || strcmp(inp_comd,"bitmap_test") == 0)
		{
			int inp_idx;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&inp_idx);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				if(strcmp(inp_comd,"bitmap_mark") == 0)bitmap_mark(t_bm,inp_idx);
				else if(strcmp(inp_comd,"bitmap_reset") == 0)bitmap_reset(t_bm,inp_idx);
				else if(strcmp(inp_comd,"bitmap_test") == 0)
				{
					if(bitmap_test(t_bm,inp_idx) == true)printf("true\n");
					else if(bitmap_test(t_bm,inp_idx) == false)printf("false\n");
				}
			}
		}
		else if(strcmp(inp_comd,"bitmap_all") == 0 || strcmp(inp_comd,"bitmap_any") == 0 || strcmp(inp_comd,"bitmap_none") == 0)
		{
			int st_idx, bm_len;
			bool re_val;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d%d",struct_nam,&st_idx,&bm_len);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				if(strcmp(inp_comd,"bitmap_all") == 0)re_val = bitmap_all(t_bm,st_idx,bm_len);
				else if(strcmp(inp_comd,"bitmap_any") == 0)re_val = bitmap_any(t_bm,st_idx,bm_len);
				else if(strcmp(inp_comd,"bitmap_none") == 0)re_val = bitmap_none(t_bm,st_idx,bm_len);
				if(re_val == true)printf("true\n");
				else if(re_val == false)printf("false\n");
			}
		}
		else if(strcmp(inp_comd,"bitmap_contains") == 0 || strcmp(inp_comd,"bitmap_count") == 0 || strcmp(inp_comd,"bitmap_set_multiple") == 0)
		{
			int st_idx, bm_len;
			char inp_val_str[INPUT_MAX];
			bool re_val, inp_val;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d%d%s",struct_nam,&st_idx,&bm_len,inp_val_str);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				if(strcmp(inp_val_str,"true") == 0)inp_val = true;
				else if(strcmp(inp_val_str,"false") == 0)inp_val = false;
				if(strcmp(inp_comd,"bitmap_contains") == 0)
				{
					re_val = bitmap_contains(t_bm,st_idx,bm_len,inp_val);
					if(re_val == true)printf("true\n");
					else if(re_val == false)printf("false\n");
				}
				else if(strcmp(inp_comd,"bitmap_count") == 0)
				{
					printf("%d\n",bitmap_count(t_bm,st_idx,bm_len,inp_val));
				}
				else if(strcmp(inp_comd,"bitmap_set_multiple") == 0)bitmap_set_multiple(t_bm,st_idx,bm_len,inp_val);
			}
		}
		else if(strcmp(inp_comd,"bitmap_set_all") == 0)
		{
			char inp_val_str[INPUT_MAX];
			bool inp_val;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%s",struct_nam,inp_val_str);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				if(strcmp(inp_val_str,"true") == 0)inp_val = true;
				else if(strcmp(inp_val_str,"false") == 0)inp_val = false;
				bitmap_set_all(t_bm,inp_val);
			}
		}
		else if(strcmp(inp_comd,"bitmap_scan") == 0)
		{
			int st_idx, bm_len;
			unsigned int re_val;
			char inp_val_str[INPUT_MAX];
			bool inp_val;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d%d%s",struct_nam,&st_idx,&bm_len,inp_val_str);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				if(strcmp(inp_val_str,"true") == 0)inp_val = true;
				else if(strcmp(inp_val_str,"false") == 0)inp_val = false;
				re_val = bitmap_scan(t_bm,st_idx,bm_len,inp_val);
				printf("%u\n",re_val);
			}
		}
		else if(strcmp(inp_comd,"bitmap_flip") == 0)
		{
			int inp_idx;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&inp_idx);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				bitmap_flip(t_bm,inp_idx);
			}
		}
		else if(strcmp(inp_comd,"bitmap_scan_and_flip") == 0)
		{
			int inp_idx, inp_cnt;
			unsigned int re_val;
			char inp_val_str[INPUT_MAX];
			bool inp_val;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d%d%s",struct_nam,&inp_idx,&inp_cnt,inp_val_str);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				if(strcmp(inp_val_str,"true") == 0)inp_val = true;
				else if(strcmp(inp_val_str,"false") == 0)inp_val = false;
				re_val = bitmap_scan_and_flip(t_bm,inp_idx,inp_cnt,inp_val);
				printf("%u\n",re_val);
			}
		}
		else if(strcmp(inp_comd,"bitmap_expand") == 0)
		{
			int inp_size;
			find_flag = -1;
			sscanf(inp_str,"%*s%s%d",struct_nam,&inp_size);
			find_flag = find_bitmap_struct(bm_arry,bm_num,struct_nam);
			if(find_flag != -1)
			{
				t_bm = bm_arry[find_flag].bm_pointer;
				bm_arry[find_flag].bm_pointer = bitmap_expand(t_bm,inp_size);
			}
		}
		else if(find_flag == -1) // if user input wrong struct name
		{
			printf("write correct struct name\n");
		}
		else // if user input wrong instruction
		{
			printf("You input wrong instruction\n");
		}
	}
	return 0;
}
