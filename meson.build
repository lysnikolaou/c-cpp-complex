project(
  'libnpymath-complex',
  'c', 'cpp',
  version: '0.0.1',
  default_options: ['c_std=c99', 'cpp_std=c++17', 'default_library=static'],
)

cc = meson.get_compiler('c')

# Add default compile flags for any compiler that supports them.
# Note that MSVC does not support strict aliasing at all, and neither do the
# Intel compilers on Windows, so the `-fno` flavor of the flag should be fine.
add_project_arguments(
  cc.get_supported_arguments( '-fno-strict-aliasing'), language : 'c'
)

# Clang defaults to a non-strict floating error point model, but we need strict
# behavior. `-ftrapping-math` is equivalent to `-ffp-exception-behavior=strict`.
# This flag is also required to prevent the activation of SIMD partial load workarounds.
# For further clarification, refer to numpy#gh-24461.
cc_id = cc.get_id()
if cc_id.startswith('clang')
  # Determine the compiler flags for trapping math exceptions.
  trapping_math = {
    'clang-cl': '/clang:-ftrapping-math',
  }.get(cc_id, '-ftrapping-math')
  # Check if the compiler supports the trapping math flag.
  if cc.has_argument(trapping_math)
    # TODO: Consider upgrading the vendored Meson to 1.3.0 to support the parameter `werror`
    # Detect whether the compiler actually supports strict handling of floating-point exceptions
    # by treating warnings as errors.
    if cc.compiles('int main() { return 0; }', args: [trapping_math, '-Werror'])
      trapping_math = [trapping_math]
    else
      # Suppress warnings about unsupported floating-point optimization.
      trapping_math = [trapping_math, '-Wno-unsupported-floating-point-opt']
      # Inform the user about the workaround.
      message(
        'NumPy is being built against a version of Clang that does not strictly enforce ' +
        'floating-point exception handling. Workarounds will be used, which may impact performance.\n' +
        'Consider upgrading Clang to the latest version.'
      )
    endif
    add_project_arguments(trapping_math, language: ['c', 'cpp'])
  endif
endif

libnpymath_includes = include_directories('libnpymath/include')

libnpymath = library(
    'npymath',
    ['libnpymath/npymath.c'],
    include_directories: libnpymath_includes,
)

libnpymath_dep = declare_dependency(
    include_directories: libnpymath_includes,
    link_with: libnpymath,
)

executable(
    'mainc',
    ['main.c'],
    dependencies: libnpymath_dep
)

executable(
    'maincpp',
    ['main.cpp'],
    dependencies: libnpymath_dep
)
