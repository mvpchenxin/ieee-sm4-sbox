# SM4 SBOX
This is a verilog implementation of SM4 S-box based on the paper of The logic design of SM4 S-box in hardware based on asymmetric matrix expansion and sub-module series connection

- sbox.v is the verilog code provided in this paper;
- sbox_ref.v is a reference.
- tb_sbox.v is a simple testbench to verify the implementation.


# SM4 SBOX TEST
In order to realize the performance test of the designed S-box, implementation of high-speed SM4 encryption and decryption on FPGA.

- rtl file is the solution verilog code in SM4;
- testbench file is the test file.

## toplevel

|   Signal Name   |   Description   |
| ---- | ---- |
|   CLK_i   |   System clock   |
|   RST_N_i   |   Asynchronous reset input (falling edge valid)   |
|   MK_i   |   128bit key input   |
|   MK_VALID_i   |   Indicates whether the key input is valid   |
|   DAT_i   |   128 bit plaintext (ciphertext) block input   |
|   DAT_VALID_i   |   Indicates whether the block input is valid   |
|   DAT_o   |   128 bit ciphertext (plaintext) block output   |
|   DAT_READY_o   |   Indicates whether the block output is valid   |

## overviev of internal modules

`keyexp` implements key expansion algorithm.

`decenc` implements nonlinear iterative algorithm for encryption / decryption.



