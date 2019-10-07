#include "mm.h"
#include "arm/mmu.h"
#include "printf.h"

static unsigned short mem_map [ PAGING_PAGES ] = {0,};

struct mm_info allocate_kernel_page(int count) {
	int num;
	struct mm_info return_mm;	
	if(count%4096){
		num = (count/4096)+1;	
	}else{
		num = count/4096;
	}	
	unsigned long page = get_free_page(num);
	if (page == 0) {
		return_mm.start = 0;
		return_mm.size = 0;
		return return_mm;
	}
	return_mm.start = page + VA_START;
	return_mm.size = num;
	return return_mm;
}
/*i*/
unsigned long allocate_pcb_space() {
        static int index = 0;
	static int flag = 0;	
	unsigned long page;
	
	if(flag == 0){	
		page = get_free_page(1);
		flag = 1;
	}
	
	if(index==64){	
		page = get_free_page(1);
		index=0;
	}

	if ((page+index*64) == 0) {
		return 0;
	}
	
	return page + (index++)*64 + VA_START;

}

unsigned long allocate_user_page(struct task_struct *task, unsigned long va) {	
	unsigned long page = get_free_page(1);
	if (page == 0) {
		return 0;
	}
	map_page(task, va, page);	
	return page + VA_START;
}
extern struct thread_mutex mm_lock;
unsigned long get_free_page(int count)
{
	int succeed = 1;
	static int pre_num = 0; /*improve*/
	_thread_mutex_lock(&mm_lock);
	for (int i = 0; i < PAGING_PAGES; i++){
		succeed = 1;
		for (int j = 0;j < count;j++){
			if (mem_map[i+j] != 0){
				succeed = 0;
				break;			
			}
		}
		if(succeed == 1){
			for (int k = 0;k < count;k++){
				mem_map[i+k] = 1;
			}
			unsigned long page = LOW_MEMORY + i*PAGE_SIZE;
			memzero(page + VA_START, PAGE_SIZE*count);
			_thread_mutex_unlock(&mm_lock);
			return page;
		
		}
	}
        _thread_mutex_unlock(&mm_lock);
	return 0;
}

void free_page(unsigned long p, int size){/*need improve*/
	int a = ((p - LOW_MEMORY) / PAGE_SIZE);
	for(int i=0 ; i<size ;i++){	
		mem_map[a+i] = 0;
	}
}

void map_table_entry(unsigned long *pte, unsigned long va, unsigned long pa) {
	unsigned long index = va >> PAGE_SHIFT;
	index = index & (PTRS_PER_TABLE - 1);
	unsigned long entry = pa | MMU_PTE_FLAGS; 
	pte[index] = entry;
}

unsigned long map_table(unsigned long *table, unsigned long shift, unsigned long va, int* new_table) {
	unsigned long index = va >> shift;
	index = index & (PTRS_PER_TABLE - 1);
	if (!table[index]){
		*new_table = 1;
		unsigned long next_level_table = get_free_page(1);
		unsigned long entry = next_level_table | MM_TYPE_PAGE_TABLE;
		table[index] = entry;
		return next_level_table;
	} else {
		*new_table = 0;
	}
	return table[index] & PAGE_MASK;
}

void map_page(struct task_struct *task, unsigned long va, unsigned long page){
	unsigned long pgd;
	if (!task->mm.pgd) {
		task->mm.pgd = get_free_page(1);
		task->mm.kernel_pages[++task->mm.kernel_pages_count] = task->mm.pgd;
	}
	
	pgd = task->mm.pgd;
	int new_table;
	unsigned long pud = map_table((unsigned long *)(pgd + VA_START), PGD_SHIFT, va, &new_table);

	if (new_table) {
		task->mm.kernel_pages[++task->mm.kernel_pages_count] = pud;
	}

	unsigned long pmd = map_table((unsigned long *)(pud + VA_START) , PUD_SHIFT, va, &new_table);
	if (new_table) {
		task->mm.kernel_pages[++task->mm.kernel_pages_count] = pmd;
	}

	unsigned long pte = map_table((unsigned long *)(pmd + VA_START), PMD_SHIFT, va, &new_table);
	if (new_table) {
		task->mm.kernel_pages[++task->mm.kernel_pages_count] = pte;
	}

	map_table_entry((unsigned long *)(pte + VA_START), va, page);
	struct user_page p = {page, va};
	task->mm.user_pages[task->mm.user_pages_count++] = p;
}

int copy_virt_memory(struct task_struct *dst) {
	struct task_struct* src = &(current->cpu_context->x19);
	for (int i = 0; i < src->mm.user_pages_count; i++) {
		unsigned long kernel_va = allocate_user_page(dst, src->mm.user_pages[i].virt_addr);
		if( kernel_va == 0) {
			return -1;
		}
		memcpy(src->mm.user_pages[i].virt_addr, kernel_va, PAGE_SIZE);/*LD X0, ST X1*/
	}
	
	return 0;
}

static int ind = 1;

int do_mem_abort(unsigned long addr, unsigned long esr) {
	unsigned long dfs = (esr & 0b111111);
	if ((dfs & 0b111100) == 0b100) {
		unsigned long page = get_free_page(1);
		if (page == 0) {

			return -1;
		}
		map_page(&(current->cpu_context->x19), addr & PAGE_MASK, page);
		/*		
		ind++;
		if (ind > 3){
			return -1;
		}
*/
		return 0;
	}
	printf("A3");
	return -1;
}

