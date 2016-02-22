        jump.i  #lab0                   ;jump.i  lab0
p:
        enter.i #16                     ;enter.i 16
        inttoreal.i #1,BP-8             ;inttoreal.i 1,$t0
        add.r   *BP+8,BP-8,BP-16        ;add.r   b,$t0,$t1
        mov.r   BP-16,*BP+12            ;mov.r   $t1,a
        leave                           ;leave   
        return                          ;return  
lab0:
        exit                            ;exit    
