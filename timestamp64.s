.global timestamp    

.text               

timestamp:          
    # Zabezpieczenie wartości rejestru %rbx przed zmianą w trakcie wykonania funkcji
    push %rbx

    # Inicjalizacja i synchronizacja wykonania procesora
    xor %rax, %rax       
    cpuid 

    # Pobranie i zapisanie liczby cykli procesora od jego uruchomienia
    rdtsc

    # Przywrócenie oryginalnej wartości rejestru %rbx i zakończenie funkcji
    pop %rbx 
    ret
