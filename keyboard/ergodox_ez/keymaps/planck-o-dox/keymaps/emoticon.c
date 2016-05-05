/*

    Emoticon Keymap

    A good source: http://www.patrisser.com/fun/emoticon.html
    Unicode: https://gist.github.com/endolith/157796

    todo: Consider moving winking to semicolon...
    todo: Consider doing mnemonic layers optimized for Dvorak, Colemak, and Workman, too

Ergodox:
S
    ,--------------------------------------------------.           ,--------------------------------------------------.
    |   Ø    |  Ø   |  Ø   |  Ø   |  Ø   |   Ø  |   Ø  |           |  Ø   |   Ø  |   Ø  |   Ø  |   Ø  |   Ø  |    Ø   |
    |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    |   Ø    | :-X  | ;-)  | =:o  | 9_9  | :‑P  |   Ø  |           |  Ø   |  \o/ |  :-( | O:‑) |  :-o |  O_o |    Ø   |
    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    |   Ø    | >:(  | :‑/  | <:‑I | :‑[  | }:-)>|------|           |------|  :-) |  %-| |  :^* |  :‑D |      |    Ø   |
    |--------+------+------+------+------+------|   Ø  |           |  Ø   |------+------+------+------+------+--------|
    |   Ø    | -_-  | x_x  | :'‑( | >.<  | |‑O  |      |           |      |  :-V |  :-I |  <3  |      |      |    Ø   |
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

    ,-----------------------------------------------------------------------------------------------.
    | Ø     |  :-X  |  ;-)  |  =:o  |  9_9  |  :‑P  |  \o/  |  :-(  |  O:‑) |  :-o  |  O_o  |     Ø |
    |-----------------------------------------------------------------------------------------------|
    | Ø     |  >:(  |  :‑/  | <:‑I  |  :‑[  | }:-)> |  :-)  |  %-|  |  :^*  |  :‑D  |       |     Ø |
    |-----------------------------------------------------------------------------------------------|
    | Ø     |  -_-  |  x_x  | :'‑(  |  >.<  |  |‑O  |  :-V  |  :-I  |  <3   |       |       |     Ø |
    |-----------------------------------------------------------------------------------------------|
    | Ø     |   Ø   |   Ø   |   Ø   |   Ø   |   Ø   |   Ø   |   Ø   |   Ø   |   Ø   |   Ø   |     Ø |
    `-----------------------------------------------------------------------------------------------'

¯\_(ツ)_/¯
*/

[EMOTICON_KEYMAP] = KEYMAP
    (
        // Left

        // --------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,     KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    E(QUIET),   E(WINKING),  E(EXTREME), E(ROLLING), E(TOUNGUE), KC_TRNS,
        KC_TRNS,    E(ANGRY),   E(SKEPTICAL),E(DUNCE),   E(FROWNING),E(GUILTY),
        KC_TRNS,    E(ZZZ),     E(X_PARROT), E(CRYING),  E(VEXED),   E(BORED),   KC_TRNS,
        // --------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,     KC_TRNS,    KC_TRNS,

                                                                     KC_TRNS,    KC_TRNS,
                                                                                 KC_TRNS,
                                                         KC_TRNS,    KC_TRNS,    KC_TRNS,


        // Right

        // --------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,     KC_TRNS,
        KC_TRNS,    E(YAY),     E(UNHAPPY), E(INNOCENT),E(OH),      E(PERPLEXED),KC_TRNS,
                    E(HAPPY),   E(JUST),    E(KISSING), E(LAUGHING),KC_NO,       KC_TRNS,
        KC_TRNS,    E(N),       E(MEH),     E(HEART),   KC_NO,      KC_NO,       KC_TRNS,
        // --------------------------------------------------------------------------------
                                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,     KC_TRNS,

        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
    )


/*

Reasoning
---------

Because, fun!

Actually, I find that they're almost essential for communicating via text. It's way too easy for others to project their own moods/sentiments onto my plain text, or to misinterpret my state of mind.

I have a predilection for minimal, old-school ASCII. Could have mapped these to Unicode emoticons. Maybe I will. Or maybe I'll make a second "hi-res" emoticon layer for those.  :-)

Again used mnemonic of first letter of name...


| Key | String    | Mnemonic                 |
| --- | --------- | ------------------------ |
| A   | >:(       | "Angry" or "Annoyed"     |
| B   | &#124;‑O  | "Bored"                  |
| C   | :'‑(      | "Crying"                 |
| D   | <:‑I      | "Dunce"                  |
| E   | =:o       | "Extreme reaction"       |
| F   | :‑[       | "Frowning"               |
| G   | }:‑)>     | "Guilty"                 |
| H   | :-)       | "Happy"                  |
| I   | O:‑)      | "Innocent"               |
| J   | %-|       | "Just messed up"         |
| K   | :^*       | "Kissing"                |
| L   | :‑D       | "Laughing"               |
| M   | :-I       | "Meh"                    |
| N   | :-V       | "Not shutting up"        |
| O   | :-o       | "Oh"                     |
| P   | O_o       | "Perplexed"              |
| Q   | :-X       | "Quiet" Holding tongue   |
| R   | 9_9       | "Rolling eyes"           |
| S   | :‑/       | "Skeptical" or "Stumped" |
| T   | :‑P       | "Tongue out"             |
| U   | :-(       | "Unhappy"                |
| V   | >.<       | "Vexed"                  |
| W   | ;-)       | "Winking"                |
| X   | x_x       | An ex-parrot, mate       |
| Y   | \o/       | "Yay!"                   |
| Z   | -_- Zzzzz | "Zzzzz"                  |



    :-V   :-v   yell, blah blah, talking

*/