#ifndef __INCLUDE_RENDER_PENALTY_H__
#define __INCLUDE_RENDER_PENALTY_H__

namespace tex {

typedef int32_t penalty_node;

const penalty_node INF_PENALTY = 10000;
const penalty_node BREAK_PENALTY = -10000;

}

#endif  // __INCLUDE_RENDER_PENALTY_H__