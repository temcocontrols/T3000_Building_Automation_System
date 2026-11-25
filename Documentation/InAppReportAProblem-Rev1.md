# In App Report a Problem (Rev1)

**Status:** proposal · **Branch:** `Maurice` only · **Related:** #509, #510

Goal: a field report should cost one click, arrive with the screen attached, and land on GitHub already structured enough to triage.

## 1. In-app reporting

`Help → Report a Problem…` and `Help → Suggest a Feature…` in T3000, also on the All Nodes / NetInfo context menus.

Three fields — **What happened** (prefilled with the active dialog name), **Steps** (prefilled from the last 60 s of menu/dialog actions), **Expected**. Below them, the captured screenshot with a toolbar: box, arrow, freehand, blur, crop.

Auto-attached, all listed in a "What gets sent" panel the reporter can untick:

| File | Contents |
|---|---|
| `screenshot.png` | active window at native DPI, parent dimmed behind a modal |
| `context.json` | versions, `.prog`, building, active dialog, selected node (serial, IP, protocol, baud) |
| `logtail.txt` | last 200 log lines |
| `nodes.csv` | All Nodes table as displayed, incl. Last contact once #510 lands |

Delivery: default mode opens a **prefilled GitHub issue** in the browser and writes the PNG beside the report — no token in the installer. Staff builds may file **directly via API**. Offline POSTs queue in `%LOCALAPPDATA%\T3000\issues\queue\` and retry on next start.

## 2. Screenshot tooling

- Capture with `PrintWindow` + `PW_RENDERFULLCONTENT`, never a screen `BitBlt` (breaks on RDP and display switches).
- Capture at the window's native DPI; include the shadow margin so it doesn't look cropped.
- Annotations stay in a separate layer, flattened only on send. Numbered markers ("1", "2") are the highest-value tool.
- Redact by blur or solid fill.
- GitHub has **no attachment API**, so images go one of two ways: commit to a throwaway `issue-assets` branch and link `raw.githubusercontent.com`, or have the reporter drag the PNG in. Mode 1 also puts ready-to-paste markdown on the clipboard.
- Reviewing an issue page from the LAN needs the cloud host (playwright → chromium → proxy), `domcontentloaded` plus a retry loop — as used for #509.

## 3. GitHub side

Issue **forms** (`bug_report.yml`, `feature_request.yml`) so fields are enforced. Fixed label set: `bug`, `enhancement`, `needs-info`, `needs-repro`, `screenshot`, `stale-state`, `good-first-issue`, `community`.

First four automations:

1. UI bug without a screenshot → comment + `needs-info`.
2. Release notes generated from closed issues and linked from commits.
3. `needs-info` untouched 14 days → nudge, 30 days → documented close.
4. Duplicate search before any bot files.

Credential custody: Credential Manager for LAN bots, GitHub App token minted at runtime for staff builds, nothing shipped to customers.

## 4. Rollout

| Phase | Done when |
|---|---|
| 1. Issue forms + labels | every new issue arrives labelled and complete |
| 2. `Report a Problem`, mode 1 | a tech files a usable report unaided |
| 3. Capture/annotate + offline queue | nothing is lost off-network |
| 4. Direct API via App token | one click, no browser |
| 5. Release notes + stale sweep | the list reads as a changelog |

## 5. Acceptance

- [ ] Active dialog captured at native DPI, original kept separate from annotations.
- [ ] Reporter can blur regions and sees exactly what will be sent.
- [ ] Offline reports queue and retry, with a pending count visible.
- [ ] Every filed issue carries a label and satisfies the form's required fields.
- [ ] No GitHub token in any installer, config file, or shipped environment variable.

## 6. Open questions

- Auto-blur PII in screenshots, or warn and rely on the reporter?
- Long-term home for images — `issue-assets` branch, or an upload endpoint on the Temco mail server?
- Who owns the weekly triage sweep?
- Customer feature requests: public issue, or private until accepted?
