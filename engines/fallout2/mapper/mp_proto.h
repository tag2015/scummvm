#ifndef FALLOUT2_MAPPER_MP_PROTO_H
#define FALLOUT2_MAPPER_MP_PROTO_H

namespace Fallout2 {

extern char *proto_builder_name;
extern bool can_modify_protos;

void init_mapper_protos();
const char *proto_wall_light_str(int flags);
int proto_pick_ai_packet(int *value);

} // namespace Fallout2

#endif
