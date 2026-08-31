# ATAEmu command table (2.11)

## Table Structure

Each entry in the table contains four values:

```c
struct ataemu_feature_table_t {
  uint8_t feature        // Feature
  uint8_t pad[3]         // Padding
  uint32_t type          // Command type. Seems to closely match PS2 command types
                          // 1, 6  = non-data command,
                          // 2     = read command,
                          // 3, 10 = write command,
                          // 4     = DMA command,
                          // 5     = command requires no handling
  int32_t nsectorFixup   // Adjusts the nsector value:
                          // -1    = set to 0x100 if nsector is 0, passthrough otherwise
                          //  0/1  = always set to 1
  uint32_t emulationType // Emulation mode for the command:
                          // 0     = not implemented,
                          // 1     = implemented,
                          // 2     = passed through to the HDD
}
```

## Additional Tables

### SMART Command Table (0x1005b9cc)

| Feature | Type | nsector | Emulation type | ATA command                   |
| ------- | ---- | ------- | -------------- | ----------------------------- |
| 0xD0    | 2    | 1       | 1              | SMART READ DATA               |
| 0xD2    | 1    | 1       | 1              | SMART READ_THRESHOLDS         |
| 0xD3    | 1    | 1       | 1              | SMART ENABLE_DISABLE_AUTOSAVE |
| 0xD4    | 1    | 1       | 1              | SMART SAVE_ATTRIBUTE_VALUES   |
| 0xD5    | 2    | 1       | 1              | SMART EXECUTE_OFFLINE_DIAGS   |
| 0xD6    | 3    | 1       | 1              | SMART READ_LOG                |
| 0xD8    | 1    | 1       | 1              | SMART WRITE_LOG               |
| 0xD9    | 1    | 1       | 1              | SMART WRITE_THRESHOLDS        |
| 0xDA    | 1    | 1       | 1              | SMART ENABLE_OPERATIONS       |

### SCE Security Command Table (0x1005ba5c)

| Feature | Type | nsector | Emulation type | ATA command            |
| ------- | ---- | ------- | -------------- | ---------------------- |
| 0xEC    | 2    | 1       | 1              | IDENTIFY DRIVE         |
| 0xED    | 3    | 1       | 1              | ?                      |
| 0xEE    | 1    | 1       | 1              | ?                      |
| 0xF3    | 1    | 1       | 0              | SECURITY ERASE PREPARE |
| 0xF4    | 1    | 1       | 0              | SECURITY ERASE UNIT    |
| 0xF5    | 1    | 1       | 0              | SECURITY FREEZE LOCK   |
| 0xF1    | 3    | 1       | 0              | SECURITY SET PASSWORD  |
| 0xF2    | 3    | 1       | 0              | SECURITY UNLOCK        |
| 0x30    | 3    | 1       | 0              | SECURITY READ ID       |
| 0x20    | 2    | 1       | 0              | SECURITY WRITE ID      |

## Complete Main Command Table (0x1005bafc)

| Index | Feature | Type | nsector | Emulation type | ATA command                      |
| ----- | ------- | ---- | ------- | -------------- | -------------------------------- |
| 0     | 0x00    | 1    | 1       | 0              | (Unimplemented)                  |
| 1     | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 2     | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 3     | 0x03    | 1    | 1       | 0              | (Unimplemented)                  |
| 4     | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 5     | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 6     | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 7     | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 8     | 0x08    | 5    | 1       | 2              | DEVICE RESET                     |
| 9     | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 10    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 11    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 12    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 13    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 14    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 15    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 16    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 17    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 18    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 19    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 20    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 21    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 22    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 23    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 24    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 25    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 26    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 27    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 28    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 29    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 30    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 31    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 32    | 0x20    | 2    | -1      | 1              | READ SECTOR(S)                   |
| 33    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 34    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 35    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 36    | 0x24    | 2    | -1      | 1              | READ SECTOR(S) EXT               |
| 37    | 0x25    | 4    | 1       | 1              | READ DMA EXT                     |
| 38    | 0x26    | 7    | 1       | 1              | READ DMA QUEUED EXT              |
| 39    | 0x27    | 1    | 1       | 1              | READ NATIVE MAX ADDRESS EXT      |
| 40    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 41    | 0x29    | 2    | -1      | 1              | READ MULTIPLE EXT                |
| 42    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 43    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 44    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 45    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 46    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 47    | 0x2F    | 2    | -1      | 1              | READ LOG EXT                     |
| 48    | 0x30    | 3    | -1      | 1              | WRITE SECTOR(S)                  |
| 49    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 50    | 0x32    | 10   | 1       | 0              | DATA SET MANAGEMENT              |
| 51    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 52    | 0x34    | 3    | -1      | 1              | WRITE SECTOR(S) EXT              |
| 53    | 0x35    | 4    | 1       | 1              | WRITE DMA EXT                    |
| 54    | 0x36    | 7    | 1       | 1              | WRITE DMA QUEUED EXT             |
| 55    | 0x37    | 1    | 1       | 1              | SET MAX ADDRESS EXT              |
| 56    | 0x38    | 3    | -1      | 0              | CFA WRITE SECTORS WITHOUT ERASE  |
| 57    | 0x39    | 3    | -1      | 1              | WRITE MULTIPLE EXT               |
| 58    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 59    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 60    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 61    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 62    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 63    | 0x3F    | 3    | -1      | 1              | WRITE LOG EXT                    |
| 64    | 0x40    | 1    | -1      | 0              | READ VERIFY SECTOR(S)            |
| 65    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 66    | 0x42    | 1    | -1      | 0              | READ VERIFY SECTOR(S) EXT        |
| 67    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 68    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 69    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 70    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 71    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 72    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 73    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 74    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 75    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 76    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 77    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 78    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 79    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 80    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 81    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 82    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 83    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 84    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 85    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 86    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 87    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 88    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 89    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 90    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 91    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 92    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 93    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 94    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 95    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 96    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 97    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 98    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 99    | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 100   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 101   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 102   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 103   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 104   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 105   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 106   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 107   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 108   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 109   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 110   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 111   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 112   | 0x70    | 1    | 1       | 2              | SEEK                             |
| 113   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 114   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 115   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 116   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 117   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 118   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 119   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 120   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 121   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 122   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 123   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 124   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 125   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 126   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 127   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 128   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 129   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 130   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 131   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 132   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 133   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 134   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 135   | 0x87    | 2    | 1       | 0              | CFA TRANSLATE SECTOR             |
| 136   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 137   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 138   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 139   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 140   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 141   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 142   | 0x8E    | 9    | 1       | 1              | SCE SECURITY CONTROL             |
| 143   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 144   | 0x90    | 6    | 1       | 0              | EXECUTE DEVICE DIAGNOSTIC        |
| 145   | 0x91    | 1    | 1       | 0              | INITIALIZE DEVICE PARAMETERS     |
| 146   | 0x92    | 3    | 1       | 0              | DOWNLOAD MICROCODE               |
| 147   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 148   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 149   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 150   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 151   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 152   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 153   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 154   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 155   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 156   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 157   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 158   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 159   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 160   | 0xA0    | 8    | 1       | 0              | PACKET                           |
| 161   | 0xA1    | 2    | 1       | 0              | IDENTIFY PACKET DEVICE           |
| 162   | 0xA2    | 8    | 1       | 0              | SERVICE                          |
| 163   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 164   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 165   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 166   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 167   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 168   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 169   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 170   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 171   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 172   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 173   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 174   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 175   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 176   | 0xB0    | 9    | 1       | 1              | SMART                            |
| 177   | 0xB1    | 9    | 1       | 0              | DEVICE CONFIGURATION OVERLAY     |
| 178   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 179   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 180   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 181   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 182   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 183   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 184   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 185   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 186   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 187   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 188   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 189   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 190   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 191   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 192   | 0xC0    | 1    | -1      | 0              | CFA ERASE SECTORS                |
| 193   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 194   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 195   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 196   | 0xC4    | 2    | -1      | 0              | READ MULTIPLE                    |
| 197   | 0xC5    | 3    | -1      | 0              | WRITE MULTIPLE                   |
| 198   | 0xC6    | 1    | 1       | 0              | SET MULTIPLE MODE                |
| 199   | 0xC7    | 7    | 1       | 0              | READ DMA QUEUED                  |
| 200   | 0xC8    | 4    | 1       | 1              | READ DMA                         |
| 201   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 202   | 0xCA    | 4    | 1       | 1              | WRITE DMA                        |
| 203   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 204   | 0xCC    | 7    | 1       | 0              | WRITE DMA QUEUED                 |
| 205   | 0xCD    | 3    | -1      | 0              | CFA WRITE MULTIPLE WITHOUT ERASE |
| 206   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 207   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 208   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 209   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 210   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 211   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 212   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 213   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 214   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 215   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 216   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 217   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 218   | 0xDA    | 1    | 1       | 0              | GET MEDIA STATUS                 |
| 219   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 220   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 221   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 222   | 0xDE    | 1    | 1       | 0              | MEDIA LOCK                       |
| 223   | 0xDF    | 1    | 1       | 0              | MEDIA UNLOCK                     |
| 224   | 0xE0    | 1    | 1       | 2              | STANDBY IMMEDIATE                |
| 225   | 0xE1    | 1    | 1       | 2              | IDLE IMMEDIATE                   |
| 226   | 0xE2    | 1    | 1       | 2              | STANDBY                          |
| 227   | 0xE3    | 1    | 1       | 2              | IDLE                             |
| 228   | 0xE4    | 2    | 1       | 0              | READ BUFFER                      |
| 229   | 0xE5    | 1    | 1       | 2              | CHECK POWER MODE                 |
| 230   | 0xE6    | 1    | 1       | 2              | SLEEP                            |
| 231   | 0xE7    | 1    | 1       | 2              | FLUSH CACHE                      |
| 232   | 0xE8    | 3    | 1       | 0              | WRITE BUFFER                     |
| 233   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 234   | 0xEA    | 1    | 1       | 2              | FLUSH CACHE EXT                  |
| 235   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 236   | 0xEC    | 2    | 1       | 1              | IDENTIFY DEVICE                  |
| 237   | 0xED    | 1    | 1       | 0              | MEDIA EJECT                      |
| 238   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 239   | 0xEF    | 1    | 1       | 2              | SET FEATURES                     |
| 240   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 241   | 0xF1    | 3    | 1       | 0              | SECURITY SET PASSWORD            |
| 242   | 0xF2    | 3    | 1       | 0              | SECURITY UNLOCK                  |
| 243   | 0xF3    | 1    | 1       | 0              | SECURITY ERASE PREPARE           |
| 244   | 0xF4    | 3    | 1       | 0              | SECURITY ERASE UNIT              |
| 245   | 0xF5    | 1    | 1       | 0              | SECURITY FREEZE LOCK             |
| 246   | 0xF6    | 3    | 1       | 0              | SECURITY DISABLE PASSWORD        |
| 247   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 248   | 0xF8    | 1    | 1       | 0              | READ NATIVE MAX ADDRESS          |
| 249   | 0xF9    | 1    | 1       | 0              | SET MAX ADDRESS                  |
| 250   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 251   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 252   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 253   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 254   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
| 255   | 0x00    | 0    | 0       | 0              | (Unimplemented)                  |
