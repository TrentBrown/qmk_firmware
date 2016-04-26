/*

    Bracket and Number Keymap

    Notes on using number pad with the Mac's Calculator app:

    Return key is enter/equals.

    Plus and divide are the same as on the base layer and could be transparent here, but for clarity they are define
    explicitly.

    All the good stuff in on the home row: Addition, equals, decimal.

    TODO: Get AC binding onto center key. also xml comments


Ergodox:

    ,--------------------------------------------------.           ,--------------------------------------------------.
    |   Ø    |  Ø   |  Ø   |  Ø   |  Ø   |   Ø  |   Ø  |           |  Ø   |   Ø  |   Ø  |   Ø  |   Ø  |   Ø  |    Ø   |
    |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------| */
//  |   Ø    |  /*  |  */  |  [   |  ]   |      |   Ø  |           | AC   |   *  |   7  |   8  |   9  |   -  |    Ø   |
/*  |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    |   Ø    |  {   |  }   |  (   |  )   |      |------|           |------| Zero |   4  |   5  |   6  |   +  |    Ø   |
    |--------+------+------+------+------+------|   Ø  |           | +/-  |------+------+------+------+------+--------|
    |   Ø    | <!-- | -->  |  <   |  >   |      |      |           |      |   .  |   1  |   2  |   3  |   /  |    Ø   |
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

    ,-----------------------------------------------------------------------. */
//  | Ø   | /*  | */  |  [  |  ]  |     |  *  |  7  |  8  |  9  |  -  |   Ø |
/*  |-----------------------------------------------------------------------|
    | Ø   |  {  |  }  |  (  |  )  |     | Zero|  4  |  5  |  6  |  +  |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |<!-- | --> |  <  |  >  |     |  .  |  1  |  2  |  3  |  /  |   Ø |
    |-----------------------------------------------------------------------|
    | Ø   |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |  Ø  |   Ø |
    `-----------------------------------------------------------------------'

*/


[BRACKET_AND_NUMBER_KEYMAP] = KEYMAP
    (
        // Left

        // -------------------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,        KC_TRNS,    KC_TRNS,
        KC_TRNS,    E(LCC),     E(RCC),     KC_LBRACKET,    KC_RBRACKET,    KC_NO,      KC_TRNS,
        KC_TRNS,    KC_LCBR,    KC_RCBR,    KC_LPRN,        KC_RPRN,        KC_NO,
        KC_TRNS,    E(LXC),     E(RXC),     LSFT(KC_COMMA), LSFT(KC_DOT),   KC_NO,      KC_TRNS,
        // -------------------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,        KC_TRNS,

                                                                            KC_TRNS,    KC_TRNS,
                                                                                        KC_TRNS,
                                                            KC_TRNS,        KC_TRNS,    KC_TRNS,


        // Right

        // --------------------------------------------------------------------------------
        KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_NUMLOCK,     KC_ASTR,    KC_7,       KC_8,       KC_9,       KC_MINUS,   KC_TRNS,
                        KC_0,       KC_4,       KC_5,       KC_6,       KC_PLUS,    KC_TRNS,
        LALT(KC_MINUS), KC_DOT,     KC_1,       KC_2,       KC_3,       KC_TRNS,    KC_TRNS,
        // --------------------------------------------------------------------------------
                                    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

        KC_TRNS,        KC_TRNS,
        KC_TRNS,
        KC_TRNS,        KC_TRNS,    KC_TRNS
    )


/*

Reasoning
---------

A dual-use layer: cursor and scroll navigation on the left and function keys on the right. These are similar in that both sides perform some kind of action instead of typing characters.

### Brackets

For the sake of symmetry, I initially thought that putting left brackets on the left hand and right brackets on the right was best. I've seen others do this. Like maybe using the center keys like this:

    -------------.           ,-------------
          |   [  |           |  ]   |
    -------------|           |------+------
          |   (  |           |  )   |
    ------|      |           |      |------
          |------|           |------|
    ------|   {  |           |  }   |------
          |      |           |      |
    -------------'           `-------------


Soon realized that this was not optimal. When programming, think about how often you type empty lists:

    char[] createEmptyString() {}

Not true with above assigments:

With brackets on different hands, to produce empty lists you'd have to:

Momentary modifer: Hold right, tap left, release right, hold right, tap right, release right.
Single-shot modifier: Left tap shift, left key, right tap shift, right key

I realized that I want to break the naive high-level symmetry here, replacing it by another: using adjacent fingers on the same hand. After all, standard keyboards do this. Much faster to type:

Hold right shift, tap left and right with adjacent fingers using left hand.

Used frequency of use to determine which get the most accessible keys and strongest fingers:

1. Parenthises
2. Curly brackets
3. Square bracket
4. Angle brackets

Pontificate...
What about typing lots of HTML or XML?
XML is being replaced by JSON.
HTML sticking around. Need them to keep my new hot girlfriend happy: Polymer Web Components.

Parens get the strongest fingers on the home row because they're used the most.

### Numbers

Again, after decades of using a keyboard, I never memorized touch-typing numbers on the upper row, partly because I had a better way to enter numbers on the keypad. Now I can use a keypad layout without moving my hand off the home row. Even better: The Ergodox is ortholinear and the numbers are in vertical columns, just like the normal keypad.

Started by staying true to the standard keypad layout, simulating the taller 2x "+" and "Enter" keys by using two 1x keys for each:

    ---------------------------------
      |      |   /  |   *  |   -  |
    --+------+------+------+------+--
      |   7  |   8  |   9  |   +  |
    --|------+------+------+------+--
      |   4  |   5  |   6  |   +  |
    --|------+------+------+------+--
      |   1  |   2  |   3  | Enter|
    --+------+------+------+------+--
      |   0  |   0  |   .  | Enter|
      `------------------------------


Decided later to match what the Mac OS X calculator uses: the standard "four-banger" (add, subtract, multiply, divide) calculator layout:
![Calculator](http://cdn.shopify.com/s/files/1/0151/4021/products/728987018855_1024x1024.jpeg?v=1332352147)
![Calculator](/images/MacCalculator.png)

Yes, angle brackets are duplicated. I justify because in the one case they can be thought of as symbols (greater than, less than) and in the other as "brackets". Having them in their original position on the standard keyboard was just too strong of a mnemonic.

Code Comment Delimiters
=======================

For Java and C.

For XML (and HTML?)

If you're not a programmer, you won't find these comment delimeters useful. Just remove, or pick something else.S

*/