/*****************************************************************************
*  Copyright (c) 2012 Duane Ryan Bailey                                      *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*****************************************************************************/

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

static inline void end_paragraph(UniquePtr<State> &state) {
  simple_line_break(state);
  state->render().set_mode(VMODE);
}

static inline void begin_paragraph(UniquePtr<State> &state) {
  RenderState &render = state->render();
  glue_node glue = skip_glue(state->mem(PARINDENT_CODE).scaled);
  RenderNode *indent = RenderNode::new_glue(glue);
  if (render.mode() == HMODE) {
    render.append(indent);
  } else {
    assert(render.mode() == VMODE && "attempted to begin paragraph in "
                                     "non-VMODE, non-HMODE.");
    render.push();
    render.set_mode(HMODE);
    render.set_head(indent);
    render.set_tail(indent);
  }
}

void TokenRender::render_input(UniquePtr<State> &state) {
  Token token;
  RenderState &render = state->render();
  bool stop = false;
  SmallVector<unichar, 16> chars;
  while (!stop) {
    input->peek_token(state, token);
    uint32_t mode_cmd = (render.mode() << 16 | (token.cmd & 0xFFFF));
    switch(mode_cmd) {
      case M(VMODE, CC_LETTER):
      case M(VMODE, CC_OTHER_CHAR): {
        // enter horizontal mode.
        begin_paragraph(state);
        break; // read the character again, this time in HMODE
      }
      case M(HMODE, CC_LETTER):
      case M(HMODE, CC_OTHER_CHAR): {
        // first, read characters into the char array.
        while (token.cmd == CC_LETTER || token.cmd == CC_OTHER_CHAR) {
          input->consume_token(state, token);
          chars.push(token.uc);
          input->peek_token(state, token);
        }
        // now we've hit a new token. token is invalidated, but we can still
        // process valid characters we've already read in. The font has not
        // changed, so we can append char/kerning/ligature nodes as normal.
        uint32_t font = state->font();
        for (unsigned idx = 0; idx < chars.entries(); idx++) {
          render.append(RenderNode::char_rnode(chars[idx], font));
        }
        chars.reset();
        break;
      }
      case M(HMODE, CC_SPACER): {
        input->consume_token(state, token);
        Font &font = state->metrics(state->font());
        RenderNode *node = RenderNode::glue_rnode(
          font.space(), font.space_stretch(), font.space_shrink(),
          GLUE_NORMAL, GLUE_NORMAL);
        render.append(node);
        break;
      }
      case M(HMODE, CC_PAR_END): {
        input->consume_token(state, token);
        if (render.head())
          end_paragraph(state);
        state->builder().build_page(state);
        break;
      }
      case M(VMODE, CC_PAR_END): {
        input->consume_token(state, token);
        break;
      }
      case M(HMODE, CC_STOP): {
        input->consume_token(state, token);
        // leave HMODE
        simple_line_break(state);
        state->builder().build_page(state);
        state->builder().ship_page(state);
        stop = true;
        break;
      }
      case M(VMODE, CC_STOP): {
        input->consume_token(state, token);
        state->builder().ship_page(state);
        stop = true;
        break;
      }
      case M(VMODE, CC_SET_FONT):
      case M(HMODE, CC_SET_FONT):
      case M(MMODE, CC_SET_FONT):
      case M(IN_VMODE, CC_SET_FONT):
      case M(IN_HMODE, CC_SET_FONT):
      case M(IN_MMODE, CC_SET_FONT): {
        input->consume_token(state, token);
        state->set_font(token.cs->operand.i64);
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