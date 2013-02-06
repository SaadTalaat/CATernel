/**
 * @addtogroup debug
 * @{
 * @file kconsole.h
 * @author Saad Talaat
 * @date 19 November, 2012
 * @brief Kernel console for debugging ease
 * @name KConsole
 * @{
 */
#include <kconsole.h>
#include <cli.h>
kcommand_t *kconsole_commands;

void
kconsole_init(void)
{
	kconsole_commands = allocate(KCONSOLE_MAX_CMDS*sizeof(kcommand_t) , PAGESZ);
	int i;
	for(i=0; i<KCONSOLE_MAX_CMDS; i++)
	{
		kconsole_commands[i] = (kcommand_t) { NULL, NULL};
	}
	kconsole_commands[0] = (kcommand_t) { "clear" ,"Clears screen",console_clear};
	kconsole_commands[1] = (kcommand_t) { "cpu_info","Prints current cpu info",kcpu_info};
	kconsole_commands[2] = (kcommand_t) { "intr_info","Prints interrupts info",interrupt_printinfo};
	kconsole_commands[3] = (kcommand_t) { "mem_info","Prints memory mapping info",memory_printinfo};
	kconsole_commands[4] = (kcommand_t) { "proc_info","Prints process manager info",proc_printinfo};
	kconsole_commands[5] = (kcommand_t) { "test_lifo","Tests LIFO consistency",test_lifo};
	kconsole_commands[6] = (kcommand_t) { "test_fifo","Tests FIFO consistency",test_fifo};
	kconsole_commands[7] = (kcommand_t) { "test_paging","Tests Paging consistency",x86_test_pgdir};
	kconsole_commands[8] = (kcommand_t) { "schedule", "Begins scheduling",sched_init};
	kconsole_commands[9] = (kcommand_t) { "help", "Prints This message",kconsole_help};
}
void
kconsole(void)
{
        const char *buf;
	kcommand_t *command_recived, null_cmd;
	null_cmd = (kcommand_t) {NULL,NULL};
        while(1){
                buf = readline("CatOS(@) ");
                if (buf!=NULL){
                        printk("Command was %s\n",buf);
                }
		if((command_recived = kcommand_match(buf)) != NULL)
		{
			command_recived->operation();
			continue;
		}
		else
			printk("Command: not found\n");
        }
}
void
kcommand_register(kcommand_t *cmd)
{

	int i;
	kcommand_t null_cmd;
	null_cmd = (kcommand_t) {NULL,NULL};
	for( i=0; i < KCONSOLE_MAX_CMDS; i++)
	{
		if( kconsole_commands[i].name == NULL)
			break;
	}
	if(i == KCONSOLE_MAX_CMDS)
		panic("[*] KConsole: Already full");

	kconsole_commands[i] = *cmd;
}

kcommand_t *
kcommand_match(const char *buf)
{
	int i;
	kcommand_t null_cmd;
	null_cmd = (kcommand_t) {NULL,NULL};
	for (i=0; i<KCONSOLE_MAX_CMDS; i++)
	{
		if(kconsole_commands[i].name == NULL)
			return NULL;
		if(strcmp(buf, kconsole_commands[i].name) == 0)
			return &kconsole_commands[i];
	}
	return NULL;
}

/** KCOMMANDS */
void
kscheduler_info(void){
	return;
}

void
kcpu_info(void){
	processor_printinfo();
}

void
kconsole_help(void){
	int i;
	printk("[*] Available commands:\n");
	for(i =0 ; i<KCONSOLE_MAX_CMDS; i++)
	{
		if(kconsole_commands[i].name == NULL)
			return;
		else
		{
			printk("\t%11s:\t%s\n",kconsole_commands[i].name, kconsole_commands[i].hint);
		}
	}
}
/**
 * @}
 * @}
 */
