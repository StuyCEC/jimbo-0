class CPU():
    def __init__(self, rompath: str, debug: bool):
        #Registers
        self.REG = [0] * 4

        #Flags
        self.ZERO = False
        self.CARRY = False
        self.BORROW = False

        #Instruction Register
        self.IX = 0
        self.IY = 0
        self.IZ = 0

        #ALU Registers
        self.ALU_A = 0
        self.ALU_B = 0

        #Program Counter
        self.PC = 0 

        #Memory
        self.ROM = [0] * 1024
        self.RAM = [0] * 1006
        
        #Stack Adresses
        self.SPL = 0xFEE
        self.SPH = 0xFEF

    def readmem(self, address) -> int:
        if (address <= 0x3FF):
            return self.ROM[address]
        elif ((address >= 0x400) and (address <= 0xFFF)):
            return self.RAM[address]
        else: 
            return 0

    def fetch_instruction(self) -> None:
        byte = self.readmem(self.PC)
        self.IX = int(format(byte, '#010b')[2:7], 2)
        self.IY = int(format(byte, '#010b')[7:9], 2)
        self.IZ = int(format(byte, '#010b')[9], 2)
        self.PC += 1
        self.PC %= 0xFFF

    def check_reg(self, register: int) -> bool:
        if reg > -1 and reg < 4:
            return True
        return False

if __name__ == "__main__":
    pass



