# DINOForge-UnityDoorstop — Operating Notes

## Language & Toolchain

- **C#** — Unity 2022 LTS+
- Unity Package Manager / `.asmdef` assembly definitions
- **Editor tests** (`Editor/` folder) + **PlayMode tests** via Unity Test Framework

## Architecture

- Unity doorstop / loader tooling for the DINOForge mod platform
- Injector pattern: entry-point shim that bootstraps game runtime
- Platform-specific: handles Windows/macOS/Linux runtime injection differences

## Quality Gates

Run before PR:
- Unity linting: `dotnet format` + Roslyn analyzers
- Build validation: ensure assemblies compile without errors
- Editor test suite passes

## Git Protocol

- Conventional commits: `feat:`, `fix:`, `chore:`, `docs:`, `refactor:`
- Merge or squash-merge to `master`
- No force-push to `master`

## Phenotype Org Policy

- Part of `phenotype-org` — see `~/.claude/CLAUDE.md` for cross-repo reuse,
  OSS-first, and Rust-default scripting hierarchy rules

