cmd_kernel/built-in.o :=  arm-hisiv100nptl-linux-ld -EL    -r -o kernel/built-in.o kernel/sched.o kernel/fork.o kernel/exec_domain.o kernel/panic.o kernel/printk.o kernel/cpu.o kernel/exit.o kernel/itimer.o kernel/time.o kernel/softirq.o kernel/resource.o kernel/sysctl.o kernel/sysctl_binary.o kernel/capability.o kernel/ptrace.o kernel/timer.o kernel/user.o kernel/signal.o kernel/sys.o kernel/kmod.o kernel/workqueue.o kernel/pid.o kernel/rcupdate.o kernel/extable.o kernel/params.o kernel/posix-timers.o kernel/kthread.o kernel/wait.o kernel/kfifo.o kernel/sys_ni.o kernel/posix-cpu-timers.o kernel/mutex.o kernel/hrtimer.o kernel/rwsem.o kernel/nsproxy.o kernel/srcu.o kernel/semaphore.o kernel/notifier.o kernel/ksysfs.o kernel/pm_qos_params.o kernel/sched_clock.o kernel/cred.o kernel/async.o kernel/range.o kernel/jump_label.o kernel/groups.o kernel/stacktrace.o kernel/time/built-in.o kernel/futex.o kernel/rtmutex.o kernel/up.o kernel/uid16.o kernel/module.o kernel/kallsyms.o kernel/acct.o kernel/irq/built-in.o kernel/rcutiny.o kernel/utsname_sysctl.o kernel/taskstats.o kernel/tsacct.o kernel/elfcore.o 
