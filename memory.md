- there are sectors of 128 kbytes each
- 1 page == 512 bytes pages == 256 pages == 1 sectors 

- 1 page == 256 words

- 4096 pa

- the first sectore is divided into 3 parts
  - 8kb
  - 8kb
  - 112kb

- 1 word = 2 bytes 

- Flash write can only convert 1 to 0
- Flash erase sets all bits to 0

- internal rom : 0x00800000
- flash : 0x400000

- lock and unlock flash addresses to protect data from write and erase.

- When security is enabled, any access to the Flash,
SRAM, core registers and internal peripherals, either through the SW-DP, the ETM interface or the Fast Flash
Programming Interface, is blocked.


- Erase by 
  - Erase al memory
  - Erase by pages 
  - Erase by sectors
