==11416== Memcheck, a memory error detector
==11416== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==11416== Using Valgrind-3.25.1 and LibVEX; rerun with -h for copyright info
==11416== Command: ./game --editor
==11416== 
==11416== Use of uninitialised value of size 8
==11416==    at 0x4005B88: world_get_chunk (world.c:105)
==11416==    by 0x4005D5D: world_get_chunk (world.c:123)
==11416==    by 0x4002625: editor_place (editor.c:11)
==11416==    by 0x4002960: game_editor_tick (editor.c:64)
==11416==    by 0x400776F: player_tick (tick.c:27)
==11416==    by 0x4005495: game_tick (tick.c:5)
==11416==    by 0x4007B4A: main (main.c:13)
==11416== 
==11416== Invalid read of size 8
==11416==    at 0x4005B88: world_get_chunk (world.c:105)
==11416==    by 0x4005D5D: world_get_chunk (world.c:123)
==11416==    by 0x4002625: editor_place (editor.c:11)
==11416==    by 0x4002960: game_editor_tick (editor.c:64)
==11416==    by 0x400776F: player_tick (tick.c:27)
==11416==    by 0x4005495: game_tick (tick.c:5)
==11416==    by 0x4007B4A: main (main.c:13)
==11416==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==11416== 
==11416== 
==11416== Process terminating with default action of signal 11 (SIGSEGV): dumping core
==11416==  Access not within mapped region at address 0x0
==11416==    at 0x4005B88: world_get_chunk (world.c:105)
==11416==    by 0x4005D5D: world_get_chunk (world.c:123)
==11416==    by 0x4002625: editor_place (editor.c:11)
==11416==    by 0x4002960: game_editor_tick (editor.c:64)
==11416==    by 0x400776F: player_tick (tick.c:27)
==11416==    by 0x4005495: game_tick (tick.c:5)
==11416==    by 0x4007B4A: main (main.c:13)
==11416==  If you believe this happened as a result of a stack
==11416==  overflow in your program's main thread (unlikely but
==11416==  possible), you can try to increase the size of the
==11416==  main thread stack using the --main-stacksize= flag.
==11416==  The main thread stack size used in this run was 8388608.
==11416== 
==11416== HEAP SUMMARY:
==11416==     in use at exit: 19,468,527 bytes in 91,409 blocks
==11416==   total heap usage: 136,850 allocs, 45,441 frees, 33,603,776 bytes allocated
==11416== 
==11416== LEAK SUMMARY:
==11416==    definitely lost: 142 bytes in 5 blocks
==11416==    indirectly lost: 0 bytes in 0 blocks
==11416==      possibly lost: 6,455,584 bytes in 10,416 blocks
==11416==    still reachable: 13,012,801 bytes in 80,988 blocks
==11416==         suppressed: 0 bytes in 0 blocks
==11416== Rerun with --leak-check=full to see details of leaked memory
==11416== 
==11416== Use --track-origins=yes to see where uninitialised values come from
==11416== For lists of detected and suppressed errors, rerun with: -s
==11416== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
