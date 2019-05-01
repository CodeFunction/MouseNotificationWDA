# MouseNotificationWDA

### How to obtain application ###

A) Project can be built using Visual Studio 2019 or newer and then use the resulting executable.

B) For Windows, you can simply download the included MouseNotificationWDA.exe.

### How to start ###

Place MouseNotificationWDA.exe where you would like for it to be and double click to start.

### Instructions ###

Once application has started, it will place an icon into the Windows tray. Drag this icon so that its always visible.

Move the mouse off of the main screen onto another monitor and it will change to red indicating that the mouse is no longer on the main monitor. Move the mouse back onto the main screen and it will change back to green indicating that the mouse is back on the main screen.

Helpful for when you don't want the mouse to be on other monitors but lost track of it. In a quick glance, you will know its on the main screen but blended in.

## To update application ###

A) Just build the project and if using from location in project dir then close and start application again or replace MouseNotificationWDA.exe and used location.

B) If a new version is available, just exit application, replace MouseNotificationWDA.exe, and then start it.

### To remove application ###

This application checks every start (in the event its moved) and adds a registry entry to Windows to auto start on Windows startup after a reboot or shutdown has occurred. Right click on the tray icon and click on clean exit to remove the registry entry and exit the application.

### Possible improvements ###

1) Show arrow (or other) in direction of mouse

2) Allow for displays that are rotated.
