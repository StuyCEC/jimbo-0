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

    def check_reg(self, r1: int) -> bool:
        if ((r1> -1) and (r1< 4)):
            return True
        return False

    def mov(self, r1, mode) -> None:
        check_reg = self.check_reg
        readmem = self.readmem
        PC = self.PC
        REG = self.REG

        if (self.check_reg(r1)):
            if (mode == 0):
                r2 = readmem(PC)
                if (check_reg(r2)):
                    REG[r1] = REG[r2]

            elif (mode == 1):
                REG[r1] = readmem(PC)

            else:
                print("INVALID Z VALUE")
                return

            PC += 1
            PC %= 0xFFF

        else:
            print("INVALID REG (R1)")







if __name__ == "__main__":
    pass



