# quasar-grep bash completion
_quasar_grep()
{
  local cur prev opts
  COMPREPLY=()
  cur="${COMP_WORDS[COMP_CWORD]}"
  opts="--help --count --json --progress --max-bytes --trace"
  COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
  return 0
}
complete -F _quasar_grep quasar-grep
