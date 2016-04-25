/*

    Symbol Keymap

    Consider infinity on N key "Never ends"
    Consider using multuply char for x


Ergodox:

    ,--------------------------------------------------.           ,--------------------------------------------------.
    |   Ø    |  Ø   |  Ø   |  Ø   |  Ø   |   Ø  |   Ø  |           |  Ø   |   Ø  |   Ø  |   Ø  |   Ø  |   Ø  |    Ø   |
    |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    |   Ø    |  ¿   |  ≈   |  =   |  ®   |  ~   |   Ø  |           |  Ø   |   —  |   _  |   |  |   ©  |   %  |    Ø   |
    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    |   Ø    |  @   |  *   |  -   |  !   |  `   |------|           |------|   #  |   "  |   '  |   &  |   +  |    Ø   |
    |--------+------+------+------+------+------|   Ø  |           |  Ø   |------+------+------+------+------+--------|
    |   Ø    | "Ø"  |  ¬   |  ^   |  √   |  •   |      |           |      |   ≠  |   $  |   °  |   …  |   \  |    Ø   |
    `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
      |  Ø   |  Ø   |  Ø   |  Ø   |  Ø   |                                       |   Ø  |   Ø  |   Ø  |   Ø  |   Ø  |
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
    | Ø   |  ¿  |  ≈  |  =  |  ®  |  ~  |  —  |  _  |  |  |  ©  |  %  |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |  @  |  *  |  -  |  !  |  `  |  #  |  "  |  '  |  &  |  +  |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   | "Ø" |  ¬  |  ^  |  √  |  •  |  ≠  |  $  |  °  |  …  |  \  |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |   Ø |
    `-----------------------------------------------------------------------'

*/

[SYMBOL_KEYMAP] = KEYMAP
    (
        // Left

        // ---------------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    LALT(LSFT(KC_SLSH)),LALT(KC_X), KC_EQUAL,   LALT(KC_R), KC_TILD,    KC_TRNS,
        KC_TRNS,    KC_AT,              KC_ASTR,    KC_MINUS,   KC_EXLM,    KC_GRAVE,
        KC_TRNS,    LALT(KC_O),         LALT(KC_L), KC_CIRC,    LALT(KC_V), LALT(KC_8), KC_TRNS,
        // ---------------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,            KC_TRNS,    KC_TRNS,    KC_TRNS,

                                                                            KC_TRNS,    KC_TRNS,
                                                                                        KC_TRNS,
                                                                KC_TRNS,    KC_TRNS,    KC_TRNS,


        // Right

        // --------------------------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,            KC_TRNS,        KC_TRNS,      KC_TRNS,        KC_TRNS,     KC_TRNS,
        KC_TRNS,    LALT(LSFT(KC_MINS)),KC_UNDS,        KC_PIPE,      LALT(KC_G),     KC_PERC,     KC_TRNS,
                    KC_HASH,            LSFT(KC_QUOT),  KC_QUOTE,     KC_AMPR,        KC_PLUS,     KC_TRNS,
        KC_TRNS,    LALT(KC_EQL),       KC_DLR,         LALT(KC_0),   LALT(KC_SCLN),  KC_BSLASH,   KC_TRNS,
        // --------------------------------------------------------------------------------------------------
                                        KC_TRNS,        KC_TRNS,      KC_TRNS,        KC_TRNS,     KC_TRNS,

        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
    )

/*

Reasoning
---------

Just looks like a random symbol soup, right? Ah, but I have reasons for everything, my friend.

My first version simply moved the symbols down from the top row to the home row, leaving them in the same finger positions:

    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    |   `    |  !   |  @   |  #   |  %   |  ^   |------|           |------|   &  |   *  |   (  |   )  |   -  |    =   |
    |--------+------+------+------+------+------|   Ø  |           |  Ø   |------+------+------+------+------+--------|

That would be more familiar and I'd have some hope of being able to use a standard keyboard again, when away from mine. As I've said, I had *some* of these memorized. As I ruminated over this, and started adding in other symbols, it just started to feel awkward and arbitrary. After a time, I decided to abandon the standard and instead use a mnemonic device to remember where the characters are, and to aid in building muscle memory was a better idea. I settled on using the first letter of the character name to determine where it lives. If I can't remember where the caret symbol key is, for example, I just say it out loud (in my head) and type the first letter, "C". As time goes on, my muscle-memory will take over. When more than one character competed for the same position, I'd let the one I used the most be closest to the home row and use stronger fingers.

A mix of uses. Both for programming and for enhancing my natural language while texting.

What I came up with:

| Key | Symbol | Mnemonics                                                                                             |
| --- | ------ | ----------------------------------------------------------------------------------------------------- |
| A   | @      | "At"                                                                                                  |
| B   | •      | "Bullet"                                                                                              |
| C   | ^      | "Caret"                                                                                               |
| D   | -      | "Dash" The visually similar "=" is just above it.                                                     |
| E   | =      | "Equals" The visually similar "-" is just below it.                                                   |
| F   | !      | "F&#42;&#42;k" (Really wanted this on home row and not using "B" for "bang" frees that up for "bullet")      |
| G   | `      | "Grave accent mark"                                                                                   |
| H   | #      | "Hash" Could have used P for "Pound", but there's too much competition (plus, percent, pipe)          |
| I   | |      | Pipe looks like a sans-serif "I".                                                                     |
| J   | "      | Can't think of a mnemonic. Just want this on home row under a strong finger because it's used so much.|
| K   | '      | Can't think of a mnemonic. Just want this on home row under a strong finger because it's used so much.|
| L   | &#124; | Lame mnemonics: "Lampersand", "Link"                                                                  |
| M   | $      | "Money"                                                                                               |
| N   | ≠      | "Not equal" (Am torn between this and "eN-dash" or "!=" sequence)                                     |
| O   | ©      | Looks circular (And "C" is taken by caret, which has nowhere else to go)                              |
| P   | %      | "Percent"                                                                                             |
| ;   | +      | Could have used "P" key just above, but "Percent" is there, and would rather have "Plus" on home row. |
| Q   | ¿      | "Question mark" (upside down). So I can show off my high school Spanish skilz.                        |
| R   | ®      | "Registration mark"                                                                                   |
| S   | *      | "Star", "Splat", "aSteriSk"                                                                           |
| T   | ~      | "Tilde"                                                                                               |
| U   | _      | "Underscore"  Would have liked this on the home row, but at least it's no longer a double-pinkie char.|
| V   | √      | Looks like a checkmark.                                                                               |
| W   | ≈      | "Water", "Wave". "eqWivalent" Whatever...                                                                          |
| X   | ¬      | Character denotes negation, kinda. "Ex it out."                                                       |
| Y   | —      | "wYde dash" (em-dash). I'm a typography geek. "Please come to my cocktail party from 6:00—10:00 PM, Saturday"   |
| Z   | Ø      | "Zip", "Zilch", "Zero".                                                                               |
| /   | \      | Same position as forward slash on base layer. Makes sense that it is being modified here.             |
| .   | …      | Ellipis character on the dot key. Replaces Option-. on a Mac.                                         |

*/