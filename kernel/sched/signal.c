#include <os/sched.h>

/* 通过how来设置procmask */
/* success return 0, fail return -1 */
int32_t do_rt_sigprocmask(int32_t how, const sigset_t *restrict set, sigset_t *restrict oldset, size_t sigsetsize)
{
    debug();
    if (oldset)
        memcpy(&oldset->sig[0], &current_running->sig_mask, sizeof(sigset_t));
    assert(set);
    if (how == SIG_BLOCK)
        current_running->sig_mask |= set->sig[0];
    else if (how == SIG_UNBLOCK)
        current_running->sig_mask &= ~(set->sig[0]);
    else if (how == SIG_SETMASK)
        current_running->sig_mask = set->sig[0];
    else
        assert(0);
    return 0;
}

/* success return 0, fail return -1 */
int32_t do_rt_sigaction(int32_t signum, struct sigaction *act, struct sigaction *oldact, size_t sigsetsize)
{
    debug();
    if (signum > NUM_SIG){
        printk_port("cannot set signum %d, too large\n", signum);
        return 0;
    }
    sigaction_t *this_sigaction = &current_running->sigactions[signum - 1];
    if (oldact)
        memcpy(oldact, this_sigaction, sizeof(sigaction_t));
    if (act)
        memcpy(this_sigaction, act, sizeof(sigaction_t));
    return 0;
}

void do_rt_sigreturn()
{
    signal_context_t *signal_context = 
        (signal_context_t *)(current_running->kernel_sp + sizeof(regs_context_t));
    change_signal_mask(current_running, signal_context->mask);
    exit_signal_trampoline(signal_context);
}

/* FOR NOW we can handle signal less than 24 */
void handle_signal()
{
    int is_handling_signal = 0;
    while (current_running->sig_recv != 0 && !is_handling_signal){
        for (uint8_t i = 0; i < NUM_SIG; i++){
            /* i == signum - 1 */
            if (ISSET_SIG(i + 1, current_running)){
                log2(0, "tid %d is handling signum %d", current_running->tid, i+1);
                // 1. clear this signal
                CLEAR_SIG(i + 1, current_running);

                // 2. change mask
                uint64_t prev_mask = current_running->sig_mask;
                change_signal_mask(current_running, current_running->sigactions[i].sa_mask.sig[0]);

                // 3. goto handler
                if (current_running->sigactions[i].sa_handler == SIG_IGN)
                    ;
                else if (current_running->sigactions[i].sa_handler == SIG_DFL){
                    /* kernel handle signal, no need to set trampoline */
                    switch (i+1){
                        case SIGKILL:
                        log2(0, "tid %d is killed SIGKILL", current_running->tid);
                        do_exit(1);
                        assert(0); /* never return */
                        case SIGSEGV:
                        log2(0, "tid %d is killed SIGSEGV", current_running->tid);
                        do_exit(1);
                        assert(0); /* never return */
                        default:
                        log(0, "signum %d hasn't been implemented", i+1);
                        assert(0);
                    }
                    change_signal_mask(current_running, prev_mask);
                }
                else{
                    /* now sp is just under user context */
                    regs_context_t *pt_regs = current_running->kernel_sp;
                    regs_context_t new_pt_regs;
                    /* set a trampoline */
                    memcpy(&new_pt_regs, pt_regs, sizeof(regs_context_t));
                    new_pt_regs.regs[10] = i + 1; /* a0 = signum */
                    new_pt_regs.sepc = current_running->sigactions[i].sa_handler;
                    new_pt_regs.regs[1] = &do_rt_sigreturn; /* should do sigreturn when handler return */
                    set_signal_trampoline(&new_pt_regs, prev_mask);
                    /* once return, go to trampoline */
                    /* twice return, go to normal(maybe another signal handler) */
                    if (!is_handling_signal)
                        is_handling_signal = 1;
                    else
                        is_handling_signal = 0;
                }
                break;
            }
        }
    }
}

void send_signal(int signum, pcb_t *pcb)
{
    if (signum == 0){
        log(0, "test signal setup");
        return ;
    }
    uintptr_t sig_value = (1lu << (signum - 1));
    if ((sig_value & pcb->sig_mask) != 0){
        log2(0, "block this signal");
        pcb->sig_pend |= sig_value;
    }
    else{
        log2(0, "recv this signal");
        pcb->sig_recv |= sig_value;
    }
    if (pcb->status == TASK_BLOCKED){
        log2(0, "receiver is unblocked", pcb->status);
        do_unblock(&pcb->list);
    }
}

static void trans_pended_signal_to_recv(pcb_t *pcb)
{
    if (pcb->sig_pend != 0){
        uint64_t prev_recv = pcb->sig_recv;
        pcb->sig_recv |= (pcb->sig_pend & ~(pcb->sig_mask));
        pcb->sig_pend &= ~(prev_recv ^ pcb->sig_recv);
    }
}

void change_signal_mask(pcb_t *pcb, uint64_t newmask)
{
    pcb->sig_mask = newmask;
    trans_pended_signal_to_recv(pcb);
}