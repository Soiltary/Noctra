# Noctra Link 1.0.0

Your PS4. On your PC. A Windows launcher and setup companion by Solitary, powered by the original chiaki-ng client.

## Download

Download **Noctra-Link-v1.0.0-Windows-x64.zip**, extract the entire folder, and run **NoctraLink.exe**. Open **Setup guide** before pairing your console.

## Included

- Native Windows x64 launcher with the new Noctra Link icon.
- Local Hex-to-Base64 Account-ID helper and clipboard copy.
- English offline quick start.
- First-run setup for the official chiaki-ng v1.10.0 Windows portable engine (one-time download of about 97 MB).

One-time console pairing is required. The launcher does not implement its own PS4 screen-capture service. Video, audio, controls, and console registration are handled by chiaki-ng using the PS4's built-in Remote Play service. Protected content may not be streamable.

## Validation

The native executable was cross-compiled for Windows x64. Account-ID known-vector and invalid-input tests passed. The pinned official engine archive hash and final package contents were checked. The new launcher has not been executed on Windows or tested against a PS4 in the build environment; report runtime issues with a screenshot and Windows version.

## Licenses and source

Noctra Link's launcher is MIT licensed. The separately downloaded chiaki-ng engine and its dependencies retain their upstream licenses; see THIRD_PARTY_NOTICES.md inside the ZIP.

- Upstream release: https://github.com/chiaki-ng/chiaki-ng/releases/tag/v1.10.0
- Corresponding engine source: https://github.com/chiaki-ng/chiaki-ng/tree/f5519640adea6ec48087c30d5b9fad62d81166a8
- Engine source archive: https://github.com/chiaki-ng/chiaki-ng/archive/f5519640adea6ec48087c30d5b9fad62d81166a8.zip
- Engine license: https://github.com/chiaki-ng/chiaki-ng/blob/f5519640adea6ec48087c30d5b9fad62d81166a8/LICENSES/AGPL-3.0-only-OpenSSL.txt
