#ifndef __ULTRAMOD
#define __ULTRAMOD


// Configuration property names

#define ULTRAMOD_TIMEOUT_SINGLE_TAP "single_tap"
#define ULTRAMOD_TIMEOUT_DOUBLE_TAP "double_tap"
#define ULTRAMOD_TIMEOUT_ONE_SHOT "one_shot"
#define ULTRAMOD_TIMEOUT_LOCKED "locked"

#define ULTRAMOD_FEATURE_FOO "foo"


// Configuration property default values

#define ULTRAMOD_TIMEOUT_SINGLE_TAP_DEFAULT 300
#define ULTRAMOD_TIMEOUT_DOUBLE_TAP_DEFAULT 300
#define ULTRAMOD_TIMEOUT_ONE_SHOT_DEFAULT 3000
#define ULTRAMOD_TIMEOUT_LOCKED_DEFAULT 3000


// Public functions

void ultramod_configure_feature(const char* p_name, bool enabled);  // Call from your matrix_init_user() function.
void ultramod_configure_timeout(const char* p_name, uint16_t milliseconds);  // Call from your matrix_init_user() function.

// TODO: Much better: Add another "plugin" hook for this!!!!
void ultramod_matrix_scan(void);  // Call from your matrix_scan_user() function.

void ultramod_set_leds(void);  // Optionally call from your matrix_scan_user() function.


#endif //__ULTRAMOD
