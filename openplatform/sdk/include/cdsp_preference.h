#ifndef __CDSP_PREFERENCE_H__
#define __CDSP_PREFERENCE_H__
#include "mach/gp_cdsp.h"

#define LENS_GP 0
#define LENS_8172 1
#define LENS_PW650 2
#define LENS_20 3
#define LENS_MJ2520B 4
#define LENS_3006 5
#define LENS_7023 6

#define LENS LENS_3006//LENS_GP

#if (LENS_GP == LENS)
#include "Lens/cdsp_preference_gp.h"
#elif (LENS_8172 == LENS)
#include "Lens/cdsp_preference_8172.h"
#elif (LENS_PW650 == LENS)
#include "Lens/cdsp_preference_pw650.h"
#elif (LENS_20 == LENS)
#include "Lens/cdsp_preference_20.h"
#elif (LENS_MJ2520B == LENS)
#include "Lens/cdsp_preference_MJ2520B.h"
#elif (LENS_3006 == LENS)
#include "Lens/cdsp_preference_3006.h"
#elif (LENS_7023 == LENS)
#include "Lens/cdsp_preference_7023.h"
#endif
#endif
