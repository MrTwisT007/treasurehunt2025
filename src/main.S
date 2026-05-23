   .global secret_start
   .global secret_byte
   .global secret_end
   .global payload_start
   .global payload_end
   .global hint_start
   .global hint_end
   .global dtmf_start
   .global dtmf_end
   .global unlock_start
   .global unlock_end
   .global src_start
   .global src_end
secret_start:
   .incbin __secret__
secret_byte:
   .byte 0xA5
   .byte 0x3C
   .byte 0x2D
secret_end:
payload_start:
   .incbin __payload__
payload_end:
hint_start:
   .incbin __hint__
hint_end:
dtmf_start:
   .incbin __dtmf__
dtmf_end:
unlock_start:
   .incbin __unlock__
unlock_end:
src_start:
   .incbin __srcblock__
src_end:
