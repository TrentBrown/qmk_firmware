/*

    Hyper Keymap

Ergodox:

    ,--------------------------------------------------.           ,--------------------------------------------------.
    |   Ø    |  Ø   |  Ø   |  Ø   |  Ø   |   Ø  |   Ø  |           |  Ø   |   Ø  |   Ø  |   Ø  |   Ø  |   Ø  |    Ø   |
    |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
    |   Ø    |      |      |Energy|Radio | Term |   Ø  |           |  Ø   |      |      | Idea |  Out | 1Pass|    Ø   |
    |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
    |   Ø    | Addr |Slack | DB   |Finder| Git  |------|           |------|HitLst|JoinMe|      |LoseIt|      |    Ø   |
    |--------+------+------+------+------+------|   Ø  |           |  Ø   |------+------+------+------+------+--------|
    |   Ø    |      |      | Cal  |Voice | Brow |      |           |      |NumClc| Mail |      |      |      |    Ø   |
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


*/

[HYPER_KEYMAP] = KEYMAP
    (
        // Left

        // --------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    HYPR(KC_Q), HYPR(KC_W), HYPR(KC_E), HYPR(KC_R), HYPR(KC_T), KC_TRNS,
        KC_TRNS,    HYPR(KC_A), HYPR(KC_S), HYPR(KC_D), HYPR(KC_F), HYPR(KC_G),
        KC_TRNS,    HYPR(KC_Z), HYPR(KC_X), HYPR(KC_C), HYPR(KC_V), HYPR(KC_B), KC_TRNS,
        // --------------------------------------------------------------------------------
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

                                                                    KC_TRNS,    KC_TRNS,
                                                                                KC_TRNS,
                                                        KC_TRNS,    KC_TRNS,    KC_TRNS,


        // Right

        // --------------------------------------------------------------------------------
        KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,  HYPR(KC_Y), HYPR(KC_U), HYPR(KC_I), HYPR(KC_O), HYPR(KC_P), KC_TRNS,
                  HYPR(KC_H), HYPR(KC_J), HYPR(KC_K), HYPR(KC_L), KC_NO,      KC_TRNS,
        KC_TRNS,  HYPR(KC_N), HYPR(KC_M), KC_NO,      KC_NO,      KC_NO,      KC_TRNS,
        // --------------------------------------------------------------------------------
                              KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

        KC_TRNS,    KC_TRNS,
        KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS
    )


/*

At first, I didn't get what a "hyper" key was, when I saw it on other people's layouts. Before the light went on in my head, I created a whole layer to map my application switching key to. Then I realized that this was pointless. The mapping from chorded-letters to an action has to be done by an external application, after all. This exactly what "hyper is for. For the sake of being interesting, I'm including here my "virtual" layer. I expect you'll have a very different set of applications in your world. There are many maps. This one is mine.


    Reasoning
    ---------

    Needs to be used with another application that maps from the emitted character sequences to launch the given app.


    ### Mapping

    A   "Address book"
    B   "Browser"
    C   "Calendar"
    D   "Database client"
    E   "Energy" (system monitor app)
    F   "Finder"
    G   "Git GUI client"
    H   "Hit List" to-do app
    I   "IntellJ Idea IDE"
    J   "JoinMe"
    K
    L   "LoseIt" diet tracking
    M   "Mail client" (Apple Mail)
    N   "Numbers" for calculator
    O   "Outside texting app" (iMessage)
    P   "Password manager"  (1Password)
    Q
    R   "Radio" music streaming (Radium)
    S   "Slack"
    T   "Terminal" app (iTerm2)
    U
    V   "Voice" (Skype or ScreenHero)
    W
    X
    Y
    Z


*/