# DINOForge-UnityDoorstop

Fork of Unity Doorstop — a tool to execute managed .NET assemblies inside Unity as early as possible. Runs user code before Unity initializes, with cross-platform support.

## Stack
| Layer | Technology |
|-------|------------|
| Core | C# (.NET) |
| Build | xmake.lua (Xmake build system) |
| Unity | Mono/IL2CPP injection |
| Platforms | Windows, Linux, macOS |

## Key Commands
```bash
# Build (all platforms)
./build.sh

# Windows build
pwsh ./build.ps1

# Build specific platform
./build.sh linux    # or: ./build.sh macos

# Check build artifacts
ls build/           # outputs per platform
```

## Key Files
- `src/` — C# source code
- `assets/` — Icons and resources
- `build.sh` / `build.ps1` / `build.bat` — Build scripts
- `xmake.lua` — Xmake build configuration
- `CHANGES.md` — Breaking changes log

## Reference
Global Phenotype rules: see `~/.claude/CLAUDE.md` or `/Users/kooshapari/CodeProjects/Phenotype/repos/CLAUDE.md`
