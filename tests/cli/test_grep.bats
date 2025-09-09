#!/usr/bin/env bats
@test "quasar-grep --help exits 0" {
  run "$BINARY" --help
  [ "$status" -eq 0 ]
}

@test "quasar-grep finds a match and counts" {
  printf "alpha\nbeta\n" > sample.txt
  run "$BINARY" --count alpha sample.txt
  [ "$status" -eq 0 ]
}
