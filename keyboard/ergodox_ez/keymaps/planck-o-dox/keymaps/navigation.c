/*
    Navigation and Function Key Keymap


Ergodox:

    ,--------------------------------------------------.           ,--------------------------------------------------.
    |   Ø    |  Ø   |  Ø   |  Ø   |  Ø   |   Ø  |   Ø  |           |  Ø   |   Ø  |   Ø  |   Ø  |   Ø  |   Ø  |    Ø   |
    |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    |   Ø    |wPage |wPara |wLine |wWord | wDoc |   Ø  |           |  Ø   |      |      |      |      |      |    Ø   |
    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    |   Ø    |sPage |sPara |sLine |sWord | sDoc |------|           |------|      |   ^  |      |      |      |    Ø   |
    |--------+------+------+------+------+------|   Ø  |           |  Ø   |------+------+------+------+------+--------|
    |   Ø    |mPage |mPara |mLine |mWord | mDoc |      |           |      |  <   |   v  |   >  |      |      |    Ø   |
    `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
      |  Ø   |  Ø   |  Ø   |  Ø   |  Ø   |                                       |   Ø  |   Ø  | (Nav)|   Ø  |   Ø  |
      `----------------------------------'                                       `----------------------------------'
                                           ,-------------.       ,-------------.
                                           |   Ø  |   Ø  |       |  Ø   |  Ø   |
                                    ,------|------|------|       |------+------+------.
                                    |      |      |   Ø  |       |  Ø   |      |      |
                                    |   Ø  |   Ø  |------|       |------|  Ø   |  Ø   |
                                    |      |      |   Ø  |       |  Ø   |      |      |
                                    `--------------------'       `--------------------'

Planck:

    ,-----------------------------------------------------------------------.
    | Ø   |wPage|wPara|wLine|wWord|wDoc |     |     |     |     |     |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |sPage|sPara|sLine|sWord|sDoc |     |  ^  |     |     |     |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |mPage|mPara|mLine|mWord|mDoc |  <  |  v  |  >  |     |     |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |(Nav)|  Ø  |   Ø |
    `-----------------------------------------------------------------------'

*/

[NAVIGATION_KEYMAP] = KEYMAP
    (
        // Left

        // --------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    E(WORD),    E(LINE),    E(PARA),    E(PAGE),    E(DOC),     KC_TRNS,
        KC_TRNS,    KC_NO,      KC_NO,      KC_NO,      KC_UP,      KC_NO,
        KC_TRNS,    KC_NO,      KC_NO,      KC_LEFT,    KC_DOWN,    KC_RIGHT,   KC_TRNS,
        // --------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

                                                                    KC_TRNS,    KC_TRNS,
                                                                                KC_TRNS,
                                                        KC_TRNS,    KC_TRNS,    KC_TRNS,

        // Right

        // --------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_TRNS,
                    KC_NO,      KC_UP,      KC_NO,      KC_NO,      KC_NO,      KC_TRNS,
        KC_TRNS,    KC_LEFT,    KC_DOWN,    KC_RIGHT,   KC_NO,      KC_NO,      KC_TRNS,
        // --------------------------------------------------------------------------------
                                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS

    )


/*

Reasoning
---------

A dual-use layer: cursor and scroll navigation on the left and function keys on the right. These are conceptually similar in that both sides perform some kind of action instead of inserting characters.

### Navigation

I was tempted to move the cursor keys on my left hand over by one column to the right to put them under stronger fingers (ring, middle, index) and give my left pinkie a break, but the WASD key standard is just too widely used. I'm not a gamer, but my beloved "The Hit List" to-do organizer uses the WASD keys and I'd have to remap those somehow. The standard trumps any tweaking I might want to do.

What I've basically done here is to lift out the standard numeric keypad cursor key layout (complete with the page-scrolling keys in the corners of the square) and overlayed them onto the standard WASD cursor keys. That results in having two cursor-down keys, which violates my "only one way" rule, but that's ok.

Having another set of cursor keys on the right side also violates my "only one way" rule, but I have compelling reasons. Full keyboards and even tenkeyless boards have a cursor key cluster over here. Since I have to move my right hand off the home row to get to them, I can choose to use my stronger fingers on them (index, middle, ring). And because they're at the bottom right corner, I can even position my hand without looking down, by feeling for the bottom and right edges of the board. Lastly, I've already memorized these keys.

Say something about loss of right-side transparent keys

### Function Keys

My brain is already trained to use a numeric keypad, so why would I lay out the numbered function keys in any other way? Using function keys used to suck badly. I'd have to move my hand off the home row up to the very top row and then look down to locate the key. Worse, on OS X some of my keyboards also require me to hold down the "fn" modifier key and some don't, and I'd never know which (Yeah, I probably could have fixed that). Now, I just tap my layer button (which is more or less on the home row) with my left index finger for a single-shot and then hit a number key on the right. So easy.

The F10—F12 keys are a little awkward up there on the top row, but, logically, they belong there. I have no interest in F13—F24. Are those ever used?

### Teensy

The Teensy bootloader function gets bound to the "T" key on this layer, which is mnemonic, and it's not right next to the cursor key cluster where it might get hit accidentally, which is nice.


*/