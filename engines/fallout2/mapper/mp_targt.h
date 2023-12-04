#ifndef FALLOUT2_MAPPER_MP_TARGT_H
#define FALLOUT2_MAPPER_MP_TARGT_H

namespace Fallout2 {

void target_override_protection();
bool target_overriden();
void target_make_path(char *path, int pid);
int target_init();
int target_exit();
int pick_rot();
int target_pick_global_var(int *value_ptr);

} // namespace Fallout2

#endif
