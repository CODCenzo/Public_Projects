main:
    #O registrador x0 é sempre zero
    # vetor(base) = 0x00000

    #O programa manipula um vetore possui um loop

    addi x1, x0, 0 # x1 = 0x00000, carrega o endereço base do vetor
    addi x2, x0, 3
    addi x3, x0, 0
    
    addi x5, x0, 12
    addi x6, x0, 25

    add x10, x5, x6 #x10 = 12 + 25
    sw x10, 0(x1) #vet[0] = 37

    sub x10, x6, x5
    sw x10, 4(x1)

    and x10, x6, x5
    sw x10, 8(x1)

    or x10, x6, x5
    sw x10, 12(x1)

    lw x5, 0(x1)
    lw x6, 4(x1)
    add x10, x5, x6
    sw x10, 16(x1)

label
    addi x3,x3,1
    beq x2,x3,label #Pular para o PC 17
    sw x3, 20(x1)

    ecall
