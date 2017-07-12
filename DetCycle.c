#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


//BRIHADISWARAN NANDIKARAN
//BT12CSE100

typedef struct e_node_tag
{
    int v;
    struct e_node_tag* next;
}e_node;
typedef struct v_node_tag
{
    int v;
    struct v_node_tag* down;
    e_node* next;
}v_node;
typedef struct visited_node_tag
{
    int v;
    struct visited_node_tag* next;
}visited_node;
typedef struct q_node_tag
{
    int v;
    struct q_node_tag* next;
}q_node;

bool IfVertExist(v_node** root, int v);

void PushInVisited(visited_node** root, int v)
{
    visited_node*ptr=*root;
    visited_node* nptr=(visited_node*)malloc(sizeof(visited_node));
    nptr->v=v;
    nptr->next=NULL;
    if(ptr==NULL)
    {
        *root=nptr;
    }
    else
    {
        nptr->next=ptr->next;
        ptr->next=nptr;
    }
}

void Pop(q_node** root, int* dptr)
{
    q_node*ptr=*root;
    q_node*prev=*root;
    if(ptr!=NULL)
    {
        ptr=ptr->next;
        *dptr=prev->v;
        free(prev);
        *root=ptr;
    }
}

void Push(q_node** qroot, v_node* vroot, int v)
{
    q_node* ptr=*qroot;
    int i=0;
    q_node* n1ptr;
    q_node* prev;
    v_node* vptr=vroot;
    IfVertExist(&vptr,v);
    e_node* eptr;
    for(eptr=vptr->next;eptr!=NULL;eptr=eptr->next)
    {
        q_node* nptr=(q_node*)malloc(sizeof(q_node));
        nptr->v=eptr->v;
        nptr->next=NULL;
        if(i==0)
        {
            n1ptr=nptr;
            prev=nptr;
        }
        else
        {
            prev->next=nptr;
            prev=nptr;
        }
        i=1;
    }
    if(ptr==NULL)
    {
        *qroot=n1ptr;
    }
    else
    {
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=n1ptr;
    }
}

bool IsVisited(visited_node* root, int v)
{
    bool ret_val=0;
    visited_node* ptr=root;
    if(ptr!=NULL)
    {
        while(ret_val==0&&ptr!=NULL)
        {
            if(ptr->v==v)
            {
                ret_val=1;
            }
            else
            {
                ptr=ptr->next;
            }
        }
    }
    return ret_val;
}

bool IfVertExist(v_node** root, int v)
{
    bool ret_val=0;
    v_node* ptr=*root;
    if(ptr!=NULL)
    {
        while(ptr!=NULL&&ret_val==0)
        {
            if(ptr->v==v)
            {
                ret_val=1;
            }
            else
            {
                ptr=ptr->down;
            }
        }
        *root=ptr;
    }
    return ret_val;
}

v_node* InsertAdjList(v_node* root, int v1, int v2)
{
    v_node* ptr=root;
    if(ptr==NULL)
    {
        v_node* vnptr=(v_node*)malloc(sizeof(v_node));
        vnptr->v=v1;
        e_node* enptr=(e_node*)malloc(sizeof(e_node));
        enptr->v=v2;
        enptr->next=NULL;
        vnptr->next=enptr;
        v_node* vnptr2=(v_node*)malloc(sizeof(v_node));
        vnptr2->v=v2;
        vnptr2->down=NULL;
        vnptr2->next=NULL;
        vnptr->down=vnptr2;
        root=vnptr;
    }
    else
    {
        v_node* lptr=root;
        if(IfVertExist(&lptr,v1))
        {
            e_node* enptr=(e_node*)malloc(sizeof(e_node));
            enptr->v=v2;
            enptr->next=lptr->next;
            lptr->next=enptr;
            lptr=root;
            if(!(IfVertExist(&lptr,v2)))
            {
                v_node* vnptr=(v_node*)malloc(sizeof(v_node));
                vnptr->v=v2;
                vnptr->next=NULL;
                vnptr->down=ptr->down;
                ptr->down=vnptr;
            }
        }
        else
        {
            v_node* vnptr=(v_node*)malloc(sizeof(v_node));
            vnptr->v=v1;
            vnptr->down=ptr->down;
            ptr->down=vnptr;
            e_node* enptr=(e_node*)malloc(sizeof(e_node));
            enptr->v=v2;
            enptr->next=NULL;
            vnptr->next=enptr;
            lptr=root;
            if(!(IfVertExist(&lptr,v2)))
            {
                v_node* vnptr2=(v_node*)malloc(sizeof(v_node));
                vnptr2->v=v2;
                vnptr2->next=NULL;
                vnptr2->down=ptr->down;
                ptr->down=vnptr2;
            }
        }
    }
    return root;
}

void BreadthFirstTraversal(v_node* root, int v)
{
    bool cyclepresent=0;
    v_node* ptr=root;
    if(ptr!=NULL)
    {
        q_node* qroot=NULL;
        visited_node* vroot=NULL;
        if(IfVertExist(&ptr,v))
        {
            Push(&qroot,root,v);
        }
        ptr=root;
        PushInVisited(&vroot,v);
        Pop(&qroot, &v);
        while(qroot!=NULL&&cyclepresent==0)
        {
            //printf("\nhello\n");
            if(!IsVisited(vroot,v))
            {
                PushInVisited(&vroot, v);
                Push(&qroot,root,v);
            }
            else
            {
                cyclepresent=1;
                printf("\ncycle present\n");
            }
        }
        if(cyclepresent==0)
        {
            printf("\nno cycle present\n");
        }
    }
}

v_node* InitAdjList(v_node* root)
{
    v_node* ptr=root;
    int v1, v2;
    FILE* fptr=fopen("data.txt","r");
    fseek(fptr,0,SEEK_END);
    unsigned int len=ftell(fptr);
    if(len>0)
    {
        rewind(fptr);
        while(!feof(fptr))
        {
            fscanf(fptr,"%d",&v1);
            fscanf(fptr,"%d",&v2);
            ptr=InsertAdjList(ptr,v1,v2);
        }
    }
    fclose(fptr);
    return ptr;
}

void PrintAdjList(v_node* root)
{
    v_node* vptr=root;
    e_node* eptr;
    if(vptr!=NULL)
    {
        for(vptr=root;vptr!=NULL;vptr=vptr->down)
        {
            printf("\n%d : ",vptr->v);
            for(eptr=vptr->next;eptr!=NULL;eptr=eptr->next)
            {
                printf("%d, ",eptr->v);
            }
        }
    }
}

void main()
{
    v_node* root=NULL;
    root=InitAdjList(root);
    //PrintAdjList(root);
    BreadthFirstTraversal(root,1);
}
