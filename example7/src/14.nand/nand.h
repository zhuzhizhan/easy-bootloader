unsigned char read_nand_status(void);
int copy_sdram_to_nand(unsigned char *sdram_addr, unsigned long nand_addr, unsigned long length);
int copy_nand_to_sdram(unsigned char *sdram_addr, unsigned long nand_addr, unsigned long length);
unsigned char nand_erase(unsigned long block_num);
void nand_read_id(void);
void nand_init(void);




