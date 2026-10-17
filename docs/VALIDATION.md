# Validation for 1.0.0

## Completed in the build environment

- Cross-compiled the native C launcher as a Windows x64 GUI executable using Zig 0.13.0 with warnings enabled.
- Linked a multi-resolution icon, version metadata, DPI declaration, and a non-elevated execution manifest into the executable.
- Compiled and ran the Account-ID tests on Linux, including a build with NDEBUG defined to verify the test assertions remain active in release configurations.
- Checked known conversion vectors, uppercase/prefixed input, malformed hex, incorrect lengths, empty input, and null input.
- Verified the pinned upstream engine archive SHA-256 against the actual official archive and checked its nested archive layout.
- Checked source/release packaging, internal documentation paths, absence of personal account/IP information, and inclusion of licenses and attribution.

## Not performed here

- Running the new Windows launcher or its PowerShell setup script on Windows.
- Running the GitHub Actions workflow on a hosted Windows runner.
- Testing the new launcher with an actual PS4, controller, or network stream.
- Code signing or a third-party security audit.

The executable is unsigned. The previously used chiaki-ng engine was reported working in the original setup, but that is not a runtime test of this new launcher. Test the release on Windows before marking it stable for public users.
