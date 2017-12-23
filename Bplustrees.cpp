#include <stdio.h>
#include <stdlib.h>
#include <cstddef>
#include <stdbool.h>
#include <iostream>
#include <string>
#include <cmath>
#include <new>
#include <vector>
#include <climits>
#include <fstream>

#include <sstream>

#define DEFAULT_ORDER 4

#define MIN_ORDER 3     //as the min order is 3 necessarily
#define MAX_ORDER 20
#define MAX_DUPLICATES 100000

using namespace std;

typedef struct key_value        //represents the value to which a given key would refer.
{
  string* value[MAX_DUPLICATES];    //in order to store duplicates
  int count;                        //no of duplicates for each key
} key_value;

typedef struct btree      //this struct represents a node in our B+ tree.
{
  void ** ptr_val;     //array of pointers corresponding to our array of keys.
  double * keys;
  struct btree * parent;
  bool is_leaf;
  int no_of_keys;
  struct btree * next;
} btree;

ofstream output ("output_file.txt");
int order = DEFAULT_ORDER;


void Search(btree * root, double key);
void Search_range(btree * root, double range1, double range2);
btree * NodeInsertionSPlit(btree * root, btree * parent, int left_index,	double key, btree * right);
btree * ParentInsertion(btree * root, btree * left, double key, btree * right);
btree * Insert( btree * root, double key, string value );


btree * Insert( btree * root, double key, string value )
{

  key_value * pointer;
  btree * leaf;

  key_value* check;
  int iii = 0;
  btree * ccc;

  int ii=0;
  btree* cc = root;
  if(cc == NULL)
  ccc = cc;
  else{
    while(!cc->is_leaf){
      ii=0;
      while(ii<cc->no_of_keys){
        if(key>=cc->keys[ii]) ii++;
        else break;
      }
      cc = (btree*)cc-> ptr_val[ii];
    }
    ccc = cc;
  }

  if (ccc == NULL) check = NULL;
  else{
    for (iii = 0; iii < ccc->no_of_keys; iii++)
    if (ccc->keys[iii] == key) break;
    if (iii == ccc->no_of_keys)
    check = NULL;
    else
    check = (key_value *)ccc->ptr_val[iii];
  }

  if (check != NULL) {
    string* xxccc = new string(value);
    check->value[check->count] = xxccc;
    check->count += 1;
    return root;
  }

  pointer = (key_value *)malloc(sizeof(key_value));
  string* temp1 = new string(value);

  (pointer->value[0]) = (temp1);
  pointer->count = 1;

  if (root == NULL){
    btree * new_nodey;
    new_nodey = (btree*)malloc(sizeof(btree));
    new_nodey->keys = (double*)malloc( (order - 1) * sizeof(double) );
    new_nodey->ptr_val = (void**)malloc( order * sizeof(void *) );
    new_nodey->is_leaf = true;
    new_nodey->no_of_keys = 1;
    new_nodey->parent = NULL;
    new_nodey->next = NULL;
    new_nodey->keys[0] = key;
    new_nodey->ptr_val[0] = pointer;
    new_nodey->ptr_val[order - 1] = NULL;
    return new_nodey;
  }

  int im = 0;
  btree * cm = root;
  if (cm == NULL)
  leaf = cm;
  else{
    while (!cm->is_leaf) {
      im = 0;
      while (im < cm->no_of_keys) {
        if (key >= cm->keys[im]) im++;
        else break;
      }
      cm = (btree *)cm->ptr_val[im];
    }
    leaf = cm;
  }

  if (leaf->no_of_keys < order - 1)
  {

    int ix, ins_pt;
    ins_pt = 0;
    while (ins_pt < leaf->no_of_keys && leaf->keys[ins_pt] < key)
    ins_pt++;

    for (ix = leaf->no_of_keys; ix > ins_pt; ix--)
    {
      leaf->keys[ix] = leaf->keys[ix - 1];
      leaf->ptr_val[ix] = leaf->ptr_val[ix - 1];
    }
    leaf->keys[ins_pt] = key;
    leaf->ptr_val[ins_pt] = pointer;
    leaf->no_of_keys++;

    return root; }

    double * temp_keysx;
    void ** temp_pointersx;
    int insertion_indexx, split, new_keyx, ic, jc;


    btree * new_leafx;
    new_leafx = (btree*)malloc(sizeof(btree));
    new_leafx->keys = (double*)malloc( (order - 1) * sizeof(double) );
    new_leafx->ptr_val = (void**)malloc( order * sizeof(void *) );
    new_leafx->no_of_keys = 0;
    new_leafx->parent = NULL;
    new_leafx->next = NULL;
    new_leafx->is_leaf = true;

    temp_keysx = (double*)malloc( order * sizeof(double) );
    temp_pointersx = (void**)malloc( order * sizeof(void *) );

    insertion_indexx = 0;
    while (insertion_indexx < order - 1 && leaf->keys[insertion_indexx] < key)
    insertion_indexx++;

    for (ic = 0, jc = 0; ic < leaf->no_of_keys; ic++, jc++) {
      if (jc == insertion_indexx) jc++;
      temp_keysx[jc] = leaf->keys[ic];
      temp_pointersx[jc] = leaf->ptr_val[ic];
    }

    temp_keysx[insertion_indexx] = key;
    temp_pointersx[insertion_indexx] = pointer;

    leaf->no_of_keys = 0;

    split = ((order - 1)%2 == 0)?((order - 1)/2):((order - 1)/2)+1;

    for (ic = 0; ic < split; ic++) {
      leaf->ptr_val[ic] = temp_pointersx[ic];
      leaf->keys[ic] = temp_keysx[ic];
      leaf->no_of_keys++;
    }

    for (ic = split, jc = 0; ic < order; ic++, jc++) {
      new_leafx->ptr_val[jc] = temp_pointersx[ic];
      new_leafx->keys[jc] = temp_keysx[ic];
      new_leafx->no_of_keys++;
    }

    free(temp_pointersx);
    free(temp_keysx);

    new_leafx->ptr_val[order - 1] = leaf->ptr_val[order - 1];
    leaf->ptr_val[order - 1] = new_leafx;

    for (ic = leaf->no_of_keys; ic < order - 1; ic++)
    leaf->ptr_val[ic] = NULL;
    for (ic = new_leafx->no_of_keys; ic < order - 1; ic++)
    new_leafx->ptr_val[ic] = NULL;

    new_leafx->parent = leaf->parent;
    new_keyx = new_leafx->keys[0];

    return ParentInsertion(root, leaf, new_keyx, new_leafx);

}

btree * NodeInsertionSPlit(btree * root, btree * old_node, int left_index, double key, btree * right)
{

  int i, j, split, k_prime;
  btree * new_node, * child;
  double * temp_keys;
  btree ** temp_pointers;

  temp_pointers = (btree**)malloc( (order + 1) * sizeof(btree *) );
  temp_keys = (double*)malloc( order * sizeof(double) );

  for (i = 0, j = 0; i < old_node->no_of_keys + 1; i++, j++) {
    if (j == left_index + 1) j++;
    temp_pointers[j] = (btree*)old_node->ptr_val[i];
  }

  for (i = 0, j = 0; i < old_node->no_of_keys; i++, j++) {
    if (j == left_index)
      j++;
    temp_keys[j] = old_node->keys[i];
  }

  temp_pointers[left_index + 1] = right;
  temp_keys[left_index] = key;

  split = (order%2==0)?order/2:order/2+1;

  new_node = (btree*)malloc(sizeof(btree));
  new_node->keys = (double*)malloc( (order - 1) * sizeof(double) );
  new_node->ptr_val = (void**)malloc( order * sizeof(void *) );
  new_node->is_leaf = false;
  new_node->no_of_keys = 0;
  new_node->parent = NULL;
  new_node->next = NULL;


  old_node->no_of_keys = 0;
  for (i = 0; i < split - 1; i++) {
    old_node->ptr_val[i] = temp_pointers[i];
    old_node->keys[i] = temp_keys[i];
    old_node->no_of_keys++;
  }
  old_node->ptr_val[i] = temp_pointers[i];
  k_prime = temp_keys[split - 1];
  for (++i, j = 0; i < order; i++, j++) {
    new_node->ptr_val[j] = temp_pointers[i];
    new_node->keys[j] = temp_keys[i];
    new_node->no_of_keys++;
  }
  new_node->ptr_val[j] = temp_pointers[i];
  free(temp_pointers);
  free(temp_keys);

  new_node->parent = old_node->parent;
  for (i = 0; i <= new_node->no_of_keys; i++) {
    child = (btree*)new_node->ptr_val[i];
    child->parent = new_node;
  }

  return ParentInsertion(root, old_node, k_prime, new_node);
}


btree * ParentInsertion(btree * root, btree * left, double key, btree * right)
{
  btree * parent;
  parent = left->parent;
  if (parent == NULL){
    btree * new_nodez;
    new_nodez = (btree*)malloc(sizeof(btree));
    new_nodez->keys = (double*)malloc( (order - 1) * sizeof(double) );
    new_nodez->ptr_val = (void**)malloc( order * sizeof(void *) );
    new_nodez->is_leaf = false;
    new_nodez->no_of_keys = 1;
    new_nodez->parent = NULL;
    new_nodez->next = NULL;
    new_nodez->keys[0] = key;
    new_nodez->ptr_val[0] = left;
    new_nodez->ptr_val[1] = right;
    left->parent = new_nodez;
    right->parent = new_nodez;
    return new_nodez;
  }

  int left_index = 0;
  while (left_index <= parent->no_of_keys &&
    parent->ptr_val[left_index] != left)
    left_index++;

    if (parent->no_of_keys < order - 1){
      for (int iv = parent->no_of_keys; iv > left_index; iv--) {
        parent->ptr_val[iv + 1] = parent->ptr_val[iv];
        parent->keys[iv] = parent->keys[iv - 1];
      }
      parent->ptr_val[left_index + 1] = right;
      parent->keys[left_index] = key;
      parent->no_of_keys++;

      return root;
    }
    return NodeInsertionSPlit(root, parent, left_index, key, right);
}

//In this function we will find the values under a given key
//and print them to our output file
void Search(btree * root, double key)
{
	if(output.is_open())
  {

  key_value * r;
  int i = 0;
  btree * c;
  int ii = 0;
  btree * cc = root;
  if (cc == NULL)
    c = cc;
  else
  {
     /*In order to find the appropriate key_value record for our key_value
     we will go down from root to the leaf searching by the given key
     to find the leaf node thay contains the key */
    while (!cc->is_leaf)
    {
      ii = 0;
      while (ii < cc->no_of_keys)
      if (key >= cc->keys[ii]) ii++;
      else break;
      cc = (btree *)cc->ptr_val[ii];
    }
    c = cc;
  }

  if (c == NULL)
  r = NULL;
  else
  {
    for (i = 0; i < c->no_of_keys; i++)
    if (c->keys[i] == key) break;
    if (i == c->no_of_keys)
    r = NULL;
    else
    r = (key_value *)c->ptr_val[i];      //this is the record to which our given key refers
  }

  if (r == NULL)
    output<<"Null"<<endl;

  else
  {
    for (int i = 0; i < r->count; i++)
    {

      output << *(r->value[i]);
      if (i != r->count - 1)
          output<<",";
    }
    output << endl;
  }
}
else
cout<<"error";
}

/* we will find and print the keys and values within the given range
of keys (key_start and key_end)*/
void Search_range( btree * root, double key_start, double key_end)
{
	if(output.is_open())
  {
  int i;
  int array_size = 1000;
  double key_return[array_size];
  void * rtnd_pointers[array_size];

  int ic, num_found;
  num_found = 0;
  btree* n;

  int im = 0;
  btree * cm = root;
  if (cm == NULL)
  n = cm;
  else
  {
    while (!cm->is_leaf)
    {
      im = 0;
      while (im < cm->no_of_keys)
      {
      if (key_start >= cm->keys[im])
        im++;
      else
        break;
      }
    cm = (btree *)cm->ptr_val[im];
  }
  n= cm;
  }
  //We find the number of keys found in the specified range and store
  //in the var num_found
  if (n == NULL)
    num_found = 0;
  else
  {
    for (ic = 0; ic < n->no_of_keys && n->keys[ic] < key_start; ic++) ;
    if (ic == n->no_of_keys)
    num_found = 0;
    else{
      while (n != NULL) {
        for ( ; ic < n->no_of_keys && n->keys[ic] <= key_end; ic++) {
          key_return[num_found] = n->keys[ic];
          rtnd_pointers[num_found] = n->ptr_val[ic];
          num_found++;
        }
        n = (btree*)n->ptr_val[order - 1];
        ic = 0;
      }
    }
  }

if (!num_found)
  output<<"Null"<<endl;

else
{
  for (i = 0; i < num_found; i++) {
    for (int j = 0; j < (((key_value *)rtnd_pointers[i])->count); j++) {
      output << "(" << key_return[i] <<",";
      output << *(((key_value *)rtnd_pointers[i])->value[j]) << ")";
      if (j != (((key_value *)rtnd_pointers[i])->count) - 1)
      output << ",";
    }
    if (i != num_found - 1)
    output << ",";
  }
  output<<endl;
  }
}
}



// MAIN
int main(int argc, char *argv[] ) {

	btree* root = NULL;
	//ifstream input(argv[1]);
	std::ifstream infile("input_bplus.txt");
	string str, cmd, value1;
	string insert_str = string("Insert");
	string search_str = string("Search");
	unsigned int i;
	string temp;
	int flag = 1;
	float key;
	int index = 0;
	if (infile.is_open())
	{
		infile >> str;  //first line gives the order
		::order = (int)::atof(str.c_str());
		int x = 0;
		while (!infile.eof())
		{
			flag = 1;
			std::getline(infile,str);
			cmd = str.substr(0, 6); // insert and search are both 6 letters long
			if (cmd.compare(insert_str) == 0)
			{
				string ss = "";
				string tt = "";
				int m = 7;
				while (str[m] != ',') {
					ss += str[m];	// ss eventually carries the key
					m++;
				}
				float kk = (float)::atof(ss.c_str());
				m++;
				while (str[m] != ')') {
					tt += str[m]; // ss eventually carries the value
					m++;
				}
				root = Insert(root, kk, tt);
			}
			else
				if (cmd.compare(search_str) == 0)
				{
					string ss1 = "";	//key1
					string ss2 = "";	//key2
					int m = 7;
					while (str[m] != ',') {
						if (str[m] == ')') {	// if we find ) then we break and get just key1
							flag = 0;
							break;
						}
						ss1 += str[m];	// ss eventually carries the key1
						m++;
					}
					float kk1 = (float)::atof(ss1.c_str());
					float kk2;
					m++;
					if (flag) {	// set flag means we didnt find ) and terminated at ,
						//so we search for key2
						while (str[m] != ')') {
							ss2 += str[m]; // ss eventually carries the key2
							m++;
						}
						kk2 = (float)::atof(ss2.c_str());
					}
					if (flag) {
						Search_range(root, kk1, kk2);
					}
					else {
						Search(root, kk1);
					}
				}
		}
	}



	infile.close();


	return 1;
}
