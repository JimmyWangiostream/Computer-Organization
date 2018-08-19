#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAXTRACENUM 3000000
typedef struct cache_content{
	unsigned long tag;
	int valid;
	int used_turn;
};


int index_width;
int way_width;
int block_width;

unsigned long turn=0;
unsigned long num_of_trace=0;
unsigned long MISS=0;
unsigned long HIT=0;

long cache_size;
long block_size;
int associativity;
int replace_method;

int hit_trace[MAXTRACENUM];
int hit_trace_num=-1;

int check_hitfull(){
	if(hit_trace_num==MAXTRACENUM)
		return 1;
	else
		return 0;
}
void hit_push(int data){
	if(!check_hitfull()){
		hit_trace_num++;
		hit_trace[hit_trace_num]=data;
	}
	else
		printf("Trace num is out of range 3000000\n");
}

int miss[MAXTRACENUM];
int miss_trace_num=-1;
int check_missfull(){
	if(miss_trace_num==MAXTRACENUM)
		return 1;
	else
		return 0;
}
void miss_push(int data){
	if(!check_missfull()){
		miss_trace_num++;
		miss[miss_trace_num]=data;
	}
	else
		printf("Trace num is out of range 3000000\n");
}


int calladdr(unsigned long addr,unsigned long *tag,unsigned long *index,unsigned long *offset)
{
	unsigned long tmp;
	*tag=addr>>(block_width+index_width);
	tmp = 0;
	for(int i=0;i<index_width;i++){
		tmp <<=1;
		tmp |= 0x01;
	}
	*index =addr >> block_width & tmp;
	tmp=0;
	for(int i=0;i<block_width;i++){
		tmp <<=1;
		tmp |= 0x01;
	}
	*offset = addr&tmp;
}

int access_cache(struct cache_content cache[][1<<index_width],unsigned long addr)
{
	unsigned long offset;
	unsigned long tag;
	unsigned long index;
	calladdr(addr,&tag,&index,&offset);
	int isHit = 0;
	
	for(int i=0;i< 1<<way_width;i++){
		if(cache[i][index].valid == 1 && cache[i][index].tag ==tag){
			if(replace_method ==1 )
				cache[i][index].used_turn = turn;
			isHit=1;
		}
	}

	if(isHit){
		HIT++;
		hit_push(num_of_trace);
	}
	else{
		MISS++;
		int replace=0;
		for(int i=0;i <1<<way_width;i++)
			if(cache[i][index].used_turn < cache[replace][index].used_turn) replace=i;
		cache[replace][index].valid=1;
		cache[replace][index].tag=tag;
		cache[replace][index].used_turn=turn;
		miss_push(num_of_trace);
	}
	turn++;
}



int main(int argc,char* argv[]){
	char input_file[35],output_file[35];

	if(strcmp(argv[1],"-input")==0 && strcmp(argv[3],"-output")==0){
		strcpy(input_file,argv[2]);
		strcpy(output_file,argv[4]);
	}
	else{
		strcpy(input_file,argv[4]);
		strcpy(output_file,argv[2]);
	}

	char buffer[13];
	int address;
	FILE *fpt;
	fpt=fopen(input_file,"r");
	
	fgets(&buffer[0],10,fpt);
	cache_size=atoi(buffer);
	fgets(&buffer[0],10,fpt);
	block_size=atoi(buffer);
	fgets(&buffer[0],10,fpt);
	associativity=atoi(buffer);
	fgets(&buffer[0],10,fpt);
	replace_method=atoi(buffer);
	
	if(associativity==0)
	{//direct cache size is KB so width +10
		index_width=(int)log2((double)cache_size)+10-(int)log2((double)block_size);
		way_width=0;
		block_width=(int)log2((double)block_size);
	}
	else if(associativity==1)
	{//four_way
		index_width=(int)log2((double)cache_size)+10-(int)log2((double)block_size)-2;
		way_width=2;
		block_width=(int)log2((double)block_size);
	}
	else if(associativity==2)
	{//full way
		index_width=0;
		way_width=(int)log2((double)cache_size)+10-(int)log2((double)block_size);
		block_width=(int)log2((double)block_size);
	}
	//malloc cache with cache_content[way_width][index_width]
	struct cache_content **cache = (struct cache_content**)malloc((1<<way_width)*sizeof(struct cache_content*));
	for(int i=0;i<(1<<way_width);i++)
		cache[i]=(struct cache_content*)malloc((1<<index_width)*sizeof(struct cache_content));

	for(int i=0;i < (1<<index_width);i++){
		for(int j=0;j< (1<<way_width);j++){
			cache[j][i].valid=0;
			cache[j][i].used_turn=-1;
			cache[j][i].tag = 0;
		}
	}
	//access
	while(fgets(&buffer[0],13,fpt)){
		num_of_trace++;
		sscanf(buffer,"0x%x",&address);
		access_cache(cache,address);
	}
	//printf("%d\n",num_of_trace);
	//output file
	fpt=fopen(output_file,"w");
	fprintf(fpt,"Hits instructions: ");
	int i =0;
	while(i<=hit_trace_num)
	{
		if(i!=hit_trace_num) fprintf(fpt,"%d,",hit_trace[i]);
		else fprintf(fpt,"%d",hit_trace[i]);
		i++;
	}
	fprintf(fpt,"\nMisses instructions: ");
	i = 0;
	while(i<=miss_trace_num)
	{
		if(i!=miss_trace_num) fprintf(fpt,"%d,",miss[i]);
		else fprintf(fpt,"%d",miss[i]);
		i++;
	}
	fprintf(fpt,"\nMiss rate: %lf\n",(double)MISS/(HIT+MISS));
	fclose(fpt);
	return 0;
}
	





