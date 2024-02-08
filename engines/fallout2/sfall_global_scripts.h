#ifndef FALLOUT2_SFALL_GLOBAL_SCRIPTS_H
#define FALLOUT2_SFALL_GLOBAL_SCRIPTS_H

#include "fallout2/interpreter.h"

namespace Fallout2 {

bool sfall_gl_scr_init();
void sfall_gl_scr_reset();
void sfall_gl_scr_exit();
void sfall_gl_scr_exec_start_proc();
void sfall_gl_scr_remove_all();
void sfall_gl_scr_exec_map_update_scripts(int action);
void sfall_gl_scr_process_main();
void sfall_gl_scr_process_input();
void sfall_gl_scr_process_worldmap();
void sfall_gl_scr_set_repeat(Program *program, int frames);
void sfall_gl_scr_set_type(Program *program, int type);
bool sfall_gl_scr_is_loaded(Program *program);
void sfall_gl_scr_update(int burstSize);

} // namespace Fallout2

#endif
