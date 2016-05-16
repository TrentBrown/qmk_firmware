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

void UltramodConfigureFeature(const char* pName, bool enabled);  // Call from your matrix_init_user() function.
void UltramodConfigureTimeout(const char* pName, uint16_t milliseconds);  // Call from your matrix_init_user() function.

Plugin* UltramodCreatePlugin(void);


#endif //__ULTRAMOD
