#include <Render/TokenRender.h>
#include <cstdio>

using namespace tex;

void TokenRender::init_from_file(const char *path, const Codec *codec, UniquePtr<TokenRender> &result) {
  UniquePtr<TokenRender> render;
  render.reset(new TokenRender());
  TokenInputStream::init_from_file(path, codec, render->input);
  result.reset(render.take());
}

void TokenRender::render_input(UniquePtr<State> &state) {
  Token token;
  while (!input->consume_token(state, token)) {
    switch(token.cmd) {
      case CC_LETTER:
      case CC_OTHER_CHAR: {
        RenderNode node = RenderNode::char_rnode(token.uc, state->font());
        state->hlist_append(node);
        break;
      }
      case CC_SPACER: {
        Font &font = state->metrics(state->font());
        RenderNode node = RenderNode::glue_rnode(
          font.space(), font.space_stretch(), font.space_shrink(),
          GLUE_NORMAL, GLUE_NORMAL);
        state->hlist_append(node);
        break;
      }
      default: {
        printf("%d\n", token.cmd);
        throw new BlameSourceDiag("Command code not implemented yet.",
          DIAG_RENDER_ERR, BLAME_HERE,
          BlameSource("file", token.line, token.line, token.col, token.col));
      }
    }
  }
}