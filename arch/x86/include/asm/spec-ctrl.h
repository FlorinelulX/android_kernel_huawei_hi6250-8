/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPECCTRL_H_
#define _ASM_X86_SPECCTRL_H_

#include <linux/thread_info.h>
#include <asm/nospec-branch.h>

/*
 * On VMENTER we must preserve whatever view of the SPEC_CTRL MSR
 * the guest has, while on VMEXIT we restore the host view. This
 * would be easier if SPEC_CTRL were architecturally maskable or
 * shadowable for guests but this is not (currently) the case.
 * Takes the guest view of SPEC_CTRL MSR as a parameter and also
 * the guest's version of VIRT_SPEC_CTRL, if emulated.
 */
extern void x86_spec_ctrl_set_guest(u64 guest_spec_ctrl,
				    u64 guest_virt_spec_ctrl);
extern void x86_spec_ctrl_restore_host(u64 guest_spec_ctrl,
				       u64 guest_virt_spec_ctrl);

/* AMD specific Speculative Store Bypass MSR data */
extern u64 x86_amd_ls_cfg_base;
extern u64 x86_amd_ls_cfg_rds_mask;

/* The Intel SPEC CTRL MSR base value cache */
extern u64 x86_spec_ctrl_base;

static inline u64 rds_tif_to_spec_ctrl(u64 tifn)
{
	BUILD_BUG_ON(TIF_RDS < SPEC_CTRL_RDS_SHIFT);
	return (tifn & _TIF_RDS) >> (TIF_RDS - SPEC_CTRL_RDS_SHIFT);
}

static inline u64 rds_tif_to_amd_ls_cfg(u64 tifn)
{
	return (tifn & _TIF_RDS) ? x86_amd_ls_cfg_rds_mask : 0ULL;
}

extern void speculative_store_bypass_update(void);

#endif
