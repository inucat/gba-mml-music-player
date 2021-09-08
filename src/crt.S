@ GBA ROM Header
@ By inucat (2021-09-03).
.text
    @ 4B    ROM Entry Point
    b   main
    @ 156B  Nintendo Logo
    .space  156
    @ 12B   Game Title
    .space  12
    @ 4B    Game Code
    .space  4
    @ Maker Code
    .byte   0x30, 0x31
    @ Fixed value
    .byte   0x96
    @ Main Unit Code
    .byte   0
    @ Device Type
    .byte   0
    @ 7B    Reserved, should be 0-filled
    .space  7
    @ Software Version
    .byte   0
    @ Header Checksum
    .byte   0xF0
    @ 2B    Reserved, should be 0-filled
    .space  2
    @ 4B    Normal/Multiplay mode Entry Point
    b   main

@ After Boot mode and followings seem not required.
    @ .byte   0
    @ .byte   0
    @ b   main
