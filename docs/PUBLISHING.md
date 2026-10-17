# Publishing Noctra Link

## Repository

Suggested repository name: **noctra-link**

Suggested description:

> A native Windows launcher and offline setup companion for local PS4 streaming with chiaki-ng. By Solitary.

Suggested topics: `ps4`, `remote-play`, `chiaki-ng`, `windows`, `launcher`, `local-network`.

1. Create a GitHub repository named `noctra-link`.
2. Upload the contents of the source package, including `.github/workflows/windows.yml`. Put README.md at the repository root.
3. Keep the MIT license, third-party notices, and all source/artwork files.
4. Do not upload the large `engine/` directory or generated ZIPs as repository source files. They are intentionally ignored.

## Ready-made release

1. Open the repository's Releases page and create a new release.
2. Use tag **v1.0.0**, title **Noctra Link 1.0.0**.
3. Attach the supplied **Noctra-Link-v1.0.0-Windows-x64.zip** and its **SHA256SUMS.txt** from `release-files/` in the publishing kit.
4. Paste the contents of `docs/RELEASE_NOTES.md` into the release description. Keep the upstream source and license links visible with the binary.
5. Check the archive on a Windows PC, then publish the release when satisfied.

The publishing kit contains a `repository/` folder (upload its contents as source) and a `release-files/` folder (attach these files to Releases).

## Build releases with GitHub Actions

The included workflow builds and tests on pushes and pull requests, then uploads the Windows launcher release artifact. The official engine is downloaded by the end user during first-run setup.

To publish a new version automatically:

1. Update the version in `CMakeLists.txt`, `src/main.c`, `src/app.rc`, `src/app.manifest`, and the default package version in `scripts/Package.ps1`.
2. Update CHANGELOG.md and release notes.
3. Push a matching version tag, for example `v1.0.0`.
4. The release job attaches the generated ZIP and checksums. It uses the repository's standard GITHUB_TOKEN with release-write permission; no personal access token is required.

No repository, tag, release, or public upload has been created by preparing this folder. Publishing remains your next action.
