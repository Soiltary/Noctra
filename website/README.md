# noctra website on Cloudflare Pages

Production: https://noctra-link.pages.dev/ . Direct Upload project: noctra-link, production branch main. Same Cloudflare account as Syntra: f9b1e5fd6056a136b90ced1f00b53bd9 (not a secret).

## Source and publishing

The complete original published website source/assets are in website/site/. This is a static HTML/CSS/JS application with bundled scripts, fonts, icons, video and original downloads. No build/transpiler or backend is required; site/ is the deployment directory. The root native Windows application is separate and was not replaced by an older website checkout. source-provenance.json identifies the original published Sites version. Existing repository privacy is unchanged.

From website/, use Node 24.22.1 and npm 11.19.1:

```sh
npm ci
npx wrangler login --scopes account:read user:read pages:write
npx wrangler pages dev site --port 5192
npm run deploy
```

The owner completes Cloudflare OAuth safely in the browser. Credentials stay outside Git. deploy.mjs fixes the project/account and publishes production on main. No automatic GitHub integration is enabled. Git push saves source only. Edit site/index.html, site/style.css and site/app.js; the offline web setup guide is site/guide.html. Preserve both languages and official download versions. Use relative links within this origin; preserve external product, donation and native updater URLs deliberately.

## Verification performed 2026-10-30

HTTPS and every deployed static file were compared byte-for-byte to this source (23 files). Browser checked English/Arabic and RTL. The local hex Account-ID helper converted 0123456789abcdef to 782riWdFIwE=. The Windows ZIP was downloaded from the new site in the browser. Official Windows download bytes matched SHA-256 and size recorded in the migration report. Native Windows runtime and live PS streaming were not tested. See the migration report for browser/download automation limitations.

## Local data and PWA

No website account or cloud user database exists. The helper does not transmit/store its input. No PWA manifest or Service Worker exists in this site.

## Backups and rollback

Pre-migration GitHub commit: 19882971271d02ef7053cbd0c36f7ecc95615300. Original source/version is recorded in source-provenance.json; independent Git archives of both source trees were saved before changes. Old Sites remains untouched and available. Native clients still use their existing legacy update/download endpoints; do not blanket-redirect or delete them. No permanent redirect was configured because the accessible Sites controls do not expose a verified path-preserving HTTP 301 feature; a page script would not be equivalent and could break compatibility.

First known-good Pages deployment: 8dfc760b-8a0b-46ef-a1d9-66bcc3a05add. Before each later publish, record `npx wrangler pages deployment list --project-name noctra-link`. In Cloudflare dashboard > noctra-link > Deployments, choose the known-good production and Rollback. Alternatively check out a known-good source commit in a separate directory and run npm ci plus npm run deploy from website/. Do not reset an existing uncommitted working copy. No old deployment/site was deleted.

## Cost

Pages Free, no payment/card/trial added. Static assets have free unlimited requests, up to 20,000 files and 25 MiB per file on the Free plan. No Functions/D1/R2 resources are required by this website. Official reference: https://developers.cloudflare.com/pages/platform/limits/ and https://developers.cloudflare.com/pages/functions/pricing/ . GitHub releases and the donation links remain external services.
