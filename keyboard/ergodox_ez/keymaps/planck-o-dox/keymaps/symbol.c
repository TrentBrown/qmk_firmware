/*

    Symbol Keymap

    todo: Consider using ✕ char for X and back to ✓ for V as complementary symbols and then put ∞ on ("Never ends" or "Nfinity")

    todo: Decide whether you like ∞ (LALT(KC_8)) or ≠ (LALT(KC_EQL)) better on the N key.

    todo: Consider doing mnemonic layers optimized for Dvorak, Colemak, and Workman, too



Ergodox:

    ,--------------------------------------------------.           ,--------------------------------------------------.
    |   Ø    |  Ø   |  Ø   |  Ø   |  Ø   |   Ø  |   Ø  |           |  Ø   |   Ø  |   Ø  |   Ø  |   Ø  |   Ø  |    Ø   |
    |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    |   Ø    |  ¿   |  ≈   |  =   |  ®   |  ~   |   Ø  |           |  Ø   |   —  |   _  |   |  |   °  |   %  |    Ø   |
    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    |   Ø    |  @   |  *   |  -   |  !   |  `   |------|           |------|   #  |   '  |   "  |   &  |   +  |    Ø   |
    |--------+------+------+------+------+------|   Ø  |           |  Ø   |------+------+------+------+------+--------|
    |   Ø    | "Ø"  |  ✕   |  ^   |  √   |  •   |      |           |      |   ∞  |   $  |   ©  |   …  |   \  |    Ø   |
    `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
      |  Ø   |  Ø   |  Ø   |  Ø   |(Sym) |                                       | (Sym)|   Ø  |   Ø  |   Ø  |   Ø  |
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
    | Ø   |  ¿  |  ≈  |  =  |  ®  |  ~  |  —  |  _  |  |  |  °  |  %  |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |  @  |  *  |  -  |  !  |  `  |  #  |  '  |  "  |  &  |  +  |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   | "Ø" |  ✕  |  ^  |  √  |  •  |  ∞  |  $  |  ©  |  …  |  \  |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |  Ø  |  Ø  |  Ø  |(Sym)|  Ø  |  Ø  |(Sym)|  Ø  |  Ø  |  Ø  |   Ø |
    `-----------------------------------------------------------------------'

*/

#ifdef ERGODOX
[SYMBOL_KEYMAP] = KEYMAP
    (
        // Left

        // ---------------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    LALT(LSFT(KC_SLSH)),LALT(KC_X), KC_EQUAL,   LALT(KC_R), KC_TILD,    KC_TRNS,
        KC_TRNS,    KC_AT,              KC_ASTR,    KC_MINUS,   KC_EXLM,    KC_GRAVE,
        KC_TRNS,    LALT(KC_O),         LALT(KC_L), KC_CIRC,    LALT(KC_5), LALT(KC_8), KC_TRNS,
        // ---------------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,            KC_TRNS,    KC_TRNS,    KC_TRNS,

                                                                            KC_TRNS,    KC_TRNS,
                                                                                        KC_TRNS,
                                                                KC_TRNS,    KC_TRNS,    KC_TRNS,


        // Right

        // ---------------------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,            KC_TRNS,    KC_TRNS,      KC_TRNS,         KC_TRNS,     KC_TRNS,
        KC_TRNS,    LALT(LSFT(KC_MINS)),KC_UNDS,    KC_PIPE,      LSFT(LALT(KC_8)),KC_PERC,     KC_TRNS,
                    KC_HASH,            KC_QUOTE,   LSFT(KC_QUOT),KC_AMPR,         KC_PLUS,     KC_TRNS,
        KC_TRNS,    LALT(KC_EQL),       KC_DLR,     LALT(KC_G),   LALT(KC_SCLN),   KC_BSLASH,   KC_TRNS,
        // ---------------------------------------------------------------------------------------------
                                        KC_TRNS,    KC_TRNS,      KC_TRNS,         KC_TRNS,     KC_TRNS,

        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
    )
#endif // ERGODOX

#ifdef PLANCK
    [SYMBOL_KEYMAP] =
    {
        {KC_TRNS, LALT(LSFT(KC_SLSH)), LALT(KC_X), KC_EQUAL,   LALT(KC_R), KC_TILD,    LALT(LSFT(KC_MINS)), KC_UNDS,  KC_PIPE,       LSFT(LALT(KC_8)), KC_PERC,   KC_TRNS},
        {KC_TRNS, KC_AT,               KC_ASTR,    KC_MINUS,   KC_EXLM,    KC_GRAVE,   KC_HASH,             KC_QUOTE, LSFT(KC_QUOT), KC_AMPR,          KC_PLUS,   KC_TRNS},
        {KC_TRNS, LALT(KC_O),          LALT(KC_L), KC_CIRC,    LALT(KC_5), LALT(KC_8), LALT(KC_EQL),        KC_DLR,   LALT(KC_G),    LALT(KC_SCLN),    KC_BSLASH, KC_TRNS},
        {KC_TRNS, KC_TRNS,             KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,             KC_TRNS,  KC_TRNS,       KC_TRNS,          KC_TRNS,   KC_TRNS}
    }
#endif // PLANCK

/*

Let me just say, this key is prime real estate. It is a thumb key. It's where my thumb naturally rests. It is a better-positioned thumb key than even the closest key on the actual thumb cluster! As such, it is ideal for the symbol layer because that is the most-used layer after the base layer.

Reasoning
---------

Just looks like a random symbol soup, right? Ah, but I have reasons for everything, my friend.

My first version simply moved the symbols down from the top row to the home row, leaving them in the same finger positions:

    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    |   `    |  !   |  @   |  #   |  %   |  ^   |------|           |------|   &  |   *  |   (  |   )  |   -  |    =   |
    |--------+------+------+------+------+------|   Ø  |           |  Ø   |------+------+------+------+------+--------|

That would be more familiar and I'd have some hope of being able to use a standard keyboard again, when away from mine. As I've said, I had *some* of these memorized. As I ruminated over this, and started adding in other symbols, it just started to feel awkward and arbitrary. After a time, I decided to abandon the standard and instead use a mnemonic device to remember where the characters are, and to aid in building muscle memory. I settled on using the first letter of the character name to determine where it lives. If I can't remember where the caret symbol key is, for example, I just say it out loud (in my head) and type the first letter, "C". As time goes on, my muscle-memory will take over. When more than one character competed for the same position, I'd let the one I used the most be closest to the home row and use stronger fingers.

A mix of uses. Both for programming and for enhancing my natural language while texting.

Single and double quote: Had these two keys reversed for a while and it just felt wrong, though I couldn't put my finger on why (haha). I think it was because I expected the progression of number of marks to go up from left to right. Also that the index finger of the left hand is used to hit the grave accent, even though that is not directly under the finger. But mostly I think it's because the number one and number two of the number layer are just below the two keys, and I was unconsciously thinking of there being one and two marks in the two characters.


What I came up with:

| Key | Symbol | Mnemonics                                                                                             |
| --- | ------ | ----------------------------------------------------------------------------------------------------- |
| A   | @      | "At"                                                                                                  |
| B   | •      | "Bullet"                                                                                              |
| C   | ^      | "Caret"                                                                                               |
| D   | -      | "Dash" The visually similar "=" is just above it.                                                     |
| E   | =      | "Equals" The visually similar "-" is just below it.                                                   |
| F   | !      | "F&#42;&#42;k!" (Really wanted this on home row and not using "B" for "bang" frees that up for "bullet")      |
| G   | `      | "Grave accent mark"                                                                                   |
| H   | #      | "Hash" Could have used P for "Pound", but there's too much competition: plus, percent, pipe           |
| I   | |      | "pIpe" A sans-serif "I" looks like a pipe character.                                                         |
| J   | '      | Can't think of a mnemonic. Just want this on home row under a strong finger because it's used so much.|
| K   | "      | Can't think of a mnemonic. Just want this on home row under a strong finger because it's used so much.|
| L   | &#124; | Lame: "Lampersand", "Link"                                                                            |
| M   | $      | "Money"                                                                                               |
| N   | ∞      | "Nfinity" Not a useful char, but fun to drop into a text conversation. "I <3 u ∞"                     |
| O   | °      | Looks circular.                                                                                       |
| P   | %      | "Percent"                                                                                             |
| ;   | +      | Same position as on the number layer.                                                                 |
| Q   | ¿      | "Question mark" (upside down). So I can show off my high school Spanish skilz.                        |
| R   | ®      | "Registration mark"                                                                                   |
| S   | *      | "Star", "Splat", "aSteriSk"                                                                           |
| T   | ~      | "Tilde"                                                                                               |
| U   | _      | "Underscore"  Would have liked this on the home row, but at least it's no longer a double-pinkie char.|
| V   | √      | Checkmark is shaped like a "v"                                                                            |
| W   | ≈      | "Water", "Wave", "eqWivalent" Whatever...                                                             |
| X   | ¬      | Character denotes negation, kinda. "Ex it out."                                                       |
| Y   | —      | "wYde dash" (em-dash). I'm a typography geek. "Please come to my cocktail party from 6:00—10:00 PM, Saturday"   |
| Z   | Ø      | "Zip", "Zilch", "Zero".                                                                               |
| /   | \      | Same position as forward slash on base layer. Makes sense that it is being modified here.             |
| ,   | ©      | The word "comma" starts with a "C", just like the word "copyright". (And "C" is taken by caret, which is used more) |
| .   | …      | Ellipis character in same position as the single dot key on base layer.                               |

*/