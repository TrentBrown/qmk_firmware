#ifndef __ULTIMOD
#define __ULTIMOD


// Configuration property names

#define ULTIMOD_TIMEOUT_SINGLE_TAP "single_tap"
#define ULTIMOD_TIMEOUT_DOUBLE_TAP "double_tap"
#define ULTIMOD_TIMEOUT_ONE_SHOT "one_shot"
#define ULTIMOD_TIMEOUT_LOCKED "locked"

#define ULTIMOD_FEATURE_FOO "foo"


// Configuration property default values

#define ULTIMOD_TIMEOUT_SINGLE_TAP_DEFAULT 300
#define ULTIMOD_TIMEOUT_DOUBLE_TAP_DEFAULT 300
#define ULTIMOD_TIMEOUT_ONE_SHOT_DEFAULT 3000
#define ULTIMOD_TIMEOUT_LOCKED_DEFAULT 3000


// Public functions

void ultimod_configure_feature(const char* p_name, bool enabled);  // Call from your matrix_init_user() function.
void ultimod_configure_timeout(const char* p_name, uint16_t milliseconds);  // Call from your matrix_init_user() function.

// TODO: Much better: Add another "plugin" hook for this!!!!
void ultimod_matrix_scan(void);  // Call from your matrix_scan_user() function.

void ultimod_set_leds(void);  // Optionally call from your matrix_scan_user() function.


#endif //__ULTIMOD
