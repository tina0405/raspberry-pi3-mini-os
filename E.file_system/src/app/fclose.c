#include<fs.h>
#include<printf.h>
#include <stddef.h>
extern unsigned char _start_;

int fclose(FILE *stream){
		

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
	sdTransferBlocks ((char*)symbolic_fs_array[real_addr->_tmpname].file_info->directory, phy_block, (unsigned long)real_addr->_base , 1);
	fatdir_t* tmp = symbolic_fs_array[real_addr->_tmpname].file_info->addr.log_addr + symbolic_fs_array[real_addr->_tmpname].file_info->num_fatdir*32;
		
	tmp->size = (real_addr->_ptr - real_addr->_base);
	block = (tmp-> size)/(512*phy_block);
                
	/*user page reset*/
	symbolic_fs_array[real_addr->_tmpname].file_info->dir_record.size =tmp->size;		
	user_dir((char*)current_page);
	//printf("block:%d,%x\n\r",block,((unsigned long*)symbolic_fs_array[real_addr->_tmpname].file_info->addr.phy_addr)[block]);
	/*device*/
	sdTransferBlocks (((unsigned long*)symbolic_fs_array[real_addr->_tmpname].file_info->addr.phy_addr)[block], phy_block, (char*)symbolic_fs_array[real_addr->_tmpname].file_info->addr.log_addr , 1);
		
	
	
	free_page(real_addr->_base,num);
	symbolic_fs_array[real_addr->_tmpname].open = 0;/*soft symbolic*/ 
	symbolic_fs_array[real_addr->_tmpname].file_info->directory = NULL;
	stream = NULL;	
	return 0;
}
