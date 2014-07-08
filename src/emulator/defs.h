//conversion ratio
#define BYTE_SIZE  			8
#define WORD_SIZE  			32
#define INT_SIZE   			WORD_SIZE
#define BYTES_IN_WORD       4
#define FIRST_BYTE_IN_WORD  0
#define LAST_BYTE_IN_WORD   3

//Registers
#define FIRST_REG 			0
#define LAST_REG			12
#define DOUBLE_DIG_REG		10
#define REG_SIZE 			4
#define NUMBER_OF_REGISTERS 17

//Memory constants
#define MEM_ADDR			0
#define MAX_MEMORY_SIZE 	(1 << 14)

//GPIO physical addresses
//Note that they converted to virtual memory locations in convertGpioAddress
#define GPIO_20_29			0x20200008
#define GPIO_10_19			0x20200004
#define GPIO_0_9			0x20200000

#define GPIO_CLEAR			0x20200028
#define GPIO_SET			0x2020001C

//general for all instructions
#define INS_TYPE_START   	26
#define INS_TYPE_STOP    	27
#define INS_TYPE_SIZE 		(INS_TYPE_STOP - INS_TYPE_START + 1)
#define INS_CON_START 		28
#define INS_CON_SIZE 		4


//DataProcessing
#define DP_IMM 				25
#define DP_OPCODE_START 	21
#define DP_SET 				20
#define DP_RN_START 		16
#define DP_RD_START 		12


//Multiply
#define ACC_START           21
#define SET_START			20
#define MP_RD_START			16
#define MP_RN_START 		12
#define MP_RS_START			 8
#define MP_RM_START			 0

//Single Data Transfer
#define SDT_IMM				25
#define SDT_PFLAG			24
#define	SDT_UFLAG			23
#define SDT_LFLAG			20
#define SDT_RN_START		16
#define SDT_RD_START		12
#define SDT_OFFSET_START     0

//Branch
#define BR_OFFSET_START         0
#define BR_OFFSET_STOP         23
#define BR_OFFSET_LENGTH       23

//Flags
#define NFLAG				31
#define ZFLAG 				30
#define CFLAG 				29
#define VFLAG				28
//ignore flag in decimal
#define AL_FLAG_VAL     	14



