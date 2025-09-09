# CLI Guide
## Commands
- `quasar-grep <pattern> <path> [--count] [--json] [--progress]`
- `quasar-ls <path>`
- `quasar-bench`
- `quasar-cpu`

## Exit codes
- `0` success, `1` not found / negative result, `2` usage error, `>=3` runtime errors.

## Accessibility
- Respect `NO_COLOR` env if color is ever added; provide `--no-color` flag if color is introduced.


## JSON schema
`schemas/quasar-grep.schema.json` documents the `--json` output fields.


## Shell completions
Bash/Zsh/Fish completions are installed with the package.
