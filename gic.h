// The addresses of the GIC registers.
//
// These are defined in sections 4.1.2 and 4.1.3 (p. 4-74 to 4-76) of the ARM
// Generic Interrupt Controller Architecture Specification (Architecture
// Version 2.0).
//
// The BCM2711 SoC used on the Raspberry Pi 4 contains an ARM GICv2 interrupt
// controller. To enable the GIC on the Pi, use the following in the config.txt
// file:  enable_gic=1


// Base addresses
#define GIC_BASE			(0xff841000)         // General GIC base address
#define GIC_GICD_BASE		(GIC_BASE)           // GICD MMIO base address
#define GIC_GICC_BASE		(GIC_BASE + 0x1000)  // GICC MMIO base address


// 4.1.2 The GIC Distributor register map
#define GIC_GICD_CTLR		((volatile unsigned int *)(GIC_GICD_BASE + 0x000)) // Distributor Control Register
#define GIC_GICD_TYPER		((volatile unsigned int *)(GIC_GICD_BASE + 0x004)) // Interrupt Controller Type Register
#define GIC_GICD_IIDR		((volatile unsigned int *)(GIC_GICD_BASE + 0x008)) // Distributor Implementer Identification Register
#define GIC_GICD_IGROUPR	((volatile unsigned int *)(GIC_GICD_BASE + 0x080)) // Interrupt Group Registers
#define GIC_GICD_ISENABLER	((volatile unsigned int *)(GIC_GICD_BASE + 0x100)) // Interrupt Set-Enable Registers
#define GIC_GICD_ICENABLER	((volatile unsigned int *)(GIC_GICD_BASE + 0x180)) // Interrupt Clear-Enable Registers
#define GIC_GICD_ISPENDR	((volatile unsigned int *)(GIC_GICD_BASE + 0x200)) // Interrupt Set-Pending Registers
#define GIC_GICD_ICPENDR	((volatile unsigned int *)(GIC_GICD_BASE + 0x280)) // Interrupt Clear-Pending Registers
#define GIC_GICD_ISACTIVER	((volatile unsigned int *)(GIC_GICD_BASE + 0x300)) // Interrupt Set-Active Registers
#define GIC_GICD_ICACTIVER	((volatile unsigned int *)(GIC_GICD_BASE + 0x380)) // Interrupt Clear-Active Registers
#define GIC_GICD_IPRIORITYR	((volatile unsigned int *)(GIC_GICD_BASE + 0x400)) // Interrupt Priority Registers
#define GIC_GICD_ITARGETSR	((volatile unsigned int *)(GIC_GICD_BASE + 0x800)) // Interrupt Processor Targets Registers
#define GIC_GICD_ICFGR		((volatile unsigned int *)(GIC_GICD_BASE + 0xc00)) // Interrupt Configuration Registers
#define GIC_GICD_NSCAR		((volatile unsigned int *)(GIC_GICD_BASE + 0xe00)) // Non-secure Access Control Registers
#define GIC_GICD_SGIR		((volatile unsigned int *)(GIC_GICD_BASE + 0xf00)) // Software Generated Interrupt Register
#define GIC_GICD_CPENDSGIR	((volatile unsigned int *)(GIC_GICD_BASE + 0xf10)) // SGI Clear-Pending Registers
#define GIC_GICD_SPENDSGIR	((volatile unsigned int *)(GIC_GICD_BASE + 0xf20)) // SGI Set-Pending Registers

// 4.3.1 GICD_CTLR, Distributor Control Register
#define GIC_GICD_CTLR_ENABLE   (0x1)  // Enable GICD interrupt forwarding
#define GIC_GICD_CTLR_DISABLE  (0x0)  // Disable GICD interrupt forwarding

// 4.3.13 GICD_ICFGR<n>, Interrupt Configuration Registers
#define GIC_GICD_ICFGR_LEVEL   (0x0)  // level-sensitive
#define GIC_GICD_ICFGR_EDGE    (0x2)  // edge-triggered


// 4.1.3 The GIC CPU interface register map
#define GIC_GICC_CTLR	((volatile unsigned int *)(GIC_GICC_BASE + 0x000))  // CPU Interface Control Register
#define GIC_GICC_PMR	((volatile unsigned int *)(GIC_GICC_BASE + 0x004))  // Interrupt Priority Mask Register
#define GIC_GICC_BPR	((volatile unsigned int *)(GIC_GICC_BASE + 0x008))  // Binary Point Register
#define GIC_GICC_IAR	((volatile unsigned int *)(GIC_GICC_BASE + 0x00C))  // Interrupt Acknowledge Register
#define GIC_GICC_EOIR	((volatile unsigned int *)(GIC_GICC_BASE + 0x010))  // End of Interrupt Register
#define GIC_GICC_RPR	((volatile unsigned int *)(GIC_GICC_BASE + 0x014))  // Running Priority Register
#define GIC_GICC_HPIR	((volatile unsigned int *)(GIC_GICC_BASE + 0x018))  // Highest Priority Pending Interrupt Register
#define GIC_GICC_ABPR	((volatile unsigned int *)(GIC_GICC_BASE + 0x01C))  // Aliased Binary Point Register
#define GIC_GICC_AIAR	((volatile unsigned int *)(GIC_GICC_BASE + 0x020))  // Aliased Interrupt Acknowledge Register
#define GIC_GICC_AEOIR	((volatile unsigned int *)(GIC_GICC_BASE + 0x024))  // Aliased End of Interrupt Register
#define GIC_GICC_AHPPIR	((volatile unsigned int *)(GIC_GICC_BASE + 0x028))  // Aliased Highest Priority Pending Interrupt Register
#define GIC_GICC_IIDR	((volatile unsigned int *)(GIC_GICC_BASE + 0x0FC))  // CPU Interface Identification Register
#define GIC_GICC_DIR	((volatile unsigned int *)(GIC_GICC_BASE + 0x100))  // Deactivate Interrupt Register

// 4.4.1 GICC_CTLR, CPU Interface Control Register
#define GICC_CTLR_ENABLE		(0x1)			// Enable GICC signaling
#define GICC_CTLR_DISABLE		(0x0)			// Disable GICC signaling

// 4.4.2 GICC_PMR, CPU Interface Priority Mask Register
#define GICC_PMR_PRIO_MIN		(0xff)			// The lowest level mask
#define GICC_PMR_PRIO_HIGH		(0x00)			// The highest level mask

// 4.4.4 GICC_IAR, CPU Interface Interrupt Acknowledge Register
#define GICC_IAR_INTR_IDMASK	(0x3ff)			// Bits 0-9: Interrupt ID
#define GICC_IAR_SPURIOUS_INTR	(0x3ff)			// 1023 means spurious interrupt
#define GICC_IAR_CPU_IDMASK		(0x1c00)		// Bits 10-12: CPU ID

