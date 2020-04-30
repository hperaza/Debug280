# RSX280 debugger

This is a simple debugger that illustrates the use of the hardware
breakpoint-on-halt and single-step capabilities of the Zilog Z280 chip.

The core of the debugger is based on the very simple debug monitor built in
the RSX280 operating system. This version runs in standalone mode using the
Z280 internal UART to communicate with the user's terminal, and assumes that
the CPU is in Z80 mode (MMU disabled). That means also that the task to be
debugged shares the same 64K address space with the debugger. Nevertheless,
the code should not be difficult to modify so that it makes full use of the
Z280 MMU and system/user modes. In fact, part of the code is already there
(i.e. the MMU state is saved/restored when entering/leaving a trap, LDUP is
used by the dump command to display memory from another bank, etc.)

The debugger includes a Z280 disassembler in a separate module that could
be used by other applications.

As built, the debugger runs from address 0100h and uses memory up to 177Fh
approximately (most of the space is taken up by the disassembler module);
page zero is reserved for the Z280 interrupt/trap table. The code can be
easily modified to run at a different address.

The debugger has been tested with Bill Shen's Z280RC board (a HEX file is
included), and will be embedded into a future version of ZZmon.

## Debugger commands:

 * D*addr1*,*addr2* -- Dump memory from *addr1* to *addr2* included. If
   *addr1* is omitted, the dump will continue from where the last dump
   operation stopped, or from the current contents of the user HL register
   after a breakpoint hit or a single-step operation. If *addr2* is omitted,
   a full 256-byte page will be output.
 * F*addr1*,*addr2*,*byte* -- Fill memory region from *addr1* to *addr2*
   inclusive with *byte*. Any omitted parameter is assumed to be zero.
 * S*addr1*,*addr2*,*byte* -- Search memory region from *addr1* to *addr2*
   inclusive for *byte*. Any omitted parameter is assumed to be zero.
 * E*addr* -- Examine/modify memory starting from *addr*. The address of the
   memory location is displayed, followed by the old contents. Entering a new
   value modifies the contents, pressing enter on an empty line leaves the
   contents unchanged. End with Ctrl-C or a dot.
 * B*addr* -- Place a breakpoint (HALT instruction) at address *addr*. The
   current version of the debugger supports only one breakpoint at a time.
   Thus, if the B command is used to enter additional breakpoints, the
   instructions at the address of the older breakpoints will not be restored
   when the corresponding breakpoint is hit. Breakpoints are not persistent,
   meaning that once hit, it will not be triggered again. That means that
   you can place a second breakpoint once the first is hit.
 * G*addr* -- Go to address *addr*. Starts executing the user code until a
   breakpoint is hit. If *addr* is not specified, the execution will continue
   from the current user PC value.
 * T -- Trace. Once a breakpoint is hit, use this command to single-step
   through the code. To exit single-stepping, use the G command without
   arguments.
 * I*port* -- Input value from *port*.
 * O*port*,*value* -- Output *value* to *port*.
 * W*addr* -- Display 16-bit word contents at the memory location *addr*.
 * X -- Display the contents of the user registers.
 * X*rp* -- Examine/modify register pair *rp*.
 * XM -- Display the user MMU state.
 * Z*addr*,*n* -- Disassemble *n* instructions starting from *addr*. If
   *addr* is not specified, the disassembly will continue from there the
   last Z command ended, or from the current user PC after a breakpoint or
   single-step operation. If *n* is omitted, the next 16 instructions are
   displayed.

## Example usage on the Z280RC board:

 1 Reset the Z280RC to get the ZZmon prompt, and send the debug180.hex file
   using your favorite terminal program.
 2 Load the program you want to debug, ensuring it will not overlay the
   debugger.
 3 Use the G0100 command to start the debugger.
 4 At the Z280> prompt, use the B command to set a breakpoint on your
   program.
 5 Start your program with the G command.
 6 When the breakpoint is hit, the CPU register contents will be displayed
   followed by the mnemonics of the instruction to be executed next (i.e.
   where the CPU stopped).
 7 Use the T command to single-step though the code, etc.

You can also start by setting the PC value with the XPC command followed by
the T command to go directly into single-step mode, etc.

