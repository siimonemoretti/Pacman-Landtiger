#ifndef _CAN_H_
#define _CAN_H_

#define STANDARD_FORMAT  0
#define EXTENDED_FORMAT  1

#define DATA_FRAME       0
#define REMOTE_FRAME     1
extern uint32_t result;
extern uint8_t icr;

typedef struct  {
  unsigned int   id;                    /* 29 bit identifier */
  unsigned char  data[8];               /* Data field */
  unsigned char  len;                   /* Length of data field in bytes */
  unsigned char  format;                /* 0 - STANDARD, 1- EXTENDED IDENTIFIER */
  unsigned char  type;                  /* 0 - DATA FRAME, 1 - REMOTE FRAME */
} CAN_msg;

/* Functions defined in module CAN.c */
void CAN_init 				 (void);
void CAN_setup         (uint32_t ctrl);
void CAN_start         (uint32_t ctrl);
void CAN_waitReady     (uint32_t ctrl);
void CAN_wrMsg         (uint32_t ctrl, CAN_msg *msg);
void CAN_rdMsg         (uint32_t ctrl, CAN_msg *msg);
void CAN_wrFilter      (uint32_t ctrl, uint32_t id, uint8_t filter_type);

extern CAN_msg       CAN_TxMsg;      /* CAN messge for sending */
extern CAN_msg       CAN_RxMsg;      /* CAN message for receiving */                                

#endif // _CAN_H_


