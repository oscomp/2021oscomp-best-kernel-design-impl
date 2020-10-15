/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *         The kernel's entry, where most of the initialization work is done.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include <common.h>
#include <os/irq.h>
#include <os/mm.h>
#include <os/sched.h>
#include <screen.h>
#include <sbi.h>
#include <stdio.h>
#include <os/time.h>
#include <os/syscall.h>
#include <os/futex.h>
#include <test.h>

#include <csr.h>
#include <asm.h>

extern void ret_from_exception();
extern void printk_task1(void);
extern void __global_pointer$();

/* for task group convenience */
#ifndef TASK_GROUP
#define TASK_GROUP

#define NUM_TASK num_lock_tasks
#define TASK_INFO_ARRAY lock_tasks

#endif

TASK_GROUP
/* end for task group convenience */


static void init_pcb_stack(
    ptr_t kernel_stack, ptr_t user_stack, ptr_t entry_point,
    pcb_t *pcb)
{
    regs_context_t *pt_regs =
        (regs_context_t *)(kernel_stack - sizeof(regs_context_t));
    
    /* TODO: initialization registers
     * note: sp, gp, ra, sepc, sstatus
     * gp should be __global_pointer$
     * To run the task in user mode,
     * you should set corresponding bits of sstatus(SPP, SPIE, etc.).
     */    
    reg_t gp, ra;
    //reg_t sepc, sstatus;
    
    gp = __global_pointer$;
    ra = entry_point;

    reg_t *regs = pt_regs->regs;
    
    regs[3] = gp;
    regs[1] = ra;

    switchto_context_t *pt_switchto =
        (switchto_context_t *)(kernel_stack - sizeof(regs_context_t) - sizeof(switchto_context_t));

    regs = pt_switchto->regs;
    regs[0] = ra;

    pcb->kernel_sp = (reg_t)(kernel_stack - sizeof(regs_context_t) - sizeof(switchto_context_t));
    pcb->user_sp = (reg_t)user_stack;

    // printk("id:%d,kernel_sp:%x,user_sp:%x\n\r",pcb->pid,kernel_stack,pcb->user_sp);

    // set sp to simulate return from switch_to
    /* TODO: you should prepare a stack, and push some values to
     * simulate a pcb context.
     */
}

static void init_pcb()
{
     /* initialize all of your pcb and add them into ready_queue
     * TODO:
     */
    int num_task = NUM_TASK;

    for (int i = 0; i < num_task; ++i)
    {
        task_info_t *task_info = *(TASK_INFO_ARRAY + i);
        pcb_t *pcb_underinit = &pcb[i];
        ptr_t kernel_stack = allocPage(1);
        ptr_t user_stack = allocPage(1);

        pcb_underinit->preempt_count = 0;
        pcb_underinit->list.pt_pcb = pcb_underinit;
        pcb_underinit->pid = process_id++;
        pcb_underinit->type = task_info->type;
        pcb_underinit->status = TASK_READY;
        pcb_underinit->cursor_x = 1; pcb_underinit->cursor_y = 1; 

        init_pcb_stack(kernel_stack, user_stack, task_info->entry_point, pcb_underinit);
        list_add_tail(&pcb_underinit->list,&ready_queue);
    }

    // printk("id:%d,kernel_sp:%x,user_sp:%x\n\r",pid0_pcb.pid,pid0_pcb.kernel_sp,pid0_pcb.user_sp);
    // printk("%d %d %d\n\r",*((ptr_t)(pcb[0].kernel_sp)) == &printk_task1, *(pcb[1].kernel_sp) == &printk_task2, *(pcb[2].kernel_sp) == &drawing_task1); 
    /* remember to initialize `current_running`
     * TODO:
     */
    current_running = &pid0_pcb;
}

static void init_syscall(void)
{
    // initialize system call table.
}

// jump from bootloader.
// The beginning of everything >_< ~~~~~~~~~~~~~~
int main()
{
    // // init Process Control Block (-_-!)
    init_pcb();
    printk("> [INIT] PCB initialization succeeded.\n\r");

    // // read CPU frequency
    // time_base = sbi_read_fdt(TIMEBASE);
	
    // // init futex mechanism
    // init_system_futex();

    // // init interrupt (^_^)
    // init_exception();
    // printk("> [INIT] Interrupt processing initialization succeeded.\n\r");

    // init system call table (0_0)
    init_syscall();
    // printk("> [INIT] System call initialized successfully.\n\r");

    // // fdt_print(riscv_dtb);

    // // init screen (QAQ)
    // init_screen();
    // printk("> [INIT] SCREEN initialization succeeded.\n\r");

    // TODO:
    // Setup timer interrupt and enable all interrupt
    
    while (1) {
        // (QAQQQQQQQQQQQ)
        // If you do non-preemptive scheduling, you need to use it
        // to surrender control do_scheduler();
        // enable_interrupt();
        // __asm__ __volatile__("wfi\n\r":::);
        do_scheduler();
    };
    return 0;
}
