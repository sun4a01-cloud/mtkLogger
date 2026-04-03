/*============================================================================
  Library: mtkLogger
  Copyright (c) Your Organization Name
  All rights reserved.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
============================================================================*/
#ifndef mtkLoggerNamespaceMacro_H
#define mtkLoggerNamespaceMacro_H

/* ── Basic namespace keyword wrappers ───────────────────────────────────── */

#ifndef MTK_CXX_NAMESPACE
#define MTK_CXX_NAMESPACE           namespace
#endif

#ifndef MTK_CXX_NAMESPACE_BEGIN
#define MTK_CXX_NAMESPACE_BEGIN     {
#endif

#ifndef MTK_CXX_NAMESPACE_EOF
#define MTK_CXX_NAMESPACE_EOF       };
#endif

/* ── Private helpers for composing namespace macros ─────────────────────── */

#ifndef MTK_CUSTOMIZE_BEGIN_NAMESPACE_PRIVATE
/* Expands to: namespace _name { */
#define MTK_CUSTOMIZE_BEGIN_NAMESPACE_PRIVATE(_namespace, _name) _namespace _name {
#endif

#ifndef MTK_CUSTOMIZE_END_NAMESPACE_PRIVATE
#define MTK_CUSTOMIZE_END_NAMESPACE_PRIVATE(_name) \
    MTK_CXX_NAMESPACE_EOF // !namespace _name
#endif

/* ── Public custom namespace macros ─────────────────────────────────────── */

#ifndef MTK_CUSTOMIZE_BEGIN_NAMESPACE
#define MTK_CUSTOMIZE_BEGIN_NAMESPACE(_model) \
    MTK_CUSTOMIZE_BEGIN_NAMESPACE_PRIVATE(MTK_CXX_NAMESPACE, _model)
#endif

#ifndef MTK_CUSTOMIZE_END_NAMESPACE
#define MTK_CUSTOMIZE_END_NAMESPACE(_model) \
    MTK_CUSTOMIZE_END_NAMESPACE_PRIVATE(_model)
#endif

/* ── Global namespace: mtk ──────────────────────────────────────────────── */

#ifndef MTK_BEGIN_NAMESPACE
#define MTK_BEGIN_NAMESPACE     MTK_CUSTOMIZE_BEGIN_NAMESPACE(mtk)
#endif

#ifndef MTK_END_NAMESPACE
#define MTK_END_NAMESPACE       MTK_CUSTOMIZE_END_NAMESPACE(mtk)
#endif

/* ── Logger namespace: mtk::logger ──────────────────────────────────────── */

#if __cpp_if_constexpr >= 201606L
// C++17 and above: nested namespace syntax is supported

#ifndef MTK_LOGGER_BEGIN_NAMESPACE
#define MTK_LOGGER_BEGIN_NAMESPACE  MTK_CUSTOMIZE_BEGIN_NAMESPACE(mtk::logger)
#endif

#ifndef MTK_LOGGER_END_NAMESPACE
#define MTK_LOGGER_END_NAMESPACE    MTK_CUSTOMIZE_END_NAMESPACE(mtk::logger)
#endif

#else
// Below C++17: fall back to two separate namespace blocks

#define MTK_LOGGER_BEGIN_NAMESPACE  \
    MTK_CUSTOMIZE_BEGIN_NAMESPACE(mtk)      \
    MTK_CUSTOMIZE_BEGIN_NAMESPACE(logger)

#define MTK_LOGGER_END_NAMESPACE    \
    MTK_CXX_NAMESPACE_EOF           \
    MTK_CXX_NAMESPACE_EOF

#endif // !__cpp_if_constexpr >= 201606L

#endif // !mtkLoggerNamespaceMacro_H
