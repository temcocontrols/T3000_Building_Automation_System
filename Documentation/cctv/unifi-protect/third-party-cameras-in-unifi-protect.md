# Third-Party Cameras in UniFi Protect

UniFi Protect simplifies the integration of ONVIF-compatible third-party cameras, making it easy to incorporate your existing security infrastructure into the UniFi ecosystem. This flexibility allows you to gradually transition to a UniFi-only setup at your own pace, ensuring a smooth upgrade path without needing to replace all your cameras at once. 
This guide will help you incorporate these cameras into your UniFi setup, enabling live view and playback while gradually upgrading your equipment. 
## Preparing a Third-Party Camera 

- Reference the OEM user manual for instructions on how to set up a specific camera. Generic steps are provided below. 
- Connect the camera to the network and lookup the camera IP address in the UniFi Network app. 
- Certain camera models ship with a static default IP Address. Connect one camera at a time to ensure there are no IP address conflicts. 
- In your browser, navigate to the camera’s local web server. 
- Log in to the camera with default credentials and configure a username and password. 
- Check if the camera requires a firmware update. Apply any updates available. 
- Enable ONVIF in the camera and configure an ONVIF username and password. This is enabled automatically on some cameras and the global username and password are used. 
- Configure the date, time, and timezone on the camera. Some cameras will fail authentication if the time is not set correctly. 
- If applicable, ensure the authentication is set to Digest&ws-username token 
## Adopting Third-Party Cameras in UniFi Protect 

- Open UniFi Protect and go to Settings > System . 
- Enable Discover Third-Party Cameras . 
- Navigate to the UniFi Devices page. Third-party cameras should appear in the list ready for adoption. 
- Select Click to Adopt 
- In the popup, enter the previously configured camera credentials. 
- If the authentication is successful, the camera will be added to the UniFi devices list and the stream will appear in the Dashboard. 
## Camera Functionality 

- The Protect application automatically detects the highest and lowest quality streams provided by the camera. These are shown as HQ and LQ within Protect. 
- LQ is used for multi views and playback when switched to LQ. 
- HQ is used for HQ live view and playback. 
- Motion detections, PTZ control, and Audio are supported on third party cameras.  Other advanced features are not supported. 
- Enabling audio on third party cameras requires transcoding and may use additional console resources. 
- Motion detections must be configured on the camera and sent to Protect. 
- The streams are configured in the third party camera setup tool. If the streams are adjusted after adopting, the camera must be removed and re-added to Protect for the updates to appear. 
## Troubleshooting 

### Camera Does Not Appear in UniFi Devices List 

- Network Check : Ensure the camera is on the same network as the UniFi NVR. 
- ONVIF Enabled : Verify that ONVIF is enabled on the camera. Refer to the OEM user manual for model-specific instructions. 
- Advanced Adoption: If the camera is not on the same subnet as the console it can be adopted using the IP address. Navigate to the help icon in the top left and select Advanced Adoption. 
### Authentication Fails When Adopting the Camera 

- ONVIF & Credentials : Double-check that ONVIF is enabled and the credentials entered are correct. 
- Date & Time Settings : Confirm that the camera’s date and time are accurate. 
- Hikvision Cameras : If using Hikvision, change the Open Network Video Interface Authentication setting to Digest & ws-username and try again. 
- Sony Cameras: Ensure there are video encoders enabled for each profile in Camera UI > Settings > Video/Audio > Video codec . 
- Test Connection : Use a third-party application to test the camera’s ONVIF connection. Several smartphone apps are available for this purpose. 
- Support : If the third-party app confirms the connection but the issue persists, submit a support ticket on ui.com .

---

**Official source:** https://help.ui.com/hc/en-us/articles/26301104828439-Third-Party-Cameras-in-UniFi-Protect

Copyright © Ubiquiti Inc. Saved for internal Temco T3000 CCTV UX inspiration only.
