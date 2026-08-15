
# DVRP DVR-SPEED workaround for HDD authentication

Usually, DVR-SPEED does not allow DMA commands (e.g. DMA read/write) until the 0x8e command is received.  
However, the ATA state machine in DVR-SPEED does not reset or take into account primary/secondary device selection when receiving results after setting ATA command register.  
The following is done:
* Set 0xe4 command to primary (write memory)
* Set 0x8e command to secondary (sce auth)
* Set back to primary WITHOUT writing to command register
* Read PIO results

The following p-code does not handle AtaEmu, only DVRP ATA initialization and dvr_hdd devctl.  
AtaEmu passes through the 0x8e command directly to `sceAtaExecCmd`.

```c
typedef struct _ata_hwport
{
    vu16 r_data;  /* 00 */
    u16 m_pad02;
    vu16 r_error; /* 04 */
#define r_feature r_error
    u16 m_pad06;
    vu16 r_nsector; /* 08 */
    u16 m_pad0A;
    vu16 r_sector;  /* 0C */
    u16 m_pad0E;
    vu16 r_lcyl;    /* 10 */
    u16 m_pad12;
    vu16 r_hcyl;    /* 14 */
    u16 m_pad16;
    vu16 r_select;  /* 18 */
    u16 m_pad1A;
    vu16 r_status;  /* 1c */
#define r_command r_status
    u16 m_pad1E;
    vu16 r_control; /* 20 */
    u16 m_pad22;
} ata_hwport_t;

#define USE_ATA_REGS ata_hwport_t *ata_hwport = \
                         (ata_hwport_t *)0x01010140

int sceAtaGetSceId(int device, void *data)
{
    u16 *param             = ata_param; /* Temporary buffer referenced by sceAtaSecuritySetPassword, sceAtaSecurityUnLock, etc. */
    int res;
    USE_ATA_REGS;

    /* Other buffer contents can also be specified */
    memset(param, 0xFF, 512);

    if ((res = sceAtaExecCmd(param, 1, 0, 0, 0, 0, 0, (device << 4) & 0xffff, ATA_C_WRITE_BUFFER)) != 0)
        return res;
    if ((res = sceAtaWaitResult()) != 0)
        return res;
    if ((res = sceAtaExecCmd(data, 1, 0, 0, 0, 0, 0, (device << 4) & 0xffff, ATA_C_READ_BUFFER)) != 0)
        return res;
    ata_hwport->r_feature = ATA_SCE_IDENTIFY_DRIVE & 0xff;
    ata_hwport->r_nsector = 0 & 0xff;
    ata_hwport->r_sector  = 0 & 0xff;
    ata_hwport->r_lcyl    = 0 & 0xff;
    ata_hwport->r_hcyl    = 0 & 0xff;
    ata_hwport->r_select  = (((((device & 1) ^ 1) /* opposite: secondary */ << 4) & 0xffff) | ATA_SEL_LBA) & 0xff;
    ata_hwport->r_command = ATA_C_SCE_SECURITY_CONTROL & 0xff;
    ata_hwport->r_select  = (((((device & 1) ^ 0) /* regular: primary */ << 4) & 0xffff) | ATA_SEL_LBA) & 0xff;
    res = sceAtaWaitResult();
    return res;
}
```