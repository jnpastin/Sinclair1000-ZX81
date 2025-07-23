out ($FD),a
ld bc,32767
jp $03CB
ld hl,($4016)
ld()$4018,hl
jr $46
and a
jp nz,$07F1
jp $07F5
rst 38h
ld hl,($4016)
ld a,(hl)
and a
ret nz
nop
nop
call 73
jr $F7
rst 38h
rst 38h
rst 38h
jp $199D
pop af
exx
ex (sp),hl
exx
ret
push bc
ld hl,($4014)
push hl
jp $1488
dec c
jp nz,$0045
pop hl
dec b
ret z
set 3,c
ld r,a
ei
jp (hl)
pop de
ret z
jr $F8
ld hl,($4016)
inc hl
ld()$4016,hl
ld a,(hl)
cp 127
ret nz
jr $F6
pop hl
lh l,(hl)
ld (iy+$00),l
ld sp,($4002)
call 519
jp $14BC
rst 38h
ex af,af
inc a
jp m,$006D
jr z,$02
ex af,af
ret
ex af,af
push af
push bc
push de
push hl
ld hl,($400C)
set 7,h
halt
out ($FD),a
jp (ix
ccf
dec a
jr z,$3B
ld h,56
add hl,hl
dec hl
inc l
ld (hl),60
ld hl,($3937)
dec e
ld e,31
jr nz,$21
inc e
dec h
inc h
inc hl
ld()$3435,hl
ld l,58
ld a,118
ld sp,12080
dec l
nop
dec de
ld ($2733),a
ld c,25
rrca
jr $E3
pop hl
call po,58085
ret nz
exx
ret po
in a,($DD)
ld (hl),l
jp c,$DFDE
ld (hl),d
ld (hl),a
ld (hl),h
ld (hl),e
ld (hl),b
ld (hl),c
dec bc
ld de,3344
call c,5241
dec d
ld d,216
inc c
ld a,(de)
ld (de),a
inc de
rla
call 49614
ld a,b
jp z, $CCCB
pop de
jp nc,$C8C7
ret
rst 08h
ld b,b
ld a,b
ld a,b
ld a,b
ld a,b
ld a,b
ld a,b
ld a,b
ld a,b
ld a,b
ld a,b
jp nz,$C4D3
sub 213
ld a,b
call nc,50630
ret nc
ld a,b
ld a,b
ld b,d
rst 10h
ld b,c
ex af,af
ld a,(bc)
add hl,bc
adc a,d
adc a,c
add a,c
add a,d
rlca
add a,h
ld b,1
ld (bc),a
add a,a
inc b
dec b
ld (hl),a
ld a,b
add a,l
inc bc
add a,e
adc a,e
sub c
sub b
adc a,l
add a,(hl)
ld a,b
sub d
sub l
sub (hl)
adc a,b
adc a,a
dec bc
adc a,e
ld h,185
add hl,sp
ld h,167
adc a,a
jr z,$34
add hl,hl
xor d
dec sp
ld h,177
ld sp,45866
jr c, $2E
or e
jr z,$34
cp b
add hl,sp
ld h,179
ld h,56
or e
ld h,40
cp b
ld h,57
or e
ld sp,10931
dec a
or l
ld l,51
cp c
jr c, $36
or a
jr c, $2C
or e
ld h,39
cp b
dec (hl)
ld hl,($B02A)
ld a,(46904)
jr c, $39
scf
adc a,l
jr z,$2D
scf
adc a,l
inc sp
inc (hl)
cp c
rla
sub a
inc (hl)
or a
ld h,51
xor c
inc de
sub h
ld (de),a
sub h
inc de
sub d
add hl,sp
dec l
ld hl,($39B3)
or h
jr c, $39
ld hl,($31B5)
dec (hl)
scf
ld l,51
cp c
ld sp,11825
jr c, $B9
jr c, $39
inc (hl)
or l
jr c, $31
inc (hl)
cp h
dec hl
ld h,56
cp c
inc sp
ld hl,($38BC)
jr z,$37
inc (hl)
ld sp,10417
inc (hl)
inc sp
cp c
add hl,hl
ld l,178
scf
ld hl,($2BB2)
inc (hl)
or a
inc l
inc (hl)
add hl,sp
or h
inc l
inc (hl)
jr c, $3A
and a
ld l,51
dec (hl)
ld a,(12729)
inc (hl)
ld h,169
ld sp,14382
cp c
ld sp,47402
dec (hl)
ld h,58
jr c, $AA
inc sp
ld hl,($B93D)
dec (hl)
inc (hl)
jr nc, $AA
dec (hl)
scf
ld l,51
cp c
dec (hl)
ld sp,47412
scf
ld a,(14515)
ld h,59
xor d
scf
ld h,51
xor c
ld l,171
jr z,$31
cp b
ld a,(13619)
ld sp,47412
jr z,$31
ld hl,($B726)
scf
ld hl,($3A39)
scf
or e
jr z,$34
dec (hl)
cp (hl)
scf
inc sp
xor c
ld l,51
jr nc, $2A
ld a,141
dec (hl)
xor (hl)
inc hl
ex de,hl
ld hl,($4014)
scf
sbc hl,de
ex de,hl
ret nc
pop hl
ld hl,16443
ld a,(hl)
rla
xor (hl)
rla
ret nc
ld a,127
ex af,af
ld b,17
out ($FE),a
djnz $FE
out ($FD),a
ex af,af
rla
jr nc, $08
set 7,(hl)
push af
push bc
push de
push hl
jr $03
res 6,(hl)
ret
ld hl,($4034)
dec hl
ld a,127
and h
or l
ld a,h
jr nz,$03
rla
jr $02
ld b,(hl)
scf
ld h,a
ld()$4034,hl
ret nc
call 699
ld bc,($4025)
ld()$4025,hl
ld a,b
add a,2
sbc hl,bc
ld a,(16423)
or h
or l
ld e,b
ld b,11
ld hl,16443
res 0,(hl)
jr nz,$08
bit 7,(hl)
set 0,(hl)
ret z
dec b
nop
scf
ld hl,16423
ccf
rl b
djnz $FE
ld b,(hl)
ld a,e
cp 254
sbc a,a
ld b,31
or (hl)
and b
rra
ld (hl),a
out ($FF),a
ld hl,($400C)
set 7,h
call 658
ld a,r
ld bc,6401
ld a,245
call 693
dec hl
call 658
jp $0229
pop ix
ld c,(iy+$28)
bit 7,(iy+59)
jr z,$0C
ld a,c
neg
inc a
ex af,af
out ($FE),a
pop hl
pop de
pop bc
pop af
ret
ld a,252
ld b,1
call 693
dec hl
ex (sp),hl
ex (sp),hl
jp (ix
ld r,a
ld a,221
ei
jp (hl)
ld hl,65535
ld bc,65278
in a,(c)
or 1
or 224
ld d,a
cpl
cp 1
sbc a,a
or b
and l
ld l,a
ld a,h
and d
ld h,a
rlc b
in a,(c)
jr c, $ED
rra
rl h
rla
rla
rla
sbc a,a
and 24
add a,31
ld ($4028),a
ret
bit 7,(iy+59)
ret z
halt
out ($FD),a
res 7,(iy+59)
ret
rst 08h
ld c,205
xor b
inc bc
jr c, $F9
ex de,hl
ld de,4811
call 3910
jr nc, $2E
djnz $FE
dec de
ld a,d
or e
jr nz,$F4
call 798
bit 7,(hl)
inc hl
jr z,$F8
ld hl,16393
call 798
call 508
jr $F8
ld e,(hl)
scf
rl e
ret z
sbc a,a
and 5
add a,4
ld c,a
out ($FF),a
ld b,35
djnz $FE
call 3910
jr nc, $72
ld b,30
djnz $FE
dec c
jr nz,$EE
and a
djnz $FD
jr $E0
call 936
rl d
rrc d
call 844
jr $FB
ld c,1
ld b,0
ld a,127
in a,($FE)
out ($FF),a
rra
jr nc, $49
rla
rla
jr c, $28
djnz $F1
pop af
cp d
jp nc,$03E5
ld h,d
ld l,e
call 844
bit 7,d
ld a,c
jr nz,$03
cp (hl)
jr nz,$D6
inc hl
rla
jr nc, $F1
inc (iy+$15)
ld hl,16393
ld d,b
call 844
ld (hl),c
call 508
jr $F6
push de
ld e,148
ld b,26
dec e
in a,($FE)
rla
bit 7,e
ld a,e
jr c, $F5
djnz $F5
pop de
jr nz,$04
cp 86
jr nc, $B2
ccf
rl c
jr nc, $AD
ret
ld a,d
and a
jr z,$BB
rst 08h
inc c
call 3925
ld a,(16385)
add a,a
jp m,$0D9A
pop hl
ret nc
push hl
call 743
call 5112
ld h,d
ld l,e
dec c
ret m
add hl,bc
set 7,(hl)
ret
call 743
ld bc,($4004)
dec bc
ld h,b
ld l,c
ld a,63
ld (hl),2
dec hl
cp h
jr nz,$FA
and a
sbc hl,bc
add hl,bc
inc hl
jr nc, $06
dec (hl)
jr z,$03
dec (hl)
jr z,$F3
ld()$4004,hl
ld hl,($4004)
dec hl
ld (hl),62
dec hl
ld sp,hl11051
ld()$4002,hl
ld a,30
ld i,a
im 1
ld iy,16384
ld (iy+$3B),$40
ld hl,16509
ld()$400C,hl
ld b,25
ld (hl),118
inc hl
djnz $FB
ld()$4010,hl
call 5274
call 5293
call 519
call 2602
ld hl,($400A)
ld de,($4023)
and a
sbc hl,de
ex de,hl
jr nc, $04
add hl,de
ld()$4023,hl
call 2520
jr z,$01
ex de,hl
call 1854
dec (iy+$1E)
jr nz,$37
ld hl,($400A)
call 2520
ld hl,($4016)
scf
sbc hl,de
ld hl,16419
jr nc, $0B
ex de,hl
ld a,(hl)
inc hl
ldi
ld (de),a
jr $C5
ld hl,16394
ld e,(hl)
inc hl
ld d,(hl)
push hl
ex de,hl
inc hl
call 2520
call 1467
pop hl
bit 5,(iy+45)
jr nz,$08
ld (hl),d
dec hl
ld (hl),e
jr $AA
call 5293
ld hl,($4014)
ld a,(hl)
cp 126
jr nz,$08
ld bc,6
call 2656
jr $F3
cp 118
inc hl
jr nz,$EE
call 1335
call 2591
ld hl,($4014)
ld (iy+$00),$FF
call 1894
bit 7,(iy+0)
jr nz,$24
ld a,(16418)
cp 24
jr nc, $1D
inc a
ld ($4022),a
ld b,a
ld c,1
call 2328
ld d,h
ld e,l
ld a,(hl)
dec hl
cp (hl)
jr nz,$FC
inc hl
ex de,hl
ld a,(16389)
cp 77
call c,2653
jr $C9
ld hl,0
ld()$4018,hl
ld hl,16443
bit 7,(hl)
call z,553
bit 0,(hl)
jr z,$FC
ld bc,($4025)
call 3915
call 1981
jr nc, $93
ld a,(16390)
dec a
jp m,$0508
jr nz,$0F
ld ($4006),a
dec e
ld a,e
sub 39
jr c, $01
ld e,a
ld hl,204
jr $0E
ld a,(hl)
cp 118
jr z,$2F
cp 64
set 7,a
jr c, $19
ld hl,199
add hl,de
jr $0D
ld a,(hl)
bit 2,(iy+1)
jr nz,$07
add a,192
cp 230
jr nc, $01
ld a,(hl)
cp 240
jp pe,$052D
ld e,a
call 1335
ld a,e
call 1318
jp $0472
call 2459
ld (de),a
ret
ld a,120
ld e,a
ld hl,1154
add hl,de
add hl,de
ld c,(hl)
inc hl
ld b,(hl)
push bc
ld hl,($4014)
bit 5,(iy+45)
jr nz,$16
res 2,(iy+1)
ld a,(hl)
cp 127
ret z
inc hl
call 1972
jr z,$F6
cp 38
jr c, $F2
cp 222
jr z,$EA
set 2,(iy+1)
jr $E8
ld bc,1
jp $0A60
sbc a,a
dec b
ld d,h
inc b
halt
dec b
ld a,a
dec b
xor a
dec b
call nz,3077
ld b,139
dec b
xor a
dec b
xor a
dec b
call 1427
ld a,(hl)
ld (hl),127
inc hl
jr $09
inc hl
ld a,(hl)
cp 118
jr z,$18
ld (hl),127
dec hl
ld (hl),a
jr $98
call 1427
call 1372
jr $F6
dec hl
ld de,($4014)
ld a,(de)
cp 127
ret nz
pop de
jr $EA
ld hl,($400A)
call 2520
ex de,hl
call 1467
ld hl,16395
jp $0464
ld a,e
and 7
ld ($4006),a
jr $E6
ex de,hl
ld de,1218
ld a,(hl)
and 192
jr nz,$F7
ld d,(hl)
inc hl
ld e,(hl)
ret
call 2591
ld hl,1135
push hl
bit 5,(iy+45)
ret nz
ld hl,($4014)
ld()$400E,hl
ld hl,6177
ld()$4039,hl
ld hl,($400A)
call 2520
call 1467
ld a,d
or e
ret z
dec hl
call 2725
inc hl
ld c,(hl)
inc hl
ld b,(hl)
inc hl
ld de,($400E)
ld a,127
ld (de),a
inc de
push hl
ld hl,29
add hl,de
add hl,bc
sbc hl,sp
pop hl
ret nc
ldir
ex de,hl
pop de
call 5286
jr $91
call 2591
ld hl,1138
bit 5,(iy+45)
jr nz,$11
ld hl,($4014)
ld a,(hl)
cp 255
jr z,$06
call 2274
call 2602
ld hl,1049
push hl
call 3258
pop hl
call 1335
call 1372
call 2675
jr nz,$15
ld a,b
or c
jp nz,$06E0
dec bc
dec bc
ld ($4007),bc
ld (iy+$22),$02
ld de,($400C)
jr $13
cp 118
jr z,$12
ld bc,($4030)
call 2328
ld de,($4029)
ld (iy+$22),$02
rst 18h
cp 118
jp z, $0413
ld (iy+$01),$80
ex de,hl
ld()$4029,hl
ex de,hl
call 77
call 3265
res 1,(iy+1)
ld a,192
ld (iy+$19),a
call 5283
res 5,(iy+45)
bit 7,(iy+0)
jr z,$22
ld hl,($4029)
and (hl)
jr nz,$1C
ld d,(hl)
inc hl
ld e,(hl)
ld ($4007),de
inc hl
ld e,(hl)
inc hl
ld d,(hl)
inc hl
ex de,hl
add hl,de
call 3910
jr c, $C7
ld hl,16384
bit 7,(hl)
jr z,$02
ld (hl),12
bit 7,(iy+56)
call z,2161
ld bc,289
call 2328
ld a,(16384)
ld bc,($4007)
inc a
jr z,$0C
cp 9
jr nz,$01
inc bc
ld ($402B),bc
jr nz,$01
dec bc
call 2027
ld a,24
rst 10h
call 2712
call 5293
jp $04C1
ld ($400A),bc
ld hl,($4016)
ex de,hl
ld hl,1043
push hl
ld hl,($401A)
sbc hl,de
push hl
push bc
call 743
call 2602
pop hl
call 2520
jr nz,$06
call 2546
call 2656
pop bc
ld a,c
dec a
or b
ret z
push bc
inc bc
inc bc
inc bc
inc bc
dec hl
call 2462
call 519
pop bc
push bc
inc de
ld hl,($401A)
dec hl
lddr
ld hl,($400A)
ex de,hl
pop bc
ld (hl),b
dec hl
ld (hl),c
dec hl
ld (hl),e
dec hl
ld (hl),d
ret
set 1,(iy+1)
call 3751
ld a,b
and 63
ld h,a
ld l,c
ld()$400A,hl
call 2520
ld e,0
call 1861
jr $FB
ld bc,($400A)
call 2538
ld d,146
jr z,$05
ld de,0
rl e
ld (iy+$1E),e
ld a,(hl)
cp 64
pop bc
ret nc
push bc
call 2725
inc hl
ld a,d
rst 10h
inc hl
inc hl
ld()$4016,hl
set 0,(iy+1)
ld bc,($4018)
ld hl,($4016)
and a
sbc hl,bc
jr nz,$03
ld a,184
rst 10h
ld hl,($4016)
ld a,(hl)
inc hl
call 1972
ld()$4016,hl
jr z,$E4
cp 127
jr z,$10
cp 118
jr z,$5D
bit 6,a
jr z,$05
call 2379
jr $D3
rst 10h
jr $D0
ld a,(16390)
ld b,171
and a
jr nz,$05
ld a,(16385)
ld b,176
rra
rra
and 1
add a,b
call 2037
jr $B9
cp 126
ret nz
inc hl
inc hl
inc hl
inc hl
inc hl
ret
ld d,0
sra b
sbc a,a
or 38
ld l,5
sub l
add a,l
scf
rr c
jr c, $FA
inc c
ret nz
ld c,b
dec l
ld l,1
jr nz,$F2
ld hl,125
ld e,a
add hl,de
scf
ret
ld a,e
and a
ret m
jr $10
xor a
add hl,bc
inc a
jr c, $FC
sbc hl,bc
dec a
jr z,$F1
ld e,28
add a,e
and a
jr z,$04
res 0,(iy+1)
exx
push hl
bit 1,(iy+1)
jr nz,$05
call 2056
jr $03
call 2129
pop hl
exx
ret
ld d,a
ld bc,($4039)
ld a,c
cp 33
jr z,$1A
ld a,118
cp d
jr z,$30
ld hl,($400E)
cp (hl)
ld a,d
jr nz,$20
dec c
jr nz,$19
inc hl
ld()$400E,hl
ld c,33
dec b
ld ($4039),bc
ld a,b
cp (iy+$22)
jr z,$03
and a
jr nz,$DD
ld l,4
jp $0058
call 2459
ex de,hl
ld (hl),a
inc hl
ld()$400E,hl
dec (iy+$39)
ret
ld c,33
dec b
set 0,(iy+1)
jp $0918
cp 118
jr z,$1C
ld c,a
ld a,(16440)
and 127
cp 92
ld l,a
ld h,64
call z,2161
ld (hl),c
inc l
ld (iy+$38),l
ret
ld d,22
ld hl,($400C)
inc hl
jr $05
ld d,1
ld hl,16444
call 743
push bc
push hl
xor a
ld e,a
out ($FB),a
pop hl
call 3910
jr c, $05
rra
out ($FB),a
rst 08h
inc c
in a,($FB)
add a,a
jp m,$08DE
jr nc, $EE
push hl
push de
ld a,d
cp 2
sbc a,a
and e
rlca
and e
ld d,a
ld c,(hl)
ld a,c
inc hl
cp 118
jr z,$24
push hl
sla a
add a,a
add a,a
ld h,15
rl h
add a,e
ld l,a
rl c
sbc a,a
xor (hl)
ld c,a
ld b,8
ld a,d
rlc c
rra
ld h,a
in a,($FB)
rra
jr nc, $FB
ld a,h
out ($FB),a
djnz $F1
pop hl
jr $D5
in a,($FB)
rra
jr nc, $FB
ld a,d
rrca
out ($FB),a
pop de
inc e
bit 3,e
jr z,$A7
pop bc
dec d
jr nz,$A0
ld a,4
out ($FB),a
call 519
pop bc
ld hl,16476
ld (hl),118
ld b,32
dec hl
ld (hl),0
djnz $FB
ld a,l
set 7,a
ld ($4038),a
ret
ld a,23
sub b
jr c, $0B
cp (iy+$22)
jp c,$0835
inc a
ld b,a
ld a,31
sub c
jp c,$0EAD
add a,2
ld c,a
bit 1,(iy+1)
jr z,$07
ld a,93
sub c
ld ($4038),a
ret
ld ($4039),bc
ld hl,($4010)
ld d,c
ld a,34
sub c
ld c,a
ld a,118
inc b
dec hl
cp (hl)
jr nz,$FC
djnz $FA
inc hl
cpir
dec hl
ld()$400E,hl
scf
ret po
dec d
ret z
push bc
call 2462
pop bc
ld b,c
ld h,d
ld l,e
ld (hl),0
dec hl
djnz $FB
ex de,hl
inc hl
ld()$400E,hl
ret
push af
call 2421
jr nc, $08
bit 0,(iy+1)
jr nz,$02
xor a
rst 10h
ld a,(bc)
and 63
rst 10h
ld a,(bc)
inc bc
add a,a
jr nc, $F7
pop bc
	bit 7,b
ret z
cp 26
jr z,$03
cp 56
ret c
xor a
set 0,(iy+1)
jp $07F5
push hl
ld hl,273
bit 7,a
jr z,$02
and 63
cp 67
jr nc, $10
ld b,a
inc b
bit 7,(hl)
inc hl
jr z,$FB
djnz $F9
bit 6,a
jr nz,$02
cp 24
ccf
ld b,h
ld c,l
pop hl
ret nc
ld a,(bc)
add a,228
ret
ld bc,1
push hl
call 3781
pop hl
call 2477
ld hl,($401C)
ex de,hl
lddr
ret
push af
push hl
ld hl,16396
ld a,9
ld e,(hl)
inc hl
ld d,(hl)
ex (sp),hl
and a
sbc hl,de
add hl,de
ex (sp),hl
jr nc, $09
push de
ex de,hl
add hl,bc
ex de,hl
ld (hl),d
dec hl
ld (hl),e
inc hl
pop de
inc hl
dec a
jr nz,$E8
ex de,hl
pop de
pop af
and a
sbc hl,de
ld b,h
ld c,l
inc bc
add hl,de
ex de,hl
ret
push hl
ld hl,16509
ld d,h
ld e,l
pop bc
call 2538
ret nc
push bc
call 2546
ex de,hl
jr $F4
ld a,(hl)
cp b
ret nz
inc hl
ld a,(hl)
dec hl
cp c
ret
push hl
ld a,(hl)
cp 64
jr c, $17
bit 5,a
jr z,$14
add a,a
jp m,$0A01
ccf
ld bc,5
jr nc, $02
ld c,17
rla
inc hl
ld a,(hl)
jr nc, $FB
jr $06
inc hl
inc hl
ld c,(hl)
inc hl
ld b,(hl)
inc hl
add hl,bc
pop de
and a
sbc hl,de
ld b,h
ld c,l
add hl,de
ex de,hl
ret
ld b,(iy+$22)
push bc
call 2604
pop bc
dec b
jr $02
ld b,24
res 1,(iy+1)
ld c,33
push bc
call 2328
pop bc
ld a,(16389)
cp 77
jr c, $14
set 7,(iy+58)
xor a
call 2037
ld hl,($4039)
ld a,l
or h
and 126
jr nz,$F3
jp $0918
ld d,h
ld e,l
dec hl
ld c,b
ld b,0
ldir
ld hl,($4010)
call 2583
push bc
ld a,b
cpl
ld b,a
ld a,c
cpl
ld c,a
inc bc
call 2477
ex de,hl
pop hl
add hl,de
push de
ldir
pop hl
ret
ld hl,($4014)
call 77
rst 18h
bit 5,(iy+45)
ret nz
ld hl,16477
ld()$401C,hl
call 5448
call 5514
jr c, $04
ld hl,55536
add hl,bc
jp c,$0D9A
cp a
jp $14BC
push de
push hl
xor a
	bit 7,b
jr nz,$20
ld h,b
ld l,c
ld e,255
jr $08
push de
ld d,(hl)
inc hl
ld e,(hl)
push hl
ex de,hl
ld e,0
ld bc,64536
call 2017
ld bc,65436
call 2017
ld c,246
call 2017
ld a,l
call 2027
pop hl
pop de
ret
call 3494
pop hl
ret z
jp (hl)
set 1,(iy+1)
ld a,(hl)
cp 118
jp z, $0B84
sub 26
adc a,0
jr z,$69
cp 167
jr nz,$1B
rst 20h
call 3474
cp 26
jp nz,$0D9A
rst 20h
call 3474
call 2894
B_CALL ($3401)
call 3061
call 2293
jr $3D
cp 168
jr nz,$33
rst 20h
call 3474
call 2894
call 3074
jp nz,$0EAD
and 31
ld c,a
bit 1,(iy+1)
jr z,$0A
sub (iy+$38)
set 7,a
add a,60
call nc,2161
add a,(iy+57)
cp 33
ld a,(16442)
sbc a,1
call 2298
set 0,(iy+1)
jr $06
call 3925
call 2901
rst 18h
sub 26
adc a,0
jr z,$06
call 3357
jp $0B84
call nc,2955
rst 20h
cp 118
ret z
jp $0AD5
call 3494
ret nz
pop hl
jr $E2
call 2757
bit 6,(iy+1)
call z,5112
jr z,$0A
jp $15DB
ld a,11
rst 10h
ld de,($4018)
ld a,b
or c
dec bc
ret z
ld a,(de)
inc de
ld ($4018),de
bit 6,a
jr z,$ED
cp 192
jr z,$E7
push bc
call 2379
pop bc
jr $E3
call 2757
ld a,118
rst 10h
ret
call 2757
set 0,(iy+1)
xor a
rst 10h
ld bc,($4039)
ld a,c
bit 1,(iy+1)
jr z,$05
ld a,93
sub (iy+$38)
ld c,17
cp c
jr nc, $02
ld c,1
call 2315
ret
call 3061
ld ($4036),bc
ld a,43
sub b
jp c,$0EAD
ld b,a
ld a,1
sra b
jr nc, $02
ld a,4
sra c
jr nc, $01
rlca
push af
call 2293
ld a,(hl)
rlca
cp 16
jr nc, $06
rrca
jr nc, $02
xor 143
ld b,a
ld de,3230
ld a,(16432)
sub e
jp m,$0BE9
pop af
cpl
and b
jr $02
pop af
or b
cp 8
jr c, $02
xor 143
exx
rst 10h
exx
ret
call 3074
ld b,a
push bc
call 3074
ld e,c
pop bc
ld d,c
ld c,a
ret
call 5581
jp c,$0EAD
ld c,1
ret z
ld c,255
ret
ld b,(iy+$22)
ld c,33
call 2328
call 2459
ld a,(hl)
ld (de),a
inc (iy+$3A)
ld hl,($400C)
inc hl
ld d,h
ld e,l
cpir
jp $0A5D
adc a,e
adc a,l
dec l
ld a,a
add a,c
ld c,c
ld (hl),l
ld e,a
ld b,b
ld b,d
dec hl
rla
rra
scf
ld d,d
ld b,l
rrca
ld l,l
dec hl
ld b,h
dec l
ld e,d
dec sp
ld c,h
ld b,l
dec c
ld d,d
ld e,d
ld c,l
dec d
ld l,d
ld bc,532
ld b,0
add a,c
ld c,6
sbc a,5
xor e
dec c
ld b,0
or l
ld c,0
call c,12
ret c
ld c,4
inc d
ld b,223
ld b,5
cp c
dec c
inc b
nop
ld l,14
dec b
rst 08h
ld a,(bc)
ld bc,59648
ld c,5
add hl,bc
inc d
dec b
ld l,d
dec c
nop
jp $0303
xor a
ld c,3
jr nc, $07
ld b,26
ld b,0
sub d
ld c,3
ld l,h
ld c,5
ld b,b
inc bc
dec b
or 2
nop
ld a,h
ld c,0
abc a,d
inc d
nop
ld hl,($060A)
ld a,(de)
ld b,0
xor a
dec bc
ld b,26
ld b,0
xor a
dec bc
nop
ld c,12
ld b,0
ld ($000F),a
dec hl
rrca
nop
inc hl
rrca
nop
ld l,c
ex af,af
dec b
rrc d
inc bc
inc l
rlca
ld (iy+$01),$01
call 2675
call 5308
ld hl,16384
ld (hl),255
ld hl,16429
bit 5,(hl)
jr z,$0E
cp 227
ld a,(hl)
jp nz,$0D6F
call 3494
ret z
rst 08h
inc c
rst 08h
ex af,af
rst 18h
ld b,0
cp 118
ret z
ld c,a
rst 20h
ld a,c
sub 225
jr c, $3B
ld c,a
ld hl,3113
add hl,bc
ld c,(hl)
add hl,bc
jr $03
ld hl,($4030)
ld a,(hl)
inc hl
ld()$4030,hl
ld bc,3316
push bc
ld c,a
cp 11
jr nc, $0B
ld hl,3350
ld b,0
add hl,bc
ld c,(hl)
add hl,bc
push hl
rst 18h
ret
rst 18h
cp c
jr nz,$12
rst 20h
ret
rla
dec h
ld d,e
rrca
ld l,e
inc de
halt
call 3494
ret nz
pop bc
ld a,(hl)
cp 118
ret z
jr $72
cp 118
call 3484
cp a
pop bc
call z,3357
ex de,hl
ld hl,($4030)
ld c,(hl)
inc hl
ld b,(hl)
ex de,hl
push bc
ret
call 4380
ld (iy+$2D),$00
jr nc, $08
set 1,(iy+45)
jr nz,$18
rst 08h
ld bc,42956
ld de,52221
ld bc,8310
dec c
xor a
call 3494
call nz,5112
ld hl,16429
or (hl)
ld (hl),a
ex de,hl
ld ($402E),bc
ld()$4012,hl
ret
pop bc
ld a,(16385)
push af
call 3925
pop af
ld bc,4897
ld d,(iy+$01)
xor d
and 64
jr nz,$1B
bit 7,d
jr nz,$B7
jr $9D
call 4380
push af
ld a,c
or 159
inc a
jr nz,$0B
pop af
jr $AD
call 3925
bit 6,(iy+1)
ret nz
rst 08h
dec bc
jr nz,$F4
call 3494
ret z
B_CALL ($34A0)
ret
bit 7,(iy+1)
ret
call 3494
jr z,$06
B_CALL ($3402)
ld a,(de)
and a
ret z
jp $0CDE
cp 224
jr nz,$09
rst 20h
call 3474
call 3357
jr $06
call 3357
B_CALL ($34A1)
B_CALL ($02C0)
ld bc,480
inc (hl)
call 4897
ld()$401F,hl
dec hl
ld a,(hl)
set 7,(hl)
ld bc,6
add hl,bc
rlca
jr c, $06
sla c
call 2462
inc hl
push hl
B_CALL ($0202)
inc (hl)
pop hl
ex de,hl
ld c,10
ldir
ld hl,($4007)
ex de,hl
inc de
ld (hl),e
inc hl
ld (hl),d
call 3674
ret nc
bit 7,(iy+8)
ret nz
ld b,(iy+$2E)
res 6,b
ld hl,($4029)
ld a,(hl)
and 192
jr nz,$17
push bc
call 2546
pop bc
inc hl
inc hl
inc hl
call 76
rst 18h
cp 243
ex de,hl
jr nz,$EA
ex de,hl
rst 20h
ex de,hl
cp b
jr nz,$E4
ld()$4029,hl
ret
bit 1,(iy+45)
jp nz,$0D4B
ld hl,($4012)
bit 7,(hl)
jr z,$1C
inc hl
ld()$401F,hl
B_CALL ($E2E0)
rrca
ret nz
ld (bc),a
inc (hl)
call 3674
ret c
ld hl,($401F)
ld de,15
add hl,de
ld e,(hl)
inc hl
ld d,(hl)
ex de,hl
jr $2E
rst 08h
nop
B_CALL ($E0E1)
jp po,$0032
ld (bc),a
ld bc,13059
nop
inc b
inc (hl)
and a
ret
inc (hl)
scf
ret
call 3751
ld a,b
or c
jr nz,$04
ld bc,($4034)
ld ($4032),bc
ret
ld hl,($402B)
jr $05
call 3751
ld h,b
ld l,c
ld a,h
cp 240
jr nc, $22
call 2520
ld()$4029,hl
ret
call 5581
jr c, $16
jr z,$02
neg
push af
call 3751
pop af
bit 7,(iy+0)
ret z
ld (bc),a
ret
call 5514
jr c, $01
ret z
rst 08h
ld a,(bc)
call 3713
jp $149A
ld hl,($4007)
inc hl
ex (sp),hl
push hl
ld ($4002),sp
call 3713
ld bc,6
ld hl,($401C)
add hl,bc
jr c, $08
ex de,hl
ld hl,36
add hl,de
sbc hl,sp
ret c
ld l,3
jp $0058
pop hl
ex (sp),hl
ld a,h
cp 62
jr z,$06
ld ($4002),sp
jr $A1
ex (sp),hl
push hl
rst 08h
ld b,253
rrc b
ld a,(hl)
jr nz,$32
call 5283
ld hl,16429
set 5,(hl)
res 6,(hl)
ld a,(16385)
and 64
ld bc,2
jr nz,$02
ld c,4
or (hl)
ld (hl),a
rst 30h
ld (hl),118
ld a,c
rrca
rrca
jr c, $05
ld a,11
ld (de),a
dec hl
ld (hl),a
dec hl
ld (hl),127
ld hl,($4039)
ld()$4030,hl
pop hl
jp $0472
rst 08h
rlca
call 743
res 6,(iy+59)
ret
set 6,(iy+59)
jp $0207
call 3751
call 743
ld h,b
ld l,c
call 557
ld (iy+$35),$FF
call 519
jr $05
ld a,127
in a,($FE)
rra
res 0,(iy+59)
ld a,255
ld ($4027),a
ret
rst 18h
ld b,0
push bc
cp 64
jr nz,$2F
call 3494
jr z,$28
ld bc,($4032)
call 5408
B_CALL ($0FA1)
jr nc, $37
ld d,4
jr nc, $80
ld b,c
nop
nop
add a,b
ld l,2
and c
inc bc
dec l
inc (hl)
call 5514
ld ($4032),bc
ld a,(hl)
and a
jr z,$03
sub 16
ld (hl),a
jr $0D
cp 66
jr nz,$0D
call 3494
jr z,$04
B_CALL ($34A3)
inc (hl)
rst 20h
jp $1083
cp 65
jr nz,$11
call 699
ld b,h
ld c,l
ld d,c
inc d
call nz,1981
ld a,d
adc a,d
ld b,d
ld c,a
ex de,hl
jr $3B
call 5330
jr c, $6E
cp 27
jp z, $1047
ld bc,2520
cp 22
jr z,$5D
cp 16
jr nz,$0F
call 73
call 3925
cp 17
jr nz,$2E
call 73
jr $22
cp 11
jr nz,$28
call 73
push hl
jr $03
call 73
cp 11
jr nz,$14
pop de
and a
sbc hl,de
ld b,h
ld c,l
ld hl,16385
res 6,(hl)
bit 7,(hl)
call nz,4803
rst 20h
jp $1088
cp 118
jr nz,$E1
jp $0D9A
sub 196
jr c, $F9
ld bc,1260
cp 19
jr z,$13
jr nc, $F0
ld b,16
add a,217
ld c,a
cp 220
jr nc, $02
res 6,c
cp 234
jr c, $02
res 7,c
push bc
rst 20h
jp $0F59
cp 38
jr c, $1E
call 4380
jp c,$0D4B
call z,4519
ld a,(16385)
cp 192
jr c, $4E
inc hl
ld de,($401C)
call 6646
ex de,hl
ld()$401C,hl
jr $40
call 3494
jr nz,$23
call 5337
rst 18h
ld bc,6
call 2462
inc hl
ld (hl),126
inc hl
ex de,hl
ld hl,($401C)
ld c,5
and a
sbc hl,bc
ld()$401C,hl
ldir
ex de,hl
dec hl
call 76
jr $14
rst 20h
cp 126
jr nz,$FB
inc hl
ld de,($401C)
call 6646
ld ($401C),de
ld()$4016,hl
set 6,(iy+1)
rst 18h
cp 16
jr nz,$0C
bit 6,(iy+1)
jr nz,$2A
call 4707
rst 20h
jr $F0
ld bc,195
cp 18
jr c, $1D
sub 22
jr nc, $04
add a,13
jr $0E
cp 3
jr c, $0A
sub 194
jr c, $0D
cp 6
jr nc, $09
add a,3
add a,c
ld c,a
ld hl,4172
add hl,bc
ld b,(hl)
pop de
ld a,d
cp b
jr c, $2C
and a
jp z, $0018
push bc
push de
call 3494
jr z,$09
ld a,e
and 63
ld b,a
B_CALL ($3437)
jr $09
ld a,e
xor (iy+$01)
and 64
jp nz,$0D9A
pop de
ld hl,16385
set 6,(hl)
bit 7,e
jr nz,$02
res 6,(hl)
pop bc
jr $CF
push de
ld a,c
bit 6,(iy+1)
jr nz,$15
and 63
add a,8
ld c,a
cp 16
jr nz,$04
set 6,c
jr $08
jr c, $D7
cp 23
jr z,$02
set 7,c
push bc
rst 20h
jp $0F59
ld b,8
ex af,af
ld a,(bc)
ld (bc),a
inc bc
dec b
dec b
dec b
dec b
dec b
dec b
ld b,253
rlc c
or 223
call 5326
jp nc,$0D9A
push hl
ld c,a
rst 20h
push hl
res 5,c
cp 16
jr z,$17
set 6,c
cp 13
jr z,$0C
set 5,c
call 5330
jr nc, $0A
res 6,c
rst 20h
jr $F6
rst 20h
res 6,(iy+1)
ld b,c
call 3494
jr nz,$08
ld a,c
and 224
set 7,a
ld c,a
jr $34
ld hl,($4010)
ld a,(hl)
and 127
jr z,$2A
cp c
jr nz,$1F
rla
add a,a
jp p,$1195
jr c, $2D
pop de
push de
push hl
inc hl
ld a,(de)
inc de
and a
jr z,$FB
cp (hl)
jr z,$F7
or 128
cp (hl)
jr nz,$06
ld a,(de)
call 5330
jr nc, $15
pop hl
push bc
call 2546
ex de,hl
pop bc
jr $D1
set 7,b
pop de
rst 18h
cp 16
jr z,$09
set 5,b
jr $0D
pop de
pop de
pop de
push hl
rst 18h
call 5330
jr nc, $03
rst 20h
jr $F8
pop hl
rl b
bit 6,b
ret
xor a
ld b,a
bit 7,c
jr nz,$4B
bit 7,(hl)
jr nz,$0E
inc a
inc hl
ld c,(hl)
inc hl
ld b,(hl)
inc hl
ex de,hl
call 4803
rst 18h
jp $125A
inc hl
inc hl
inc hl
ld b,(hl)
bit 6,c
jr z,$0A
dec b
jr z,$E8
ex de,hl
rst 18h
cp 16
jr nz,$61
ex de,hl
ex de,hl
jr $24
push hl
rst 18h
pop hl
cp 26
jr z,$20
bit 7,c
jr z,$52
bit 6,c
jr nz,$06
cp 17
jr nz,$3C
rst 20h
ret
cp 17
jr z,$6C
cp 223
jr nz,$32
rst 18h
dec hl
ld()$4016,hl
jr $5E
ld hl,0
push hl
rst 20h
pop hl
ld a,c
cp 192
jr nz,$09
rst 18h
cp 17
jr z,$51
cp 223
jr z,$E5
push bc
push hl
call 4863
ex (sp),hl
ex de,hl
call 4829
jr c, $19
dec bc
call 4869
add hl,bc
pop de
pop bc
djnz $B3
bit 7,c
jr nz,$66
push hl
bit 6,c
jr nz,$13
ld b,d
ld c,e
rst 18h
cp 17
jr z,$02
rst 08h
ld (bc),a
rst 20h
pop hl
ld de,5
call 4869
add hl,bc
ret
call 4863
ex (sp),hl
call 4869
pop bc
add hl,bc
inc hl
ld b,d
ld c,e
ex de,hl
call 4802
rst 18h
cp 17
jr z,$07
cp 26
jr nz,$DB
call 4707
rst 20h
cp 16
jr z,$F8
res 6,(iy+1)
ret
call 3494
call nz,5112
rst 20h
cp 17
jr z,$50
push de
xor a
push af
push bc
ld de,1
rst 18h
pop hl
cp 223
jr z,$17
pop af
call 4830
push af
ld d,b
ld e,c
push hl
rst 18h
pop hl
cp 223
jr z,$09
cp 17
jp nz,$0D9A
ld h,d
ld l,e
jr $13
push hl
rst 20h
pop hl
cp 17
jr z,$0C
pop af
call 4830
push af
rst 18h
ld h,b
ld l,c
cp 17
jr nz,$E6
pop af
ex (sp),hl
add hl,de
dec hl
ex (sp),hl
and a
sbc hl,de
ld bc,0
jr c, $07
inc hl
and a
jp m,$1231
ld b,h
ld c,l
pop de
res 6,(iy+1)
call 3494
ret z
xor a
push bc
call 6635
pop bc
ld hl,($401C)
ld (hl),a
inc hl
ld (hl),e
inc hl
ld (hl),d
inc hl
ld (hl),c
inc hl
ld (hl),b
inc hl
ld()$401C,hl
res 6,(iy+1)
ret
xor a
push de
push hl
push af
call 3474
pop af
call 3494
jr z,$12
push af
call 3751
pop de
ld a,b
or c
scf
jr z,$05
pop hl
push hl
and a
sbc hl,bc
ld a,d
sbc a,0
pop hl
pop de
ret
ex de,hl
inc hl
ld e,(hl)
inc hl
ld d,(hl)
ret
call 3494
ret z
push bc
ld b,16
ld a,h
ld c,l
ld hl,0
add hl,hl
jr c, $06
rl c
rla
jr nc, $04
add hl,de
jp c,$0ED3
djnz $F2
pop bc
ret
ld hl,($4012)
bit 1,(iy+45)
jr z,$44
ld bc,5
inc bc
inc hl
ld a,(hl)
and a
jr z,$FB
call 5330
jr c, $F5
cp 13
jp z, $13C8
rst 30h
push de
ld hl,($4012)
dec de
ld a,c
sub 6
ld b,a
ld a,64
jr z,$0E
inc hl
ld a,(hl)
and a
jr z,$FB
inc de
ld (de),a
djnz $F7
or 128
ld (de),a
ld a,128
ld hl,($4012)
xor (hl)
pop hl
call 5095
push hl
B_CALL ($3402)
pop hl
ld bc,5
and a
sbc hl,bc
jr $40
bit 6,(iy+1)
jr z,$06
ld de,6
add hl,de
jr $E7
ld hl,($4012)
ld bc,($402E)
bit 0,(iy+45)
jr nz,$30
ld a,b
or c
ret z
push hl
rst 30h
push de
push bc
ld d,h
ld e,l
inc hl
ld (hl),0
lddr
push hl
call 5112
pop hl
ex (sp),hl
and a
sbc hl,bc
add hl,bc
jr nc, $02
ld b,h
ld c,l
ex (sp),hl
ex de,hl
ld a,b
or c
jr z,$02
ldir
pop bc
pop de
pop hl
ex de,hl
ld a,b
or c
ret z
push de
ldir
pop hl
ret
dec hl
dec hl
dec hl
ld a,(hl)
push hl
push bc
call 5070
pop bc
pop hl
inc bc
inc bc
inc bc
jp $0A60
ld a,96
ld hl,($4012)
xor (hl)
push af
call 5112
ex de,hl
add hl,bc
push hl
inc bc
inc bc
inc bc
rst 30h
ex de,hl
pop hl
dec bc
dec bc
push bc
lddr
ex de,hl
pop bc
dec bc
ld (hl),b
dec hl
ld (hl),c
pop af
push af
call 5319
pop af
dec hl
ld (hl),a
ld hl,($401A)
ld()$4014,hl
dec hl
ld (hl),128
ret
ld hl,($401C)
dec hl
ld b,(hl)
dec hl
ld c,(hl)
dec hl
ld d,(hl)
dec hl
ld e,(hl)
dec hl
ld a,(hl)
ld()$401C,hl
ret
call 4380
jp nz,$0D9A
call 3494
jr nz,$08
res 6,c
call 4519
call 3357
jr c, $08
push bc
call 2546
call 2656
pop bc
set 7,c
ld b,0
push bc
ld hl,1
bit 6,c
jr nz,$02
ld l,5
ex de,hl
rst 20h
ld h,64
call 4829
jp c,$1231
pop hl
push bc
inc h
push hl
ld h,b
ld l,c
call 4869
ex de,hl
rst 18h
cp 26
jr z,$E8
cp 17
jr nz,$BB
rst 20h
pop bc
ld a,c
ld l,b
ld h,0
inc hl
inc hl
add hl,hl
add hl,de
jp c,$0ED3
push de
push bc
push hl
ld b,h
ld c,l
ld hl,($4014)
dec hl
call 2462
inc hl
ld (hl),a
pop bc
dec bc
dec bc
dec bc
inc hl
ld (hl),c
inc hl
ld (hl),b
pop af
inc hl
ld (hl),a
ld h,d
ld l,e
dec de
ld (hl),0
pop bc
lddr
pop bc
ld (hl),b
dec hl
ld (hl),c
dec hl
dec a
jr nz,$F8
ret
ld hl,($401A)
dec hl
call 2462
inc hl
inc hl
pop bc
ld ($4014),bc
pop bc
ex de,hl
inc hl
ret
ld hl,($4010)
ld (hl),128
inc hl
ld()$4014,hl
ld hl,($4014)
ld()$401A,hl
ld()$401C,hl
ret
ld hl,($4014)
ld (hl),127
inc hl
ld (hl),118
inc hl
ld (iy+$22),$02
jr $EA
ld hl,16477
ld()$401F,hl
ld hl,($401A)
jr $E2
ld de,($4014)
jp $0A5D
cp 38
jr $02
cp 28
ccf
ret nc
cp 64
ret
call 5448
cp 27
jr nz,$15
B_CALL ($C0A1)
ld (bc),a
inc (hl)
rst 20h
call 5396
jr c, $0A
B_CALL ($A4E0)
dec b
ret nz
inc b
rrca
inc (hl)
jr $F0
cp 42
ret nz
ld (iy+$5D),$FF
rst 20h
cp 21
jr z,$07
cp 22
jr nz,$04
inc (iy+$5D)
rst 20h
call 5448
B_CALL ($00E0)
ld (bc),a
jr $38
inc (hl)
ret
cp 28
ret c
cp 38
ccf
ret c
sub 28
ld c,a
ld b,0
ld iy,16384
push bc
B_CALL ($34A0)
pop bc
ld (hl),145
ld a,b
and a
jr nz,$07
ld (hl),a
or c
ret z
ld b,c
ld c,(hl)
ld (hl),137
dec (hl)
sla c
rl b
jr nc, $F9
srl b
rr c
inc hl
ld (hl),b
inc hl
ld (hl),c
dec hl
dec hl
ret
push af
B_CALL ($34A0)
pop af
call 5396
ret c
B_CALL ($A401)
inc b
rrca
inc (hl)
rst 20h
jr $F3
B_CALL ($322D)
ret nz
ld (bc),a
daa
and c
inc bc
dec l
ld ($2200),a
dec l
jr nc, $33
ld b,b
inc bc
dec l
ld ($0C00),a
ld bc,258
jr nc, $80
ld c,b
jr $96
add a,b
cpl
inc b
ld (bc),a
ld bc,57508
nop
inc b
inc b
cpl
ld (bc),a
dec b
ld bc,55855
ld (bc),a
inc (hl)
ret
call 5112
and a
jr nz,$05
ld b,a
ld c,a
push af
jr $31
ld b,e
ld e,c
ld c,d
sub 145
ccf
	bit 7,b
push af
set 7,b
jr c, $24
inc a
neg
cp 8
jr c, $06
ld e,c
ld c,b
ld b,0
sub 8
and a
ld d,a
ld a,e
rlca
jr z,$07
srl b
rr c
dec d
jr nz,$F9
jr nc, $08
inc bc
ld a,b
or c
jr nz,$03
pop af
scf
push af
push bc
B_CALL ($C134)
pop af
ld a,c
ret
call 5514
ret c
push af
dec b
inc b
jr z,$03
pop af
scf
ret
pop af
ret
B_CALL ($322D)
nop
dec bc
dec l
inc sp
nop
dec c
ld (bc),a
inc (hl)
ld a,28
rst 10h
ret
daa
inc (hl)
ld a,22
rst 10h
B_CALL ($7E34)
call 5405
B_CALL ($7830)
nop
add a,b
inc bc
jr nc, $EF
ld a,(de)
jr nz,$9A
add a,l
inc b
inc h
pop bc
jr nc, $34
nop
inc bc
jr $38
and d
rrca
inc h
inc (hl)
ld hl,16491
ld (hl),144
ld b,10
inc hl
push hl
push bc
B_CALL ($2EA4)
ld bc,52532
call 62997
sub b
pop bc
pop hl
ld (hl),a
djnz $EE
inc hl
ld bc,8
push hl
dec hl
ld a,(hl)
cp 144
jr z,$FA
sbc hl,bc
push hl
ld a,(hl)
add a,107
push af
pop af
inc hl
ld a,(hl)
adc a,0
daa
push af
and 15
ld (hl),a
set 7,(hl)
jr z,$F2
pop af
pop hl
ld b,6
ld (hl),128
dec hl
djnz $FB
B_CALL ($E102)
inc (hl)
call 5581
jr z,$02
neg
ld e,a
inc e
inc e
pop hl
dec hl
dec e
ld a,(hl)
and 15
jr z,$F9
ld a,e
sub 5
cp 8
jp p,$1682
cp 246
jp m,$1682
add a,6
jr z,$48
jp m,$16B2
ld b,a
call 5840
djnz $FB
jr $40
ld b,e
call 5840
call 5826
ld a,42
rst 10h
ld a,b
and a
jp p,$1698
neg
ld b,a
ld a,22
jr $02
ld a,21
rst 10h
ld a,b
ld b,255
inc b
sub 10
jr nc, $FB
add a,10
ld c,a
ld a,b
and a
jr z,$03
call 2027
ld a,c
call 2027
ret
neg
ld b,a
ld a,27
rst 10h
ld a,28
rst 10h
djnz $FD
jr $09
ld a,28
rst 10h
dec (hl)
inc (hl)
ret pe
ld a,27
rst 10h
dec (hl)
inc (hl)
ret pe
call 5840
jr $F8
ld a,(hl)
and 15
call 2027
dec hl
ret
ld a,(hl)
ld (hl),0
and a
ret z
inc hl
bit 7,(hl)
set 7,(hl)
dec hl
ret z
push bc
ld bc,5
add hl,bc
ld b,c
ld c,a
scf
dec hl
ld a,(hl)
cpl
adc a,0
ld (hl),a
djnz $F8
ld a,c
pop bc
ret
push hl
push af
ld c,(hl)
inc hl
ld b,(hl)
ld (hl),a
inc hl
ld a,c
ld c,(hl)
push bc
inc hl
ld c,(hl)
inc hl
ld b,(hl)
ex de,hl
ld d,a
ld e,(hl)
push de
inc hl
ld d,(hl)
inc hl
ld e,(hl)
push de
exx
pop de
pop hl
pop bc
exx
inc hl
ld d,(hl)
inc hl
ld e,(hl)
pop af
pop hl
ret
and a
ret z
cp 33
jr nc, $16
push bc
ld b,a
exx
sra l
rr d
rr e
exx
rr d
rr e
djnz $F2
pop bc
ret nc
call 5953
ret nz
exx
xor a
ld l,0
ld d,a
ld e,l
exx
ld de,0
ret
inc e
ret nz
inc d
ret nz
exx
inc e
jr nz,$01
inc d
exx
ret
ld a,(de)
and a
ret z
inc de
ld a,(de)
xor 128
ld (de),a
dec de
exx
push hl
exx
push de
push hl
call 5848
ld b,a
ex de,hl
call 5848
ld c,a
cp b
jr nc, $03
ld a,b
ld b,c
ex de,hl
push af
sub b
call 5879
call 5914
pop af
pop hl
ld (hl),a
push hl
ld l,b
ld h,c
add hl,de
exx
ex de,hl
adc hl,bc
ex de,hl
ld a,h
adc a,l
ld l,a
rra
xor l
exx
ex de,hl
pop hl
rra
jr nc, $08
ld a,1
call 5914
inc (hl)
jr z,$23
exx
ld a,l
and 128
exx
inc hl
ld (hl),a
dec hl
jr z,$1F
ld a,e
neg
ccf
ld e,a
ld a,d
cpl
adc a,0
ld d,a
exx
ld a,e
cpl
adc a,0
ld e,a
ld a,d
cpl
adc a,0
jr nc, $07
rra
exx
inc (hl)
jp z, $1880
exx
ld d,a
exx
xor a
jr $6C
scf
dec (hl)
inc (hl)
ret z
inc hl
xor (hl)
set 7,(hl)
dec hl
ret
xor a
call 6076
ret c
exx
push hl
exx
push de
ex de,hl
call 6076
ex de,hl
jr c, $5A
push hl
call 5879
ld a,b
and a
sbc hl,hl
exx
push hl
sbc hl,hl
exx
ld b,33
jr $11
jr nc, $05
add hl,de
exx
adc hl,de
exx
exx
rr h
rr l
exx
rr h
rr l
exx
rr b
rr c
exx
rr c
rra
djnz $E4
ex de,hl
exx
ex de,hl
exx
pop bc
pop hl
ld a,b
add a,c
jr nz,$01
and a
dec a
ccf
rla
ccf
rra
jp p,$1819
jr nc, $68
and a
inc a
jr nz,$08
jr c, $06
exx
bit 7,d
exx
jr nz,$5C
ld (hl),a
exx
ld a,b
exx
jr nc, $15
ld a,(hl)
and a
ld a,128
jr z,$01
xor a
exx
and d
call 5944
rlca
ld (hl),a
jr c, $2E
inc hl
ld (hl),a
dec hl
jr $29
ld b,32
exx
bit 7,d
exx
jr nz,$12
rlca
rl e
rl d
exx
rl e
rl d
exx
dec (hl)
jr z,$D7
djnz $EA
jr $D7
rla
jr nc, $0C
call 5953
jr nz,$07
exx
ld d,128
exx
inc (hl)
jr z,$18
push hl
inc hl
exx
push de
exx
pop bc
ld a,b
rla
rl (hl)
rra
ld (hl),a
inc hl
ld (hl),c
inc hl
ld (hl),d
inc hl
ld (hl),e
pop hl
pop de
exx
pop hl
exx
ret
rst 08h
dec b
ex de,hl
xor a
call 6076
jr c, $F7
ex de,hl
call 6076
ret c
exx
push hl
exx
push de
push hl
call 5879
exx
push hl
ld h,b
ld l,c
exx
ld h,c
ld l,b
xor a
ld b,223
jr $10
rla
rl c
exx
rl c
rl b
exx
add hl,hl
exx
adc hl,hl
exx
jr c, $10
sbc hl,de
exx
sbc hl,de
exx
jr nc, $0F
add hl,de
exx
adc hl,de
exx
and a
jr $08
and a
sbc hl,de
exx
sbc hl,de
exx
scf
inc b
jp m,$18A2
push af
jr z,$E1
ld e,a
ld d,c
exx
ld e,c
ld d,b
pop af
rr b
pop af
rr b
exx
pop bc
pop hl
ld a,b
sub c
jp $1810
ld a,(hl)
cp 129
jr nc, $06
ld (hl),0
ld a,32
jr $05
sub 160
ret p
neg
push de
ex de,hl
dec hl
ld b,a
srl b
srl b
srl b
jr z,$05
ld (hl),0
dec hl
djnz $FB
and 7
jr z,$09
ld b,a
ld a,255
sla a
djnz $FC
and (hl)
ld (hl),a
ex de,hl
pop de
ret
nop
or b
nop
ld sp,12288
nop
pop af
ld c,c
rrca
jp c,$34A2
jr nz,$2F
inc e
ld (hl),d
ld a,(de)
ex (sp),hl
add hl,de
ld c,h
rla
add a,23
add a,d
jr $E2
dec e
undefined
di
ld a,(de)
inc bc
dec de
inc bc
dec de
inc bc
dec de
inc bc
dec de
inc bc
dec de
inc bc
dec de
ld d,l
rla
ret m
ld a,(de)
inc bc
dec de
inc bc
dec de
inc bc
dec de
inc bc
dec de
inc bc
dec de
inc bc
dec de
ld h,d
dec de
and b
ld a,(de)
ld b,28
and h
dec de
ld de,18716
dec e
ld a,29
lh l,(hl)
dec e
call nz,54301
dec e
halt
dec e
xor c
inc e
ld e,e
inc e
ld b,(hl)
inc e
in a,($1D)
xor a
ld a,(de)
xor d
ld a,(de)
cp (hl)
ld a,(de)
push bc
ld a,(de)
push de
dec de
adc a,a
dec de
push de
ld a,(de)
or 25
scf
inc e
inc hl
inc e
call m,5913
inc e
in a,($1A)
adc a,26
dec hl
nop
jr $1D
call po,58392
add hl,de
ld e,d
dec d
ld a,a
ld a,(de)
ld d,c
ld a,(de)
ld h,e
ld a,(de)
ld b,l
ld a,(de)
call 7045
ld a,b
ld ($401E),a
exx
ex (sp),hl
exx
ld ($401C),de
exx
ld a,(hl)
inc hl
push hl
and a
jp p,$19C2
ld d,a
and 96
rrca
rrca
rrca
rrca
add a,114
ld l,a
ld a,d
and 31
jr $0E
cp 24
jr nc, $08
exx
ld bc,65531
ld d,h
ld e,l
add hl,bc
exx
rlca
ld l,a
ld de,6435
ld h,0
add hl,de
ld e,(hl)
inc hl
ld d,(hl)
ld hl,6567
ex (sp),hl
push de
exx
ld bc,($401D)
ret
pop af
ld a,(16414)
exx
jr $C3
push de
push hl
ld bc,5
call 3781
pop hl
pop de
ret
call 6635
ldir
ret
ld h,d
ld l,e
call 6635
exx
push hl
exx
ex (sp),hl
push bc
ld a,(hl)
and 192
rlca
rlca
ld c,a
inc c
ld a,(hl)
and 63
jr nz,$02
inc hl
ld a,(hl)
add a,80
ld (de),a
ld a,5
sub c
inc hl
inc de
ld b,0
ldir
pop bc
ex (sp),hl
exx
pop hl
exx
ld b,a
xor a
dec b
ret z
ld (de),a
inc de
jr $FA
and a
ret z
push af
push de
ld de,0
call 6654
pop de
pop af
dec a
jr $F2
ld c,a
rlca
rlca
add a,c
ld c,a
ld b,0
add hl,bc
ret
push de
ld hl,($401F)
call 6716
call 6646
pop hl
ret
ld h,d
ld l,e
exx
push hl
ld hl,6421
exx
call 6701
call 6654
exx
pop hl
exx
ret
push hl
ex de,hl
ld hl,($401F)
call 6716
ex de,hl
call 6646
ex de,hl
pop hl
ret
ld b,5
ld a,(de)
ld c,(hl)
ex de,hl
ld (de),a
ld (hl),c
inc hl
inc de
djnz $F7
ex de,hl
ret
ld b,a
call 6560
dec l
rrca
ret nz
ld (bc),a
and b
jp nz,$E02D
inc b
jp po,$03C1
inc (hl)
call 6652
call 6564
rrca
ld bc,706
ld sp,57838
inc bc
inc (hl)
ret
ld a,(hl)
and a
ret z
inc hl
ld a,(hl)
xor 128
ld (hl),a
dec hl
ret
inc hl
res 7,(hl)
dec hl
ret
inc hl
ld a,(hl)
dec hl
dec (hl)
inc (hl)
scf
call nz,6880
inc hl
rlca
rr (hl)
dec hl
ret
call 3751
ld a,(bc)
jp $151D
call 3751
ld hl,5408
push hl
push bc
ret
ld a,(hl)
and a
ret z
ld a,255
jr $07
ld a,(hl)
neg
ccf
jr $05
xor a
inc hl
xor (hl)
dec hl
rlca
push hl
ld b,5
ld (hl),0
inc hl
djnz $FB
pop hl
ret nc
ld (hl),129
ret
ld a,(de)
and a
ret z
scf
jr $ED
ld a,(de)
and a
ret nz
jr $E8
ld a,(de)
and a
ret nz
push de
dec de
xor a
ld (de),a
dec de
ld (de),a
pop de
ret
ld a,b
sub 8
bit 2,a
jr nz,$01
dec a
rrca
jr nc, $08
push af
push hl
call 6770
pop de
ex de,hl
pop af
bit 2,a
jr nz,$07
rrca
push af
call 5964
jr $33
rrca
push af
call 5112
push de
push bc
call 5112
pop hl
ld a,h
or l
ex (sp),hl
ld a,b
jr nz,$0B
or c
pop bc
jr z,$04
pop af
ccf
jr $16
pop af
jr $13
or c
jr z,$0D
ld a,(de)
sub (hl)
jr c, $09
jr nz,$ED
dec bc
inc de
inc hl
ex (sp),hl
dec hl
jr $DF
pop bc
pop af
and a
push af
B_CALL ($34A0)
pop af
push af
call c,6869
call 6862
pop af
rrca
call nc,6869
ret
call 5112
push de
push bc
call 5112
pop hl
push hl
push de
push bc
add hl,bc
ld b,h
ld c,l
rst 30h
call 4803
pop bc
pop hl
ld a,b
or c
jr z,$02
ldir
pop bc
pop hl
ld a,b
or c
jr z,$02
ldir
ld hl,($401C)
ld de,65531
push hl
add hl,de
pop de
ret
call 5581
jr c, $0E
jr nz,$0C
push af
ld bc,1
rst 30h
pop af
ld (de),a
call 4803
ex de,hl
ret
rst 08h
ld a,(bc)
ld hl,($4016)
push hl
call 5112
push de
inc bc
rst 30h
pop hl
ld ($4016),de
push de
ldir
ex de,hl
dec hl
ld (hl),118
res 7,(iy+1)
call 3474
call 3362
pop hl
ld()$4016,hl
set 7,(iy+1)
call 3925
pop hl
ld()$4016,hl
jr $B0
ld bc,1
rst 30h
ld (hl),118
ld hl,($4039)
push hl
ld l,255
ld()$4039,hl
ld hl,($400E)
push hl
ld ($400E),de
push de
call 5595
pop de
ld hl,($400E)
and a
sbc hl,de
ld b,h
ld c,l
pop hl
ld()$400E,hl
pop hl
ld()$4039,hl
call 4803
ex de,hl
ret
call 5112
ld a,b
or c
jr z,$01
ld a,(de)
jp $151D
call 5112
jp $1520
exx
push hl
ld hl,16414
dec (hl)
pop hl
jr nz,$04
inc hl
exx
ret
exx
ld e,(hl)
xor a
bit 7,e
jr z,$01
cpl
ld d,a
add hl,de
exx
ret
ld a,(de)
and a
jr nz,$F0
exx
inc hl
exx
ret
B_CALL ($02C0)
dec l
ret po
dec b
inc h
ret po
ld bc,1216
inc bc
ret po
inc (hl)
ret
B_CALL ($322D)
nop
inc b
ld (hl),52
ret
dec l
ld (hl),192
inc bc
ret po
ld bc,44
inc bc
and c
inc bc
inc (hl)
ret
B_CALL ($F130)
jr c, $AA
dec sp
add hl,hl
inc b
dec l
inc h
jp $2D03
rrca
and c
inc bc
adc a,b
inc de
ld (hl),88
ld h,l
ld h,(hl)
sbc a,l
ld a,b
ld h,l
ld b,b
and d
ld h,b
ld ($E7C9),a
ld hl,45047
inc h
ex de,hl
cpl
or b
or b
inc d
xor 126
cp e
sub h
ld e,b
pop af
ld a,(63614)
rst 08h
ex (sp),hl
inc (hl)
call 5581
jr nz,$07
jr c, $03
add a,(hl)
jr nc, $09
rst 08h
dec b
jr c, $07
sub (hl)
jr nc, $04
neg
ld (hl),a
ret
B_CALL ($A002)
inc (hl)
ret
B_CALL ($332D)
nop
inc b
inc (hl)
rst 08h
add hl,bc
and b
ld (bc),a
inc (hl)
ld a,(hl)
ld (hl),128
call 5405
B_CALL ($3830)
nop
inc bc
ld bc,12333
ret p
ld c,h
call z,52684
inc bc
inc sp
nop
ex af,af
ld bc,929
ld bc,13364
B_CALL ($3001)
ret p
ld sp,6002
ret m
inc b
ld bc,930
and d
inc bc
dec l
jr nc, $32
jr nz,$04
and d
inc bc
adc a,h
ld de,5292
add hl,bc
ld d,(hl)
jp c,$59A5
jr nc, $C5
ld e,h	
sub b
xor d
sbc a,(hl)
ld (hl),b
ld l,a
ld h,c
and c
set 3,d
sub (hl)
and h
ld sp,46239
rst 20h
and b
cp 92
call m,7146
ld b,e
jp z, $ED36
and a
sbc a,h
ld a,(hl)
ld e,(hl)
ret p
lh l,(hl)
inc hl
add a,b
sub e
inc b
rrca
inc (hl)
ret
B_CALL ($EE30)
ld()$83F9,hl
lh l,(hl)
inc b
dec l
and d
rrca
inc h
inc bc
dec l
rrca
dec l
rrca
dec l
daa
and c
inc bc
dec l
inc sp
ret nz
nop
inc b
ld (bc),a
inc (hl)
ret
and c
inc bc
ld bc,50
ld (bc),a
jr $34
ret
B_CALL ($2735)
and c
inc bc
ret po
nop
ld b,24
cpl
inc bc
B_CALL ($2D35)
dec l
inc b
dec l
rrca
and c
inc bc
add a,(hl)
inc d
and 92
rra
dec bc
and e
adc a,a
jr c, $EE
jp (hl)
dec d
ld h,e
cp e
inc hl
xor 146
dec c
call 61933
inc hl
ld e,l
dec de
jp pe,$3404
ret
B_CALL ($1C2D)
ld bc,1309
inc (hl)
ret
ld a,(hl)
cp 129
jr c, $0E
B_CALL ($18A1)
ld bc,11525
ld ($01A3),a
nop
ld b,24
cpl
inc bc
B_CALL ($01A0)
dec l
dec l
inc b
dec l
rrca
and c
inc bc
adc a,h
djnz $B2
inc de
ld c,85
call po,22669
add hl,sp
cp h
ld e,e
sbc a,b
sbc a,(iy+0)
ld (hl),117
and b
in a,($E8)
or h
ld h,e
ld b,d
call nz,46566
add hl,bc
ld (hl),190
jp (hl)
ld (hl),115
dec de
ld e,l
call pe,57048
ld h,e
cp (hl)
ret p
ld h,c
and c
or e
inc c
inc b
rrca
inc (hl)
ret
B_CALL ($2D2D)
inc b
and c
inc bc
jr $25
and c
rrca
dec b
ld hl,3885
inc (hl)
ret
B_CALL ($A31F)
inc bc
jr $34
ret
B_CALL ($2C2D)
nop
ld e,162
inc (hl)
B_CALL ($2D01)
inc l
nop
rlca
ld()$3404,hl
jp $1C5B
ld (bc),a
dec l
inc l
nop
add hl,bc
and b
ld bc,51
ld b,161
ld bc,517
and c
inc (hl)
ret
rst 38h
nop
nop
nop
nop
nop
nop
nop
nop
ret p
ret p
ret p
ret p
nop
nop
nop
nop
rrca
rrca
rrca
rrca
nop
nop
nop
nop
rst 38h
rst 38h
rst 38h
rst 38h
nop
nop
nop
nop
nop
nop
nop
nop
ret p
ret p
ret p
ret p
ret p
ret p
ret p
ret p
ret p
ret p
ret p
ret p
rrca
rrca
rrca
rrca
ret p
ret p
ret p
ret p
rst 38h
rst 38h
rst 38h
rst 38h
ret p
ret p
ret p
ret p
xor d
ld d,l
xor d
ld d,l
xor d
ld d,l
xor d
ld d,l
nop
nop
nop
nop
xor d
ld d,l
xor d
ld d,l
xor d
ld d,l
xor d
ld d,l
nop
nop
nop
nop
nop
inc h
inc h
nop
nop
nop
nop
nop
nop
inc e
ld()$2078,hl
jr nz,$7E
nop
nop
ex af,af
ld a,40
ld a,10
ld a,8
nop
nop
nop
djnz $00
nop
djnz $00
nop
inc a
ld b,d
inc b
ex af,af
nop
ex af,af
nop
nop
inc b
ex af,af
ex af,af
ex af,af
ex af,af
inc b
nop
nop
jr nz,$10
djnz $10
djnz $20
nop
nop
nop
djnz $08
inc b
ex af,af
djnz $00
nop
nop
inc b
ex af,af
djnz $08
inc b
nop
nop
nop
nop
ld a,0
ld a,0
nop
nop
nop
ex af,af
ex af,af
ld a,8
ex af,af
nop
nop
nop
nop
nop
ld a,0
nop
nop
nop
nop
inc d
ex af,af
ld a,8
inc d
nop
nop
nop
ld (bc),a
inc b
ex af,af
djnz $20
nop
nop
nop
djnz $00
nop
djnz $10
jr nz,$00
nop
nop
nop
nop
ex af,af
ex af,af
djnz $00
nop
nop
nop
nop
jr $18
nop
nop
inc a
ld b,(hl)
ld c,d
ld d,d
ld h,d
inc a
nop
nop
jr $28
ex af,af
ex af,af
ex af,af
ld a,0
nop
inc a
ld b,d
ld (bc),a
inc a
ld b,b
ld a,(hl)
nop
nop
inc a
ld b,d
inc c
ld (bc),a
ld b,d
inc a
nop
nop
ex af,af
jr $28
ld c,b
ld a,(hl)
ex af,af
nop
nop
ld a,(hl)
ld b,b
ld a,h
ld (bc),a
ld b,d
inc a
nop
nop
inc a
ld b,b
ld a,h
ld b,d
ld b,d
inc a
nop
nop
ld a,(hl)
ld (bc),a
inc b
ex af,af
djnz $10
nop
nop
inc a
ld b,d
inc a
ld b,d
ld b,d
inc a
nop
nop
inc a
ld b,d
ld b,d
ld a,2
inc a
nop
nop
inc a
ld b,d
ld b,d
ld a,(hl)
ld b,d
ld b,d
nop
nop
ld a,h
ld b,d
ld a,h
ld b,d
ld b,d
ld a,h
nop
nop
inc a
ld b,d
ld b,b
ld b,b
ld b,d
inc a
nop
nop
ld a,b
ld b,h
ld b,d
ld b,d
ld b,h
ld a,b
nop
nop
ld a,(hl)
ld b,b
ld a,h
ld b,b
ld b,b
ld a,(hl)
nop
nop
ld a,(hl)
ld b,b
ld a,h
ld b,b
ld b,b
ld b,b
nop
nop
inc a
ld b,d
ld b,b
ld c,(hl)
ld b,d
inc a
nop
nop
ld b,d
ld b,d
ld a,(hl)
ld b,d
ld b,d
ld b,d
nop
nop
ld a,8
ex af,af
ex af,af
ex af,af
ld a,0
nop
ld (bc),a
ld (bc),a
ld (bc),a
ld b,d
ld b,d
inc a
nop
nop
ld b,h
ld c,b
ld (hl),b
ld c,b
ld b,h
ld b,d
nop
nop
ld b,b
ld b,b
ld b,b
ld b,b
ld b,b
ld a,(hl)
nop
nop
ld b,d
ld h,(hl)
ld e,d
ld b,d
ld b,d
ld b,d
nop
nop
ld b,d
ld h,d
ld d,d
ld c,d
ld b,(hl)
ld b,d
nop
nop
inc a
ld b,d
ld b,d
ld b,d
ld b,d
inc a
nop
nop
ld a,h
ld b,d
ld b,d
ld a,h
ld b,b
ld b,b
nop
nop
inc a
ld b,d
ld b,d
ld d,d
ld c,d
inc a
nop
nop
ld a,h
ld b,d
ld b,d
ld a,h
ld b,h
ld b,d
nop
nop
inc a
ld b,b
inc a
ld (bc),a
ld b,d
inc a
nop
nop
cp 16
djnz $10
djnz $10
nop
nop
ld b,d
ld b,d
ld b,d
ld b,d
ld b,d
inc a
nop
nop
ld b,d
ld b,d
ld b,d
ld b,d
inc h
jr $00
nop
ld b,d
ld b,d
ld b,d
ld b,d
ld e,d
inc h
nop
nop
ld b,d
inc h
jr $18
inc h
ld b,d
nop
nop
add a,d
ld b,h
jr z,$10
djnz $10
nop
nop
ld a,(hl)
inc b
ex af,af
djnz $20
ld a,(hl)
nop
