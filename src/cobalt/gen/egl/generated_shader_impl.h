// Copyright 2019 The Cobalt Authors.  All Rights Reserved.
// This file is generated by:
//   cobalt/renderer/rasterizer/egl/generate_shader_impl.py
// Do not edit!

#ifndef GENERATED_GENERATED_SHADER_IMPL_H_
#define GENERATED_GENERATED_SHADER_IMPL_H_

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "renderer_stub/rasterizer/skgl/shader_base.h"

namespace cobalt {
namespace renderer {
namespace rasterizer {
namespace egl {

class ShaderFragmentColor : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }


 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
  }
  void InitializePostUse() override {
  }


  static const char kSource[];
};

class ShaderFragmentColorBlur : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_color() const { return u_color_; }
  GLuint u_blur_rect() const { return u_blur_rect_; }
  GLuint u_scale_add() const { return u_scale_add_; }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_color_ = GetUniformLocation(program, "u_color");
    u_blur_rect_ = GetUniformLocation(program, "u_blur_rect");
    u_scale_add_ = GetUniformLocation(program, "u_scale_add");
  }
  void InitializePostUse() override {
  }

  GLuint u_color_;
  GLuint u_blur_rect_;
  GLuint u_scale_add_;

  static const char kSource[];
};

class ShaderFragmentColorBlurRrects : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_blur_start_top() const { return u_blur_start_top_; }
  GLuint u_blur_start_bottom() const { return u_blur_start_bottom_; }
  GLuint u_blur_scale_top() const { return u_blur_scale_top_; }
  GLuint u_blur_scale_bottom() const { return u_blur_scale_bottom_; }
  GLuint u_blur_extent() const { return u_blur_extent_; }
  GLuint u_scale_add() const { return u_scale_add_; }
  GLuint u_color() const { return u_color_; }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_blur_start_top_ = GetUniformLocation(program, "u_blur_start_top");
    u_blur_start_bottom_ = GetUniformLocation(program, "u_blur_start_bottom");
    u_blur_scale_top_ = GetUniformLocation(program, "u_blur_scale_top");
    u_blur_scale_bottom_ = GetUniformLocation(program, "u_blur_scale_bottom");
    u_blur_extent_ = GetUniformLocation(program, "u_blur_extent");
    u_scale_add_ = GetUniformLocation(program, "u_scale_add");
    u_color_ = GetUniformLocation(program, "u_color");
  }
  void InitializePostUse() override {
  }

  GLuint u_blur_start_top_;
  GLuint u_blur_start_bottom_;
  GLuint u_blur_scale_top_;
  GLuint u_blur_scale_bottom_;
  GLuint u_blur_extent_;
  GLuint u_scale_add_;
  GLuint u_color_;

  static const char kSource[];
};

class ShaderFragmentColorInclude : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_include() const { return u_include_; }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_include_ = GetUniformLocation(program, "u_include");
  }
  void InitializePostUse() override {
  }

  GLuint u_include_;

  static const char kSource[];
};

class ShaderFragmentColorTexcoord : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_texcoord_clamp_rgba() const { return u_texcoord_clamp_rgba_; }
  GLuint u_texture_rgba() const { return u_texture_rgba_; }
  GLenum u_texture_rgba_texunit() const { return GL_TEXTURE0; }
  GLuint u_texture(int index) const {
    if (index == 0) return u_texture_rgba();
    NOTREACHED();
    return u_texture_rgba();
  }
  GLenum u_texture_texunit(int index) const {
    if (index == 0) return u_texture_rgba_texunit();
    NOTREACHED();
    return u_texture_rgba_texunit();
  }
  GLuint u_texcoord_clamp(int index) const {
    if (index == 0) return u_texcoord_clamp_rgba();
    NOTREACHED();
    return u_texcoord_clamp_rgba();
  }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_texcoord_clamp_rgba_ = GetUniformLocation(program, "u_texcoord_clamp_rgba");
    u_texture_rgba_ = GetUniformLocation(program, "u_texture_rgba");
  }
  void InitializePostUse() override {
    SetTextureUnitForUniformSampler(u_texture_rgba(), u_texture_rgba_texunit());
  }

  GLuint u_texcoord_clamp_rgba_;
  GLuint u_texture_rgba_;

  static const char kSource[];
};

class ShaderFragmentColorTexcoordYuv3 : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_texcoord_clamp_y() const { return u_texcoord_clamp_y_; }
  GLuint u_texcoord_clamp_u() const { return u_texcoord_clamp_u_; }
  GLuint u_texcoord_clamp_v() const { return u_texcoord_clamp_v_; }
  GLuint u_texture_y() const { return u_texture_y_; }
  GLuint u_texture_u() const { return u_texture_u_; }
  GLuint u_texture_v() const { return u_texture_v_; }
  GLuint u_color_transform_matrix() const { return u_color_transform_matrix_; }
  GLenum u_texture_y_texunit() const { return GL_TEXTURE0; }
  GLenum u_texture_u_texunit() const { return GL_TEXTURE1; }
  GLenum u_texture_v_texunit() const { return GL_TEXTURE2; }
  GLuint u_texture(int index) const {
    if (index == 0) return u_texture_y();
    if (index == 1) return u_texture_u();
    if (index == 2) return u_texture_v();
    NOTREACHED();
    return u_texture_y();
  }
  GLenum u_texture_texunit(int index) const {
    if (index == 0) return u_texture_y_texunit();
    if (index == 1) return u_texture_u_texunit();
    if (index == 2) return u_texture_v_texunit();
    NOTREACHED();
    return u_texture_y_texunit();
  }
  GLuint u_texcoord_clamp(int index) const {
    if (index == 0) return u_texcoord_clamp_y();
    if (index == 1) return u_texcoord_clamp_u();
    if (index == 2) return u_texcoord_clamp_v();
    NOTREACHED();
    return u_texcoord_clamp_y();
  }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_texcoord_clamp_y_ = GetUniformLocation(program, "u_texcoord_clamp_y");
    u_texcoord_clamp_u_ = GetUniformLocation(program, "u_texcoord_clamp_u");
    u_texcoord_clamp_v_ = GetUniformLocation(program, "u_texcoord_clamp_v");
    u_texture_y_ = GetUniformLocation(program, "u_texture_y");
    u_texture_u_ = GetUniformLocation(program, "u_texture_u");
    u_texture_v_ = GetUniformLocation(program, "u_texture_v");
    u_color_transform_matrix_ = GetUniformLocation(program, "u_color_transform_matrix");
  }
  void InitializePostUse() override {
    SetTextureUnitForUniformSampler(u_texture_y(), u_texture_y_texunit());
    SetTextureUnitForUniformSampler(u_texture_u(), u_texture_u_texunit());
    SetTextureUnitForUniformSampler(u_texture_v(), u_texture_v_texunit());
  }

  GLuint u_texcoord_clamp_y_;
  GLuint u_texcoord_clamp_u_;
  GLuint u_texcoord_clamp_v_;
  GLuint u_texture_y_;
  GLuint u_texture_u_;
  GLuint u_texture_v_;
  GLuint u_color_transform_matrix_;

  static const char kSource[];
};

class ShaderFragmentOpacityTexcoord1d : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_texture() const { return u_texture_; }
  GLuint u_texcoord_transform() const { return u_texcoord_transform_; }
  GLuint u_opacity() const { return u_opacity_; }
  GLenum u_texture_texunit() const { return GL_TEXTURE0; }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_texture_ = GetUniformLocation(program, "u_texture");
    u_texcoord_transform_ = GetUniformLocation(program, "u_texcoord_transform");
    u_opacity_ = GetUniformLocation(program, "u_opacity");
  }
  void InitializePostUse() override {
    SetTextureUnitForUniformSampler(u_texture(), u_texture_texunit());
  }

  GLuint u_texture_;
  GLuint u_texcoord_transform_;
  GLuint u_opacity_;

  static const char kSource[];
};

class ShaderFragmentRcornerColor : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_color() const { return u_color_; }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_color_ = GetUniformLocation(program, "u_color");
  }
  void InitializePostUse() override {
  }

  GLuint u_color_;

  static const char kSource[];
};

class ShaderFragmentRcorner2Color : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_color() const { return u_color_; }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_color_ = GetUniformLocation(program, "u_color");
  }
  void InitializePostUse() override {
  }

  GLuint u_color_;

  static const char kSource[];
};

class ShaderFragmentRcornerTexcoordColor : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_color() const { return u_color_; }
  GLuint u_texcoord_clamp_rgba() const { return u_texcoord_clamp_rgba_; }
  GLuint u_texture_rgba() const { return u_texture_rgba_; }
  GLenum u_texture_rgba_texunit() const { return GL_TEXTURE0; }
  GLuint u_texture(int index) const {
    if (index == 0) return u_texture_rgba();
    NOTREACHED();
    return u_texture_rgba();
  }
  GLenum u_texture_texunit(int index) const {
    if (index == 0) return u_texture_rgba_texunit();
    NOTREACHED();
    return u_texture_rgba_texunit();
  }
  GLuint u_texcoord_clamp(int index) const {
    if (index == 0) return u_texcoord_clamp_rgba();
    NOTREACHED();
    return u_texcoord_clamp_rgba();
  }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_color_ = GetUniformLocation(program, "u_color");
    u_texcoord_clamp_rgba_ = GetUniformLocation(program, "u_texcoord_clamp_rgba");
    u_texture_rgba_ = GetUniformLocation(program, "u_texture_rgba");
  }
  void InitializePostUse() override {
    SetTextureUnitForUniformSampler(u_texture_rgba(), u_texture_rgba_texunit());
  }

  GLuint u_color_;
  GLuint u_texcoord_clamp_rgba_;
  GLuint u_texture_rgba_;

  static const char kSource[];
};

class ShaderFragmentRcornerTexcoordColorYuv3 : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_color() const { return u_color_; }
  GLuint u_texcoord_clamp_y() const { return u_texcoord_clamp_y_; }
  GLuint u_texcoord_clamp_u() const { return u_texcoord_clamp_u_; }
  GLuint u_texcoord_clamp_v() const { return u_texcoord_clamp_v_; }
  GLuint u_texture_y() const { return u_texture_y_; }
  GLuint u_texture_u() const { return u_texture_u_; }
  GLuint u_texture_v() const { return u_texture_v_; }
  GLuint u_color_transform_matrix() const { return u_color_transform_matrix_; }
  GLenum u_texture_y_texunit() const { return GL_TEXTURE0; }
  GLenum u_texture_u_texunit() const { return GL_TEXTURE1; }
  GLenum u_texture_v_texunit() const { return GL_TEXTURE2; }
  GLuint u_texture(int index) const {
    if (index == 0) return u_texture_y();
    if (index == 1) return u_texture_u();
    if (index == 2) return u_texture_v();
    NOTREACHED();
    return u_texture_y();
  }
  GLenum u_texture_texunit(int index) const {
    if (index == 0) return u_texture_y_texunit();
    if (index == 1) return u_texture_u_texunit();
    if (index == 2) return u_texture_v_texunit();
    NOTREACHED();
    return u_texture_y_texunit();
  }
  GLuint u_texcoord_clamp(int index) const {
    if (index == 0) return u_texcoord_clamp_y();
    if (index == 1) return u_texcoord_clamp_u();
    if (index == 2) return u_texcoord_clamp_v();
    NOTREACHED();
    return u_texcoord_clamp_y();
  }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_color_ = GetUniformLocation(program, "u_color");
    u_texcoord_clamp_y_ = GetUniformLocation(program, "u_texcoord_clamp_y");
    u_texcoord_clamp_u_ = GetUniformLocation(program, "u_texcoord_clamp_u");
    u_texcoord_clamp_v_ = GetUniformLocation(program, "u_texcoord_clamp_v");
    u_texture_y_ = GetUniformLocation(program, "u_texture_y");
    u_texture_u_ = GetUniformLocation(program, "u_texture_u");
    u_texture_v_ = GetUniformLocation(program, "u_texture_v");
    u_color_transform_matrix_ = GetUniformLocation(program, "u_color_transform_matrix");
  }
  void InitializePostUse() override {
    SetTextureUnitForUniformSampler(u_texture_y(), u_texture_y_texunit());
    SetTextureUnitForUniformSampler(u_texture_u(), u_texture_u_texunit());
    SetTextureUnitForUniformSampler(u_texture_v(), u_texture_v_texunit());
  }

  GLuint u_color_;
  GLuint u_texcoord_clamp_y_;
  GLuint u_texcoord_clamp_u_;
  GLuint u_texcoord_clamp_v_;
  GLuint u_texture_y_;
  GLuint u_texture_u_;
  GLuint u_texture_v_;
  GLuint u_color_transform_matrix_;

  static const char kSource[];
};

class ShaderFragmentTexcoord : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_texture_rgba() const { return u_texture_rgba_; }
  GLenum u_texture_rgba_texunit() const { return GL_TEXTURE0; }
  GLuint u_texture(int index) const {
    if (index == 0) return u_texture_rgba();
    NOTREACHED();
    return u_texture_rgba();
  }
  GLenum u_texture_texunit(int index) const {
    if (index == 0) return u_texture_rgba_texunit();
    NOTREACHED();
    return u_texture_rgba_texunit();
  }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_texture_rgba_ = GetUniformLocation(program, "u_texture_rgba");
  }
  void InitializePostUse() override {
    SetTextureUnitForUniformSampler(u_texture_rgba(), u_texture_rgba_texunit());
  }

  GLuint u_texture_rgba_;

  static const char kSource[];
};

class ShaderFragmentTexcoordYuv3 : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint u_texture_y() const { return u_texture_y_; }
  GLuint u_texture_u() const { return u_texture_u_; }
  GLuint u_texture_v() const { return u_texture_v_; }
  GLuint u_color_transform_matrix() const { return u_color_transform_matrix_; }
  GLenum u_texture_y_texunit() const { return GL_TEXTURE0; }
  GLenum u_texture_u_texunit() const { return GL_TEXTURE1; }
  GLenum u_texture_v_texunit() const { return GL_TEXTURE2; }
  GLuint u_texture(int index) const {
    if (index == 0) return u_texture_y();
    if (index == 1) return u_texture_u();
    if (index == 2) return u_texture_v();
    NOTREACHED();
    return u_texture_y();
  }
  GLenum u_texture_texunit(int index) const {
    if (index == 0) return u_texture_y_texunit();
    if (index == 1) return u_texture_u_texunit();
    if (index == 2) return u_texture_v_texunit();
    NOTREACHED();
    return u_texture_y_texunit();
  }

 private:
  void InitializePreLink(GLuint program) override {
  }
  void InitializePostLink(GLuint program) override {
    u_texture_y_ = GetUniformLocation(program, "u_texture_y");
    u_texture_u_ = GetUniformLocation(program, "u_texture_u");
    u_texture_v_ = GetUniformLocation(program, "u_texture_v");
    u_color_transform_matrix_ = GetUniformLocation(program, "u_color_transform_matrix");
  }
  void InitializePostUse() override {
    SetTextureUnitForUniformSampler(u_texture_y(), u_texture_y_texunit());
    SetTextureUnitForUniformSampler(u_texture_u(), u_texture_u_texunit());
    SetTextureUnitForUniformSampler(u_texture_v(), u_texture_v_texunit());
  }

  GLuint u_texture_y_;
  GLuint u_texture_u_;
  GLuint u_texture_v_;
  GLuint u_color_transform_matrix_;

  static const char kSource[];
};

class ShaderVertexColor : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_color() const { return 1; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_color(), "a_color");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

class ShaderVertexColorOffset : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_color() const { return 1; }
  GLuint a_offset() const { return 2; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_color(), "a_color");
    BindAttribLocation(program, a_offset(), "a_offset");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

class ShaderVertexColorTexcoord : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_color() const { return 1; }
  GLuint a_texcoord() const { return 2; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_color(), "a_color");
    BindAttribLocation(program, a_texcoord(), "a_texcoord");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

class ShaderVertexOffset : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_offset() const { return 1; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_offset(), "a_offset");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

class ShaderVertexOffsetRcorner : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_offset() const { return 1; }
  GLuint a_rcorner() const { return 2; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_offset(), "a_offset");
    BindAttribLocation(program, a_rcorner(), "a_rcorner");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

class ShaderVertexRcorner : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_rcorner() const { return 1; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_rcorner(), "a_rcorner");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

class ShaderVertexRcorner2 : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_rcorner_inner() const { return 1; }
  GLuint a_rcorner_outer() const { return 2; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_rcorner_inner(), "a_rcorner_inner");
    BindAttribLocation(program, a_rcorner_outer(), "a_rcorner_outer");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

class ShaderVertexRcornerTexcoord : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_rcorner() const { return 1; }
  GLuint a_texcoord() const { return 2; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_rcorner(), "a_rcorner");
    BindAttribLocation(program, a_texcoord(), "a_texcoord");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

class ShaderVertexTexcoord : public ShaderBase {
 public:
  const char* GetSource() const override { return kSource; }

  GLuint a_position() const { return 0; }
  GLuint a_texcoord() const { return 1; }
  GLuint u_clip_adjustment() const { return u_clip_adjustment_; }
  GLuint u_view_matrix() const { return u_view_matrix_; }

 private:
  void InitializePreLink(GLuint program) override {
    BindAttribLocation(program, a_position(), "a_position");
    BindAttribLocation(program, a_texcoord(), "a_texcoord");
  }
  void InitializePostLink(GLuint program) override {
    u_clip_adjustment_ = GetUniformLocation(program, "u_clip_adjustment");
    u_view_matrix_ = GetUniformLocation(program, "u_view_matrix");
  }
  void InitializePostUse() override {
  }

  GLuint u_clip_adjustment_;
  GLuint u_view_matrix_;

  static const char kSource[];
};

}  // namespace egl
}  // namespace rasterizer
}  // namespace renderer
}  // namespace cobalt

#endif  // GENERATED_GENERATED_SHADER_IMPL_H_
