void wait_idle(void);
void nand_select_chip(void);
void nand_deselect_chip(void);
void write_cmd(int cmd);
void write_addr(unsigned int addr);
static void nand_reset(void);
unsigned char read_data(void);
void nand_init(void);
void nand_read_id(char id[]);
void nand_read(unsigned char *buf, unsigned long start_addr, int size);

