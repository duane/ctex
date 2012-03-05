#include <Render/TokenRender.h>

#include <cstdio>

#include <Render/SimpleBreaker.h>

using namespace tex;

void TokenRender::init_from_file(const char *path, const Codec *codec, UniquePtr<TokenRender> &result) {
  UniquePtr<TokenRender> render;
  render.reset(new TokenRender());
  TokenInputStream::init_from_file(path, codec, render->input);
  result.reset(render.take());
}

#define M(mode, cmd) ((mode) << 16 | (cmd))

void TokenRender::render_input(UniquePtr<State> &state) {
  Token token;
  RenderState &r_state = state->render();
  while (!input->peek_token(state, token)) {
    uint32_t mode_cmd = (r_state.mode() << 16 | (token.cmd & 0xFFFF));
    switch(mode_cmd) {
      case M(VMODE, CC_LETTER):
      case M(VMODE, CC_OTHER_CHAR): {
        // enter horizontal mode.
        r_state.push();
        r_state.set_mode(HMODE);
        r_state.set_head(NULL);
        r_state.set_tail(NULL);
        break; // read the character again, this time in HMODE
      }
      case M(HMODE, CC_LETTER):
      case M(HMODE, CC_OTHER_CHAR): {
        input->consume_token(state, token);
        r_state.append(RenderNode::char_rnode(token.uc, state->font()));
        break;
      }
      case M(HMODE, CC_SPACER): {
        input->consume_token(state, token);
        Font &font = state->metrics(state->font());
        RenderNode *node = RenderNode::glue_rnode(
          font.space(), font.space_stretch(), font.space_shrink(),
          GLUE_NORMAL, GLUE_NORMAL);
        state->render().append(node);
        break;
      }
      case M(HMODE, CC_PAR_END): {
        input->consume_token(state, token);
        simple_line_break(state);
        r_state.set_mode(VMODE);
        break;
      }
      case M(VMODE, CC_PAR_END): {
        input->consume_token(state, token);
        break;
      }
      default: {
        throw new BlameSourceDiag("Command code not implemented yet.",
          DIAG_RENDER_ERR, BLAME_HERE,
          BlameSource("file", token.line, token.line, token.col, token.col));
      }
    }
  }
}