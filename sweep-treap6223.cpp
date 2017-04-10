/* This is a c++ program to implement the sweepline algorithm to find the 2D closest pair in a given plane
** The program is built and compiled on visual studio 2012.
** The program uses linked list to store the initial values and uses quick sort to sort the 'n' given points according to the  'X' coordinate of the point.
** Duplicates are removed --> the points with similar X & Y coordinates are removed.
** The nodes are then moved to the treaps in a relavent way
** finally the values of the closest pair and the distance are printed.

** The source code for sorting the input values are taken from :http://www.geeksforgeeks.org/quicksort-for-linked-list/
** The source code for the implementation of the treaps are taken from : http://www.sanfoundry.com/cpp-program-implement-treap/
** The source and algorithm inspiration was taken from the description of sweepline algorithm for closest pair from : https://www.topcoder.com/community/data-science/data-science-tutorials/line-sweep-algorithms/

** The program is taking the input from the user on console-> specify the file name in "main()" function
** Also a provision has been made to take input from the file as well. Please specify the file in the "main()" function.
*/

#pragma comment(linker, "/STACK:2000000")
#pragma comment(linker, "/HEAP:2000000")

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include <math.h>

using namespace std;

int dmin,Rx1,Ry1,Rx2,Ry2; // result values.
int cx, cy; //current x and y values of a point.
int node_count; //to keep count of the nodes
int rotrc=0, rotlc=0; //counter variables for tracking rotations
int ctr=0;	//counter variable for max nodes in the treap

typedef struct treapnode *tree; 

/* a node of the doubly linked list */
struct node
{
    int x;
	int y;
    struct node *next;
    struct node *prev;
};

/*Treap Node declaration*/

struct treapnode
{
    int key_y, key_x, fix;
    tree left, right;
};
tree nullnode, root;

/*
 * Initializing treap node 
 */
void treap_init()
{
    nullnode = new treapnode;
    nullnode->left = nullnode->right = nullnode;
    root = nullnode;
}

/* function to count the nodes in the tree*/

int counttreap(tree &t)
{
	int c=1;
 
	if (t == nullnode)
        c=0;
	else if( t->left == nullnode && t->right == nullnode)
	{
		c=1;
	}
    else
    {
        c += counttreap(t->left);
        c += counttreap(t->right);
    }
	return c;
}

/*function to create a new random node*/

int Randint(int a, int b)
{

	int rand1= a + rand() % (b-a+1);
	return rand1;
}

/*
 * Left Rotation
 */
void rotate_l(tree &k1)
{
    tree k2;
    k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1 = k2;
	rotlc++;
}

/*
 * Right Rotation
 */
void rotate_r(tree &k1)
{
    tree k2;
    k2 = k1->left;
    k1->left = k2->right;
    k2->right = k1;
    k1 = k2;
	rotrc++;
}

void insert_treap(tree &t, int x, int y)
{
    if (t == nullnode)
    {
        t = new treapnode;
        t->left = t->right = nullnode;
        t->key_x = x;
		t->key_y = y;
		t->fix = Randint(0,100);
	}
    else
    {
		if (t->key_x == x && t->key_y == y )
        {
            return;
		}
        else
        {
            if (y < t->key_y)
            {
				insert_treap(t->left, x, y);
                if (t->left->fix > t->fix)
				{
                    rotate_r(t);
				}
            }
            else
            {
				insert_treap(t->right, x, y);
                if (t->right->fix > t->fix)
				{
					 rotate_l(t);
				}
            }
        }   
    }
}
 

/* A function to swap two elements */
void swap ( int* a, int* b, int *c, int *d )
{   int tx = *a;
	int ty = *c; 
	*a = *b; 
	*c = *d; 
	*b = tx;
	*d = ty;
}
 
// A function to find last node of linked list to sort
struct node *lastNode(node *root)
{
    while (root && root->next)
        root = root->next;
    return root;
}
 
/* Considers last element as pivot, places the pivot element at its
   correct position in sorted array, and places all smaller (smaller than
   pivot) to left of pivot and all greater elements to right of pivot */
node* partition(node *l, node *h)
{
    // set pivot as h element
	int d  = h->x;

    node *i = l->prev;
 
    for (node *j = l; j != h; j = j->next)
    {
		if (j->x <= d)
        {
            i = (i == NULL)? l : i->next;
 
            swap(&(i->x), &(j->x), &(i->y), &(j->y));
        }
    }
    i = (i == NULL)? l : i->next; 
    swap(&(i->x), &(h->x), &(i->y), &(h->y));
    return i;
}
 
/* A recursive implementation of quicksort for linked list */
void _quickSort(struct node* l, struct node *h)
{
    if (h != NULL && l != h && l != h->next)
    {
        struct node *p = partition(l, h);
        _quickSort(l, p->prev);
        _quickSort(p->next, h);
    }
}
 
// The main function to sort a linked list. It mainly calls _quickSort()
void quickSort(struct node *head)
{
    // Find last node
    struct node *h = lastNode(head);
 
    // Call the recursive QuickSort
    _quickSort(head, h);
}
 
/* A function to print contents of arr
void printList(struct node *head)
{
    while (head)
    {
		cout <<"X-> " << head->x <<"y-> " << head->y << "  "<<"\n";
        head = head->next;
    }
    cout << endl;
}
 */

/* Function to insert a node at the beginging of the Doubly Linked List */
void push(struct node** head_ref, int datax, int datay)
{
    struct node* new_node = new node;     /* allocate node */
	new_node->x  = datax;
	new_node->y = datay;
 
    /* since we are adding at the begining, prev is always NULL */
    new_node->prev = NULL;
 
    /* link the old list off the new node */
    new_node->next = (*head_ref);
 
    /* change prev of head node to new node */
    if ((*head_ref) !=  NULL)  (*head_ref)->prev = new_node ;
 
    /* move the head to point to the new node */
    (*head_ref)    = new_node;
}
 
/* The function removes duplicates from a sorted list */
void removeDuplicates(struct node* head)
{
    struct node* c = head;
 
    /* Pointer to store the next pointer of a node to be deleted*/
    struct node* next_next; 
    if (c == NULL) 
       return; 
    while (c->next != NULL) 
    {
       /* Compare current node with next node */
		if (c->x == c->next->x && c->y == c->next->y) 
       {
                       
           next_next = c->next->next;
           c->next = next_next;  
       }
       else 
       {
          c = c->next; 
       }
    }
}

int count_node(struct node *head)
{
     int i=0;
     struct node* temp=head;
	 while(temp!=NULL)
     {
          i++;
          temp=temp->next;
     }
	 node_count=i;

	 cout<<"nodes = "<<node_count;
     return 0;
}

/* function to find 
the distance between the 2 points for sweepline*/

int distance(int x1, int y1, int x2, int y2)
{
	double Dx=(x2-x1);
	double Dy=y2-y1;
	int dist= pow(Dx,2)+pow(Dy,2);
	return dist;
}
/*
* Remove Element from Treap
 */
void treap_remove(tree &t, int x, int y)
{
	int dist=cx-dmin;
    if (t == nullnode)
        return;
    else
    {
		if (t->key_x < dist)
		{
			if (t->left == nullnode && t->right == nullnode)
		    {

	            delete t;
	            t = nullnode;
	        }
	        else if (t->left == nullnode)
	        {
	            tree tmp = t;
	            t = t->right;
	            delete tmp;
	        }
	        else if (t->right == nullnode)
	        {
	            tree tmp = t;
			    t = t->left;
			    delete tmp;
			}
		    else
		    {
		        if (t->left->fix < t->right->fix)
		        {
		            rotate_r(t);
					treap_remove(t->left, x, y);
		        }
		        else
		        {
		            rotate_l(t);
					treap_remove(t->right, x, y);
		        }
			}
		}
    }
}

/* function to find the 
minimum distance between the points*/

void sweep_find(int dx1, int dy1)
{
	int yd=cy-dmin;			// y down(current y-dmin)
	int yu=cy+dmin;			// y up )current y+dmin)
	if( dy1<yu && dy1>yd)
	{
		int d_loc=distance(dx1, dy1, cx, cy);
		if(d_loc<dmin)
		{
			dmin=d_loc;
			Rx1=dx1; Ry1=dy1; Rx2=cx; Ry2=cy;
		}

	}
	else
	{
		return;
	}
}

 /*
 * Inorder Travesal of Treapto find
	the next point to calculate the distance between p2 and p1
 */
void inorder_find(tree &tempin)
{
	int dx, dy;
	if (tempin == nullnode)
        return;
	if (tempin != NULL)
    {
		inorder_find(tempin->left);
		dx=tempin->key_x; 
		dy=tempin->key_y;
		sweep_find(dx,dy);
		inorder_find(tempin->right);
    }
}

/*
 * Inorder Travesal of Treap to remove items
	from the treap for next iteration
 */
void inorder_remove(tree &tempinr)
{
	if (tempinr == nullnode)
        return;
	if (tempinr != NULL)
    {
		inorder_remove(tempinr->left);
		int dx=tempinr->key_x; int dy=tempinr->key_y;
		treap_remove(tempinr, dx,dy);
		inorder_remove(tempinr->right);
    }
}


/* Sweepline initialization function*/

void sweepline_init(struct node *head)
{
	int dx1, dx2, dy1, dy2;
	struct node* tempd=head;
	dx1=tempd->x;
	dy1=tempd->y;
	dx2=tempd->next->x;
	dy2=tempd->next->y;
	dmin=distance(dx1,dy1,dx2,dy2);
	Rx1=dx1; 
	Rx2=dx2;
	Ry1=dy1;
	Ry2=dy2;
}

/*sweepline_treap Creation function
	the function creates a treap and inserts the values of concern into the treap.
*/

void sweepline_insert(struct node *head)
{
	int dx1, dy1;
	int d_pro;
	struct node* frtemp = head->next->next;
	struct node* tempb = head;
	cx=frtemp->x; 
	cy=frtemp->y;
	while(frtemp->next!=NULL)
	{
		while(tempb->next!=frtemp->next)
		{
			dx1=tempb->x; 
			dy1=tempb->y;
			d_pro=cx-dmin;
			if(dx1>=d_pro)
			{
				while(tempb->next!=frtemp->next)
				{
					insert_treap(root, tempb->x, tempb->y);
					tempb = tempb->next;
				}
			}
			else
			{
				tempb = tempb->next;
			}
		}
		inorder_find(root);
		inorder_remove(root);
		int c = counttreap(root);
		if(c > ctr)
		{
			ctr=c;
		}
		frtemp = frtemp->next;
		cx=frtemp->x;
		cy=frtemp->y;
		
	}
}

/*driver program to test above function */

int main()
{
    struct node *a = NULL;
	int n1=0, x1, y1;
	/*ifstream fileread;   //input from a file
	fileread.open("input.txt");
	fileread>>n1;					//reading number of points
	while(fileread>>x1>>y1)	
	{
		push(&a, x1, y1);			// reading coordinate values of the point
	}*/
	// input if taken from console
	
	cin>>n1;
	for(int i=0;i<n1;i++)
	{
		cin>>x1>>y1;
		push(&a, x1, y1);
	}
	
	cout<< "number of points:"<<n1<<"\n";
	cout << "Applying quicksort on"<<n1<<"points \n";
	clock_t begin = clock();
    quickSort(a);
	clock_t end = clock();

	double tot_t = (double)(end-begin)/CLOCKS_PER_SEC;
	
	cout<<"duplicate removal process\n";
	removeDuplicates(a);
	cout<<"Sorting time is: "<<tot_t<<"\n";
	clock_t sweep_begin = clock();
	sweepline_init(a);
	treap_init();
	sweepline_insert(a);
	clock_t sweep_end = clock();
	double sweep_t= (double)(sweep_end-sweep_begin)/CLOCKS_PER_SEC;
	cout<<"\nMax BST-Treaps nodes are: \t"<<ctr<<endl;
	cout<<"\nsweeptime is:\t"<<sweep_t<<endl;
	cout<<"Right Rotations:\t"<<rotrc<<endl;
	cout<<"Left Rotations:\t"<<rotlc<<endl;
	cout<<"closest pair distance: \t"<< dmin <<endl;
	cout<<"closest pair points are\t"<<"P1:("<<Rx1<<","<<Ry1<<")\t"<<"P2:("<<Rx2<<","<<Ry2<<")"<<endl;
//	fileread.close();
	return 0;
}
