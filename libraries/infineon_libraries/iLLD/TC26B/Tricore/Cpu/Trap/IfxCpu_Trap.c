/**
 * \file IfxCpu_Trap.c
 * \brief This file contains the APIs for Trap related functions.
 *
 *
 * \version iLLD_1_0_1_11_0
 * \copyright Copyright (c) 2012 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Use of this file is subject to the terms of use agreed between (i) you or 
 * the company in which ordinary course of business you are acting and (ii) 
 * Infineon Technologies AG or its licensees. If and as long as no such 
 * terms of use are agreed, use of this file is subject to following:


 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or 
 * organization obtaining a copy of the software and accompanying 
 * documentation covered by this license (the "Software") to use, reproduce,
 * display, distribute, execute, and transmit the Software, and to prepare
 * derivative works of the Software, and to permit third-parties to whom the 
 * Software is furnished to do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 *
 *
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "IfxCpu_Trap.h"
#include "Cpu/Std/IfxCpu.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "IfxCpu_reg.h"
#include "Ifx_Cfg.h"
#include "zf_common_debug.h"
#ifdef IFX_CFG_EXTEND_TRAP_HOOKS
#include "Ifx_Cfg_Trap.h"
#endif

/******************************************************************************/
/*                           Macros                                           */
/******************************************************************************/
/** \brief Configuration for CpuX enable.
 *
 */
#ifndef IFX_CFG_CPU_TRAP_TSR_HOOK
#   define IFX_CFG_CPU_TRAP_TSR_HOOK(trapWatch)          /**< By default macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_SYSCALL_CPU0_HOOK
#   define IFX_CFG_CPU_TRAP_SYSCALL_CPU0_HOOK(trapWatch) ((void)trapWatch) /**< By default macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_SYSCALL_CPU1_HOOK
#   define IFX_CFG_CPU_TRAP_SYSCALL_CPU1_HOOK(trapWatch) ((void)trapWatch) /**< By default macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_SYSCALL_CPU2_HOOK
#   define IFX_CFG_CPU_TRAP_SYSCALL_CPU2_HOOK(trapWatch) ((void)trapWatch) /**< By default macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_SYSCALL_CPU3_HOOK
#   define IFX_CFG_CPU_TRAP_SYSCALL_CPU3_HOOK(trapWatch) ((void)trapWatch) /**< By default macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_SYSCALL_CPU4_HOOK
#   define IFX_CFG_CPU_TRAP_SYSCALL_CPU4_HOOK(trapWatch) ((void)trapWatch) /**< By default macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_SYSCALL_CPU5_HOOK
#   define IFX_CFG_CPU_TRAP_SYSCALL_CPU5_HOOK(trapWatch) ((void)trapWatch) /**< By default macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_NMI_HOOK
#   define IFX_CFG_CPU_TRAP_NMI_HOOK(trapWatch)          ((void)trapWatch) /**< By default NMI macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_MME_HOOK
#   define IFX_CFG_CPU_TRAP_MME_HOOK(trapWatch)    ((void)trapWatch)      /**< By default memory Management Error macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_IPE_HOOK
#   define IFX_CFG_CPU_TRAP_IPE_HOOK(trapWatch)    ((void)trapWatch)      /**< By default internal Protection Error macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_IE_HOOK
#   define IFX_CFG_CPU_TRAP_IE_HOOK(trapWatch)     ((void)trapWatch)      /**< By default instruction Error macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_CME_HOOK
#   define IFX_CFG_CPU_TRAP_CME_HOOK(trapWatch)    ((void)trapWatch)      /**< By default context Management Error macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_BE_HOOK
#   define IFX_CFG_CPU_TRAP_BE_HOOK(trapWatch)     ((void)trapWatch)      /**< By default bus Error macro is empty*/
#endif
#ifndef IFX_CFG_CPU_TRAP_ASSERT_HOOK
#   define IFX_CFG_CPU_TRAP_ASSERT_HOOK(trapWatch) ((void)trapWatch)     /**< By default assertion is empty*/
#endif

#ifndef IFX_CFG_CPU_TRAP_DEBUG
 #define IFX_CFG_CPU_TRAP_DEBUG __debug()
#endif
/*******************************************************************************
**                      variables                                     **
*******************************************************************************/

/*******************************************************************************
**                      Function definitions                          **
*******************************************************************************/
IFX_INLINE IfxCpu_Trap IfxCpu_Trap_extractTrapInfo(uint8 trapClass, uint32 tin)
{
    IfxCpu_Trap trapInfo;
    trapInfo.tAddr  = (unsigned int)__getA11();
    trapInfo.tClass = trapClass;
    trapInfo.tId    = tin;
    trapInfo.tCpu   = IfxCpu_getCoreId();
    return trapInfo;
}


void IfxCpu_Trap_memoryManagementError(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_memoryManagement, tin);
    IFX_CFG_CPU_TRAP_MME_HOOK(trapWatch);

    // 如果单片机卡死在这里，则说明出现了内存管理报错，报错原因需自行排查

    IFX_CFG_CPU_TRAP_DEBUG;
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}


void IfxCpu_Trap_internalProtectionError(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_internalProtection, tin);
    IFX_CFG_CPU_TRAP_IPE_HOOK(trapWatch);

    // 如果单片机卡死在这里，则说明单片机访问到了空的内存位置，也就是常说的访问越界

    IFX_CFG_CPU_TRAP_DEBUG;
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}


void IfxCpu_Trap_instructionError(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_instructionErrors, tin);
    IFX_CFG_CPU_TRAP_IE_HOOK(trapWatch);

    // 如果单片机卡死在这里，则说明执行了错误的指令，具体原因通过debug查找

    IFX_CFG_CPU_TRAP_DEBUG;
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}


void IfxCpu_Trap_contextManagementError(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_contextManagement, tin);
    IFX_CFG_CPU_TRAP_CME_HOOK(trapWatch);

    // 单片机调度管理错误，一般不会触发此类报错，如若触发则自行debug查找问题

    IFX_CFG_CPU_TRAP_DEBUG;
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}


void IfxCpu_Trap_busError(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_bus, tin);
    IFX_CFG_CPU_TRAP_BE_HOOK(trapWatch);

    // 如果单片机卡死在了这里 可能是使用了未初始化的外设资源
    // 举个例子，没调用pwm_init初始化函数，然后直接调用pwm_set_duty来赋值输出
    // 也可能是访问内存失败导致 如果访问失败请仔细检查使用指针访问数据的地方

    IFX_CFG_CPU_TRAP_DEBUG;
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}


void IfxCpu_Trap_assertion(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_assertion, tin);
    IFX_CFG_CPU_TRAP_ASSERT_HOOK(trapWatch);

    // 如果单片机卡死在这里，则说明触发了英飞凌的断言，通过调试来查看具体触发断言的位置并修正

    IFX_CFG_CPU_TRAP_DEBUG;
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}


void IfxCpu_Trap_systemCall_Cpu0(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_systemCall, tin);
    IFX_CFG_CPU_TRAP_SYSCALL_CPU0_HOOK(trapWatch);
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}

#if IFXCPU_NUM_MODULES >= 2
void IfxCpu_Trap_systemCall_Cpu1(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_systemCall, tin);
    IFX_CFG_CPU_TRAP_SYSCALL_CPU1_HOOK(trapWatch);
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}
#endif

#if IFXCPU_NUM_MODULES >= 3
void IfxCpu_Trap_systemCall_Cpu2(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_systemCall, tin);
    IFX_CFG_CPU_TRAP_SYSCALL_CPU2_HOOK(trapWatch);
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}
#endif

#if IFXCPU_NUM_MODULES >= 4
void IfxCpu_Trap_systemCall_Cpu3(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_systemCall, tin);
    IFX_CFG_CPU_TRAP_SYSCALL_CPU3_HOOK(trapWatch);
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}
#endif

#if IFXCPU_NUM_MODULES >= 5
void IfxCpu_Trap_systemCall_Cpu4(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_systemCall, tin);
    IFX_CFG_CPU_TRAP_SYSCALL_CPU4_HOOK(trapWatch);
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}
#endif

#if IFXCPU_NUM_MODULES >= 6
void IfxCpu_Trap_systemCall_Cpu5(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_systemCall, tin);
    IFX_CFG_CPU_TRAP_SYSCALL_CPU5_HOOK(trapWatch);
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}
#endif

void IfxCpu_Trap_nonMaskableInterrupt(uint32 tin)
{
    volatile IfxCpu_Trap trapWatch;
    trapWatch = IfxCpu_Trap_extractTrapInfo(IfxCpu_Trap_Class_nonMaskableInterrupt, tin);
    IFX_CFG_CPU_TRAP_NMI_HOOK(trapWatch);    
    __asm("rslcx"); /* Restore lower context before returning. lower context was stored in the trap vector */
    __asm("rfe");
}


#if defined(__HIGHTEC__)
#pragma section
#pragma section ".traptab_cpu0" awx
#pragma GCC optimize ("O2")
#endif
#if defined(__DCC__)
#pragma section
#pragma section CODE ".traptab_cpu0" X
#endif
#if defined(__TASKING__)
#pragma protect on
#pragma section code "traptab_cpu0"
#endif
#if defined(__ghs__)
#pragma ghs section
#pragma ghs section text=".traptab_cpu0"
#endif
void IfxCpu_Trap_vectorTable0(void)
{
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_memoryManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_internalProtectionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_instructionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_contextManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_busError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_assertion);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_systemCall_Cpu0);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_nonMaskableInterrupt);
}


#if IFXCPU_NUM_MODULES >= 2
#if defined(__HIGHTEC__)
#pragma section
#pragma section ".traptab_cpu1" awx
#endif
#if defined(__DCC__)
#pragma section
#pragma section CODE ".traptab_cpu1" X
#endif
#if defined(__TASKING__)
#pragma section code "traptab_cpu1"
#endif
#if defined(__ghs__)
#pragma ghs section
#pragma ghs section text=".traptab_cpu1"
#endif
void IfxCpu_Trap_vectorTable1(void)
{
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_memoryManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_internalProtectionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_instructionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_contextManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_busError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_assertion);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_systemCall_Cpu1);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_nonMaskableInterrupt);
}


#endif

#if IFXCPU_NUM_MODULES >= 3
#if defined(__HIGHTEC__)
#pragma section
#pragma section ".traptab_cpu2" awx
#endif
#if defined(__DCC__)
#pragma section
#pragma section CODE ".traptab_cpu2" X
#endif
#if defined(__TASKING__)
#pragma section code "traptab_cpu2"
#endif
#if defined(__ghs__)
#pragma ghs section
#pragma ghs section text=".traptab_cpu2"
#endif

void IfxCpu_Trap_vectorTable2(void)
{
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_memoryManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_internalProtectionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_instructionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_contextManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_busError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_assertion);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_systemCall_Cpu2);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_nonMaskableInterrupt);
}


#endif

#if IFXCPU_NUM_MODULES >= 4
#if defined(__HIGHTEC__)
#pragma section
#pragma section ".traptab_cpu3" awx
#endif
#if defined(__DCC__)
#pragma section
#pragma section CODE ".traptab_cpu3" X
#endif
#if defined(__TASKING__)
#pragma section code "traptab_cpu3"
#endif
#if defined(__ghs__)
#pragma ghs section
#pragma ghs section text=".traptab_cpu3"
#endif


void IfxCpu_Trap_vectorTable3(void)
{
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_memoryManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_internalProtectionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_instructionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_contextManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_busError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_assertion);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_systemCall_Cpu3);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_nonMaskableInterrupt);
}


#endif

#if IFXCPU_NUM_MODULES >= 5
#if defined(__HIGHTEC__)
#pragma section
#pragma section ".traptab_cpu4" awx
#endif
#if defined(__DCC__)
#pragma section
#pragma section CODE ".traptab_cpu4" X
#endif
#if defined(__TASKING__)
#pragma section code "traptab_cpu4"
#endif
#if defined(__ghs__)
#pragma ghs section
#pragma ghs section text=".traptab_cpu4"
#endif

void IfxCpu_Trap_vectorTable4(void)
{
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_memoryManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_internalProtectionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_instructionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_contextManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_busError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_assertion);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_systemCall_Cpu4);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_nonMaskableInterrupt);
}


#endif

#if IFXCPU_NUM_MODULES >= 6
#if defined(__HIGHTEC__)
#pragma section
#pragma section ".traptab_cpu5" awx
#endif
#if defined(__DCC__)
#pragma section
#pragma section CODE ".traptab_cpu5" X
#endif
#if defined(__TASKING__)
#pragma section code "traptab_cpu5"
#endif
#if defined(__ghs__)
#pragma ghs section
#pragma ghs section text=".traptab_cpu5"
#endif

void IfxCpu_Trap_vectorTable5(void)
{
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_memoryManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_internalProtectionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_instructionError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_contextManagementError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_busError);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_assertion);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_systemCall_Cpu5);
    IfxCpu_Tsr_CallTSR(IfxCpu_Trap_nonMaskableInterrupt);
}


#endif

#if defined(__HIGHTEC__)
#pragma section
#endif
#if defined(__DCC__)
#pragma section
#endif
#if defined(__DCC__)
#pragma interrupt IfxInterruptEx
#endif
#if defined(__TASKING__)
#pragma endprotect
#endif
#if defined(__ghs__)
#pragma ghs section
#endif
