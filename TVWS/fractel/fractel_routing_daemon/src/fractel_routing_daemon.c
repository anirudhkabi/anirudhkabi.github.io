#include<stdio.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

struct tree{
	unsigned int key;
	unsigned int parent;
	int num_children;
	struct tree **children;
};

char *ip_long_to_string(unsigned long ip)
{
        unsigned long d1,d2,d3,d4;
        unsigned long ip1;
        char *k1 = (char*)malloc(sizeof(char)*16);
        char *k2 = (char *)malloc(sizeof(char)*4);
        d1 = 4278190080;
        d2 = 16711680;
        d3 = 65280;
        d4 = 255;
        ip1 = ip & d1; 
        ip1 = ip1>>24;
        sprintf(k2,"%d",ip1);
        strcpy(k1,k2);
        strcat(k1,".");
        ip1 = ip & d2; 
        ip1 = ip1>>16;
        sprintf(k2,"%d",ip1);
        strcat(k1,k2);
        strcat(k1,".");
        ip1 = ip & d3; 
        ip1 = ip1>>8;
        sprintf(k2,"%d",ip1);
        strcat(k1,k2);
        strcat(k1,".");
        ip1 = ip & d4; 
        sprintf(k2,"%d",ip1);
        strcat(k1,k2);
        printf("%s\n",k1);
        free(k2);
        return k1;
}

int add_to_routing_table(unsigned int host_i, unsigned int gw_i)
{
	char buff[255];
	char *host, *gw;
	int retval;
	host = ip_long_to_string(host_i);
	gw = ip_long_to_string(gw_i);
	//first delete the entry and then add it
	sprintf(buff,"route del %s",host);
	retval = system(buff);
	printf("deleted -  %s\n", buff);
	sprintf(buff,"route add -host %s gw %s ath0",host,gw);
	retval = system(buff);
	printf("deleted -  %s\n", buff);
	//free the memory before returning
	free(host);
	free(gw);
	return retval;
}
int add_parent_routing_entries(struct tree *self,unsigned int gw, unsigned int node_key)
{
	//add children first
	int i = 0;
	printf("adding parent gw %u\n", gw);
	if (self->key == node_key)
		return 1;
	for (i=0; i < self->num_children; i++)
	{
		if(self->children[i]->key != node_key)
		{
			add_parent_routing_entries(self->children[i], gw, node_key);
		}
	}
	//add entry for self
	if (self->key != gw)
		return add_to_routing_table(self->key, gw);

}

int add_children_routing_entries(struct tree *self, unsigned int gw_self, unsigned int gw)
{
	//first add children, then add self
	int i = 0;
	printf("adding children gw %u\n", gw);
	for (i = 0; i < self->num_children; i++)
	{
		if (gw_self)
		{
			add_children_routing_entries(self->children[i], 0, self->key);
		}else {
			add_children_routing_entries(self->children[i], 0,gw);
		}	
	}
	//add self, if self is not gw
	if (!gw_self)
		return add_to_routing_table(self->key, gw);
}

struct tree* get_pointer(unsigned int key, struct tree* node)
{
	struct tree* ret_val = NULL;
	int i = 0;
	printf("get_pointer called\n");
	if (node == NULL)
		return NULL;
	else if (node->key == key)
		return node;
	else {
		for(i=0; i < node->num_children; i++)
		{
			ret_val = get_pointer(key, node->children[i]);
			if (ret_val != NULL)
				return ret_val;
		}
	} 
	return NULL;
}


int add_routing_entries(unsigned int self_key, struct tree* root)
{
	int i =0;
	struct tree* self = (struct tree *)get_pointer(self_key, root);
	printf("add_routing_entries called %u\n", self_key);

	if (self == NULL)
	{
		printf("Serious error!! -- dude what have you done:\(\n");
		return 1;
	}
	//enter routing entries for each of the children
	for (i = 0; i < self->num_children; i++)
	{
		add_children_routing_entries(self->children[i],1,0);
	}
	//enter routing entry for parent
	add_parent_routing_entries(root,self->parent, self_key);
}

void add_to_tree(unsigned int parent_key, unsigned int child, struct tree **root)
{

	struct tree *parent = get_pointer(parent_key, *root);
	printf("adding to tree %u %u\n", parent_key, child);
	if(parent == NULL)
	{
		printf("constructing tree\n");
		//construct tree
		parent = (struct tree *)malloc(sizeof(struct tree));
		parent->key = parent_key;
		parent->parent = parent_key;
		parent->num_children = child;
		parent->num_children = 1;
		parent->children = (struct tree **)malloc(sizeof(struct tree*)*10);
		parent->children[0] = (struct tree *)malloc(sizeof(struct tree));
		parent->children[0]->key = child;
		parent->children[0]->parent = parent_key;
		parent->children[0]->num_children = 0;
		parent->children[0]->children = (struct tree **)malloc(sizeof(struct tree*)*10);
		*root = parent;
	}else{
		printf("adding children no %d\n",parent->num_children);
		parent->children[parent->num_children] = (struct tree *)malloc(sizeof(struct tree));
		parent->children[parent->num_children]->key = child;
		parent->children[parent->num_children]->parent = parent_key;
		parent->children[parent->num_children]->num_children = 0;
		parent->children[parent->num_children]->children = (struct tree **)malloc(sizeof(struct tree*)*10);
	  	parent->num_children++;		
	}	
}

void delete_tree(struct tree *node)
{
	//let the children delete their children, then delete the children
	int i = 0;
	printf("delete tree called\n");
	for (i = 0; i < node->num_children; i++)
	{
		delete_tree(node->children[i]);	
	}
	for (i = 0; i < node->num_children; i++)
	{
		free(node->children[i]);
	}
	free(node->children);
}
int main(int argc, char **argv)
{
	FILE *fdproc, *fduser;	
	char buff[255];
	int bytesread=0;
	int status = 0;
	unsigned int ip1=0, ip2=0, self_ip=0, count=0, loop_counter=0, routing_tree_length = 0, temp = 0;
	unsigned int **routing_entries = NULL;
	struct tree* root = NULL;
	
	if (argc==2)
	{
		if ((fduser=fopen(argv[1], "w")) == NULL) {
			printf("Could not open output file\n");
		}

	}
	else
	{
		if ((fduser=stdout))//open("log_fractel.log", O_WRONLY | O_CREAT | O_TRUNC ))==-1) {
		{
			printf("writing to standard output\n");
		}
	}
	while (1)
	{
		if ((fdproc=fopen("/proc/net/madwifi/ath0/fractel_routing_entries", "r"))!=NULL) {
			if ((bytesread=fread(buff, 1, 255, fdproc))!=0)
			{	
				if (fwrite(buff, 1, bytesread, fduser)==0)
				{
					printf("Problem while writing\n");
					break;
				}
				sscanf(buff,"%u %u %u", &temp, &ip1,&ip2);
				if (ip1 == 11111111)
				{
					//start of routing tree
					routing_tree_length = ip2;
					count = ip2;
					count++;
				}else if (count == routing_tree_length + 1)
				{
					printf("storing subsequent ips %u %u\n",count,routing_tree_length);
					//first ip
					self_ip = ip1;
					count--;
				}
				else {
					add_to_tree(ip1,ip2,&root);
					count--;
					if (count == 0)
					{
						
						int retval = system("/scripts/del_routes.sh");
						if(retval != 0)
							printf("could not delete previous routing entries\n");
						printf("executed del_routes.sh %d\n",retval);
						add_routing_entries(self_ip, root);
						delete_tree(root);
						free(root);
						root = NULL;
					}
				}	
			}
			else
			{	
				sleep(1);
			}	
			
		}
		else
		{	
			sleep(1);
			//printf("no data : %s\n",buff);			
			//break;
		}
		fclose(fdproc);
	}
	fclose(fduser);
	printf("returnin\n");
	return 0;
}
