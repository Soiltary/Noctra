# Third-party notices

## chiaki-ng (downloaded separately)

Noctra's source and launcher release ZIP do not contain chiaki-ng or its dependency binaries. At the user's request, first-run setup downloads the original archive directly from its official GitHub release, verifies its SHA-256, and extracts it unchanged. Noctra opens chiaki-ng as a separate program and does not link against it.

- Project: https://github.com/chiaki-ng/chiaki-ng
- Official binary release: https://github.com/chiaki-ng/chiaki-ng/releases/tag/v1.10.0
- Asset: `chiaki-ng-win_x64-MSYS2-Release-portable.zip`
- SHA-256: `4f1e824fc522560730e0b7a901e8811de2bab4a45ad5000386f7124bd855e4ac`
- Inner upstream archive identifies build commit: `f5519640adea6ec48087c30d5b9fad62d81166a8`
- Corresponding engine source: https://github.com/chiaki-ng/chiaki-ng/tree/f5519640adea6ec48087c30d5b9fad62d81166a8
- Source archive: https://github.com/chiaki-ng/chiaki-ng/archive/f5519640adea6ec48087c30d5b9fad62d81166a8.zip
- Submodule references: https://github.com/chiaki-ng/chiaki-ng/blob/f5519640adea6ec48087c30d5b9fad62d81166a8/.gitmodules
- Dependency build recipe: https://github.com/chiaki-ng/chiaki-ng/tree/f5519640adea6ec48087c30d5b9fad62d81166a8/.github/workflows
- License copy: `licenses/chiaki-ng-AGPL.txt`

The upstream engine is licensed under AGPL-3.0-only with the upstream OpenSSL exception. The engine's dependencies retain their own licenses, including the notices and source references in the upstream project. Noctra's MIT license does **not** relicense those components.

If you independently redistribute downloaded engine binaries, preserve upstream notices and fulfill the applicable source-availability obligations for that exact engine and its dependencies. Default Noctra packaging excludes engine/.

## Apollo Save Tool

Mentioned only as a separate optional offline-account setup utility. No Apollo code or executable is bundled.

https://github.com/bucanero/apollo-ps4

## The Screen Mirroring

TheDa3's project was reviewed as background inspiration. No code, artwork, executable, or PKG from it is included.

https://github.com/TheDa3/The-Screen-Mirroring
