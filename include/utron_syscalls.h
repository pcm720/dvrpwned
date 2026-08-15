/*
 * Inline UTron system-call macros for the GCC fr30-elf toolchain.
 *
 * Generated from ghidra-fr60/data/fr60_utron_syscall_numbers
 * https://github.com/desrdev/ghidra-fr60/blob/main/data/fr60_utron_syscall_numbers
 *
 * These macros expand directly to the UTron trap sequence at the call site,
 * so no external assembly stubs are linked into the payload.  They are
 * intended for calls whose arguments are compile-time constants.
 *
 * Each macro uses GCC extended asm to declare its register clobbers
 * (r4-r7 and r12), so the compiler will not keep live values in those
 * registers across the syscall.
 */

#ifndef UTRON_SYSCALLS_H
#define UTRON_SYSCALLS_H

#define trcv_msg(mbxid, ppk_msg, tmout) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:32 #%2, r6\n\t" \
    "ldi:8 #0x54, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mbxid), "n" (ppk_msg), "n" (tmout) : "r4", "r5", "r6", "r12")

#define twai_sem(semid, tmout) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0x55, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (semid), "n" (tmout) : "r4", "r5", "r12")

/* twai_flg: too many arguments for inline macro; use asm stub */
#define tget_blf(mpfid, p_blf, tmout) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:32 #%2, r6\n\t" \
    "ldi:8 #0x59, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mpfid), "n" (p_blf), "n" (tmout) : "r4", "r5", "r6", "r12")

#define rel_blk(mplid, blk) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0x71, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mplid), "n" (blk) : "r4", "r5", "r12")

#define get_blk(mplid, p_blk, blksz) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:32 #%2, r6\n\t" \
    "ldi:8 #0x73, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mplid), "n" (p_blk), "n" (blksz) : "r4", "r5", "r6", "r12")

#define ref_mpl(mplid, pk_rmpl) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0x74, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mplid), "n" (pk_rmpl) : "r4", "r5", "r12")

#define prcv_msg(mbxid, ppk_msg) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0x94, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mbxid), "n" (ppk_msg) : "r4", "r5", "r12")

#define preq_sem(semid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0x95, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (semid) : "r4", "r12")

#define pol_flg(flgid, waiptn, wfmode, p_flgptn) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:32 #%2, r6\n\t" \
    "ldi:32 #%3, r7\n\t" \
    "ldi:8 #0x96, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (flgid), "n" (waiptn), "n" (wfmode), "n" (p_flgptn) : "r4", "r5", "r6", "r7", "r12")

#define pget_blk(mplid, p_blk, blksz) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:32 #%2, r6\n\t" \
    "ldi:8 #0x98, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mplid), "n" (p_blk), "n" (blksz) : "r4", "r5", "r6", "r12")

#define pget_blf(mpfid, p_blf) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0x99, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mpfid), "n" (p_blf) : "r4", "r5", "r12")

#define act_cyc(cycid, cycact) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xa2, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (cycid), "n" (cycact) : "r4", "r5", "r12")

#define ret_tmr() __asm__ volatile ( \
    "ldi:8 #0xa3, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define ref_cyc(cycid, pk_rcyc) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xa4, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (cycid), "n" (pk_rcyc) : "r4", "r5", "r12")

#define ref_alm(almid, pk_ralm) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xa5, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (almid), "n" (pk_ralm) : "r4", "r5", "r12")

#define def_cyc(cycid, pk_dcyc) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xa6, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (cycid), "n" (pk_dcyc) : "r4", "r5", "r12")

#define def_alm(almid, pk_dalm) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xa7, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (almid), "n" (pk_dalm) : "r4", "r5", "r12")

#define dly_Tsk() __asm__ volatile ( \
    "ldi:8 #0xab, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define get_tim(p_systim) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xac, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (p_systim) : "r4", "r12")

#define set_tim(p_systim) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xad, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (p_systim) : "r4", "r12")

#define ref_blf() __asm__ volatile ( \
    "ldi:8 #0xb1, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define get_blf(mpfid, p_blf) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xb3, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mpfid), "n" (p_blf) : "r4", "r5", "r12")

#define ref_mpf(mpfid, pk_rmpf) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xb4, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mpfid), "n" (pk_rmpf) : "r4", "r5", "r12")

#define ret_int() __asm__ volatile ( \
    "ldi:8 #0xbb, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define ref_ilm(p_ilmmask) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xbc, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (p_ilmmask) : "r4", "r12")

#define chg_ilm(ilmmask) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xbd, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (ilmmask) : "r4", "r12")

#define snd_msg(mbxid, pk_msg) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xc1, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mbxid), "n" (pk_msg) : "r4", "r5", "r12")

#define rcv_msg(mbxid, ppk_msg) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xc3, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mbxid), "n" (ppk_msg) : "r4", "r5", "r12")

#define ref_mbx(mbxid, pk_rmbx) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xc4, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (mbxid), "n" (pk_rmbx) : "r4", "r5", "r12")

#define sig_sem(semid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xc9, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (semid) : "r4", "r12")

#define wai_sem(semid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xcb, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (semid) : "r4", "r12")

#define ref_sem(semid, pk_rsem) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xcc, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (semid), "n" (pk_rsem) : "r4", "r5", "r12")

#define set_flg(flgid, setptn) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xd0, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (flgid), "n" (setptn) : "r4", "r5", "r12")

#define clr_flg(flgid, clrptn) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xd1, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (flgid), "n" (clrptn) : "r4", "r5", "r12")

#define wai_flg(flgid, waiptn, wfmode, p_flgptn) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:32 #%2, r6\n\t" \
    "ldi:32 #%3, r7\n\t" \
    "ldi:8 #0xd2, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (flgid), "n" (waiptn), "n" (wfmode), "n" (p_flgptn) : "r4", "r5", "r6", "r7", "r12")

#define ref_flg(flgid, pk_rflg) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xd4, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (flgid), "n" (pk_rflg) : "r4", "r5", "r12")

#define can_wup(tskid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xd8, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid) : "r4", "r12")

#define wup_tsk(tskid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xd9, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid) : "r4", "r12")

#define slp_tsk() __asm__ volatile ( \
    "ldi:8 #0xda, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define tslp_tsk(tmout) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xdb, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tmout) : "r4", "r12")

#define frsm_tsk(tskid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xdc, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid) : "r4", "r12")

#define rsm_tsk(tskid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xdd, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid) : "r4", "r12")

#define sus_tsk(tskid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xdf, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid) : "r4", "r12")

#define rel_wai(tskid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xe1, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid) : "r4", "r12")

#define dis_dsp() __asm__ volatile ( \
    "ldi:8 #0xe2, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define ena_dsp() __asm__ volatile ( \
    "ldi:8 #0xe3, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define rot_rdq(tskpri) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xe4, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskpri) : "r4", "r12")

#define chg_pri(tskid, tskpri) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xe5, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid), "n" (tskpri) : "r4", "r5", "r12")

#define ter_tsk(tskid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xe7, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid) : "r4", "r12")

#define get_tid(p_tskid) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xe8, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (p_tskid) : "r4", "r12")

#define sta_tsk(tskid, stacd) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xe9, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid), "n" (stacd) : "r4", "r5", "r12")

#define ext_tsk() __asm__ volatile ( \
    "ldi:8 #0xeb, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define ref_tsk(tskid, pk_rtsk) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:32 #%1, r5\n\t" \
    "ldi:8 #0xec, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (tskid), "n" (pk_rtsk) : "r4", "r5", "r12")

#define get_ver(pk_ver) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xf0, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (pk_ver) : "r4", "r12")

#define ref_sys(pk_rsys) __asm__ volatile ( \
    "ldi:32 #%0, r4\n\t" \
    "ldi:8 #0xf4, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : "n" (pk_rsys) : "r4", "r12")

#define loc_cpu() __asm__ volatile ( \
    "ldi:8 #0xf8, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")

#define unl_cpu() __asm__ volatile ( \
    "ldi:8 #0xf9, r12\n\t" \
    "extsb r12\n\t" \
    "int #0x40" \
    : : : "r12")


#endif /* UTRON_SYSCALLS_H */
