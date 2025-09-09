# Secure Coding Guidelines
- Prefer RAII and value semantics; avoid raw new/delete.
- Validate all untrusted input; define limits and timeouts in CLI.
- Avoid UB; compile with sanitizers in CI for PRs.
- No dynamic loading of untrusted libraries or executing external commands.
- Keep dependencies minimal; pin versions for tools.
