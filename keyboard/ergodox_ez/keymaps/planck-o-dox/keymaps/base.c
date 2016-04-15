/*

Base (Alphabet) Keymap

Ergodox:

    ,--------------------------------------------------.           ,--------------------------------------------------.
    | Escape | Undo | Cut  | Copy |Paste |      |      |           |  <<  | Pause|  >>  |  V-  | Mute |  V+  |   Base |
    |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    | Tab    |  Q   |  W   |  E   |  R   |  T   |  :-) |           | (-:  |   Y  |   U  |   I  |   O  |   P  |   Swap |
    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    | Command|  A   |  S   |  D   |  F   |  G   |------|           |------|   H  |   J  |   K  |   L  |   ;  |  Enter |
    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    | Shift  |  Z   |  X   |  C   |  V   |  B   |      |           |      |   N  |   M  |   ,  |   .  |   /  |  Shift |
    `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
      | Ctl  |Option| NAF  | BAN  |Symbol|                                       |Symbol|  BAN |  NAF |Option|  Ctl |
      `----------------------------------'                                       `----------------------------------'
                                           ,-------------.       ,-------------.
                                           |      |      |       |      |      |
                                    ,------|------|------|       |------+------+------.
                                    |Back  |      | Home |       | PgUp |      |      |
                                    |Space |Delete|------|       |------| Hyper| Space|
                                    |      |      |  End |       | PgDn |      |      |
                                    `--------------------'       `--------------------'


Planck:

    ,-----------------------------------------------------------------------.
    |Tab  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  | Swap|
    |-----------------------------------------------------------------------|
    |Cmd  |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |Enter|
    |-----------------------------------------------------------------------|
    |Shift|  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |Shift|
    |-----------------------------------------------------------------------|
    |Ctl  | Opt | NAF | BAN | Sym |BkSpc|Space| Sym | BAN | NAF | Opt |  Ctl|
    `-----------------------------------------------------------------------'

*/


[BASE_KEYMAP] = KEYMAP
    (
        // Left

        // --------------------------------------------------------------------------------
        KC_ESCAPE,  LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), KC_NO,      KC_NO,
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       MO(A(EMO)),
        KC_LGUI,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,
        KC_LSHIFT,  KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_NO,
        // --------------------------------------------------------------------------------
        KC_LCTRL,   KC_LALT,    MO(A(NAF)), MO(A(BAN)), MO(A(SYM)),

                                                                    KC_NO,      KC_NO,
                                                                                KC_HOME,
                                                        KC_BSPACE,  KC_DELETE,  KC_END,


        // Right  KC_MPRV,    KC_MPLY,    KC_MNXT

        // --------------------------------------------------------------------------------
        KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_VOLD,    KC_MUTE,    KC_VOLU,    TO(K(BAS), ON_PRESS),
        MO(A(EMO)), KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       LGUI(KC_TAB),
                    KC_H,       KC_J,       KC_K,       KC_L,       KC_SCOLON,  KC_ENTER,
        KC_NO,      KC_N,       KC_M,       KC_COMMA,   KC_DOT,     KC_SLASH,   KC_RSHIFT,
        // --------------------------------------------------------------------------------
                                MO(A(SYM)), MO(A(BAN)), MO(A(NAF)),    KC_RALT,    KC_RCTRL,

        KC_NO,      KC_NO,
        KC_PGUP,
        KC_PGDOWN,  MO(K(HYPER)),     KC_SPACE
    )


/*

Reasoning
---------


### Alpha Layout

I've spent a long time getting fast with QWERTY. As tempting as it may be, rewiring my brain for the end-game [Workman layout](http://www.workmanlayout.com/) is just too much for me. I'll learn it from scratch when I'm reborn into my next lifetime --when they're teaching it to every kid in grade school. Or will we all just have wireless I/O chips implanted in our brainstems at birth?


### Clipboard

I'm ambi-mousterous and do a lot of cut-copy-pasting with one hand while drag-selecting and cursor-placing with my other hand on the mouse. I therefore want dedicated Cut, Copy and Paste keys on both sides of my base layer. Is this an excessive use of bottom-row real estate? Yes, and I don't care. It's simple, symmetrical, I use it all the time, and I have plenty of room for stuff on other layers. Plus, I have an excuse to add a splash of color to my keyboard by putting my delicious blue PBT keycaps to highlight these six keys. (Pretty!)[add to GitHub repop]

Damn. I really like this row of keys. Use them all the time and so easy to get to.
S

### Top Row

What? No numbers and symbols row? No function key row? Even after all my years of typing, I'm embarrassed to admit that don't have these all memorized. I still look down at the board to type some of them. They're just too darn far away and, worse, they have ever-so-slightly different locations on my various standard and "ergonomic" keyboards and so I often mistype even the ones I do know. I've got a much better idea: Move them down near the home keys on their own layer! See my "Symbol" layer below.

Here's another reason: I have this crazy idea that I'd also like a Planck keyboard. Have you seen these? They're beautiful. They look like an open box of See's candies, all in neat little rows, so delicious that you just want to pick out the caps and eat them one by one. https://i.warosu.org/data/g/img/0463/93/1423017533925.png And somehow make it similar enough to my Ergodox that I can use it. I wouldn't have a top row for this, and so I'd rather train myself not to expect one.

So, what *do* I want on my top row? Things that I don't mind taking my hands of the home row for. Media keys. Most of these show through on all my layers. In particular, I want to be able to mute or stop my music when I get a phone call, without worrying about which layer I might be on.

The Mac has volume controls on the far right, so I kept that convention. The standard layout has Mute, Volume Down, and Volume Up, in that order. This has always seemed wrong to me. Just as the play/pause button is in the middle, between the previous and next track keys, the mute ought to be between the volume down and volume up keys. Because "mute" is conceptually like "pause", right?. Symmetry and common sense trump the standard, in this case, so I've moved the mute key.


### Quotes vs. Backslash/Pipe

I use quotation marks all the time, so keeping them on the base layer makes sense. I use double-quotes more and hate having to shift to get at them, so why not have a dedicated key for them, too? I put them just above the single-quote because I don't use the pipe or backslash as much (Yay, *nix pathname strings) and moved those both to my symbol layer. If you don't like this, just put the backslach/pipe key back and get to double-quote the standard way.


### Thumb Clusters

Am sticking with the biggest thumb keys. Because I realize that I always use my right thumb for spaces.

Talk about smaller thumb keys. Have low and high profile caps on these. Is pretty easy to reach the center keys with index fingers, but it's yet easier to hit these cluster keys with my thumb. Currently I'm using the very low profile Signature Plastics DSA blank keycaps on my board. I use one of my blank OEM profile row 4 caps in this position. It sticks up far enough that I can position the side of my thumb over it and depress it without depressing the low profile key just below it.

What I've realized is that even though my xxx key is at the furthest extreme on the board, it's the easiest modifier key to hit and does not require taking my fingers off the home row. Because of this, I give it to the non-base layer that I use most: Symbols. Super easy


### Keyswitches

This isn't a layout thing, but I want to talk about it anyway, because it's relevant, and my choice of keyswitches is such a big reason that I love my keybaord. I spent ages thinking about what kind (ie. "color") of mechanical keyswitch to put in my board. I bought keyswitch testers to really get the feel of each, since reading about other people's impressions of their switches is like watching a dance about architecture, so to speak. I highly recommend buying these kits. Even well after you've made your decision, you'll have a great little fiddly-toy for your desk to play with neurotically to reduce stress.

After trying them all for many days (keeping my key testers in my hoodie pockets to covertly diddle in idle moments) I decided that I love two switches almost equally: MX Blues (light, tactile, clicky) and Purple Zealios (many weights, tactile, quiet).

O, Little Switches, how do I love thee? Let me count the ways:

Pressing an MX Blue key is like popping a tiny little plastic bubble-wrap balloon, with the same satisfying, addictive quality. I hear and feel the sharp click, and a character appears on my screen. A little hit of pleasure, repeated thousands of times each day.

Pressing a purple Zealio is like pressing your finger against the skin of a ripe peach until it suddenly breaks through into the ripe juicy insides. I can't take credit for this last image. Someone more creative than me came up with it, but I've since lost track of who. And, for the keyboard cognoscenti I will assert: A 62 gram Zealio is what a Cherry MX Brown *should* feel like. A 67 gram Zealio is what a Cherry MX Clear *should* feel like.

Which of these to pick, then? Why, both, of course. This is my keyswitch love triangle:

I use Blues for any key that produces a character on the screen, or performs some immediate action --generally the keys on the interior of the board. Click == something happens.

I use Zelios for modifier keys and thumb keys. Bump == something's about to happen.

The space, return, delete and backspace thumb keys *do* produce or remove characters on the screen and so, by my reasoning above, should rightly be clicky Blues, but I break this rule because it avoids the most common complaint about Blues: They're not so good for quick multiple presses --something commonly done with these keys. Blues have what's called "hysterisis", which means that the point where they reset for a another press, is significantly above where they actuate (making electrical contact). This means it's easy to mis-fire because you're not letting the key travel up far enough to reset before pushing it back down again. That's why many (though not all) gamers don't like them.

Since Zelios come in a wide range of weights (62, 65, 67, 78 grams), I also have the wonderful luxury of optimizing for finger strength. I use lighter 62 gram switches for my pinkies (extreme left and right columns on the board) and heavier 67 gram switches for the thumb cluster and center modifier columns. Why not also use 65 gram switches? Dunno. I could optimize further, I guess, but frankly the 65's feel very slightly "mushy" to me, not having either the sharper contrast of the 62's, or the softer bottom-out of the 67's. YMMV.

The other main complaint about the Blues is that they're loud. This isn't a problem for me because:

1. I work alone in a my home office, currently. There's nobody to annoy but my own sorry self.
2. I sometimes crave human companionship and will leave my cell to walk down to the local cafe, bringing my laptop and Ergodox with me. The cafe is loud. People are talking, canned music is playing. Almost nobody can hear my Blues. And if they do, it's a great conversation icebreaker. "What *is* that thing?", they will ask. (Usually a dude, but sometimes an attractive female of the species.)
3. In terms of raw sound energy, *way* more is produced by the bottoming-out sound when the cap hits the top of the switch (the "clack"). Take a listen to one of the YouTube videos made by Blues owners and you'll hear what I mean. In these, the "clicks" are entirely obscured by the "clacks". What I've done is to add rubber O-rings to my caps. This eliminates the "clacks" almost entirely, making the board much quieter, highliting the "clicks", which I like, and protecting my fingers from the stress of bottoming out.


### Layer Keys

"Alpha" Clarify: This is a "set" command. The other layer keys (eg. kkk and kkk) have momentary, single-tap, and double-tap modes, but this key only does one thing: It takes you back to the base layer unconditionally, no matter where you are. There's no place like home.

Explain the modes
Almost always, I just want to capitalize a single charater, so I use the tap-for-single-shot mode.
    To handle typing a longer sequence of caps, like for abbreviations, or program constants, I have a trick: I've hacked
    the software to turn a double-tap into a modifier lock. This is how *all* modifier keys should work! Hold for momentary,
    tap for single-shot, double-tap for lock. I use this same convention for all my modifier keys: shift, option, command,
    control.


### Command and Caps Lock Keys

I love having the Mac's Command key where the caps lock usually lives. Soooo easy to tap my left pinkie on the home row to set up the single-shot for the commands I use a hundred times a day: Cmd-S, Cmd-A, Cmd-Z, Cmd-W, Cmd-F... No more contorting my thumb to reach under, or moving my whole hand away from the home row to hold down the modifier to produce the chord.

And what have I done with caps lock, you ask? Almost always, I just want to capitalize a single character, so I use the tap-for-single-shot mode on the shift key. To handle typing a longer sequence of caps, like for abbreviations, or program constants, I have a trick: I've hacked the software to turn a double-tap into a modifier lock.

1. Hold for momentary
2. Tap for single-shot
3. Double-tap for lock
4. Tap to unlock

1. Rarely use this for typing anymore, but do for cursor keys and paging
2. 95% of all uses, I've found. Capitalize words, camel-case identifiers, hit single symbols, like dash and underscore)
3. For typing abbreviations, or paging through documents

Since I've been using this, I now believe that this is how *all* keyboards in the world should work! I use this convention for *all* my modifier keys: shift, option, command, control.

### Blanks

You fill in. Strange, but I've just lefy yhem blank.
Reccommend leaving the xxx unbound until you get used to Ergodox. Or you'll be hitting it insted of "b" and doing soemtning you didnt want. better to do niothing insted of cleaning uo.

### Swap key

Try it. You'll like it. Is like having another monitor, almost. Especially nice when you're away from your multiple-monitor setup at home or the office.

*/