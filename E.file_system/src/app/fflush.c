#include<fs.h>
#include<printf.h>
#include <stddef.h>
#include <utils.h>
extern unsigned char _start_;
extern int cd_rem;
int fflush(FILE *stream){
	/*Todo List*/	
	FILE * real_addr = (&_start_ + (unsigned int)stream);
	int num = 0;
	int block = 0, phy_block=0;


 
	if(real_addr->_bufsize%4096){
		num = (real_addr->_bufsize/4096)+1;	
	}else{
		num = real_addr->_bufsize/4096;
	}
        
	if(real_addr->_bufsize%512){
		phy_block = (real_addr->_bufsize/512)+1;	
	}else{
		phy_block = real_addr->_bufsize/512;
	}	

	
	/*send to file system*/
	
	fatdir_t* tmp = symbolic_fs_array[real_addr->_tmpname].file_info->addr.log_addr + symbolic_fs_array[real_addr->_tmpname].file_info->num_fatdir*32;
		
	tmp->size = (real_addr->_ptr - real_addr->_base);
	
	int write_size = 0;
	if(tmp->size%512){
		write_size = (tmp->size/512)+1;	
	}else{
		write_size = tmp->size/512;
	}	

	
	
	//sdTransferBlocks ((char*)symbolic_fs_array[real_addr->_tmpname].file_info->directory, write_size, (unsigned long)real_addr->_base , 1);

	//printf("fclose:%d %d %d %x\n\r",real_addr->_bufsize,phy_block,write_size,symbolic_fs_array[real_addr->_tmpname].file_info->directory);
	
	block = (tmp-> size)/(512*phy_block);
                
	/*user page reset*/
	symbolic_fs_array[real_addr->_tmpname].file_info->dir_record.size =tmp->size;		
	user_dir((char*)current_page);
	//printf("block:%d,%x\n\r",block,((unsigned long*)symbolic_fs_array[real_addr->_tmpname].file_info->addr.phy_addr)[block]);
	/*device*/
	//sdTransferBlocks (((unsigned long*)symbolic_fs_array[real_addr->_tmpname].file_info->addr.phy_addr)[block], 1, (char*)symbolic_fs_array[real_addr->_tmpname].file_info->addr.log_addr , 1);
	
	struct fs_unit* return_fs = fs_type_support(partition[cd_rem].type);
        if(return_fs){
		unsigned long* clu = (unsigned long*)symbolic_fs_array[real_addr->_tmpname].file_info->director;
		char* phy = (char*)clu[real_addr->_cnt];
		char* log = (char*)real_addr->_base;
		int* result = bl_init(&_start_+ (unsigned int)return_fs->addr_writebuf, &partition[cd_rem],phy, write_size, log);

		result = bl_init(&_start_+ (unsigned int)return_fs->addr_writedir, ((unsigned long*)symbolic_fs_array[real_addr->_tmpname].file_info->addr.phy_addr)[block], (char*)symbolic_fs_array[real_addr->_tmpname].file_info->addr.log_addr);
	}else{
		printf("Not support %x type in File system",partition[cd_rem].type);
		return NULL;
	}

	
	
	free_page(real_addr->_base,num);
	symbolic_fs_array[real_addr->_tmpname].open = 0;/*soft symbolic*/ 
	symbolic_fs_array[real_addr->_tmpname].file_info->director = NULL;
	stream = NULL;	
	return 0;

}
