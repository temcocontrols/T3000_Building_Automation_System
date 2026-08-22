# CCTV UX inspiration (internal only)

Vendor help pages and manuals saved on branch **`CCTV`** so Temco can study how homeowners and building operators use popular NVR products.

**These files are inspiration only.** They are not a spec and they do not change T3000. T3000 CCTV still uses the **go2rtc + MediaMTX** localhost sidecars and **Tools → CCTV**. Do not copy vendor UI chrome, trademarks, or copy into the shipping product.

## Operator ranking (homeowner / building operator)

Use this order when looking for UX patterns (live tiles, add-camera, record, export):

1. **UniFi Protect** — clearest adopt / live / record / export story for a non-IT operator.
2. **Reolink** — simple app + PC client + PoE NVR manuals; good for “add camera and see video.”
3. **Synology Surveillance Station 9.0** — full NVR feature set; denser, more IT-oriented.

## Copyright

- **UniFi Protect / UNVR G2** — © Ubiquiti Inc.
- **Reolink App, Client, PoE NVR manuals** — © Reolink
- **Surveillance Station 9.0** — © Synology Inc.

Internal Temco inspiration only. Do not republish, ship, or present these as Temco documentation.

Official HTML for some help-center articles was blocked by bot protection (`help.ui.com` returned 403). Those pages were retrieved from the vendor’s public Help Center JSON API and saved as HTML plus a short Markdown extract. PDFs and other HTML were downloaded with curl from the official URLs below.

---

## UniFi Protect — `unifi-protect/`

| Local file | Official source |
| --- | --- |
| [getting-started-with-unifi-protect.html](unifi-protect/getting-started-with-unifi-protect.html) / [`.md`](unifi-protect/getting-started-with-unifi-protect.md) | https://help.ui.com/hc/en-us/articles/27878858632599-Getting-Started-with-UniFi-Protect |
| [third-party-cameras-in-unifi-protect.html](unifi-protect/third-party-cameras-in-unifi-protect.html) / [`.md`](unifi-protect/third-party-cameras-in-unifi-protect.md) | https://help.ui.com/hc/en-us/articles/26301104828439-Third-Party-Cameras-in-UniFi-Protect |
| [recording-modes-schedules.html](unifi-protect/recording-modes-schedules.html) / [`.md`](unifi-protect/recording-modes-schedules.md) | https://help.ui.com/hc/en-us/articles/27723320017815-UniFi-Protect-Recording-Modes-Schedules |
| [exporting-clips-cloud-archiving.html](unifi-protect/exporting-clips-cloud-archiving.html) / [`.md`](unifi-protect/exporting-clips-cloud-archiving.md) | https://help.ui.com/hc/en-us/articles/27723188678935-Exporting-Clips-Cloud-Archiving-with-UniFi-Protect |
| [unvr-g2-qig.html](unifi-protect/unvr-g2-qig.html) / [`.md`](unifi-protect/unvr-g2-qig.md) | https://dl.ui.com/qig/unvr-g2/ |

No obsolete UniFi Video PDFs.

## Reolink — `reolink/`

| Local file | Official source |
| --- | --- |
| [Reolink_PoE_NVR_User_Manual.pdf](reolink/Reolink_PoE_NVR_User_Manual.pdf) | https://home-cdn.reolink.us/wp-content/assets/multiple-languages/manual/Reolink_PoE_NVR_User_Manual.pdf |
| [Reolink_Client_User_Manual.pdf](reolink/Reolink_Client_User_Manual.pdf) | https://home-cdn.reolink.us/wp-content/assets/multiple-languages/manual/Reolink_Client_User_Manual.pdf |
| [introduction-to-reolink-client.html](reolink/introduction-to-reolink-client.html) (support page) + [introduction-to-reolink-client-article.html](reolink/introduction-to-reolink-client-article.html) / [`.md`](reolink/introduction-to-reolink-client.md) | https://support.reolink.com/articles/900003769906-Introduction-to-Reolink-Client-New-Client/ |
| [getting-started-with-reolink-app.html](reolink/getting-started-with-reolink-app.html) (support page) + [getting-started-with-reolink-app-article.html](reolink/getting-started-with-reolink-app-article.html) / [`.md`](reolink/getting-started-with-reolink-app.md) | https://support.reolink.com/articles/360003493474-Getting-Started-with-Reolink-App/ |

## Synology Surveillance Station 9.0 — `synology-surveillance-station/`

| Local file | Official source |
| --- | --- |
| [Surveillance_Station_User_Guide_9_enu.pdf](synology-surveillance-station/Surveillance_Station_User_Guide_9_enu.pdf) | https://global.download.synology.com/download/Document/Software/UserGuide/Package/SurveillanceStation/9.0/enu/Surveillance_Station_User_Guide_9_enu.pdf |
| [ss9-user-guide-ch11.html](synology-surveillance-station/ss9-user-guide-ch11.html) / [`.md`](synology-surveillance-station/ss9-user-guide-ch11.md) | https://kb.synology.com/en-us/UG/Surveillance_Station_User_Guide_9/11 |
| [add-ip-camera.html](synology-surveillance-station/add-ip-camera.html) / [`.md`](synology-surveillance-station/add-ip-camera.md) | https://kb.synology.com/en-us/SurveillanceStation/help/SurveillanceStation/Tutorial/add_ipcamera |

## T3000 CCTV (what we actually ship)

- [Windows demo](../../DEMO.md) — checkout `CCTV`, build, drop go2rtc, **Tools → CCTV**
- [First-slice notes](../nvr.md) — sidecar ports, launcher, installer notes
