project('quasar-apex', 'cpp', default_options: ['cpp_std=c++20', 'warning_level=2'])
inc = include_directories('include')
quasar = library('quasar',
  ['src/main.cpp'],  # replace with real sources
  include_directories: inc,
  install: true)
executable('quasar-grep', ['apps/quasar-grep.cpp'], link_with: quasar, install: true)
