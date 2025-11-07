; Parking Management System - Single-digit hours (0-9), actual duration, totals only
.model small
.stack 100h

.data
menu    db '***************** PARKING MANAGEMENT SYSTEM *****************',0Dh,0Ah,'$'
menu1   db 'Press 1 for Auto',0Dh,0Ah,'$'
menu2   db 'Press 2 for Car',0Dh,0Ah,'$'
menu3   db 'Press 3 for Bus',0Dh,0Ah,'$'
menu4   db 'Press 4 to Show Records & Compute Payment',0Dh,0Ah,'$'
menu5   db 'Press 5 to Delete Records',0Dh,0Ah,'$'
menu6   db 'Press 6 to Exit',0Dh,0Ah,'$'

msg_full       db 'Parking is Full (Max 8 Vehicles)',0Dh,0Ah,'$'
msg_wrong      db 'Wrong Input',0Dh,0Ah,'$'
msg_space      db 'Vehicle Parked Successfully',0Dh,0Ah,'$'
msg_total      db 0Dh,0Ah,'Total Amount = $'
msg_totveh     db 0Dh,0Ah,'Total Number of Vehicles Parked = $'
msg_deleted    db '*** All Records Deleted Successfully ***',0Dh,0Ah,'$'
msg_entry      db 'Enter entry hour (0-9): $'
msg_exit       db 'Enter leaving hour (0-9): $'
crlf           db 0Dh,0Ah,'$'

; Arrays (max 8)
vehicle_type    db 8 dup(0)
entry_hour      db 8 dup(0)
paid_flag       db 8 dup(0)
vehicle_payment dw 8 dup(0)

; Counters
count  dw 0
amount dw 0

.code
start:
    mov ax,@data
    mov ds,ax
    mov es,ax

main_loop:
    ; Display menu
    lea dx, menu
    mov ah,09h
    int 21h
    lea dx, menu1
    mov ah,09h
    int 21h
    lea dx, menu2
    mov ah,09h
    int 21h
    lea dx, menu3
    mov ah,09h
    int 21h
    lea dx, menu4
    mov ah,09h
    int 21h
    lea dx, menu5
    mov ah,09h
    int 21h
    lea dx, menu6
    mov ah,09h
    int 21h

    ; Read choice
    mov ah,01h
    int 21h
    mov bl,al
    ; newline
    lea dx, crlf
    mov ah,09h
    int 21h

    cmp bl,'1'
    je park_vehicle
    cmp bl,'2'
    je park_vehicle
    cmp bl,'3'
    je park_vehicle
    cmp bl,'4'
    je show_record
    cmp bl,'5'
    je delete_record
    cmp bl,'6'
    je exit_program

    lea dx,msg_wrong
    mov ah,09h
    int 21h
    jmp main_loop

; -----------------------
; Park Vehicle
; -----------------------
park_vehicle:
    mov ax,count
    cmp ax,8
    jae parking_full

    sub bl,'0'           ; convert ASCII to 1..3
    mov si,count
    mov di,offset vehicle_type
    add di,si
    mov [di],bl

    ; Read single-digit entry hour
    lea dx,msg_entry
    mov ah,09h
    int 21h
    mov ah,01h
    int 21h
    sub al,'0'
    cmp al,9
    ja invalid_hour
    mov di,offset entry_hour
    add di,si
    mov [di],al

    ; mark unpaid
    mov di,offset paid_flag
    add di,si
    mov byte ptr [di],0

    inc word ptr count

    lea dx,msg_space
    mov ah,09h
    int 21h
    jmp main_loop

invalid_hour:
    lea dx,msg_wrong
    mov ah,09h
    int 21h
    jmp main_loop

parking_full:
    lea dx,msg_full
    mov ah,09h
    int 21h
    jmp main_loop

; -----------------------
; Show Records & Compute Payment
; -----------------------
show_record:
    mov ax,count
    cmp ax,0
    je no_vehicles
    xor si,si
    mov word ptr amount,0

show_loop:
    ; skip if already paid
    mov di,offset paid_flag
    add di,si
    cmp byte ptr [di],1
    je cont_next

    ; Read single-digit exit hour
    lea dx,msg_exit
    mov ah,09h
    int 21h
    mov ah,01h
    int 21h
    sub al,'0'
    cmp al,9
    ja invalid_exit
    mov bl,al    ; exit hour

    ; get entry hour
    mov di,offset entry_hour
    add di,si
    mov al,[di]  ; entry hour

    ; compute duration = (exit - entry) mod 10
    mov cl,bl
    sub cl,al
    jc add_10
    jmp duration_ok
add_10:
    add cl,10
duration_ok:

    ; compute charge
    mov di,offset vehicle_type
    add di,si
    mov al,[di]
    cmp al,1
    je rate_auto
    cmp al,2
    je rate_car
    mov bx,400
    jmp do_mul
rate_auto: mov bx,200
jmp do_mul
rate_car: mov bx,300

do_mul:
    mov al,cl
    xor ah,ah
    mul bx
    mov di,offset vehicle_payment
    mov bx,si
    shl bx,1
    add di,bx
    mov [di],ax
    add word ptr amount,ax

    mov di,offset paid_flag
    add di,si
    mov byte ptr [di],1

cont_next:
    inc si
    mov ax,count
    cmp si,ax
    jb show_loop

    ; print totals only
    lea dx,msg_total
    mov ah,09h
    int 21h
    mov ax,amount
    call print_num
    lea dx,crlf
    mov ah,09h
    int 21h

    lea dx,msg_totveh
    mov ah,09h
    int 21h
    mov ax,count
    call print_num
    lea dx,crlf
    mov ah,09h
    int 21h

    jmp main_loop

invalid_exit:
    lea dx,msg_wrong
    mov ah,09h
    int 21h
    jmp main_loop

no_vehicles:
    lea dx,msg_wrong
    mov ah,09h
    int 21h
    jmp main_loop

; -----------------------
; Delete Records
; -----------------------
delete_record:
    mov word ptr count,0
    mov di,offset vehicle_type
    mov cx,8
    xor al,al
    rep stosb
    mov di,offset entry_hour
    mov cx,8
    rep stosb
    mov di,offset paid_flag
    mov cx,8
    rep stosb
    mov di,offset vehicle_payment
    mov cx,8
clrpay: mov word ptr [di],0
    add di,2
    loop clrpay
    mov word ptr amount,0
    lea dx,msg_deleted
    mov ah,09h
    int 21h
    jmp main_loop

; -----------------------
exit_program:
    mov ah,4Ch
    int 21h

; -----------------------
; print_num (AX = number 0–65535)
; -----------------------
print_num proc
    push ax
    push bx
    push cx
    push dx
    push si
    cmp ax,0
    jne pn_conv
    mov dl,'0'
    mov ah,02h
    int 21h
    jmp pn_done
pn_conv:
    xor cx,cx
    mov bx,10
pn_div:
    xor dx,dx
    div bx
    push dx
    inc cx
    cmp ax,0
    jne pn_div
pn_print:
    pop dx
    add dl,'0'
    mov ah,02h
    int 21h
    loop pn_print
pn_done:
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
print_num endp

end start
